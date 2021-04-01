#include"des.h"
#include<stdio.h>
/*
    W -> 10101110 -> 0xae
    H -> 10010001 -> 0x91
    I -> 10010010 -> 0x92
    T -> 10101000 -> 0xa8
    E -> 10001010 -> 0x8a
    A -> 10000010 -> 0x82
    B -> 10000101 -> 0x85
    L -> 10011000 -> 0x98
    C -> 10000110 -> 0x86
    K -> 10010111 -> 0x97
    result:
    L1=FF899955
    R1=D660AD45
*/
int main()
{
    unsigned long code;
    /*plain text = WHITEHAT = 0xae9192a88a9182a8*/
    unsigned long plain_text=0xae9192a88a9182a8;
    /*code = BLACKHAT = 0x85988286979182a8*/
    unsigned long key=0x85988286979182a8;
    Encode(&plain_text,&key,&code);
    printf("code=%llX\n",code);
    unsigned long plain_text_out;
    Decode(&code,&key,&plain_text_out);
    printf("text=%llx\n",plain_text_out);
    return 0;
}