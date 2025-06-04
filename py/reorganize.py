#!/usr/bin/env python3
"""
reorg_components_simple.py

For each pair include/A.h and src/A.cpp, this script will:

  1. Create a folder `A/` at the project root containing two subfolders:
       A/
       ├── include/
       └── src/
  2. Move:
       include/A.h → A/include/A.h
       src/A.cpp   → A/src/A.cpp

It does NOT rewrite any #include lines. It only moves matching files into per-component folders.

Usage:
    1. Place this script in your project root (next to `include/` and `src/`).
    2. Run: python3 reorg_components_simple.py
    3. The console will show each moved file.

Before running, commit or back up your project, since files will be moved.
"""

import os
import shutil

# ─────────────────── CONFIGURATION ───────────────────
INCLUDE_DIR = "include"   # relative path to your headers
SRC_DIR     = "src"       # relative path to your .cpp files
# ─────────────────────────────────────────────────────

def find_matching_bases(include_dir, src_dir):
    """
    Return a list of base names (e.g. "A", "Foo", "MyClass") for which both
    include/<Base>.h and src/<Base>.cpp exist.
    """
    bases = []
    if not os.path.isdir(include_dir) or not os.path.isdir(src_dir):
        return bases

    for fname in os.listdir(include_dir):
        if not fname.endswith(".h"):
            continue
        base = fname[:-2]  # drop the ".h"
        h_path   = os.path.join(include_dir, fname)
        cpp_path = os.path.join(src_dir, base + ".cpp")
        if os.path.isfile(h_path) and os.path.isfile(cpp_path):
            bases.append(base)
    return bases

def move_into_component_dirs(bases, include_dir, src_dir):
    """
    For each base in `bases`, create a directory `<base>/` at the project root,
    then:
      - create `<base>/include/` and move `include/<base>.h` → `<base>/include/<base>.h`
      - create `<base>/src/`     and move `src/<base>.cpp`   → `<base>/src/<base>.cpp`
    """
    root = os.getcwd()
    for base in bases:
        comp_root = os.path.join(root, base)
        comp_inc  = os.path.join(comp_root, "include")
        comp_src  = os.path.join(comp_root, "src")

        # Create component folder structure
        os.makedirs(comp_inc, exist_ok=True)
        os.makedirs(comp_src, exist_ok=True)

        # Move the header
        old_h = os.path.join(include_dir, f"{base}.h")
        new_h = os.path.join(comp_inc, f"{base}.h")
        if os.path.isfile(old_h):
            print(f"Moving header: {old_h} → {new_h}")
            shutil.move(old_h, new_h)

        # Move the source
        old_cpp = os.path.join(src_dir, f"{base}.cpp")
        new_cpp = os.path.join(comp_src, f"{base}.cpp")
        if os.path.isfile(old_cpp):
            print(f"Moving source: {old_cpp} → {new_cpp}")
            shutil.move(old_cpp, new_cpp)

def main():
    project_root = os.getcwd()
    include_dir = os.path.join(project_root, INCLUDE_DIR)
    src_dir     = os.path.join(project_root, SRC_DIR)

    # 1) Find all base names where include/Base.h and src/Base.cpp both exist
    bases = find_matching_bases(include_dir, src_dir)
    if not bases:
        print("No matching pairs (include/A.h + src/A.cpp) found. Nothing to do.")
        return

    # 2) Move each pair into its own component folder
    move_into_component_dirs(bases, include_dir, src_dir)

    print("✔ Files moved into per-component folders for:", ", ".join(bases))

if __name__ == "__main__":
    main()
