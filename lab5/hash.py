#! /usr/bin/python
import sys
import hashlib

blocks = []
filename=sys.argv[1]
f=open(filename, 'rb')

while True:
	data = f.read(1024)
	if len(data) ==0 :
		break;
	blocks.append(data)

block_num=len(blocks)
hashes=[None]*(block_num+1);
hashes[block_num]='';
print "block_num=",block_num
for i in range(block_num-1,-1,-1):
	hashes[i]=hashlib.sha256(blocks[i]+hashes[i+1]).digest()
print(hashes[0].encode('hex'))
f2=open(filename+".hashed",'wb')
for i in range(0,block_num):
	f2.write(hashes[i]+blocks[i]);
f2.close()
