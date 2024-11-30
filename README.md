# LegoGamesFIB
It's first commit, so after packing it may be artefacts. In future I want to fix it, but it need refpack-compression.
Tool can unpack only fib-archives version 2 and maybe 2.5

There are 5 variants of the FIB format, which differ slightly from each other. Unfortunately, it's not possible to definitively identify a fibfile variant from the file alone, so I have instead listed out the chronological release spans of each variant, as well as what changed in brief.

Fib version 1: LSW:TCS - GBWRTVG

Fib version 2 (Slightly different refpack format): LIJ2 (DS, PSP) - LHP2 (DS ports only)

Fib version 2.5 (Added in deflate compression option): LHP1 - LHP2 (Non DS ports)

Fib version 3 (Changed length/compression bitfield): LB2 - LEGO Friends (DS ports only)

Fib version 3.5 (Added in deflate compression option): LB2 - LSW:TFA (Non DS ports)

# USAGE
Unpacking:
tool.exe file.fib
Packing:
tool.exe directory

# Warning
Don't touch files into directory after unpacking, if you want to pack it and nothing crack in the tool

# Authors
Efim Andreev - full author of this software. Russian Hacker & bwockchain engenyaw ;D
