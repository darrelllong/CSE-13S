import re, sys, json, gitlab, subprocess, argparse, logging

SERVER = "https://git.ucsc.edu"
DOMAIN = "ucsc.edu"

repo_ops = {
    "access_created": 0,
    "access_updated": 0,
    "access_removed": 0,
    "newrepo": 0,
    "nouser": 0,
    "error": 0
}

roles = {
    "developer": gitlab.DEVELOPER_ACCESS,
    "maintainer": gitlab.MASTER_ACCESS,
    "master": gitlab.MASTER_ACCESS,
    "reporter": gitlab.REPORTER_ACCESS
}

parser = argparse.ArgumentParser()
parser.add_argument(
    "-c", "--config-file", nargs="?", default="config.json",
    help="File path of course config file."
)
parser.add_argument(
    "-r", "--remove-unlisted", default=False, action="store_true",
    help="Remove unlisted members from group repos."
)
parser.add_argument(
    "-l", "--logging-level", default="INFO",
    choices=['debug', 'info', 'warning', 'error', 'critical'],
    help="Set logging level."
)
args = parser.parse_args()

logging.basicConfig(
    stream=sys.stdout, level=args.logging_level.upper(),
    format='%(levelname)s: %(message)s'
)

def validate_config(config):
    if "server" not in config:
        config["server"] = SERVER
    if "domain" not in config:
        config["domain"] = DOMAIN
    if "token" not in config:
        raise ValueError("Missing token.")
    if "studentlist" not in config and "studentrepolist" not in config:
        raise ValueError("Missing both studentlist and studentrepolist (pick one).")
    if "course" not in config:
        raise ValueError("Missing course.")
    if "quarter" not in config:
        raise ValueError("Missing quarter.")
    if "year" not in config:
        raise ValueError("Missing year.")
    if "repo" not in config:
        raise ValueError("Missing template repo to import.")
    if "role" not in config:
        raise ValueError("Missing student repo role.")
    if config["role"] not in roles:
        raise ValueError("Invalid student repo role.")


def create_repo(gl, group, name, users, role, import_repo, remove_unlisted):
    fullname = group.full_path + "/" + name

    try:
        # Try creating the repo.
        repo = gl.projects.create({
            "name": name,
            "namespace_id": group.id,
            "visibility": "private",
            "lfs_enabled": False,
            "import_url": import_repo
        })
        logging.info(f"{fullname}: repo created.")
        repo_ops["newrepo"] += 1
    except:
        # Maybe the repo already exists?
        try:
            repo = gl.projects.get(fullname)
        except:
            logging.error(f"{fullname}: error creating or finding repo.")
            repo_ops["error"] += 1
            return

    curr_members = repo.members.list()
    allowed_users = []

    for username in users:
        try:
            # Find user and add to list of allowed users.
            user = gl.users.list(username=username)[0]
            allowed_users.append(user.id)
        except:
            # User doesn't exist.
            logging.warning(f"{fullname}: user {username} doesn\'t (yet) exist... skipping.")
            repo_ops["nouser"] += 1
            continue

        if user.id not in [member.id for member in curr_members]:
            try:
                member = repo.members.create({
                    "user_id": user.id,
                    "access_level": roles[role]
                })
                repo_ops["access_created"] += 1
            except:
                logging.error(f"{fullname}: unable to add {username} for access.")
                repo_ops["error"] += 1

        try:
            member = repo.members.get(user.id)
            member.access_level = roles[role]
            member.save()
            logging.info(f"{fullname}: {username} given {role} access.")
            repo_ops["access_updated"] += 1
        except:
            logging.error(f"{fullname}: {username} not given {role} access.")
            repo_ops["error"] += 1

    if remove_unlisted:
        for member in curr_members:
            if member.id not in allowed_users:
                try:
                    member.delete()
                    logging.info(f"{fullname}: {member.username} removed as {role}.")
                    repo_ops["access_removed"] += 1
                except:
                    logging.error(f"{fullname}: unable to remove {role} access for {member.username}.")
                    repo_ops["error"] += 1


def create_individual_repos(f, base_email, gl, group, import_repo, role):
    name_re = re.compile(r"([a-z0-9_.]+)")
    email_re = re.compile(r"([a-z0-9_.]+)@([a-z0-9_.-]+)")
    base_email = base_email.lower()

    for line in f.readlines():
        line = line.strip().lower()
        if len(line) == 0 or line[0] == "3":
            continue

        is_name = name_re.match(line)
        is_email = email_re.match(line)

        if is_name:
            username = is_name.group(1)
            create_repo(gl, group, username, [username], role, import_repo, False)
        elif is_email:
            if is_email.group(2) != base_email:
                logging.warning(f"{line}: incorrect base email address.")
                continue
            username = is_email.group(1)
            create_repo(gl, group, username, [username], role, import_repo, False)
        else:
            logging.warning(f"{line}: malformed line.")


def create_shared_repos(f, base_email, gl, group, import_repo, role, remove_unlisted):
    for line in f.readlines():
        line = line.strip().lower()
        if "#" in line:
            line = line[:line.rindex("#")]
        if len(line == 0):
            continue
        fields = line.split()
        if len(fields) > 1:
            create_repo(gl, group, fields[0], fields[1:], role, import_repo, remove_unlisted)
        else:
            logging.warning(f"{fields[0]}: no users specified for shared repo.")


def group_search(user, path):
    for group in user.groups.list():
        if group.full_path == path:
            return group
    else:
        logging.warning(f"{path}: group not found.")
        return None


def subgroup_search(user, group, path):
    for subgroup in user.groups.list():
        if subgroup.path == path and subgroup.parent_id == group.id:
            return subgroup
    else:
        logging.warning(f"{group.full_path}/{path}: subgroup not found.")
        return None


def group_check(user, course):
    group = group_search(user, course)
    group_name = course.upper()

    if not group:
        user.groups.create({
            'name': group_name,
            'path': path,
        })

        while not group:
            group = group_search(user, path)

        logging.info(f"{group.full_path}: group created.")

    settings = group.notificationsettings.get()
    settings.level = gitlab.NOTIFICATION_LEVEL_DISABLED
    settings.save()
    logging.info(f"{group.full_path}: group notifications disabled.")

    return group


def subgroup_check(user, group, quarter, year):
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

        logging.info(f"{subgroup.full_path}: subgroup created.")

    settings = subgroup.notificationsettings.get()
    settings.level = gitlab.NOTIFICATION_LEVEL_DISABLED
    settings.save()
    logging.info(f"{subgroup.full_path}: subgroup notifications disabled.")

    return subgroup


def course_check(user, course, quarter, year):
    group = group_check(user, course)
    return subgroup_check(user, group, quarter, year)


# Load the configuration.
with open(args.config_file, "r") as configfile:
    config = json.load(configfile)

# Validate the configuration.
try:
    validate_config(config)
except ValueError as err:
    logging.error(err)
    sys.exit(1)

# Authenticate the token.
try:
    user = gitlab.Gitlab(config["server"], config["token"], api_version=4)
    user.auth()
except:
    logging.error("Invalid GitLab token.")
    sys.exit(1)

# Validate the template repo to import.
try:
    subprocess.check_call(["git", "ls-remote", config["repo"]],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
except:
    logging.error("Unable to import template repo.")
    sys.exit(1)

# Check if group and subgroup for class exist, create if they don't.
try:
    course = course_check(user, config["course"], config["quarter"], config["year"])
except:
    logging.error("Unable to find or create GitLab course.")
    sys.exit(1)

# Repos are either shared or not shared at this point.
if "studentlist" in config:
    with open(config["studentlist"], "r") as repofile:
        create_individual_repos(repofile, config["domain"], user, course, config["repo"], config["role"])
else:
    with open(config["studentrepolist"], "r") as repofile:
        create_shared_repos(repofile, config["domain"], user, course, config["repo"], config["role"], args.remove_unlisted)

# Print statistics of repo creation.
print('Overall statistics:')
for k in sorted(repo_ops.keys()):
    print(f" - {k}: {repo_ops[k]}")
