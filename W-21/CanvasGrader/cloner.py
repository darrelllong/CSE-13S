from canvasapi import Canvas
from git import Repo, GitCommandError, InvalidGitRepositoryError
import argparse
import csv
import json
import os
import re
import shutil
import sys
import subprocess as sp

# Returns the specified assignment object.
def get_assignment(course, asgn_num):
    for assignment in course.get_assignments():
        if f"Assignment {asgn_num}" in assignment.name:
            return assignment

    raise ValueError

# Returns the commit ID included in the text-entry box.
def get_commit_id(cruz_id, body):
    match = re.search(r"\b([a-fA-F0-9]{8,40})\b", body)

    if not match:
        raise ValueError

    return match.group(1)

# Clones repos for students in CSV file for specified assignment.
# Existing repos are pulled from instead.
def clone_assignment_repos(course, student_csvfile, asgn_num):
    if not os.path.exists("repos"):
        os.makedirs("repos")

    try:
        assignment = get_assignment(course, asgn_num)
    except ValueError:
        print(f"Couldn't find assignment {asgn_num}.", file=sys.stderr)
        sys.exit(1)
    else:
        with open(student_csvfile, "r", newline='') as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            students = sorted(list(map(tuple, reader)), key=lambda s: s[0])

            failed_clones = []
            failed_pulls = []
            invalid_commit_ids = []
            failed_checkouts = []

            for _, canvas_id, cruz_id, repo in students:
                try:
                  submission = assignment.get_submission(canvas_id)
                except Exception as e:
                  print(cruz_id,"submission has an error:", e)
                  failed_checkouts.append(cruz_id)
                  continue

                if submission.submitted_at is not None:
                    repo_path = os.path.join("repos", cruz_id)
                    repo_clone = None

                    if not os.path.exists(repo_path):
                        try:
                            print(f"Cloning {cruz_id}'s repo... ", end='')
                            repo_clone = Repo.clone_from(repo, repo_path)
                        except (GitCommandError, InvalidGitRepositoryError) as err:
                            failed_clones.append(cruz_id)
                            print(f"failed to clone: {err}.")
                    else:
                        try:
                            print(f"Pulling {cruz_id}'s repo... ", end='')
                            repo_clone = Repo(repo_path)
                            repo_clone.git.checkout("master")
                            repo_clone.git.pull()
                        except (GitCommandError, InvalidGitRepositoryError) as err:
                            failed_pulls.append(cruz_id)
                            print(f"failed to pull: {err}")

                    try:
                        commit_id = get_commit_id(cruz_id, submission.body)
                    except ValueError:
                        invalid_commit_ids.append(cruz_id)
                        print("invalid or missing commit ID submission.")
                    else:
                        try:
                            repo_clone.git.checkout(commit_id)
                            print(f"checked out commit {commit_id}.")
                        except (ValueError, GitCommandError) as err:
                            failed_checkouts.append(cruz_id)
                            print(f"failed to checkout to commit {commit_id}: {err}.")

            if len(failed_clones) > 0:
                print("\nStudents with failed repo clone attempts:")
                for student in failed_clones:
                    print(f" - {student}")

            if len(failed_pulls) > 0:
                print("\nStudents with failed repo pull attempts:")
                for student in failed_pulls:
                    print(f" - {student}")

            if len(invalid_commit_ids) > 0:
                print("\nStudents with missing or invalid commit IDs:")
                for student in invalid_commit_ids:
                    print(f" - {student}")

            if len(failed_checkouts) > 0:
                print("\nStudents with failed checkout attempts:")
                for student in failed_checkouts:
                    print(f" - {student}")

    print()
    return

# Create blank rubric/score files for each student in the CSV with a submission.
# Existing rubrics will be preserved and not overwritten.
def generate_score_files(course, student_csvfile, asgn_num):
    assignment = get_assignment(course, asgn_num)
    asgn = f"asgn{asgn_num}"

    scores_dir = "scores"
    if not os.path.exists(scores_dir):
        os.makedirs(scores_dir)

    asgn_scores_dir = os.path.join(scores_dir, asgn)
    if not os.path.exists(asgn_scores_dir):
        os.makedirs(asgn_scores_dir)

    with open(student_csvfile, "r", newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        students = sorted(list(map(tuple, reader)), key=lambda s: s[0])

        missing_submissions = []

        for _, canvas_id, cruz_id, _ in students:
            try:
              submission = assignment.get_submission(canvas_id)

              if submission.submitted_at is not None:
                  assignment_rubric = f"rubrics/{asgn}.rubric"
                  student_scorefile = os.path.join(asgn_scores_dir, f"{cruz_id}.score")

                  if not os.path.exists(student_scorefile):
                      shutil.copyfile(assignment_rubric, student_scorefile)
                      print(f"Created rubric file for {cruz_id}.")
                  else:
                      print(f"Rubric file for {cruz_id} already exists.")

              else:
                  missing_submissions.append(cruz_id)
            except Exception as e:
              print(f"error creating score file for {cruz_id}:  {e}")
              missing_submissions.append(cruz_id)

        if len(missing_submissions) > 0:
            print("\nStudents not given a rubric/score file due to missing submission:")
            for student in missing_submissions:
                print(f" - {student}")


parser = argparse.ArgumentParser(
    formatter_class=argparse.RawTextHelpFormatter,
    description="Clones CSE 13S student repos for specific assignments."
)

parser.add_argument(
    "-a", "--assignment", nargs="?", required=True,
    help="The assignment number to clone repos and checkout code for."
)

parser.add_argument(
    "-c", "--csv", nargs="?", required=True,
    help="CSV file with student name, CanvasID, CruzID, and GitLab repo.",
)

args = parser.parse_args()

with open("config.json", "r") as configfile:
    config = json.load(configfile)

    canvas_url = config["canvas_url"]
    course_id = config["course_id"]
    token = config["token"]

    canvas = Canvas(canvas_url, token)
    course = canvas.get_course(course_id)

    clone_assignment_repos(course, args.csv, args.assignment)
    generate_score_files(course, args.csv, args.assignment)
