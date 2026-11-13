#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#define NUM1 100
#define NUM2 4000
#define OVERFLOW -1
#define ERROR 0
#define OK 1
#define STACK_INIT_SIZE 100     //储存空间的初始分配量
#define STACKINCREMENT 10       //储存空间分配增量

//状态栈和文法符号栈
typedef int Status;
typedef int SElemType_s;
typedef struct
{
    SElemType_s *base;      //栈底指针
    SElemType_s *top;       //栈顶指针
    int stacksize_s;        //当前已分配的储存空间，以元素为单位
}SqStack_s;
Status InitStack_state(SqStack_s &s);
Status Push_state(SqStack_s &s,SElemType_s e);
Status Pop_state(SqStack_s &s,SElemType_s &e);
Status StackEmpty_state(SqStack_s s);
Status GetTop_state(SqStack_s s);
Status PrintStack_state(SqStack_s s);
Status GetLength_state(SqStack_s s);

typedef char SElemType_c;
typedef struct
{
    SElemType_c *base;       //栈底指针
    SElemType_c *top;        //栈顶指针
    int stacksize_c;         //当前已分配的储存空间，以元素为单位
}SqStack_c;
Status InitStack_char(SqStack_c &s);
Status Push_char(SqStack_c &s,SElemType_c e);
Status Pop_char(SqStack_c &s,SElemType_c &e);
Status StackEmpty_char(SqStack_c s);
char GetTop_char(SqStack_c s);
Status PrintStack_char(SqStack_c s);
Status GetLength_char(SqStack_c s);



//语义栈
typedef attribute SElemType;
typedef struct
{
    SElemType *base;        //栈底指针
    SElemType *top;         //栈顶指针
    int stacksize;          //当前已分配的储存空间，以元素为单位
}SqStack;
Status InitStack(SqStack &s);
Status Push(SqStack &s,SElemType e);
Status Pop(SqStack &s,SElemType &e);
Status StackEmpty(SqStack s);
Status GetTop(SqStack s);
Status PrintStack_semantic(SqStack s);
Status PrintStack_table();
#endif // STACK_H_INCLUDED

