import argparse
import os
from subprocess import Popen, PIPE

parser = argparse.ArgumentParser()
parser.add_argument("test_directory", help="directory with test executable and config")
parser.add_argument("-i", "--interpreter", default="../../build/rv32-interpreter",
                                      help="interpreter executable path")

args = parser.parse_args()
directory = args.test_directory
interpreter =  args.interpreter

def read_if_exists(filename):
    if not os.path.isfile(filename):
        return ""

    with open(filename) as file:
        return file.read()

test_num = 0
tests_succeed = 0
while os.path.isfile(directory + f"/{str(test_num+1)}_config.txt"):
    test_num += 1
    print(f"Starting test {test_num}")

    with open(directory + f"/{str(test_num)}_config.txt") as config:
        argv_str = [directory + "/test"] + config.readline().split()
        ret_code = int(config.readline())

    stdin  = read_if_exists(directory + f"/{str(test_num)}_stdin.txt")
    stdout = read_if_exists(directory + f"/{str(test_num)}_stdout.txt")
    stderr = read_if_exists(directory + f"/{str(test_num)}_stderr.txt")

    p = Popen([interpreter] + argv_str, stdin=PIPE, stdout=PIPE, stderr=PIPE, text=True)

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
        print("\treturn code doesn't match")
        fail = True

    if fail:
        print(f"\t[FAIL]")
    else:
        print(f"\t[PASS]")
        tests_succeed += 1

assert test_num > 0, "Test config not found"

if test_num != tests_succeed:
    print(f"Summary: [FAIL]. {tests_succeed}/{test_num} succeed")
    exit(-1)
else:
    print(f"Summary: [PASS]. All {test_num} tests succeed")
    exit(0)

