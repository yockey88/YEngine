import globals
import os, sys
import subprocess

args = globals.ProcessArguments(sys.argv)
proj = globals.GetArgValue(args, "proj", globals.MAIN_PROJ_NAME)

ret = 0

if globals.IsWindows():
    rundir = "{}/{}/".format(os.getcwd(), proj)
    ret = subprocess.call(["cmd.exe", "/c", "mklink", "/D", "resources", "{}\\resources".format(proj)], cwd=rundir)
else:
    print("No symlink necessary")
sys.exit(ret)