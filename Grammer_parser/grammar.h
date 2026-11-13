#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#define NUM 4000
#define NUM1 100
#define OVERFLOW -1
#define ERROR 0
#define OK 1
#define STACK_INIT_SIZE 100     //储存空间的初始分配量
#define STACKINCREMENT 10       //储存空间分配增量

using namespace std;
typedef struct
{
    char left;                  //左部的非终结符
    char right[10];             //文法右部的符号
    int length;                 //文法右部的符号长度
    char project[10][10];       //储存产生式项目的二维数组
}RULE;
extern RULE grammar[NUM1];      //储存产生式的结构体数组
//声明为extern形式允许外部调用它
typedef struct
{
    char non_termin;             //非终结符
    int flag;                    //判断该非终结符的First集是否求完，为1为求完
    char Fir[10];                //该非终结符的FIRST集合
    char temp_noend[10];         //右部第一个是非终结符时，将该非终结符暂存入数组
}NON_TER;
extern NON_TER nonter_first[30]; //储存非终结符及其属性的结构体数组

typedef struct
{
    char left_lr1;               //储存LR(1)文法项目的产生式左部
    char project_lr1[10];        //储存LR(1)文法项目的产生式右部
    char fwd_char[10];           //向前搜索符号的集合
}PROJECT;                        //LR(1)文法项目的结构体
typedef struct
{
    int order;                   //状态序号
    int ends;                    //该状态是否可以结束
    int pro_num;                 //该状态的项目个数
    PROJECT closure[NUM1];       //closure闭包集合
}CLOSURE;                        //项目集的结构体
extern CLOSURE state[NUM];      //储存状态的结构体数组

typedef struct
{
    int start_state;            //GO函数中一个状态从start_state这个状态编号经过某个符号
    char pass_char;             //GO函数中经过的字符
}GO;
extern GO go_to[NUM];

typedef struct
{
    char symbol;                //表示操作是移进还是归约，s代表移进，r代表归约
    int mark;                   //表示用哪一条产生式归约或过弧到达的状态
}ACT;                           //表示操作的结构体
extern ACT action_table[NUM][NUM]; //action表
extern ACT go_to_table[NUM][NUM];  //goto表

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


void Table();                                        //保存所有的产生式及其项目集
void GetProject(char product[],int n,int order);    //得到每一条产生式的项目
int FindChar(char c,char lists[]);                  //在一个字符串中找字符，找到返回其位置
void GetNonter(int n);                              //判断非终结符是否能推空，求出First集
void FIRST(char strings[],char *first);             //求一个串的First集
void GetClosure(char l,char pro[],char forwardchar[],int ord);  //输出一个状态的项目集
int FindProject(PROJECT newpro,int n,int ord);      //判断新项目是否已存在在该状态中
void GetState(RULE grammar[]);                      //传入文法及其项目，构造项目集族
int FindState(CLOSURE newstate,int n);              //判断当前所得状态是否与已存在状态有相同
int FindRules(char strings[],char l);               //在文法中匹配产生式的位置
void GetAction(CLOSURE state[],int n);              //构造Action表和Goto表
int FindOrder(int order,int n);                     //判断当前的状态序号是否出现过
void Analysis(char simp_sentence[],ACT action_table[][NUM],ACT go_to_table[][NUM]);  //语法分析

extern int state_num;
extern int number;                  //产生式的数量
extern char terminal[];
extern char non_terminal_l[];
extern char non_terminal[];
extern char terminal_r[][10];
extern int sum[];

extern int random1[];
extern string r1;
extern char random2[];
extern string r2;

#endif // GRAMMAR_H_INCLUDED
