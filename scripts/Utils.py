import requests
import sys
import time
import ctypes
from fake_useragent import UserAgent
from pathlib import Path

def DownloadFile(url, filepath):
    try:
        with requests.get(url, stream=True, timeout=30) as r:
            r.raise_for_status()
            total = int(r.headers.get("content-length", 0))
            with open(filepath, "wb") as f:
                start = time.time()
                for chunk in r.iter_content(chunk_size=1024 * 1024):
                    f.write(chunk)
                    if total:
                        done = f.tell()
                        percent = done / total * 100
                        speed = done / (time.time() - start + 0.1)
                        speed_str = f"{speed/1024:.2f} KB/s" if speed < 1e6 else f"{speed/1024/1024:.2f} MB/s"
                        bar = int(50 * done / total)
                        sys.stdout.write(f"\r[{'█' * bar}{'.' * (50 - bar)}] {percent:.2f}% ({speed_str})")
                        sys.stdout.flush()
        sys.stdout.write("\n")
    except Exception as e:
        print(f"Download error: {e}")

def YesOrNo():
    while True:
        answer = input("[Y/N]: ").lower()
        if answer in ["y", "yes"]:
            return True
        if answer in ["n", "no"]:
            return False

def IsRunningAdmin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False

def CreateSymlink(path, dest):
    try:
        path = Path(path)
        if not path.exists():
            path.symlink_to(dest)
    except OSError as e:
        print(f"Symlink error: {e}")