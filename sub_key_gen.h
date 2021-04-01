extern unsigned long max_one;
extern unsigned int max_one_32;
extern unsigned int min_one_32;
int key_replace_table_1[28]={
    //密钥置换选择PC-1
    57,49,41,33,25,17,9,1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,19,11,3,60,50,44,36  //50改成52
};
int key_replace_table_2[28]={
    //密钥置换选择PC-2
    63,55,47,39,31,23,15,7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,21,13,5,28,20,12,4
};
int loop_shift_left_table[16]={
    //循环左移表
    1,1,2,2,2,2,2,2,
    1,2,2,2,2,2,2,1
};

int replace_table_pc2[48]={
    //置换选择PC-2
    14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,
    26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,
    51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32
};


void replace_select(unsigned long* init_key,unsigned int*C,unsigned int*D);//置换选择pc-1
void shift_left_once(unsigned int * in);//循环左移1次
void shift_left(unsigned int *in, int N);//循环左移N次
void replace_pc2(unsigned int* a,unsigned int*b,unsigned long*out);//置换选择pc-2
void gen_sub_key(unsigned long*init_key,unsigned long*sub_key);//生成子密钥

void gen_sub_key(unsigned long*init_key,unsigned long*sub_key)
{

    unsigned int C,D;C=D=0;//C,D寄存器
    replace_select(init_key,&C,&D);//置换选择pc1
    for(int i=0;i<16;i++)
    {
        shift_left(&C,loop_shift_left_table[i]);
        shift_left(&D,loop_shift_left_table[i]);
        //置换选择pc-2
        replace_pc2(&C,&D,&sub_key[i]);
    }
}

void replace_select(unsigned long* init_key,unsigned int*C,unsigned int*D)
{
    //pc-1
    unsigned long temp=0;
    for(int i=0;i<28;i++)
    {
        temp = (max_one >> (key_replace_table_1[i]-1))&(*init_key);
        if(temp!=0)
            *C |= (max_one_32 >> (4+i));
        else
            *C &= (~(max_one_32 >> (4+i)));
    }
    //pc-2
    temp=0;
    for(int i=0;i<28;i++)
    {
        temp = (max_one >> (key_replace_table_2[i]-1))&(*init_key);
        if(temp!=0)
            *D |= (max_one_32 >> (4+i));
        else
            *D &= (~(max_one_32 >> (4+i)));
    }
}





void shift_left_once(unsigned int * in)
{
    *in &= 0b00001111111111111111111111111111;
    *in=((*in)<<1);
    //将第29位（从1开始数）移到1
    unsigned int temp=0;
    temp = (max_one_32 >> 3)&(*in);
    if(temp!=0)
        *in |= min_one_32;
    else
        *in &= ~min_one_32;
    //将前四位清零
    *in &= 0b00001111111111111111111111111111;
}

void shift_left(unsigned int *in, int N)
{
    for(int i=0;i<N;i++)
    {
        shift_left_once(in);
    }
}



void replace_pc2(unsigned int* a,unsigned int*b,unsigned long*out)
{
    //printf("a=%08X\nb=%08X\nout=%llX\n",*a,*b,*out);
    *a &= 0b00001111111111111111111111111111;
    *b &= 0b00001111111111111111111111111111;
    unsigned int temp=0;
    for(int i=0;i<48;i++)
    {
        if(replace_table_pc2[i]<=28)
        {
            //在a中
            temp = (max_one_32 >> (4+replace_table_pc2[i]-1))&(*a);
            if(temp!=0)
                *out |= (unsigned long)(max_one>>(16+i));
            else
                *out &= ~(unsigned long)(max_one>>(16+i));
        }
        else//[28,26]在b中
        {
            temp = (max_one_32 >> (4+replace_table_pc2[i]-28-1))&(*b);
            if(temp!=0)
                *out |= (unsigned long)(max_one>>(16+i));
            else
                *out &= ~(unsigned long)(max_one>>(16+i));
        }
    }
    //printf("%llX\n",*out);
}