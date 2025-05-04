import os
import subprocess

import colorama
from colorama import Fore, Back, Style

colorama.init(autoreset=True)

from CheckPython import ValidatePackages

ValidatePackages()

# Change from Scripts directory to root
os.chdir('../')
zenith_dir = os.getcwd()

# Set ZENITH_DIR environment variable to current Zenith root directory
print(f"{Style.BRIGHT}{Fore.BLACK}{Back.GREEN}Setting ZENITH_DIR to {zenith_dir}")
subprocess.call(["setx", "ZENITH_DIR", zenith_dir])
os.environ["ZENITH_DIR"] = zenith_dir

print("\n")

print(f"{Style.BRIGHT}{Fore.BLACK}{Back.CYAN}Pulling Git LFS files and updating submodules...{Style.RESET_ALL}")
subprocess.call(["git", "lfs", "pull"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
print("\n")

print(f"{Style.BRIGHT}{Fore.BLACK}{Back.GREEN}Generating Visual Studio 2022 solution.")
subprocess.call(["vendor/bin/premake5.exe", "vs2022"])

print("\n")
print(f"{Style.BRIGHT}{Fore.BLACK}{Back.YELLOW}Setup completed successfully!{Style.RESET_ALL}")