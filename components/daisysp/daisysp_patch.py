#!/usr/bin/env python3
"""
daisysp_patch.py

Patch DaisySP reverbsc.h so that DSY_REVERBSC_MAX_SIZE is wrapped in #ifndef/#endif.
"""

import sys
from pathlib import Path
import re

REVERB_H_PATH = Path(__file__).parent / "../../third_party/daisysp/DaisySP-LGPL/Source/Effects/reverbsc.h"

def patch_reverbsc(file_path: Path):
    if not file_path.exists():
        print(f"Error: {file_path} does not exist.")
        sys.exit(1)

    with file_path.open("r", encoding="utf-8") as f:
        content = f.read()

    # Check if already patched
    if "#ifndef DSY_REVERBSC_MAX_SIZE" in content:
        print(f"{file_path} already contains #ifndef guard — nothing to do.")
        return

    # Regex to match the unconditional define line
    pattern = r'^[ \t]*#define[ \t]+DSY_REVERBSC_MAX_SIZE[ \t]+([0-9]+)'
    match = re.search(pattern, content, flags=re.MULTILINE)

    if not match:
        print(f"No unconditional DSY_REVERBSC_MAX_SIZE define found in {file_path}")
        return

    number = match.group(1)
    # Replace the line with guarded block
    replacement = f"#ifndef DSY_REVERBSC_MAX_SIZE\n#define DSY_REVERBSC_MAX_SIZE {number}\n#endif"

    patched_content = re.sub(pattern, replacement, content, count=1, flags=re.MULTILINE)

    # Write back to the file
    file_path.write_text(patched_content, encoding="utf-8")
    print(f"Patched {file_path}: added #ifndef guard around DSY_REVERBSC_MAX_SIZE")

if __name__ == "__main__":
    patch_reverbsc(REVERB_H_PATH)
