import argparse
import os
from subprocess import Popen, PIPE
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("test_directory", help="directory with test executable and config")
parser.add_argument("-i", "--interpreter", default="../../build/rv32-interpreter",
                                      help="interpreter executable path")
parser.add_argument("--no-silent-make", action='store_true', default=False,
                                      help="enable GNU make output")

args = parser.parse_args()
directory = args.test_directory
interpreter =  args.interpreter

with open(directory + "/build_config.txt") as build_config:
    marches = build_config.read().split()

    for march in marches:
        build_res = subprocess.run(["make", f"{directory}/build/test_{march}",
                                   f"SRC_DIR={directory}", f"ARCH={march}"] +
                                   (["--silent"] if not args.no_silent_make else []))

        if build_res.returncode != 0:
            print(f"[FAIL] [BUILD_FAIL] Failed to build for -march={march}")
            exit(1)

def read_if_exists(filename):
    if not os.path.isfile(filename):
        return ""

    with open(filename) as file:
        return file.read()

test_num = 0
tests_run_count = 0
tests_run_succeed = 0
while os.path.isfile(directory + f"/{str(test_num+1)}_config.txt"):
    test_num += 1
    print(f"Starting test {test_num}")

    with open(directory + f"/{str(test_num)}_config.txt") as config:
        marches  = config.readline().split()
        argv_str = config.readline().split()
        ret_code = int(config.readline())

    stdin  = read_if_exists(directory + f"/{str(test_num)}_stdin.txt")
    stdout = read_if_exists(directory + f"/{str(test_num)}_stdout.txt")
    stderr = read_if_exists(directory + f"/{str(test_num)}_stderr.txt")

    for march_num, march in enumerate(marches):
        tests_run_count += 1
        print(f"\tRunning test {test_num}.{march_num + 1} for -march={march}")

        p = Popen([interpreter] + [directory + f"/build/test_{march}"] + argv_str,
                  stdin=PIPE, stdout=PIPE, stderr=PIPE, text=True)

        stdout_data, stderr_data = p.communicate(input=stdin)
        ret = p.returncode

        fail = False
        if stdout != stdout_data:
            print("\tstdout doesn't match")
            fail = True

        if stderr != stderr_data:
            print("\tstderr doesn't match")
            fail = True

        if ret_code != ret:
            print(f"\treturn code doesn't match: expected {ret_code}, got {ret}")
            fail = True

        if fail:
            print(f"\t[FAIL]")
        else:
            print(f"\t[PASS]")
            tests_run_succeed += 1

assert test_num > 0, "Test config not found"

if tests_run_count != tests_run_succeed:
    print(f"Summary: [FAIL]. {tests_run_succeed}/{tests_run_count} succeed")
    exit(-1)
else:
    print(f"Summary: [PASS]. All {tests_run_count} tests succeed")
    exit(0)

