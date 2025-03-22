import subprocess
import sys
import importlib.metadata

def InstallPackage(package):
    try:
        print(f"Checking installation for: {package}")
        if not IsPackageInstalled(package):
            print(f"Installing {package}...")
            subprocess.run([sys.executable, '-m', 'pip', 'install', package], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            print(f"{package} installed successfully.")
        else:
            print(f"{package} is already installed.")
    except subprocess.CalledProcessError as e:
        print(f"Error installing {package}: {e}")

def IsPackageInstalled(package):
    try:
        importlib.metadata.version(package)
        return True
    except importlib.metadata.PackageNotFoundError:
        return False

def ValidatePackages():
    packages = ['setuptools', 'requests', 'fake-useragent', 'colorama']
    for package in packages:
        InstallPackage(package)

if __name__ == "__main__":
    ValidatePackages()