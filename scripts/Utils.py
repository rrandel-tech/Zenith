import requests
import sys
import time
import ctypes

from fake_useragent import UserAgent
from pathlib import Path

def DownloadFile(url, filepath):
    try:
        with requests.get(url, stream=True) as response:
            response.raise_for_status()
            total = int(response.headers.get('content-length', 0))
            
            with open(filepath, 'wb') as f:
                downloaded = 0
                startTime = time.time()
                
                for data in response.iter_content(chunk_size=1024 * 1024):
                    downloaded += len(data)
                    f.write(data)
                    
                    elapsedTime = time.time() - startTime
                    speed = (downloaded / 1024) / elapsedTime if elapsedTime > 0 else 0
                    speedStr = f"{speed:.2f} KB/s" if speed < 1024 else f"{speed / 1024:.2f} MB/s"
                    
                    progress = int(50 * downloaded / total) if total else 50
                    percentage = (downloaded / total * 100) if total else 100
                    sys.stdout.write(f"\r[{'█' * progress}{'.' * (50 - progress)}] {percentage:.2f}% ({speedStr})")
                    sys.stdout.flush()
        sys.stdout.write('\n')
    except requests.RequestException as e:
        print(f"Error downloading file: {e}")

def YesOrNo():
    while True:
        reply = input('[Y/N]: ').strip().lower()
        if reply in ('y', 'yes'):
            return True
        if reply in ('n', 'no'):
            return False

def IsRunningAsAdmin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except AttributeError:
        return False # Assume non-admin if not on Windows

# NOTE: requires elevated privileges on Windows
# NOTE: For directories, dest is relative to symlink path
def CreateSymlink(path, dest):
    symlink = Path(path)
    try:
        if not symlink.exists():
            symlink.symlink_to(dest)
    except OSError as e:
        print(f"Failed to create symlink: {e}")