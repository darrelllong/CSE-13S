from canvasapi import Canvas
import argparse
import re
import sys
import os
import cloner
import csv
import json

# Get response to question.
def get_confimation(question):
    confirmation = None
    print(f"{question} ", end='')

    while True:
        choice = input().lower()

        if choice == "yes":
            confirmation = True
            break
        elif choice == "no":
            confirmation = False
            break
        else:
            print("Please enter a valid option [yes/no]: ", end='')

    return confirmation

# Parses out the criterion's score from its corresponding section.
def get_score(criterion, section):
    regex = re.compile(
        f"^.*{criterion} score: (?P<score>[0-9]+)/(?P<total>[0-9]+)", re.DOTALL
    )

    match = regex.search(section)

    if not match:
        raise ValueError
    elif not match.groups("score"):
        raise ValueError

    if int(match.group("score")) > int(match.group("total")):
        raise ValueError

    return int(match.group("score"))

# Parses out the criterion's comments from its corresponding section.
def get_comments(criterion, section):
    regex = re.compile(
        f"^.*{criterion} comments:.*?<<>>(?P<comments>.*)<<>>", re.DOTALL
    )

    match = regex.search(section)

    if not match:
        raise ValueError
    elif not match.group("comments"):
        raise ValueError

    return match.group("comments")

# Creates the scored and commented rubric to upload to Canvas.
def create_rubric(assignment_rubric, scores, comments):
    rubric = {}

    for criterion in assignment_rubric:
        assessment = {}
        assessment["points"] = scores[criterion["description"]]
        assessment["comments"] = comments[criterion["description"]]
        rubric[criterion["id"]] = assessment

    return rubric

def get_rubric_criteria(assignment_rubric):
    criteria = []

    for criterion in assignment_rubric:
        criteria.append(criterion["description"])

    return criteria

def get_assignment_comments(rubric):
    regex = re.compile(
        f".*?(?P<comments>ASSIGNMENT COMMENTS (.*?).*?"
        f"END ASSIGNMENT COMMENTS CRITERION)",
        re.DOTALL
    )

    match = regex.search(rubric)

    if not match:
        return None
    elif not match.group("comments"):
        return None

    comments = get_comments("assignment", match.group("comments"))

    return None if not comments or comments.isspace() else comments

# Parses and returns a list of graded student rubrics.
def parse_rubrics(assignment_rubric, asgn_num, student_csvfile):
    criteria = get_rubric_criteria(assignment_rubric)

    missing = []
    errors = []
    rubrics = []

    with open(student_csvfile, "r", newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        students = list(map(tuple, reader))

        for _, canvas_id, cruz_id, _ in students:
            print(f"Parsing rubric for {cruz_id}... ", end='')

            student_rubric = f"scores/asgn{asgn_num}/{cruz_id}.score"

            if not os.path.exists(student_rubric):
                print("missing rubric file.")
                missing.append(cruz_id)
                continue

            scores = {}
            comments = {}
            auto_zero = False
            error = False

            with open(student_rubric, "r", newline='') as scorefile:
                rubric = scorefile.read()

                for criterion in criteria:
                    capturegroup = criterion.replace(' ', '_')
                    capturegroup = capturegroup.replace('-', '_')
                    capturegroup = capturegroup.replace(':', '_')

                    category = ' '.join(criterion.upper().replace(':', '').split())
                    criterion_ = ' '.join(criterion.lower().replace(':', '').split())

                    regex = re.compile(
                        f".*?(?P<{capturegroup}>{category} (.*?).*?"
                        f"END {category} CRITERION)",
                        re.DOTALL
                    )

                    match = regex.search(rubric)

                    if match:
                        section = match.group(capturegroup)

                        try:
                            scores[criterion] = get_score(criterion_, section)
                        except ValueError:
                            print(f"format error with {criterion_} score.")
                            errors.append(cruz_id)
                            error = True
                            break

                        try:
                            comments[criterion] = get_comments(criterion_, section)
                        except ValueError:
                            print(f"format error with {criterion_} comments.")
                            errors.append(cruz_id)
                            error = True
                            break

                    else:
                        print(f"format error under {category} in rubric.")
                        errors.append(cruz_id)
                        error = True

                if error:
                    continue

            if scores["Files"] == 0 or scores["Compilation"] == 0:
                auto_zero = True

            try:
                asgn_comments = get_assignment_comments(rubric)
            except ValueError:
                print("format error with assignment comments.")
            else:
                graded_rubric = create_rubric(assignment_rubric, scores, comments)
                rubrics.append((cruz_id, canvas_id, graded_rubric, auto_zero, asgn_comments))
                print("done.")

    if len(missing) > 0:
        print("\nStudents with missing or invalid commit IDs/submissions:")
        for student in missing:
            print(f" - {student}")

    if len(errors) > 0:
        print("\nStudents with rubric files that had errors when parsing:")
        for student in errors:
            print(f" - {student}")

    print()
    return rubrics

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawTextHelpFormatter,
    description="Uploads student scored rubric files to Canvas."
)

parser.add_argument(
    "-a", "--assignment", nargs="?", required=True,
    help="The assignment number the scored rubrics are designated for."
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

    try:
        assignment = cloner.get_assignment(course, args.assignment)
    except ValueError:
        print(f"Couldn't find assignment {args.assignment}.", file=sys.stderr)
        sys.exit(1)
    else:
        if not hasattr(assignment, "rubric"):
            print("Can't grade assignment: no associated rubric.", file=sys.stderr)
            sys.exit(1)

        rubrics = parse_rubrics(assignment.rubric, args.assignment, args.csv)

        if get_confimation("Upload rubrics? [yes/no]:"):
            print()

            skipped = []

            for cruz_id, canvas_id, graded_rubric, auto_zero, asgn_comments in rubrics:
                print(f"Uploading graded rubric for {cruz_id}... ", end='')

                submission = assignment.get_submission(canvas_id)
                if not submission.submitted_at:
                    print("skipping due to missing assignment submission.")
                    continue

                submission.edit(rubric_assessment=graded_rubric)

                if auto_zero:
                    submission.edit(submission={'posted_grade': 0})

                if asgn_comments is not None:
                    submission.edit(comment={'text_comment': asgn_comments})

                print("done.")

            if len(skipped) > 0:
                print("\nStudents skipped due to missing submissions:")
                for student in skipped:
                    print(f" - {student}")
