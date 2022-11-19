import globals
import os , subprocess

exepath = "{}/bin/{}/{}/".format(os.getcwd() , globals.CONFIGEXE , globals.PROJECT_NAME)

if globals.IsWindows():
    subprocess.call(["cmd.exe" , "/c" , "{}\\run.bat".format(globals.TOOLS_DIR) , globals.CONFIGEXE , globals.ENGINE_NAME] , cwd=os.getcwd())