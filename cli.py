import os, sys
import subprocess

TOOLS_DIR = "tools"

def RunCmnd(cmnd):
    subprocess.call(["python3" , "{}/{}/{}.py".format(os.getcwd() , TOOLS_DIR , cmnd)])


for i in range(1 , len(sys.argv)):
    cmnd = sys.argv[i]
    print("\n---------------------------")
    print("[Executing Command]--> ", cmnd , "\n")
    RunCmnd(cmnd)
    print("\n")