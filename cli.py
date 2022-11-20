import os, sys
import subprocess

TOOLS_DIR = "tools"

def RunCmnd(cmnd):
    ret = 0
    script =  "{}/{}/{}.py".format(os.getcwd() , TOOLS_DIR , cmnd)
    if os.path.exists(script):
        print("\n---------------------------")
        print("[Executing Command]--> ", cmnd , "\n")
        ret = subprocess.call(["python3" , script])
    else:
        print("Invalid Command->[{}] | See Documentation for Machine Y Commands".format(cmnd))
        ret = -1

    return ret

for i in range(1 , len(sys.argv)):
    cmnd = sys.argv[i]
    if RunCmnd(cmnd) != 0:
        break
    print("\n")