#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED

#define NUM 1000
extern int nextstate;
extern int nt;
typedef int Status;

typedef struct
{
    int place;
    int codebegin;
    int truechain;
    int falsechain;
    int chain;
    int symbol;         //针对赋值语句中A->i=D;中的D是由归约得到还是直接赋值得到，归约为0，直接赋值为1
    char type;          //某符号的类型为int还是float
}attribute; //属性

extern attribute node_attri[40];    //暂时存放每个归约产生的右部符号的属性
extern attribute nodes_attri[NUM];  //存放该源程序每个结点的属性
extern int length;
extern int length1;
extern int length2;
extern char newtem[5][10];          //储存临时变量的字符数组

//四元式的结构体
typedef struct
{
    int order;              //序号
    char one[5];             //元素1
    char two[5];           //元素2
    char three[5];           //元素3
    char four[5];         //元素4
}Qua;
extern Qua quate[NUM];      //储存四元式的结构体数组

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


void InitAttri(attribute &a);
void BackPatch(int p,int t);
int Merge(int p1,int p2);
void CreatQua(int l,char o[],char a1[],char a2[],char r[]);
void PrintQua();
char* NewTemp();
void Program(int index,attribute &attri,attribute node_attri[]);

#endif // SEMANTIC_H_INCLUDED
