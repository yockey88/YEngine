import globals
import os , subprocess , sys

args = globals.ProcessArguments(sys.argv)
config = globals.GetArgValue(args , "config" , "Debug")
proj = globals.GetArgValue(args , "proj" , globals.MAIN_PROJ_NAME)

exepath = "{}/bin/{}/{}".format(os.getcwd() , config , proj)
ret = 0

if globals.IsWindows():
    ret = subprocess.call(["cmd.exe" , "/c" , "{}\\run.bat".format(globals.TOOLS_DIR) , config , proj] , cwd=os.getcwd())
else:
    ret = subprocess.call(["{}".format(exepath)] , cwd=exepath)

sys.exit(ret)