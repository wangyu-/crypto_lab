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

string char_list="!.,;? %#$@";
set<u8> char_set;

const int msg_num=11;

int is_valid(u8 c)
{
    if(char_set.find(c)==char_set.end())
        return 0;
    else return 1;
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
struct result
{
    int weight;
    u8 c;
};
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
    string &target=vs[msg_num-1];
    for(int i=0;i<char_list.size();i++)
    {
        int weight=0;
        u8 key=char_list[i]^target[index];
        for(int j=0;j<msg_num-1;j++)
        {
            string & text=vs[j];
            weight+=is_valid(text[index]^key);
        }
        if(weight>=9)
        printf("<%c,%d>",char_list[i],weight);
    }
    printf("\n");
}
int main()
{
    for(int i=0;i<26;i++)
    {
        char_list += 'a' + i;
        char_list += 'A' + i;
    }
    for(int i=0;i<10;i++)
        char_list += '0'+i;
    for(int i=0;i<char_list.length();i++)
        char_set.insert(char_list[i]);
    printf("<<<%d>>>\n",char_set.size());
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