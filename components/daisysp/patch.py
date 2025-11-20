#!/usr/bin/env python3

import sys
import shutil
from pathlib import Path
import filecmp
import re

def copy_tree(src: Path, dst: Path):
    if not src.exists():
        print(f"Source path {src} does not exist.")
        return
    for item in src.rglob('*'):
        rel = item.relative_to(src)
        dest_item = dst / rel
        if item.is_dir():
            dest_item.mkdir(parents=True, exist_ok=True)
        else:
            if not dest_item.exists() or not filecmp.cmp(item, dest_item, shallow=False):
                shutil.copy2(item, dest_item)
                print(f"Copied: {item} -> {dest_item}")

def patch_macros_in_file(file_path: Path, macros):
    #if file_path.suffix not in [".h", ".cpp"]:
    #    return
    with file_path.open("r", encoding="utf-8") as f:
        content = f.read()
    changed = False
    for macro in macros:
        pattern = rf'^[ \t]*#define[ \t]+{re.escape(macro)}[ \t]+([^\n]+)'
        if f"#ifndef {macro}" in content:
            continue
        match = re.search(pattern, content, flags=re.MULTILINE)
        if match:
            value = match.group(1)
            replacement = f"#ifndef {macro}\n#define {macro} {value}\n#endif"
            content = re.sub(pattern, replacement, content, count=1, flags=re.MULTILINE)
            print(f"Patched {macro} in {file_path}")
            changed = True
    if changed:
        file_path.write_text(content, encoding="utf-8")

def patch_headers(dst_root: Path, macros):
    for file in dst_root.rglob('*'):
        if file.suffix in [".h", ".cpp"]:
            patch_macros_in_file(file, macros)

def main():
    if len(sys.argv) < 3:
        print("Usage: patch.py <DAISYSP_ROOT> MACRO1 [MACRO2 ...]")
        sys.exit(1)
    daisysp_root = Path(sys.argv[1]).resolve()
    macros = sys.argv[2:]
    src1 = daisysp_root / "Source"
    src2 = daisysp_root / "DaisySP-LGPL/Source"
    dst = Path(__file__).parent.parent.parent / "build/patch/daisysp/Source"
    print(f"Copying {src1} to {dst}")
    copy_tree(src1, dst)
    print(f"Copying {src2} to {dst}")
    copy_tree(src2, dst)
    print("Patching macros in headers...")
    patch_headers(dst, macros)
    print("Done.")

if __name__ == "__main__":
    main()