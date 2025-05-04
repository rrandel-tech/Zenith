import subprocess
import sys
import importlib.metadata

def IsPackageInstalled(package):
	try:
		importlib.metadata.version(package)
		return True
	except importlib.metadata.PackageNotFoundError:
		return False

def InstallPackage(package):
	if not IsPackageInstalled(package):
		print(f"Installing {package}...")
		try:
			subprocess.run(
				[sys.executable, "-m", "pip", "install", package],
				check=True,
				stdout=subprocess.DEVNULL,
				stderr=subprocess.DEVNULL
			)
			print(f"{package} installed successfully.")
		except subprocess.CalledProcessError:
			print(f"Failed to install {package}.")
	else:
		print(f"{package} already installed.")

def ValidatePackages():
	for package in ['setuptools', 'requests', 'fake-useragent', 'colorama']:
		InstallPackage(package)

if __name__ == "__main__":
	ValidatePackages()