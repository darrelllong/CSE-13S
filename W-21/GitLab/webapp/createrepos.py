#!/usr/bin/env python3

import re,sys,os.path

#=========================================================
#
# Local customization section
#
# Append my local pip3 install path so that we can find my gitlab python installation
# This was installed by doing:
# pip3 install --upgrade --user python-gitlab
sys.path.append (os.path.expanduser ('~/.local/lib/python3.6/site-packages/'))
default_server_path = 'https://gitlab.soe.ucsc.edu/gitlab/'
#
#=========================================================

# Shouldn't need to modify anything past this point
import gitlab
import argparse, subprocess

repo_ops = {'access_created' : 0, 'access_updated' : 0, 'access_removed': 0, 'newrepo': 0, 'nouser': 0, 'error' : 0}
student_roles = {'developer': gitlab.DEVELOPER_ACCESS,
                 'maintainer': gitlab.MASTER_ACCESS,
                 'master': gitlab.MASTER_ACCESS,
                 'reporter': gitlab.REPORTER_ACCESS,
                }
remove_unlisted_members = False

def repo_create_single (gl, group, repo_name, users, role, import_repo):
    repo_fullname = group.full_path + '/' + repo_name
    try:
        repo = gl.projects.create ({'name': repo_name, 'namespace_id': group.id, 'visibility' : 'private',
            'lfs_enabled' : False, 'import_url' : import_repo, 'overwrite': True})
        print ('+++ {0}: repo created'.format (repo_fullname))
        repo_ops['newrepo'] += 1
    except:
        try:
            repo = gl.projects.get (repo_fullname)
        except:
            print ('!!! {0}: error creating or finding repo!'.format (repo_fullname))
            repo_ops['error'] += 1
            return None

    current_members = repo.members.list ()
    allowed_users = []
    for user in users:
        try:
            u = gl.users.list (username=user)[0]
            allowed_users.append (u.id)
        except:
            print ('    {0}: User {1} doesn\'t (yet) exist ... skipping'.format (repo_fullname, user))
            repo_ops['nouser'] += 1
            continue
        if u.id not in [x.id for x in current_members]:
            try:
                member = repo.members.create ({'user_id' : u.id, 'access_level': student_roles[role]})
                repo_ops['access_created'] += 1
            except:
                print ("!!! {0}: unable to add {1} for access!".format (repo_fullname, user))
                repo_ops['error'] += 1
        try:
            member = repo.members.get (u.id)
            member.access_level = student_roles[role]
            member.save()
            print ('+++ {0}: {1} has access as {2}'.format (repo_fullname, user, role))
            repo_ops['access_updated'] += 1
        except:
            print ("!!! {0}: {1} not set as {2} for repo".format (repo_fullname, user, role))
            repo_ops['error'] += 1

    if remove_unlisted_members:
        # Reuse the current members list.  It's OK that it doesn't have the users we just added,
        # since we know they're not 
        for member in current_members:
            if member.id not in allowed_users:
                try:
                    nm = member.username
                    member.delete ()
                    print ('--- {0}: {1} removed as {2}'.format (repo_fullname, nm, role))
                    repo_ops['access_removed'] += 1
                except:
                    print ('!!! {0}: unable to remove access for {1}!'.format (repo_fullname, member.username))
                    repo_ops['error'] += 1

    return repo

def create_repos_from_list (f, gl, group, import_repo, role):
    '''
    Create a lot of repositories from a list.  The format of a line in the file is:
    repo user [user ...]

    This will create a repository named 'repo' (if it doesn't already exist) and ensure that all users
    on the line are given access at level 'role'.
    '''

    for line in f.readlines ():
        line = line.strip().lower()
        if '#' in line:
            line = line[:line.rindex('#')]
        if len(line) == 0:
            continue
        fields = line.split ()
        if len (fields) > 1:
            repo_create_single (gl, group, fields[0], fields[1:], role, import_repo)
        else:
            print ('!!! {0}: no users specified, so no action taken!'.format (fields[0]))

def create_student_repos_many (f, base_email, gl, group, import_repo, role):
    '''
    Create lots of student repos, taking names from file f and using gitlab object gl
    to create a repo for each one in group gl.

    Blank lines are ignored, as are lines starting with #.
    If there's an email address specified on a line, it must match base_email.
    '''
    global remove_unlisted_members

    name_pat = re.compile (r'([a-z0-9._]+)')
    email_addr_pat = re.compile (r'([a-z0-9._]+)@([a-z0-9._-]+)')
    base_email = base_email.lower ()
    for line in f.readlines():
        line = line.strip().lower()
        if len(line) == 0 or line[0] == '#':
            continue
        m1 = name_pat.match (line)
        m2 = email_addr_pat.match (line)
        if m1:
            username = m1.group(1)
        elif m2:
            if m2.group(2) != base_email:
                print ('!!! Skipping {0}: email address doesn\'t match'.format (line))
                continue
            username = m2.group(1)
        else:
            print ('!!! Skipping {0}'.format (line))
        repo_create_single (gl, group, username, [username], role, import_repo)

def openfile (fname):
    if fname == '-':
        return sys.stdin
    else:
        return open (fname, "r")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument ('--server', default=default_server_path,
                         help='Full URL of the gitlab server to connect to' )
    parser.add_argument ('--token', default=None,
                         help='Private token used to authenticate access')
    parser.add_argument ('--domain', default='ucsc.edu',
                         help='Domain name to expect if one is specified')
    parser.add_argument ('--import_repo', default='https://gitlab.soe.ucsc.edu/gitlab/classes/empty_repo.git',
                         help="URL of repository to import with default contents")
    parser.add_argument ('--role', default='developer', choices=student_roles,
                         help='Student role for repo (reporter, developer, maintainer)')
    parser.add_argument ('--repolist', default=None,
                         help='Read repos and users from the supplied list')
    parser.add_argument ('--studentlist', default=None,
                         help='Read users from the supplied list (one user per line, no shared repos)')
    parser.add_argument ('--remove_unlisted', default=False, action='store_true',
                         help='Remove access for users not listed on a repo line')
    parser.add_argument ('group', default=None,
                         help='Full class group name (e.g., "cmpe012/fall17")')
    args = parser.parse_args()

    try:
        subprocess.check_call (['git', 'ls-remote', args.import_repo],
                               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError:
        print ('Unable to access repo ({0}) for import - exiting!'.format (args.import_repo))
        sys.exit (1)
    except:
        print ('Error checking status of import repo {0} - exiting!'.format (args.import_repo))
        sys.exit (1)

    try:
        gl = gitlab.Gitlab (args.server, args.token, api_version = 4)
    except:
        print ('Could not connect to {0} - check network connection and {1}?'.format (args.server, check))
        sys.exit (1)

    try:
        group = gl.groups.get (args.group)
    except:
        print ('Unable to read group {0}'.format (args.group))
        sys.exit (1)

    remove_unlisted_members = args.remove_unlisted

    if (args.repolist and args.studentlist) or not (args.repolist or args.studentlist):
        print ('Must specify exactly one of repolist or studentlist!')
        sys.exit (1)
    elif args.repolist:
        with openfile (args.repolist) as fp:
            create_repos_from_list (fp, gl, group, args.import_repo, args.role)
    else:
        with openfile (args.studentlist) as fp:
            create_student_repos_many (fp, args.domain, gl, group, args.import_repo, args.role)

    print ('Overall statistics:')
    for k in sorted(repo_ops.keys()):
        print ('\t{0}: {1}'.format (k, repo_ops[k]))

