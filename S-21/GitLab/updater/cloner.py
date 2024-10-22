from git import Repo, GitCommandError, InvalidGitRepositoryError
import argparse, json, os, re, shutil, sys, logging
import subprocess as sp

SERVER = "https://git.ucsc.edu"
DOMAIN = "ucsc.edu"

repo_ops = {
    "cloned": 0,
    "pulled": 0,
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
    if "base_url" not in config:
        raise ValueError("missing base repo url.")
    if "studentlist" not in config:
        raise ValueError("missing studentlist.")

# Clones a single repo, pulls if possible.
def clone_repo(repo_url, username):
    repo_path = os.path.join("repos", username)
    if not os.path.exists(repo_path):
        try:
            logging.info(f"cloning {username}'s repo.")
            repo_clone = Repo.clone_from(repo_url, repo_path)
            repo_ops["cloned"] += 1
        except (GitCommandError, InvalidGitRepositoryError):
            logging.error(f"unable to clone {username}'s repo.")
            repo_ops["error"] += 1
    else:
        try:
            logging.info(f"pulling {username}'s repo.")
            repo_clone = Repo(repo_path)
            repo_clone.git.checkout("master")
            repo_clone.git.pull()
            repo_ops["pulled"] += 1
        except (GitCommandError, InvalidGitRepositoryError):
            logging.error(f"unable to pull {username}'s repo.")
            repo_ops["error"] += 1


def clone_repos(f, base_repo_url, base_email):
    if not os.path.exists("repos"):
        os.makedirs("repos")

    name_re = re.compile(r"([a-z0-9_.]+)")
    email_re = re.compile(r"([a-z0-9_.]+)@([a-z0-9_.-]+)")
    base_email = base_email.lower()

    for line in f.readlines():
        line = line.strip().lower()
        is_name = name_re.match(line)
        is_email = email_re.match(line)

        if is_name:
            username = is_name.group(1)
            clone_repo(os.path.join(base_repo_url, username), username)
        elif is_email:
            if is_email.group(2) != base_email:
                logging.warning(f"{line}: incorrect base email address.")
                continue
            username = is_email.group(1)
            clone_repo(os.path.join(base_repo_url, username), username)
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
    clone_repos(repofile, config["base_url"], config["domain"])

print('Overall statistics:')
for k in sorted(repo_ops.keys()):
    print(f" - {k}: {repo_ops[k]}")
