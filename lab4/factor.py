#! /usr/bin/python
def is_sqrt(a,x):
	if a**2<=x and (a+1)**2>x:
		return True
	return False
def find_sqrt0(x):
	l=0;
	h=x;
	while(l<h):	
		mid=(l+h)/2;
		if is_sqrt(mid,x):
			return mid;
		elif mid**2<=x: 
			l=mid+1;
		else :
			h=mid-1;
	return l
def find_sqrt(x):
    if x < 0:
        raise ValueError('square root not defined for negative numbers')
    n = int(x)
    if n == 0:
        return 0
    a, b = divmod(n.bit_length(), 2)
    x = 2**(a+b)
    while True:
        y = (x + n//x)//2
        if y >= x:
            return x
        x = y
for i in range(0,100):
	print str(i)+":"+str(find_sqrt(i))
def factor(N):
	S=find_sqrt(N);
	print "N=",N
	for i in range(0,2**20):
		if i%10000==0:
			print i
		A=S+i;
		tmp=A**2-N;
		if tmp<0:
			continue;
		x=find_sqrt(tmp)
		if x**2==tmp:
			print "X=",x;
			print "P=",A-x;
			print "Q=",A+x;
			print "P*Q=",A**2-x**2;
			return;
	print "Not found";
print "=========case 1=========";
factor(179769313486231590772930519078902473361797697894230657273430081157732675805505620686985379449212982959585501387537164015710139858647833778606925583497541085196591615128057575940752635007475935288710823649949940771895617054361149474865046711015101563940680527540071584560878577663743040086340742855278549092581)
print ""
print "=========case 2=========";
factor(648455842808071669662824265346772278726343720706976263060439070378797308618081116462714015276061417569195587321840254520655424906719892428844841839353281972988531310511738648965962582821502504990264452100885281673303711142296421027840289307657458645233683357077834689715838646088239640236866252211790085787877)
