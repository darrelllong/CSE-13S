import argparse, csv, glob, os, sys, importlib, shutil, json
import subprocess as sp
import ag_tools as ag

# Assignment battery script is kept global.
# Doing this avoids having to import it multiple times.
# Same with the assignment configuration script.
asgn_module = None
asgn_config = None


def check_files(tester):
    """Checks if all required files are present."""

    asgn = tester.asgn
    asgn_dir = tester.asgn_dir

    tester.start_section("Files", "are all required files present?")

    with open(f"scripts/{asgn}/{asgn}.json", "r") as configfile:
        required_files = json.load(configfile)["files"]
        submitted_files = os.listdir(asgn_dir)

        for file in required_files:
            tester.start_test(f"{file} was submitted")
            tester.end_test(file in submitted_files)

    tester.end_section(scoring_method=ag.LINEAR, points=15)


def check_makefile(tester):
    """Checks if all specified Makefile targets can be run properly."""

    asgn = tester.asgn
    asgn_dir = tester.asgn_dir
    orig_files = os.listdir(asgn_dir)

    with open(f"scripts/{asgn}/{asgn}.json", "r") as configfile:
        config = json.load(configfile)

    executables = config["executable"]
    makefile_targets = config["makefile_targets"]

    if not makefile_targets:
        return
    elif len(makefile_targets) == 0:
        return

    tester.start_section("Makefile", "are all required targets present and working?")

    tester.start_test("Makefile exists")
    makefile_present = os.path.exists(f"{asgn_dir}/Makefile")
    tester.end_test(makefile_present)

    if not makefile_present:
        return # Can't check targets of a non-existent Makefile.

    for target in makefile_targets:
        if target == "clean":
            continue
        tester.start_test(f"Makefile implements {target} target")
        cmd = f"make -C {asgn_dir} {target}"
        _, _, status = tester.run_command(cmd, return_all=True, output_lines=0)
        tester.end_test(status == 0)

    if "clean" in makefile_targets:
        tester.start_test(f"\"make clean\" cleans all generated files")
        cmd = f"make clean -C {asgn_dir}"
        _, _, status = tester.run(cmd, return_all=True, output_lines=1)
        new_files = [f for f in os.listdir(asgn_dir) if f not in orig_files]
        if status != 0 or len(new_files) > 0:
            if status != 0:
                tester.print(ag.GRADING, "\"make clean\" returned code {status}")
            else:
                tester.print(ag.GRADING, "\"make clean\" didn't clean directory")
                for f in new_files:
                    tester.print(ag.GRADING, f" ! {f} not cleaned")
            tester.end_test(False)
        else:
            tester.end_test(True)

    if not isinstance(executables, list):
        executables = [executables]

    executables = ",".join(executables)
    cmd = f"rm -rf {asgn_dir}/{{{executables},*.o}}"
    sp.run(cmd, capture_output=True, shell=True)


def check_compilation(tester):
    """Check if code can be compiled to specified executables."""

    asgn = tester.asgn
    asgn_dir = tester.asgn_dir

    if asgn == "asgn0":
        return

    tester.start_section("Compilation", "Can executables be compiled?")

    with open(f"scripts/{asgn}/{asgn}.json", "r") as configfile:
        executables = json.load(configfile)["executable"]
        if not isinstance(executables, list):
            executables = [executables]

        for executable in executables:
            tester.start_test(f"Compiling {executable}")
            cmd = f"make -C {asgn_dir}"
            _, _, status = tester.run_command(cmd, return_all=True)
            if status != 0:
                error = f"\"make\" returned with error code"
                tester.end_test(False, fail_explain=error)
            elif not os.path.exists(f"{asgn_dir}/{executable}"):
                error = f"\"make\" did not produce {executable}"
                tester.end_test(False, fail_explain=error)
            else:
                tester.end_test(True)

    tester.end_section(scoring_method=ag.LINEAR, points=5)


def check_clang_format(tester):
    """Are all submitted files properly clang-formatted?"""

    asgn_dir = tester.asgn_dir

    tester.start_section("Clang-Format", "Are files clang-formatted?")

    unformatted = []
    source = [f for f in os.listdir(asgn_dir) if f.endswith(".c") or f.endswith(".h")]
    os.makedirs(f"{asgn_dir}/formatted", exist_ok=True)


    for file in source:
        tester.start_test(f"{file} is formatted")

        code = f"{asgn_dir}/{file}"
        temp = f"{asgn_dir}/formatted/{file}"

        cmd = f"clang-format -style=file {code} > {temp}"
        sp.run(cmd, capture_output=True, shell=True)

        with open(code, "r", errors="ignore") as f:
            before = f.read()

        with open(temp, "r", errors="ignore") as f:
            after = f.read()

        if before != after:
            unformatted.append(file)

        tester.end_test(before == after)

    tester.end_section(scoring_method=ag.LINEAR, points=3)


def check_readme(tester):
    asgn_dir = tester.asgn_dir
    tester.start_section("README")

    tester.start_test("README.md has correct file format")

    output = tester.run_command(f"file {asgn_dir}/README.md")
    file_type = output.split(":")[1].strip()

    error=f"file type is {file_type} instead of ASCII text or empty"
    tester.end_test(file_type == "ASCII text" or file_type == "empty", fail_explain=error)
    tester.end_section(scoring_method=ag.LINEAR, points=2)


parser = argparse.ArgumentParser(description="Autotest launcher.")
parser.add_argument(
    "-a", "--assignment", nargs="?", required=True,
    help="Assignment number to test."
)
parser.add_argument(
    "-c", "--csv", nargs="?", required=True,
    help="Path to CSV with student name, CanvasID, CruzID, and GitLab repo."
)
parser.add_argument(
    "-s", "--speedy", required=False, action="store_true",
    help="Skip chmod/permissions change. Should NOT be used on UNIX timeshare."
)
parser.add_argument(
    "-v", "--verbosity", required = False, default="GRADING",
    choices=["OVERVIEW", "GRADING", "DEBUG"],
    help="Optional argument to add additional information to the output."
)

args = parser.parse_args()

# Import assignment script early on; better for catching errors.
asgn = f"asgn{args.assignment}"
sys.path.insert(1, f"scripts/{asgn}")
asgn_module = importlib.import_module(asgn)

with open(args.csv, "r", newline="") as csvfile:
    reader = csv.reader(csvfile, delimiter=",")
    students = sorted(list(map(tuple, reader)), key=lambda s: s[2])

skipped = []

for _, _, cruz_id, _ in students:
    asgn_dir = f"repos/{cruz_id}/{asgn}"
    print(f"Grading {cruz_id} ({asgn_dir})...")

    if not os.path.isdir(f"repos/{cruz_id}"):
        print(f" missing repo.")
        skipped.append(cruz_id)
        if cruz_id != students[-1][2]:
            print() # Newline unless it's the last student.
        continue

    tester = ag.Tester(cruz_id, asgn, asgn_dir, verbosity=args.verbosity)
    tester.print(ag.OVERVIEW, f"Running autotester for {cruz_id}, {asgn}...")

    cmd = f"git --git-dir=repos/{cruz_id}/.git show --oneline -s"
    tester.print(ag.GRADING, "Getting current commit...")
    commit = tester.run_command(cmd)
    tester.print(ag.OVERVIEW, f"Current commit is {commit}")

    check_files(tester)
    check_makefile(tester)
    check_compilation(tester)
    check_clang_format(tester)
    check_readme(tester)
    asgn_module.check_functionality(tester)
    asgn_module.check_valgrind(tester)

    tester.generate_summary()

if len(skipped) > 0:
    print("\nStudents not graded (due to missing submissions):")
    for student in skipped:
        print(f" - {student}")
