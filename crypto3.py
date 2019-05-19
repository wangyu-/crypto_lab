b = 2**20
p = 13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084171
g = 11717829880366207009516117596335367088558084999998952205599979459063929499736583746670572176471460312928594829675428279466566527115212748467589894601965568
h = 3239475104050450443565264378728065788649097520952449527834792452971981976143292558073856937958553180532878928001494706097394108577585732452307673444020333

def power(a,n,p):
	if n==0:
		return 1;
	elif n&1==0:
		tmp=power(a,n/2,p)%p;
		return (tmp*tmp)%p;
	else:
		tmp=power(a,n/2,p)%p
		return (((tmp*tmp)%p)*a)%p

def inv(a,p):
	return power(a,p-2,p)

def inv2(a, m):   # assume m is prime
    u1, u2, u3 = 1, 0, a
    v1, v2, v3 = 0, 1, m
    while v3 != 0:
        q = u3 // v3
        v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
    return u1 % m

dic={}
def make_dic():
	gx1=1
	f = open('gx1.txt', 'w')
	for i in range(b):
		if i%1024==0:
			print i
		tmp=(inv2(gx1,p)*h)%p;
		dic[tmp]=i
		f.write(str(tmp)+"\n");
		gx1=(gx1*g)%p;
	f.close()
def read_dic():
	f =open('gx1.txt','r')
	cnt=0;
	for i in f:
		tmp=int(i.split('\n')[0])
		dic[tmp]=cnt
		cnt=cnt+1;
#make_dic()
read_dic()
gb=power(g,b,p);
gbx0=1;
for i in range(b):
	if i%1024==0:
		print i
	tmp=gbx0
	#tmp=power(gb,i,p)
	if dic.has_key(tmp):
	    print "x0=",i,"  x1=",dic[tmp],"  x=",i*b+dic[tmp]
	    break
	gbx0=(gbx0*gb)%p
#print( pow(g,100,p) )
#print( powmod(g,100,p) )

