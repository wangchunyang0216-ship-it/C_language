#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include "grammar.h"
#define STACK_INIT_SIZE 100     //储存空间的初始分配量
#define STACKINCREMENT 10       //储存空间分配增量
using namespace std;
#include<iomanip>//对齐方式

#include <sstream>


Status InitStack_state(SqStack_s &s)        //构造一个空栈
{
    s.base = (SElemType_s *)malloc(STACK_INIT_SIZE*sizeof(SElemType_s));
    if (!s.base)   exit(OVERFLOW);              //空间分配失败，程序结束
    s.top = s.base;
    s.stacksize_s = STACK_INIT_SIZE;
    return OK;
}

Status Push_state(SqStack_s &s,SElemType_s e)   //插入元素e为新的栈顶
{
    if (s.top - s.base>=s.stacksize_s)    //栈满，追加空间
    {
        s.base = (SElemType_s *)realloc(s.base, (s.stacksize_s + STACKINCREMENT)*sizeof(SElemType_s));
        if (!s.base)   exit(OVERFLOW);          //空间分配失败，程序结束
        s.top = s.base + s.stacksize_s;
        s.stacksize_s += STACKINCREMENT;
    }
    *s.top++ = e;
    return OK;
}

Status Pop_state(SqStack_s &s,SElemType_s &e)   //若栈不空，栈顶元素出栈
{
    if (s.top == s.base)   return ERROR;
    e = * --s.top;
    return OK;
}

Status StackEmpty_state(SqStack_s s)
{
    if (s.base == s.top)   return OK;
    else    return ERROR;
}

Status GetTop_state(SqStack_s s)
{
    return *(s.top-1);
}

Status InitStack_char(SqStack_c &s)      //构造一个空栈
{
    s.base = (SElemType_c *)malloc(STACK_INIT_SIZE*sizeof(SElemType_c));
    if (!s.base)   exit(OVERFLOW);          //空间分配失败，程序结束
    s.top = s.base;
    s.stacksize_c = STACK_INIT_SIZE;
    return OK;
}

Status Push_char(SqStack_c &s,SElemType_c e)  //插入元素e为新的栈顶
{
    if (s.top - s.base>=s.stacksize_c)           //栈满，追加空间
    {
        s.base = (SElemType_c *)realloc(s.base, (s.stacksize_c + STACKINCREMENT)*sizeof(SElemType_c));
        if (!s.base)   exit(OVERFLOW);              //储存空间分配失败
        s.top = s.base + s.stacksize_c;
        s.stacksize_c += STACKINCREMENT;
    }
    *s.top++ = e;
    return OK;
}

Status Pop_char(SqStack_c &s,SElemType_c &e)  //若栈不空，栈顶元素出栈
{
    if (s.top == s.base)   return ERROR;
    e = * --s.top;
    return OK;
}

Status StackEmpty_char(SqStack_c s)
{
    if (s.base == s.top)   return OK;
    else    return ERROR;
}

char GetTop_char(SqStack_c s)
{
    return *(s.top-1);
}

Status PrintStack_char(SqStack_c s)
{
     string r1;
    stringstream ss;

    int i;
    for (i=0; i<s.top-s.base; i++)
    {
        //cout<<*(s.base+i)<<" ";//原来的原来
        //random2[i]=*(s.base+i);
         char xx=*(s.base+i);
        ss << xx;
        r1=ss.str();

    }
    //r2=random2;

    cout<<setw(30)<<r1<<" ";
    r1.clear();

    cout<<"\t\t";

    //cout<<endl;//原来
    return OK;
}

Status PrintStack_state(SqStack_s s)
{
    //int random1[NUM];
    string r1;
    stringstream ss;

    int i;
    for (i=0; i<s.top-s.base; i++)
    {
        //cout<<*(s.base+i)<<" ";//原来
       // random1[i]=*(s.base+i);
       int xx=*(s.base+i);
        ss << xx;
        r1=ss.str();

    }

    //string r1=string(random1);
    cout<<setw(30)<<r1<<" ";
    r1.clear();


   // cout<<"\t\t\t\t";//原来
    cout<<"\t\t";

    return OK;
}
