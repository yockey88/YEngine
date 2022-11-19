import globals
import os , subprocess

if globals.IsWindows():
    VS_BUILD_PATH = os.environ["VS_BUILD_PATH"][8:-1]
    VS_BUILD_PATH = "C:\\\\" + VS_BUILD_PATH
    subprocess.call(["cmd.exe" , "/c" , VS_BUILD_PATH , "{}.sln".format(globals.PROJECT_NAME) , "/property:Configuration={}".format(globals.CONFIG)])