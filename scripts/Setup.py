import os
import subprocess
import CheckPython

# Make sure everything we need is installed
CheckPython.ValidatePackages()

import Utils

import colorama
from colorama import Fore, Back, Style

colorama.init()

# Change from Scripts directory to root
os.chdir('../')

# Set ZENITH_DIR environment variable to current Zenith root directory
print(f"{Style.BRIGHT}{Fore.BLACK}{Back.GREEN}Setting ZENITH_DIR to {os.getcwd()}{Style.RESET_ALL}")
subprocess.call(["setx", "ZENITH_DIR", os.getcwd()])
os.environ['ZENITH_DIR'] = os.getcwd()

subprocess.call(["git", "lfs", "pull"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

print(f"{Style.BRIGHT}{Fore.BLACK}{Back.GREEN}Generating Visual Studio 2022 solution.{Style.RESET_ALL}")
subprocess.call(["vendor/bin/premake5.exe", "vs2022"])