#include"sub_key_gen.h"

/*一些用到的常数*/
unsigned long max_one = 0x8000000000000000;
unsigned long min_one = 1;
unsigned int max_one_32 = 0x80000000;
unsigned int min_one_32 = 1;
unsigned char char_min_one = 0b00000001;
unsigned char char_max_one = 0b10000000;


int init_ip_table[64]={
    //初始置换ip
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};

int select_expand_table[48]={
    //选择扩展运算表
    32,1,2,3,4,5,
    4,5,6,7,8,9,
    8,9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32,1
};

int replace_table[32]={
    //置换运算
    16,7,20,21,29,12,28,17,
    1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,
    19,13,30,6,22,11,4,25
};

unsigned char select_compress_table[8][64]={//选择压缩表
    {
        0xe,0x0,0x4,0xf,0xd,0x7,0x1,0x4,0x2,0xe,0xf,0x2,0xb,0xd,0xb,0xe,
        0x3,0xa,0xa,0x6,0x6,0xc,0xc,0xb,0x5,0x9,0x9,0x5,0x0,0x3,0x7,0x8,
        0x4,0xf,0x1,0xc,0xe,0x8,0x8,0x2,0xd,0x4,0x6,0x9,0x2,0x1,0xb,0x7,
        0xf,0x5,0xc,0xb,0x9,0x3,0x7,0xe,0x3,0xa,0xa,0x0,0x5,0x6,0x0,0xd
    },
    {
        0xf,0x3,0x1,0xd,0x8,0x4,0xe,0x7,0x6,0xf,0xb,0x2,0x3,0x8,0x4,0xf,
        0x9,0xc,0x7,0x0,0x2,0x1,0xd,0xa,0xc,0x6,0x0,0x9,0x5,0xb,0xa,0x5,
        0x0,0xd,0xe,0x8,0x7,0xa,0xb,0x1,0xa,0x3,0x4,0xf,0xd,0x4,0x1,0x2,
        0x5,0xb,0x8,0x6,0xc,0x7,0x6,0xc,0x9,0x0,0x3,0x5,0x2,0xe,0xf,0x9
    },
    {
        0xa,0xd,0x0,0x7,0x9,0x0,0xe,0x9,0x6,0x3,0x3,0x4,0xf,0x6,0x5,0xa,
        0x1,0x2,0xd,0x8,0xc,0x5,0x7,0xe,0xb,0xc,0x4,0xb,0x2,0xf,0x8,0x1,
        0xd,0x1,0x6,0xa,0x4,0xd,0x9,0x0,0x8,0x6,0xf,0x9,0x3,0x8,0x0,0x7,
        0xb,0x4,0x1,0xf,0x2,0xe,0xc,0x3,0x5,0xb,0xa,0x5,0xe,0x2,0x7,0xc
    },
    {
        0x7,0xd,0xd,0x8,0xe,0xb,0x3,0x5,0x0,0x6,0x6,0xf,0x9,0x0,0xa,0x3,
        0x1,0x4,0x2,0x7,0x8,0x2,0x5,0xc,0xb,0x1,0xc,0xa,0x4,0xe,0xf,0x9,
        0xa,0x3,0x6,0xf,0x9,0x0,0x0,0x6,0xc,0xa,0xb,0xa,0x7,0xd,0xd,0x8,
        0xf,0x9,0x1,0x4,0x3,0x5,0xe,0xb,0x5,0xc,0x2,0x7,0x8,0x2,0x4,0xe
    },
    {
        0x2,0xe,0xc,0xb,0x4,0x2,0x1,0xc,0x7,0x4,0xa,0x7,0xb,0xd,0x6,0x1,
        0x8,0x5,0x5,0x0,0x3,0xf,0xf,0xa,0xd,0x3,0x0,0x9,0xe,0x8,0x9,0x6,
        0x4,0xb,0x2,0x8,0x1,0xc,0xb,0x7,0xa,0x1,0xd,0xe,0x7,0x2,0x8,0xd,
        0xf,0x6,0x9,0xf,0xc,0x0,0x5,0x9,0x6,0xa,0x3,0x4,0x0,0x5,0xe,0x3
    },
    {
        0xc,0xa,0x1,0xf,0xa,0x4,0xf,0x2,0x9,0x7,0x2,0xc,0x6,0x9,0x8,0x5,
        0x0,0x6,0xd,0x1,0x3,0xd,0x4,0xe,0xe,0x0,0x7,0xb,0x5,0x3,0xb,0x8,
        0x9,0x4,0xe,0x3,0xf,0x2,0x5,0xc,0x2,0x9,0x8,0x5,0xc,0xf,0x3,0xa,
        0x7,0xb,0x0,0xe,0x4,0x1,0xa,0x7,0x1,0x6,0xd,0x0,0xb,0x8,0x6,0xd
    },
    {
        0x4,0xd,0xb,0x0,0x2,0xb,0xe,0x7,0xf,0x4,0x0,0x9,0x8,0x1,0xd,0xa,
        0x3,0xe,0xc,0x3,0x9,0x5,0x7,0xc,0x5,0x2,0xa,0xf,0x6,0x8,0x1,0x6,
        0x1,0x6,0x4,0xb,0xb,0xd,0xd,0x8,0xc,0x1,0x3,0x4,0x7,0xa,0xe,0x7,
        0xa,0x9,0xf,0x5,0x6,0x0,0x8,0xf,0x0,0xe,0x5,0x2,0x9,0x3,0x2,0xc
    },
    {
        0xd,0x1,0x2,0xf,0x8,0xd,0x4,0x8,0x6,0xa,0xf,0x3,0xb,0x7,0x1,0x4,
        0xa,0xc,0x9,0x5,0x3,0x6,0xe,0xb,0x5,0x0,0x0,0xe,0xc,0x9,0x7,0x2,
        0x7,0x2,0xb,0x1,0x4,0xe,0x1,0x7,0x9,0x4,0xc,0xa,0xe,0x8,0x2,0xd,
        0x0,0xf,0x6,0xc,0xa,0x9,0xd,0x0,0xf,0x3,0x3,0x5,0x5,0x6,0x8,0xb
    }
};

int inverse_ip_table[64]={
    //逆初始置换
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25
};


void init_replace(unsigned long* plain_text, unsigned int* L0,unsigned int* R0);//初始置换
void select_expand(unsigned long*out,unsigned int*in);//选择扩展
void key_add(unsigned long* in_result,unsigned long*key);//密钥加运算
void select_cpomress(unsigned long* in , unsigned int *out);//48bit输入32bit输出，选择压缩
void replace(unsigned int *out,unsigned int * in);//置换运算
void init_inverse_replace(unsigned long*out,unsigned int*L0,unsigned int*R0);//逆初始置换
void Encode(unsigned long* plain_text,unsigned long*init_key,unsigned long*code_out);//加密函数
void Decode(unsigned long* code_int,unsigned long*init_key,unsigned long*plain_text);//解密函数


void Decode(unsigned long* code_in,unsigned long*init_key,unsigned long*plain_text)
{
    /*三个参数分别为密文、密钥、译出来的明文*/
    /*生成子密钥*/
    unsigned long sub_key[16];
    gen_sub_key(init_key,sub_key);
    /*
        解密
    */
    unsigned int left,right,last_right;
    unsigned long select_expand_out=0;
    unsigned int select_compress_out=0;
    unsigned int replace_out=0;
    //初始置换
    init_replace(code_in,&left,&right);
    //16轮迭代
    for(int i=0;i<16;i++)
    {
        last_right=right;
        //选择扩展
        select_expand(&select_expand_out,&right);
        //密钥加
        key_add(&select_expand_out,&sub_key[15-i]);
        //选择压缩
        select_cpomress(&select_expand_out,&select_compress_out);
        //置换运算
        replace(&replace_out,&select_compress_out);
        right = replace_out ^ left;
        left = last_right;
    }
    //初始逆置换
    init_inverse_replace(plain_text,&right,&left);
}




void Encode(unsigned long* plain_text,unsigned long*init_key,unsigned long*code_out)
{
    /*三个参数分别为明文、密钥、加密后的结果*/
    /*
        生成子密钥
    */
    unsigned long sub_key[16];
    gen_sub_key(init_key,sub_key);
    /*
        加密
    */
    unsigned int left,right,last_right;
    unsigned long select_expand_out=0;
    unsigned int select_compress_out=0;
    unsigned int replace_out=0;
    //初始置换
    init_replace(plain_text,&left,&right);
    //16轮迭代
    for(int i=0;i<16;i++)
    {
        last_right=right;
        //选择扩展
        select_expand(&select_expand_out,&right);
        //密钥加
        key_add(&select_expand_out,&sub_key[i]);
        //选择压缩
        select_cpomress(&select_expand_out,&select_compress_out);
        //置换运算
        replace(&replace_out,&select_compress_out);
        right = replace_out ^ left;
        left = last_right;
        if(i==0)
        {
            printf("L1=%08X\nR1=%08X\n",left,right);
        }
    }
    //初始逆置换
    init_inverse_replace(code_out,&right,&left);
}




void init_replace(unsigned long* plain_text, unsigned int* L0,unsigned int* R0)
{
    unsigned long temp;   
    for(int i=0;i<32;i++)
    {
        temp = (max_one >> (init_ip_table[i]-1)) & (*plain_text);
        if(temp!=0)//不为0就说明那一位是1
            *L0 |= (max_one_32 >> i);
        else
            *L0 &= (~(max_one_32 >> i));
    }
    for(int i=0;i<32;i++)
    {
        temp = (max_one >> (init_ip_table[i+32]-1)) & (*plain_text);
        if(temp!=0)
            *R0 |= (max_one_32 >> i);
        else
            *R0 &= (~(max_one_32 >> i));
    }
}



void select_expand(unsigned long*out,unsigned int*in)
{
    //out应为48bit但是只能用64,in是32bit
    unsigned int temp;
    for(int i=0;i<48;i++)
    {
        temp = (max_one_32>>(select_expand_table[i]-1)) & (*in);
        //temp = (unsigned int)(min_one << (32-select_expand_table[i])) & (*in);
        if(temp!=0)
            *out |= (max_one >> (16+i));
        else 
            *out &= ~(max_one >> (16+i));
    }
}

void key_add(unsigned long* in_result,unsigned long*key)
{
    unsigned long temp=*in_result;
    *in_result = temp ^ *key;
}





void select_cpomress(unsigned long* in , unsigned int *out)
{
    //先把输入视为8个6位二进制块，这里视为8个8进制
    unsigned char a,b,c,d,e,f,g,h;
    unsigned long temp=0;
    a=0;b=0;c=0;d=0;e=0;f=0;g=0;h=0;
    for(int i=0;i<48;i++)
    {
        if(i>=0&&i<=5)//a
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                a |= (char_max_one>>(2+i));
            else
                a &= ~(char_max_one>>(2+i));
            continue;
        }
        if(i>=6&&i<=11)//b
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                b |= (char_max_one>>(2+i-6));
            else
                b &= ~(char_max_one>>(2+i-6));
            continue;
        }
        if(i>=12&&i<=17)//c
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                c |= (char_max_one>>(2+i-12));
            else
                c &= ~(char_max_one>>(2+i-12));
            continue;
        }
        if(i>=18&&i<=23)//d
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                d |= (char_max_one>>(2+i-18));
            else
                d &= ~(char_max_one>>(2+i-18));
            continue;
        }
        if(i>=24&&i<=29)//e
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                e |= (char_max_one>>(2+i-24));
            else
                e &= ~(char_max_one>>(2+i-24));
            continue;
        }
        if(i>=30&&i<=35)//f
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                f |= (char_max_one>>(2+i-30));
            else
                f &= ~(char_max_one>>(2+i-30));
            continue;
        }
        if(i>=36&&i<=41)//g
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                g |= (char_max_one>>(2+i-36));
            else
                g &= ~(char_max_one>>(2+i-36));
            continue;
        }
        if(i>=42&&i<=47)//h
        {
            temp = (max_one >> (16+i))&(*in);
            if(temp!=0)
                h |= (char_max_one>>(2+i-42));
            else
                h &= ~(char_max_one>>(2+i-42));
            continue;
        }
    }
    a = select_compress_table[0][a];
    b = select_compress_table[1][b];
    c = select_compress_table[2][c];
    d = select_compress_table[3][d];
    e = select_compress_table[4][e];
    f = select_compress_table[5][f];
    g = select_compress_table[6][g];
    h = select_compress_table[7][h];
    //把abcdefgh塞入out
    unsigned char temp_=0;
    for(int i=0;i<32;i++)
    {
        if(i>=0&&i<=3)//a
        {
            temp_ = (char_max_one >> (4+i))&a;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=4&&i<=7)//b
        {
            temp_ = (char_max_one >> (4+i-4))&b;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=8&&i<=11)//c
        {
            temp_ = (char_max_one >> (4+i-8))&c;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=12&&i<=15)//d
        {
            temp_ = (char_max_one >> (4+i-12))&d;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=16&&i<=19)//e
        {
            temp_ = (char_max_one >> (4+i-16))&e;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=20&&i<=23)//f
        {
            temp_ = (char_max_one >> (4+i-20))&f;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=24&&i<=27)//g
        {
            temp_ = (char_max_one >> (4+i-24))&g;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
        if(i>=28&&i<=31)//h
        {
            temp_ = (char_max_one >> (4+i-28))&h;
            if(temp_!=0)
                *out |= (max_one_32 >> i);
            else
                *out &= ~(max_one_32 >> i);
            continue;
        }
    }
}




void replace(unsigned int *out,unsigned int * in)
{
    unsigned int temp = 0;
    for(int i=0;i<32;i++)
    {
        temp = (max_one_32 >> (replace_table[i]-1))&(*in);
        if(temp!=0)
            *out |= (max_one_32>>i);
        else 
            *out &= ~(max_one_32>>i);
    }
}




void init_inverse_replace(unsigned long*out,unsigned int*L0,unsigned int*R0)
{
    unsigned long res=0;//存放合并以后的数
    unsigned long t=*L0;
    res+=*R0;
    res+=(t<<32);
    unsigned long temp=0;
    for(int i=0;i<64;i++)
    {
        temp = (max_one >> (inverse_ip_table[i]-1))&res;
        if(temp!=0)
            *out |= max_one >> i;
        else 
            *out &= ~(max_one >> i);
    }
}