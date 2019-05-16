#include<stdio.h>
#include<assert.h>
#include<vector>
#include<string>
#include<algorithm>
#include<map>
#include<set>
using namespace std;
typedef unsigned char u8;
typedef unsigned int u32;


const int msg_num=11;

long cal_weight(unsigned char c)
{
    if(c==' ') return 100000;
    if(c>='a'&&c<='z') return 1000;
    if(c>='A'&&c<='Z') return 1000;
    if(c>='0'&&c<='9') return 500;
    if(c=='.'||c==',') return 100;
    if(c>127) return -100000;
    return -1000;
}
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
struct result_t
{
    long weight;
    u8 key;
    u8 c;
    string all;
};
bool cmp(const result_t &a,const result_t &b)
{
    return a.weight>b.weight;
}
char get_key(vector<string> &vs,int index)
{
    int cnt0=0;
    int cnt1=0;
    int found0=-1;
    int found1=-1;
    unsigned char last_c=vs[vs.size()-1][index];
    for(int i=0;i<(int)vs.size();i++)
    {
        unsigned char c=vs[i][index];
        printf("<%02x:%s>",(u8)(c),binary(c));
    }
    printf("\n");
    string &target=vs[msg_num-1];
    vector<result_t> result_list;
    for(int i=0;i<=255;i++)
    {
        result_t result;
        long weight=0;
        u8 key=u8(i);
        for(int j=0;j<msg_num;j++)
        {
            string & text=vs[j];
            weight+=cal_weight(text[index]^key);
            result.all+=text[index]^key;
        }
        result.weight=weight;
        result.c=target[index]^key;
        result.key=key;
        result_list.push_back(result);
        //if(weight>=0)
        //printf("<%02x,%ld,%c>",(u32)key,weight,target[index]^key);
    }
    sort(result_list.begin(),result_list.end(),cmp);
    for(int i=0;i<result_list.size()&&i<10;i++)
    {
        printf("<%c,%02x,%ld,%s>",result_list[i].c,(u32)result_list[i].key,result_list[i].weight,result_list[i].all.c_str());
    }

    printf("\n");
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
    assert(vs0.size()==msg_num);
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
        printf("<%02d:>",i);
        get_key(vs,i);
    }

    return 0;
}