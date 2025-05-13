import os
import re
import sys
import subprocess
from pathlib import Path

import colorama
from colorama import Fore, Back, Style

import Utils

VULKAN_SDK = os.environ.get("VULKAN_SDK")
REQUIRED_VERSION = "1.3"
INSTALL_VERSION = "1.3.239.0"
INSTALLER_URL = f"https://sdk.lunarg.com/sdk/download/{INSTALL_VERSION}/windows/VulkanSDK-{INSTALL_VERSION}-Installer.exe"
SDK_DIR = "Zenith/vendor/VulkanSDK"
INSTALLER_PATH = f"{SDK_DIR}/VulkanSDK.exe"

colorama.init()

def InstallVulkanSDK():
	Path(SDK_DIR).mkdir(parents=True, exist_ok=True)
	print(f"Downloading Vulkan SDK from {INSTALLER_URL}")
	Utils.download_file(INSTALLER_URL, INSTALLER_PATH)
	print(f"{Style.BRIGHT}{Fore.BLACK}{Back.YELLOW}Run the Vulkan SDK installer manually.")
	os.startfile(os.path.abspath(INSTALLER_PATH))
	print(f"{Style.BRIGHT}{Fore.BLACK}{Back.RED}Re-run this script after installation.")

def InstallVulkanPrompt():
	print("Would you like to install the Vulkan SDK?")
	if Utils.yes_or_no():
		InstallVulkanSDK()
		sys.exit(0)

def ExtractVersion(path):
	match = re.search(r"\d+\.\d+", path)
	return match.group(0) if match else None

def CheckVulkanSDK():
	if not VULKAN_SDK:
		print(f"{Style.BRIGHT}{Fore.BLACK}{Back.RED}Vulkan SDK not found!{Style.RESET_ALL}")
		InstallVulkanPrompt()
		return False

	version = ExtractVersion(VULKAN_SDK)
	if not version or float(version) < float(REQUIRED_VERSION):
		print(f"{Style.BRIGHT}{Fore.BLACK}{Back.RED}Incorrect Vulkan SDK version. Required: {REQUIRED_VERSION}+")
		InstallVulkanPrompt()
		return False

	print(f"{Style.BRIGHT}{Fore.BLACK}{Back.GREEN}Vulkan SDK located at {VULKAN_SDK}")
	return True

def CheckVulkanSDKDebugLibs():
	path = Path(f"{VULKAN_SDK}/Lib/shaderc_sharedd.lib")
	if not path.exists():
		print(f"{Style.BRIGHT}{Fore.BLACK}{Back.YELLOW}Missing Vulkan SDK debug libs at {path}")
		return False
	return True