from canvasapi import Canvas
import argparse
import csv
import json
import os
import re
import subprocess as sp

# Creates student CSV file with name, CanvasID, CruzID, and GitLab repo.
def create_student_csv(course, outfile):
    if os.path.exists(outfile):
        os.remove(outfile)

    students = []

    for user in course.get_users(enrollment_type=['student'], enrollment_state=['active']):
        name = user.sortable_name
        canvas_id = user.id
        email = user.email
        cruz_id = re.search(r'(.*?)@ucsc.edu', email).group(1)
        repo = f"git@git.ucsc.edu:cse13s/winter2021/{cruz_id}.git"
        students.append((name, canvas_id, cruz_id, repo))

    students.sort(key=lambda s: s[0])

    with open(outfile, "w", newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=",")

        for name, canvas_id, cruz_id, repo in students:
            writer.writerow([name, canvas_id, cruz_id, repo])

    return

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawTextHelpFormatter,
    description="Generates CSV of entire CSE 13S Canvas roster."
)

parser.add_argument(
    "-o", "--outfile", nargs="?", default="students.csv",
    help="File path of generated CSV (default: students.csv)."
)

args = parser.parse_args()

with open("config.json", "r") as configfile:
    config = json.load(configfile)

    canvas_url = config["canvas_url"]
    course_id = config["course_id"]
    token = config["token"]

    canvas = Canvas(canvas_url, token)
    course = canvas.get_course(course_id)

    create_student_csv(course, args.outfile)
