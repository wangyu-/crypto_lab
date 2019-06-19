#include <stdio.h>
#include "lib/aes.h"
#include <string>
#include <assert.h>
using namespace std;

typedef unsigned char u8_t;
typedef unsigned int u32_t;

string hex_cbc_key="140b41b22a29beb4061bda66b6747e14";
string hex_cbc_t1="4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
string hex_cbc_t2="5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253";

string hex_ctr_key="36f18357be4dbd77f050515c73fcf9f2";
string hex_ctr_t1="69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329";
string hex_ctr_t2="770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451";

string hex_to_char(const string &in)
{
    string res;
    assert(in.length()%2==0);
    for(int i=0;i<in.length();i+=2)
    {
        char buf2[3];
        buf2[2]=0;
        unsigned int num;
        buf2[0]=in[i];
        buf2[1]=in[i+1];
        sscanf(buf2,"%x",&num);
        res.push_back((unsigned char) num);
    }
    return res;
}
void prt(const string &a)
{
    for(int i=0;i<a.length();i++)
    {
        printf("<%02x>",(u32_t)(u8_t)a[i]);
    }
    printf("\n");
}

int aes_crypt_ctr( unsigned char *key,unsigned char *nonce_counter,size_t length,
                   const unsigned char *input,
                   unsigned char *output )
{

    aes_context ctx;
    aes_init( &ctx);
    aes_setkey_enc(&ctx,key,128);

    int c, i;
    size_t n = 0;

    unsigned char stream_block[16];
    memset(stream_block,0,sizeof(stream_block));

    while( length-- )
    {
        if( n == 0 ) {
            aes_crypt_ecb( &ctx, AES_ENCRYPT, nonce_counter, stream_block );
            for( i = 16; i > 0; i-- )
                if( ++nonce_counter[i - 1] != 0 )
                    break;
        }
        c = *input++;
        *output++ = (unsigned char)( c ^ stream_block[n] );
        n = ( n + 1 ) & 0x0F;
    }
    return( 0 );
}

string unpad(string &input)
{
    string res=input;
    assert(!res.empty());
    int cnt=(u8_t)res[res.length()-1];
    while(cnt--)
    {
        assert(!res.empty());
        res.pop_back();
    }
    return res;
}
string pad(string &input)
{
    string res=input;
    int pad_len;
    if(input.length()%16==0) pad_len=16;
    else pad_len=16-input.length()%16;
    u8_t c=pad_len;
    while(pad_len--)
        res.push_back(c);
    return res;
}

int aes_encrypt_cbc(
        unsigned char key[16],
                   unsigned char iv[16],
                     size_t length,
                   const unsigned char *input,
                   unsigned char *output )
{
    aes_context ctx;
    aes_init( &ctx);
    aes_setkey_enc(&ctx,key,128);

    int i;
    unsigned char temp[16];

    assert(length%16==0);

    while( length > 0 )
    {
        for( i = 0; i < 16; i++ )
            output[i] = (unsigned char)( input[i] ^ iv[i] );

        aes_crypt_ecb( &ctx, AES_ENCRYPT, output, output );
        memcpy( iv, output, 16 );

        input  += 16;
        output += 16;
        length -= 16;
    }

    return 0;
}

int aes_decrypt_cbc(
        unsigned char key[16],
        unsigned char iv[16],
        size_t length,
        const unsigned char *input,
        unsigned char *output )
{
    aes_context ctx;
    aes_init( &ctx);
    aes_setkey_dec(&ctx,key,128);

    int i;
    unsigned char temp[16];

    assert(length%16==0);

    while( length > 0 )
    {
        memcpy( temp, input, 16 );
        aes_crypt_ecb( &ctx, AES_DECRYPT, input, output );

        for( i = 0; i < 16; i++ )
            output[i] = (unsigned char)( output[i] ^ iv[i] );

        memcpy( iv, temp, 16 );

        input  += 16;
        output += 16;
        length -= 16;
    }

    return 0;
}

int main()
{
    {
        printf("=====ctr1======\n");

        string ctr_key = hex_to_char(hex_ctr_key);
        string input = hex_to_char(hex_ctr_t1);
        char buf[1000] = {0};
        //prt(input);
        aes_crypt_ctr((u8_t *) ctr_key.c_str(), (u8_t *) input.c_str(), input.length() - 16,
                      (u8_t *) input.c_str() + 16, (u8_t *) buf);
        printf("%s\n", buf);

        string output=buf;
        string counter= hex_to_char(hex_ctr_t1).substr(0,16);
        aes_crypt_ctr((u8_t *) ctr_key.c_str(), (u8_t *) counter.c_str(), output.length(),
                      (u8_t *) output.c_str(), (u8_t *) buf);
        //memset(buf,0,sizeof(buf));
        counter= hex_to_char(hex_ctr_t1).substr(0,16);
        string tmp(buf,buf+output.length());
        output=counter+tmp;
        prt(hex_to_char(hex_ctr_t1));
        prt(output);
    }

    {
        printf("=====ctr2======\n");

        string ctr_key = hex_to_char(hex_ctr_key);
        string input = hex_to_char(hex_ctr_t2);
        char output[1000] = {0};
        //prt(input);
        aes_crypt_ctr((u8_t *) ctr_key.c_str(), (u8_t *) input.c_str(), input.length() - 16,
                      (u8_t *) input.c_str() + 16, (u8_t *) output);
        printf("%s\n", output);
    }

    {
        printf("=====cbc1======\n");

        string ctr_key = hex_to_char(hex_cbc_key);
        string input = hex_to_char(hex_cbc_t1);
        char output[1000] = {0};
        //prt(input);
        aes_decrypt_cbc((u8_t *) ctr_key.c_str(), (u8_t *) input.c_str(), input.length() - 16,
                      (u8_t *) input.c_str() + 16, (u8_t *) output);
        string tmp=output;
        tmp=unpad(tmp);
        printf("%s\n", tmp.c_str());
    }

    {
        printf("=====cbc2======\n");

        string ctr_key = hex_to_char(hex_cbc_key);
        string input = hex_to_char(hex_cbc_t2);
        char buf[1000] = {0};
        //prt(input);
        aes_decrypt_cbc((u8_t *) ctr_key.c_str(), (u8_t *) input.c_str(), input.length() - 16,
                        (u8_t *) input.c_str() + 16, (u8_t *) buf);
        string tmp=buf;
        //string tmp2=tmp;
        string res=unpad(tmp);
        printf("%s\n", res.c_str());

        input = hex_to_char(hex_cbc_t2);
        prt(input);

        string tmp2=pad(res);
        memset(buf,0,sizeof(buf));
        aes_encrypt_cbc((u8_t *) ctr_key.c_str(), (u8_t *) input.c_str(), tmp2.length(),
                        (u8_t *) tmp2.c_str(), (u8_t *) buf);
        input = hex_to_char(hex_cbc_t2);
        string res2(buf,buf+tmp2.length());
        res2=input.substr(0,16)+res2;
        //printf("<%d>\n",res2.length());
        prt(res2);
    }
    return 0;
}
