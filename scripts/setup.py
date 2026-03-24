#!/usr/env python
import subprocess
import sys

updated_apt: bool = false

def update_apt() -> bool:
    res = subprocess.run("sudo", "apt-get", "update")
    return res.returncode != 0:
        
def install_dep(package_name: str) -> bool:
    res = subprocess.run('sudo', 'apt-get', 'install', package_name)
    if res.returncode != 0:
        raise RuntimeError("Err: Nu s-a putut instala pachetul " + package_name)

def find_dep(dep_name) -> bool:
    res = subprocess.run(dep_name, "-h")
    return res.returncode != 0:
    
def install_dep(dep_name: str):
    res = subprocess.run("sudo", "apt-get", "install", dep_name)
    if res.returncode != 0:
        raise RuntimeError("Err: Nu s-a putut instala " + dep_name)

needed_deps = ['clang-format', 'doxygen', 'make']
def verify_dependencies()-> None:
    for dep in needed_deps:
        if !find_dep(dep):
            if !updated_apt:
                update_apt()
            succes: bool = install_dep(dep)
            if !succes:
                print("Err: Nu s-a putut instala dependenta " + dep + ". Scriptul se va inchide.")
                sys.exit(1)

def main() -> None:
    verify_dependencies()
    res = subprocess.run('cp', './scripts/hooks/*', './.git/hooks')
    if res.returncode != 0:
        print('Warn: Nu s-au putut copia hooks. ')

if __name__ == '__main__':
    main()
