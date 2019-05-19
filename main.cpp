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
const int top_to_show=20;
long cal_weight_old(unsigned char c)
{
    if(c==' ') return 100000;
    if(c>='a'&&c<='z') return 1000;
    if(c>='A'&&c<='Z') return 1000;
    if(c>='0'&&c<='9') return 500;
    if(c=='.'||c==',') return 100;
    if(c>127) return -100000;
    return -1000;
}
int is_print_able(char c)
{
    string char_list=",.<>;:{}[]~`!@#$%^&*()-=_+'\"";
    if(c==' ') return 1;
    else if(c>='a'&&c<='z') return 1;
    else if(c>='A'&&c<='Z') return 1;
    else if(c>='0'&&c<='9') return 1;
    else if(char_list.find(c)!=string::npos) return 1;
    return 0;
}

long cal_weight(string &s)
{
    int cnt_space=0;
    int cnt_low=0;
    int cnt_up=0;
    int cnt_digit=0;
    int cnt_printable=0;
    int cnt_non_printable=0;
    int cnt_invaild=0;
    for(int i=0;i<s.length();i++)
    {
        u8 c=s[i];
        if(c==' ') cnt_space++;
        else if(c>='a'&&c<='z') cnt_low++;
        else if(c>='A'&&c<='Z') cnt_up++;
        else if(c>='0'&&c<='9') cnt_digit++;
        else if(is_print_able(c)) cnt_printable++;
        else if(c>127) cnt_invaild++;
        else cnt_non_printable++;
    }
    long weight= cnt_invaild*(-1000)+cnt_space*5+cnt_low*5+cnt_up*5+cnt_digit*(1)+cnt_printable*0 +cnt_non_printable*(-100);
    //if(cnt_space >msg_num/3) weight-=cnt_space*1000;
    return weight;
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
vector<string> final_result(top_to_show);
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
            //weight+=cal_weight(text[index]^key);
            result.all+=text[index]^key;
        }
        result.weight=cal_weight(result.all);
        result.c=target[index]^key;
        result.key=key;
        result_list.push_back(result);
    }
    sort(result_list.begin(),result_list.end(),cmp);
    for(int i=0;i<result_list.size()&&i<top_to_show;i++)
    {
        for(int j=0;j<result_list[i].all.length();j++)
        {
                if(!is_print_able(result_list[i].all[j])) result_list[i].all[j]='*';
        }

        printf("<[%c],k:%02x,w:%ld,%s>",result_list[i].c,(u32)result_list[i].key,result_list[i].weight,result_list[i].all.c_str());
        if(is_print_able(result_list[i].c))
            final_result[i]+=result_list[i].c;
        else
            final_result[i]+='*';
    }

    printf("\n");
}
string target_msg_after_correction="The secret message is: When using a stream cipher, never use the key more than once";
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
    for(int i=0;i<msg_num;i++)
    {
        assert(vs0[i].length() % 2 == 0);
        assert(vs0[i].length()>=vs0[msg_num-1].length());
    }
    vector<string> vs;
    for(int i=0;i<vs0.size();i++)
    {
        string tmp;
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
    int min_len=vs[msg_num-1].length();
    for(int i=0;i<min_len;i++)
    {
        printf("<%02d:>",i);
        get_key(vs,i);
    }
    printf("\n");
    printf("01234567890123456789012345678901234567890123456789012345678901234567890123456789012\n");
    printf("============================top results============================================\n");
    for(int i=0;i<top_to_show;i++)
        printf("%s\n",final_result[i].c_str());

    printf("============================key====================================================\n");
    string key=target_msg_after_correction;
    for(int i=0;i<key.length();i++)
    {
        key[i]^=vs[msg_num-1][i];
        printf("%02x",(u32)(u8)key[i]);
    }
    printf("\n");

    printf("============================all messages============================================\n");
    for(int i=0;i<msg_num;i++)
    {
        for(int j=0;j<key.length();j++)
        {
            vs[i][j]^=key[j];
            if(!is_print_able(vs[i][j]))
            {
                //printf("<%02x>",(u32)(u8)vs[i][j]);
                //vs[i][j]='*';
            }

        }
        vs[i]=vs[i].substr(0,min_len);
        printf("%s#\n",vs[i].c_str());
    }


    return 0;
}
