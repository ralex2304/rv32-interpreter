import argparse
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("test_directory", help="directory with test executable and config")
parser.add_argument("--no-silent-make", action='store_true', default=False,
                                      help="enable GNU make output")

args = parser.parse_args()
directory = args.test_directory

with open(directory + "/build_config.txt") as build_config:
    marches = build_config.read().split()

    for march in marches:
        build_res = subprocess.run(["make", f"{directory}/build/test_{march}",
                                   f"SRC_DIR={directory}", f"ARCH={march}"] +
                                   (["--silent"] if not args.no_silent_make else []))

        if build_res.returncode != 0:
            print(f"[FAIL] [BUILD_FAIL] Failed to build for -march={march}")
            exit(1)

