import globals
import os , subprocess , sys

exepath = "{}/bin/{}/{}/".format(os.getcwd() , globals.CONFIGEXE , globals.PONG_NAME)
ret = 0

if globals.IsWindows():
    ret = subprocess.call(["cmd.exe" , "/c" , "{}\\run.bat".format(globals.TOOLS_DIR) , globals.CONFIGEXE , globals.PONG_NAME] , cwd=os.getcwd())
else:
    ret = subprocess.call(["{}{}".format(exepath , globals.ENGINE_NAME)] , cwd=exepath)

sys.exit(ret)