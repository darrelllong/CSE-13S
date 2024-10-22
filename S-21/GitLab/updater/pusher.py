from git import Repo, GitCommandError, InvalidGitRepositoryError
import argparse, json, os, re, shutil, sys, logging
import subprocess as sp

SERVER = "https://git.ucsc.edu"
DOMAIN = "ucsc.edu"

repo_ops = {
    "pushed": 0,
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
    if "files" not in config:
        raise ValueError("missing files to push.")
    if "studentlist" not in config:
        raise ValueError("missing studentlist.")

# Pushes files to a single repo.
def push_to_repo(files, username):
    repo_path = os.path.join("repos", username)
    repo = Repo(repo_path)

    try:
        logging.info(f"[{username}]: adding: {files}")
        repo.index.add(files)
    except:
        repo_ops["errors"] += 1
        return

    try:
        logging.info(f"[{username}]: committing: {files}")
        repo.index.commit(f"pushing: {files}")
    except:
        repo_ops["errors"] += 1
        return

    try:
        logging.info(f"[{username}]: pushing: {files}")
        origin = repo.remote(name="origin")
        origin.push()
        repo_ops["pushed"] += 1
    except:
        repo_ops["errors"] += 1
        return


def push_to_repos(f, files, base_email):
    name_re = re.compile(r"([a-z0-9_.]+)")
    email_re = re.compile(r"([a-z0-9_.]+)@([a-z0-9_.-]+)")
    base_email = base_email.lower()

    for line in f.readlines():
        line = line.strip().lower()
        is_name = name_re.match(line)
        is_email = email_re.match(line)

        if is_name:
            username = is_name.group(1)
            push_to_repo(files, username)
        elif is_email:
            if is_email.group(2) != base_email:
                logging.warning(f"{line}: incorrect base email address.")
                continue
            username = is_email.group(1)
            push_to_repo(files, username)
        else:
            logging.warning(f"{line}: malformed line.")


with open("config.json", "r") as configfile:
    config = json.load(configfile)

try:
    validate_config(config)
except ValueError as err:
    logging.error(err)
    sys.exit(1)

with open(config["studentlist"], "r") as repofile:
    push_to_repos(repofile, config["files"], config["domain"])

print('Overall statistics:')
for k in sorted(repo_ops.keys()):
    print(f" - {k}: {repo_ops[k]}")
