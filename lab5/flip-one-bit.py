#!/usr/bin/python
"""Toggle the bit at the specified offset.
Syntax: <cmdname> filename newfilename"""
import sys
import os
fname0 = sys.argv[1]
fname = sys.argv[2]
cmd="cp -f "+fname0+" "+fname;
ret=os.system(cmd);
if ret!=0:
	print "["+cmd+"]"+" failed";
nbytes=9999
nbits=0
# Open in read+write, binary mode; read 1 byte 
fp = open(fname, "r+b")
fp.seek(nbytes, 0)
c = fp.read(1)

# Toggle bit at byte position `nbits`
print(ord(c))
toggled = bytearray ( [ ord(c)^(1<<nbits) ] ) 
print(toggled) # diagnostic output

# Back up one byte, write out the modified byte
fp.seek(-1, 1)  # or absolute: fp.seek(nbytes, 0)
fp.write(toggled)
fp.close()
