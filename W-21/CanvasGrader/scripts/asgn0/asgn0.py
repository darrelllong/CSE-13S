import os, sys
sys.path.append(os.path.join("..",".."))

import subprocess as sp
import ag_tools as ag

def check_valgrind(tester):
    return

def check_functionality(tester):
    asgn_dir = tester.asgn_dir
    executable = f"{asgn_dir}/hello"

    tester.start_section("Functionality")

    cmd = f"clang -Wall -Werror -Wpedantic -Wextra -o {executable} {asgn_dir}/hello.c"
    _, _, status = tester.run_command(cmd, return_all=True)

    tester.start_test("Program prints: \"Hello World!\\n\"")

    if status != 0 or not os.path.exists(f"{executable}"):
        error = "Couldn't compile hello.c"
        tester.end_test(False, fail_explain=error)
    else:
        result = sp.run(f"./{executable}", capture_output=True, shell=True)
        student = result.stdout
        expected = b"Hello World!\n"
        diff = ag.fuzzy_s(student, expected)
        error = f"Program output incorrect, differs from reference by {diff}"
        tester.end_test(diff == 0, fail_explain=error)

    tester.end_section(scoring_method=ag.LINEAR, points=5)
