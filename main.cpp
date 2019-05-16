#include<stdio.h>
#include<assert.h>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
char * binary(unsigned char c)
{
    static char buf[9];
    buf[8]=0;
    unsigned int num=c;
    for(int i=0;i<8;i++)
    {
        if(num%2==0) buf[8-i-1]='0';
        else buf[8-i-1]='1';
        num=num/2;
    }
    return buf;
}
char get_key(vector<string> &vs,int index)
{
    int cnt0=0;
    int cnt1=0;
    int found0=-1;
    int found1=-1;
    unsigned char last_c=vs[vs.size()-1][index];
    printf("\n");
    for(int i=0;i<(int)vs.size();i++)
    {
        unsigned char c=vs[i][index];
        printf("<%02x:%s>\n",(unsigned int)(c),binary(c));
        //printf("<%02x:%d>",(unsigned int)(c),(unsigned int)(c&0x20));
        if((c&0x20)==0)
            cnt0++,found0=i;
        else
            cnt1++,found1=i;

    }
    unsigned char ret=0;
    int ok=0;
    if(cnt0==1)
    {
        ok=1;
        ret=(unsigned char)(last_c^(unsigned char)vs[found0][index]^(unsigned char)0x20);
    } else if(cnt1==1)
    {
        ok=1;
        ret=(unsigned char)(last_c^(unsigned char)vs[found1][index]^(unsigned char)0x20);
    }
    if(ok)
        printf("<<%c,%02x,%s>>",ret,(unsigned int)ret,binary(ret));
    printf("<%d %d>\n",cnt0,cnt1);
}
int main()
{
    char buf[1000];
    freopen("/home/wangyu/Desktop/1.txt","r",stdin);
    vector<string> vs0;
    while(scanf("%s",buf)==1)
    {
        vs0.push_back(buf);
    }
    assert(vs0.size()==11);
    vector<string> vs;
    for(int i=0;i<vs0.size();i++)
    {
        string tmp;
        assert(vs0[i].size()%2==0);
        for(int j=0;j<vs0[i].length();j+=2)
        {
            char buf2[3];
            buf[2]=0;
            unsigned int num;
            buf2[0]=vs0[i][j];
            buf2[1]=vs0[i][j+1];
            sscanf(buf2,"%x",&num);
            //printf("<%x>",num);
            tmp.push_back((unsigned char) num);
        }
        vs.push_back(tmp);
    }
    int min_len=vs[10].size();
    for(int i=0;i<min_len;i++)
    {
        printf("====<%d:>====",i);
        get_key(vs,i);
    }

    return 0;
}