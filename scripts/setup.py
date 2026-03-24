#!/usr/bin/env python3
import os
import shutil
import subprocess
import sys

# --- Pas 1: Copierea fișierelor din ./scripts/hooks în ./.git/hooks ---
src_hooks = "./scripts/hooks"
dst_hooks = "./.git/hooks"

if not os.path.exists(src_hooks):
    print(f"Folderul sursă '{src_hooks}' nu există!")
    sys.exit(1)

if not os.path.exists(dst_hooks):
    print(f"Folderul destinație '{dst_hooks}' nu există. Creare folder...")
    os.makedirs(dst_hooks)

for filename in os.listdir(src_hooks):
    src_file = os.path.join(src_hooks, filename)
    dst_file = os.path.join(dst_hooks, filename)
    shutil.copy2(src_file, dst_file)
    # Se face fișierul executabil
    os.chmod(dst_file, 0o755)
    print(f"Copiat {filename} în {dst_hooks}")

# --- Pas 2: Detectarea distribuției Linux ---
def detect_linux_distribution():
    try:
        with open("/etc/os-release") as f:
            content = f.read()
            if "Linux Mint" in content:
                return "mint"
            elif "Arch Linux" in content:
                return "arch"
            else:
                return "other"
    except FileNotFoundError:
        return "unknown"

distro = detect_linux_distribution()
print(f"Distribuția Linux detectată: {distro}")

# --- Pas 3: Funcție pentru verificarea unui program ---
def command_exists(cmd):
    return shutil.which(cmd) is not None

# --- Pas 4: Instalarea clang-format și doxygen dacă nu există ---
packages = ["clang-format", "doxygen"]

if distro == "mint":  # Mint folosește apt-get
    installer = "sudo apt-get install -y"
elif distro == "arch":  # Arch folosește pacman
    installer = "sudo pacman -S --noconfirm"
else:
    print("Distribuție Linux necunoscută. Nu se poate instala automat clang-format și doxygen.")
    sys.exit(1)

for pkg in packages:
    if command_exists(pkg):
        print(f"{pkg} este deja instalat.")
    else:
        print(f"{pkg} nu este instalat. Instalare...")
        subprocess.run(f"{installer} {pkg}", shell=True, check=True)
        print(f"{pkg} instalat cu succes.")
