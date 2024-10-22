###############################################################################
#
# Ethan's existing GitLab stuff
#
###############################################################################

import os.path, re, gitlab, subprocess

log = []

repo_ops = {
    'access_created' : 0,
    'access_updated' : 0,
    'access_removed': 0,
    'newrepo': 0,
    'nouser': 0,
    'error' : 0
}

student_roles = {
    'developer': gitlab.DEVELOPER_ACCESS,
    'maintainer': gitlab.MASTER_ACCESS,
    'master': gitlab.MASTER_ACCESS,
    'reporter': gitlab.REPORTER_ACCESS,
}

remove_unlisted_members = False

def repo_create_single(gl, group, repo_name, users, role, import_repo):
    global log

    fullname = group.full_path + "/" + repo_name
    try:
        repo = gl.projects.create({
            'name': repo_name,
            'namespace_id': group.id,
            'visibility' :
            'private',
            'lfs_enabled' : False,
            'import_url' : import_repo
        })
        log += [f"+++ {fullname}: repo created"]
        repo_ops['newrepo'] += 1
    except:
        try:
            repo = gl.projects.get(fullname)
        except:
            log += [f"!!! {fullname}: error creating or finding repo!"]
            repo_ops['error'] += 1
            return log

    current_members = repo.members.list()
    allowed_users = []
    for user in users:
        try:
            u = gl.users.list(username=user)[0]
            allowed_users.append (u.id)
        except:
            log += [f"{fullname}: User {user} doesn\'t (yet) exist ... skipping"]
            repo_ops['nouser'] += 1
            continue
        if u.id not in [x.id for x in current_members]:
            try:
                member = repo.members.create({
                    'user_id' : u.id,
                    'access_level': student_roles[role]
                })
                repo_ops['access_created'] += 1
            except:
                log += [f"!!! {fullname}: unable to add {user} for access!"]
                repo_ops['error'] += 1
        try:
            member = repo.members.get(u.id)
            member.access_level = student_roles[role]
            member.save()
            log += [f"+++ {fullname}: {user} has access as {role}"]
            repo_ops['access_updated'] += 1
        except:
            log += [f"!!! {fullname}: {user} not set as {role} for repo"]
            repo_ops['error'] += 1

    if remove_unlisted_members:
        # Reuse the current members list.  It's OK that it doesn't have the
        # users we just added, since we know they're not
        for member in current_members:
            if member.id not in allowed_users:
                nm = member.username
                try:
                    member.delete()
                    log += [f"--- {fullname}: {nm} removed as {role}"]
                    repo_ops['access_removed'] += 1
                except:
                    log += [f"!!! {fullname}: unable to remove access for {nm}!"]
                    repo_ops['error'] += 1

    print("Damn it Beevis!")
    return repo


def create_repos_from_list(f, gl, group, import_repo, role):
    '''
    Create a lot of repositories from a list.  The format of a line in the file
    is: repo user [user ...]

    This will create a repository named 'repo' (if it doesn't already exist) and
    ensure that all users on the line are given access at level 'role'.
    '''

    global log

    for line in f.readlines():
        line = line.strip().lower()
        if '#' in line:
            line = line[:line.rindex('#')]
        if len(line) == 0:
            continue
        fields = line.split()
        if len(fields) > 1:
            repo_create_single(gl, group, fields[0], fields[1:], role, import_repo)
        else:
            log += [f"!!! {fields[0]}: no users specified, so no action taken!"]

    print("Eat my shorts!")
    return


def create_student_repos_many(f, base_email, gl, group, import_repo, role):
    '''
    Create lots of student repos, taking names from file f and using gitlab object gl
    to create a repo for each one in group gl.

    Blank lines are ignored, as are lines starting with #.
    If there's an email address specified on a line, it must match base_email.
    '''

    global log
    global remove_unlisted_members

    name_pat = re.compile(r'([a-z0-9._]+)')
    email_addr_pat = re.compile(r'([a-z0-9._]+)@([a-z0-9._-]+)')
    base_email = base_email.lower()

    for line in f.readlines():
        line = line.strip().lower()

        if len(line) == 0 or line[0] == '#':
            continue

        m1 = name_pat.match(line)
        m2 = email_addr_pat.match(line)

        if m1:
            username = m1.group(1)
        elif m2:
            if m2.group(2) != base_email:
                log += [f"!!! Skipping {line}: email address doesn\'t match"]
                continue

            username = m2.group(1)
        else:
            log += [f"!!! Skipping {line}"]

        repo_create_single(gl, group, username, [username], role, import_repo)

    print("Kyle's mom is a bitch!")
    return


###############################################################################
#
# Eugene's stuff
#
###############################################################################

def group_search(user, path):
    '''
    Searches for a user's group based off its path.
    Returns the group if found, else None.
    '''

    global log

    for group in user.groups.list():
        if group.full_path == path:
            return group
    else:
        log += [f"!!! {path}: group not found"]
        return None

def subgroup_search(user, group, path):
    '''
    Searches for a user's subgroup based off its path.
    The subgroup's parent ID must match the ID of the group it belongs to.
    '''

    global log

    for subgroup in user.groups.list():
        if subgroup.path == path and subgroup.parent_id == group.id:
            return subgroup
    else:
        log += [f"!!! {group.full_path}/{path}: subgroup not found"]
        return None

def group_check(user, department, course_id):
    '''
    Checks to see if a group exists.
    Creates the group and disables notifications if it doesn't.
    Won't return the created group until the group has been created.
    '''

    global log

    path = department + course_id
    group = group_search(user, path)
    group_name = department.upper() + " " + course_id.upper()

    if not group:
        user.groups.create({
            'name': group_name,
            'path': path,
        })

        while not group:
            group = group_search(user, path)

        log += [f"+++ {group.full_path}: group created"]


    settings = group.notificationsettings.get()
    settings.level = gitlab.NOTIFICATION_LEVEL_DISABLED
    settings.save()
    log += [f"{group.full_path}: group notifications disabled"]

    return group

def subgroup_check(user, group, quarter, year):
    '''
    Checks to see if a group exists.
    Creates the group and disables notifications if it doesn't.
    Won't return the created subgroup until the subgroup has been created.
    '''

    global log

    path = quarter + year
    subgroup = subgroup_search(user, group, path)
    subgroup_name = quarter.capitalize() + " " + year

    if not subgroup:
        user.groups.create({
            'name': subgroup_name,
            'path': path,
            'parent_id': group.id
        })

        while not subgroup:
            subgroup = subgroup_search(user, group, path)

        log += [f"+++ {subgroup.full_path}: subgroup created"]

    settings = subgroup.notificationsettings.get()
    settings.level = gitlab.NOTIFICATION_LEVEL_DISABLED
    settings.save()
    log += [f"{subgroup.full_path}: subgroup notifications disabled"]

    return subgroup

def course_check(user, department, course_id, quarter, year):
    '''
    Checks/creates a group based on department, course ID, quarter, and year.
    Returns created subgroup when finished.
    '''

    group = group_check(user, department, course_id)
    return subgroup_check(user, group, quarter, year)


###############################################################################
#
# Webform related stuff
#
###############################################################################

from flask import Flask, render_template, flash, request, redirect, url_for

app = Flask(__name__)
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = 0

SERVER = "https://git.ucsc.edu"
DOMAIN = "ucsc.edu"
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
UPLOAD_PATH = os.path.join(BASE_DIR, "files")

@app.route("/")
def index():
    global log
    global remove_unlisted_members

    log = []
    remove_unlisted_members = False
    repo_ops['access_created'] = 0
    repo_ops['access_updated'] = 0
    repo_ops['access_removed'] = 0
    repo_ops['newrepo'] = 0
    repo_ops['nouser'] = 0
    repo_ops['error'] = 0

    # if app.debug:
    #     return render_template('test.html')

    return render_template('index.html')

@app.route("/create", methods=['GET', 'POST'])
def create():
    global log
    global remove_unlisted_members

    token = request.form['token']
    template = request.form['template']
    role = request.form['role']
    department = request.form['department']
    course_id = request.form['course_id'].lower()
    quarter = request.form['quarter']
    year = request.form['year']
    shared = request.form['shared']
    file = request.files['file']

    # Authenticate token.
    try:
        user = gitlab.Gitlab(SERVER, token, api_version=4)
        user.auth()
    except:
        error = "Invalid GitLab token"
        return render_template("error.html", error=error)

    # Validate repo to import.
    try:
        subprocess.check_call(['git', 'ls-remote', template],
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError:
        error = f"Unable to access repo ({template}) for import"
        return render_template("error.html", error=error)
    except:
        error = f"Error checking status of import repo {template}"
        return render_template("error.html", error=error)

    # Check if group and subgroup for class exist, create if they don't.
    try:
        course = course_check(user, department, course_id, quarter, year)
    except:
        error = "Error checking/creating group/subgroup for the course"
        return render_template("error.html", error=error)

    # Make sure saved roster/repolist doesn't overwrite another course's.
    fname = department + course_id + quarter + year + "_" + file.filename
    filepath = os.path.join(UPLOAD_PATH, fname)
    file.save(filepath)

    # Should students no longer enrolled be removed from shared repos?
    if 'remove' in request.form.keys():
        remove_unlisted_members = True

    # Create repos, delete roster/repolist after.
    try:
        with open(filepath, "r") as fp:
            if shared == "no":
                create_student_repos_many(fp, DOMAIN, user, course, template,
                        role)
            else:
                create_repos_from_list(fp, user, course, template, role)

            stats = []
            for k in sorted(repo_ops.keys()):
                stats += [f"{k}: {repo_ops[k]}"]

        os.remove(filepath)
        return render_template("output.html", log=log, stats=stats)
    except:
        if os.path.exists(filepath):
            os.remove(filepath)
        error = "Bad student roster/repolist file"
        return render_template("error.html", error=error)

    # Throw an error if something unaccounted for occurred.
    error = "Unknown error"
    return render_template("error.html", error=error)


if __name__ == "__main__":
    app.run(debug=True, port=8080)
