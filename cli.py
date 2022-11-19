import os, sys
import subprocess

TOOLS_DIR = "tools"

def RunCmnd(cmnd):
    script =  "{}/{}/{}.py".format(os.getcwd() , TOOLS_DIR , cmnd)
    if os.path.exists(script):
        print("\n---------------------------")
        print("[Executing Command]--> ", cmnd , "\n")
        subprocess.call(["python3" , script])
    else:
        print("Invalid Command->[{}] | See Documentation for Machine Y Commands".format(cmnd))

for i in range(1 , len(sys.argv)):
    cmnd = sys.argv[i]
    RunCmnd(cmnd)
    print("\n")