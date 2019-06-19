#! /usr/bin/python
import sys
import hashlib

filename=sys.argv[1]
f=open(filename, 'rb')
h=f.read(32)
cnt=0;
while True:
	if len(h)==0:
		print "verify ok"
		break;
	data= f.read(1024+32)
	cnt=cnt+1;
	if h != hashlib.sha256(data).digest():
		print "error in block:",cnt
		break;
	h=data[1024:]

