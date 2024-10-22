import re, sys, json, gitlab, subprocess, argparse, logging

SERVER = "https://git.ucsc.edu"
DOMAIN = "ucsc.edu"

repo_ops = {
    "access_created": 0,
    "access_updated": 0,
    "nouser": 0,
    "error": 0
}

parser = argparse.ArgumentParser()
parser.add_argument(
    "-c", "--config-file", nargs="?", default="config.json",
    help="File path of course config file."
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
    if "studentlist" not in config:
        raise ValueError("Missing studentlist.")
    if "resource_repo_id" not in config:
        raise ValueError("Missing project ID of resource repo to add users to.")


def add_user(gl, repo, username):
    try:
        # Find user and add to list of allowed users.
        user = gl.users.list(username=username)[0]
    except:
        # User doesn't exist.
        logging.warning(f"user {username} doesn\'t (yet) exist... skipping.")
        repo_ops["nouser"] += 1
        return

    if not repo.members.list(query=username) and not repo.members.all(query=username):
        try:
            member = repo.members.create({
                "user_id": user.id,
                "access_level": gitlab.REPORTER_ACCESS
            })
            logging.info(f"{username} added as reporter.")
            repo_ops["access_created"] += 1
        except:
            logging.error(f"unable to add {username} for access.")
            repo_ops["error"] += 1
    else:
        try:
            member = repo.members.all(query=username)[0]
            if member.access_level != gitlab.REPORTER_ACCESS:
                if member.access_level > gitlab.REPORTER_ACCESS:
                    logging.warning(f"{username} has higher access than reporter.")
                    return
                member.access_level = gitlab.REPORTER_ACCESS
                member.save()
                logging.info(f"{username} given reporter access.")
                repo_ops["access_updated"] += 1
            else:
                logging.info(f"{username} is already a reporter.")
        except:
            logging.error(f"{username} not given reporter access.")
            repo_ops["error"] += 1


def add_users(f, base_email, gl, repo):
    name_re = re.compile(r"([a-z0-9_.]+)")
    email_re = re.compile(r"([a-z0-9_.]+)@([a-z0-9_.-]+)")
    base_email = base_email.lower()

    for line in f.readlines():
        line = line.strip().lower()
        is_name = name_re.match(line)
        is_email = email_re.match(line)

        if is_name:
            username = is_name.group(1)
            add_user(gl, repo, username)
        elif is_email:
            if is_email.group(2) != base_email:
                logging.warning(f"{line}: incorrect base email address.")
                continue
            username = is_email.group(1)
            add_user(gl, repo, username)
        else:
            logging.warning(f"{line}: malformed line.")


def repo_search(user, project_id):
    repo = user.projects.get(project_id)
    if not repo:
        raise ValueError
    return repo


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

# Find the resource repo.
try:
    resource_repo = repo_search(user, config["resource_repo_id"])
except:
    logging.error("Invalid resource repo ID.")
    repo_ops["error"] += 1
    sys.exit(1)

with open(config["studentlist"], "r") as repofile:
    add_users(repofile, config["domain"], user, resource_repo)

# Print statistics of adding students to resource repo.
print('Overall statistics:')
for k in sorted(repo_ops.keys()):
    print(f" - {k}: {repo_ops[k]}")
