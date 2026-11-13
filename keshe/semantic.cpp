#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
using namespace std;
//#include "morphology.h"
//#include "grammar.h"
//#include "semantic.h"


#include"keshe.h"
#include"stack.h"

char jump[5]= {'j','u','m','p','\0'};
char duanhengxian[2]= {'-','\0'};  //代表空
char zero[2]= {'0','\0'};
char denghao[2]= {'=','\0'};
//char add[3]= {'+','n','\0'};
//char mul[3]= {'*','n','\0'};
//char add1[3]= {'+','t','\0'};
//char mul1[3]= {'*','t','\0'};
char add[3]= {'+','\0'};
char mul[3]= {'*','\0'};
char add1[3]= {'+','\0'};
char mul1[3]= {'*','\0'};
char sub[2]= {'-','\0'};

Qua quate[NUM];             //储存四元式的结构体数组
int nextstate=1;
int nt=0;
attribute node_attri[40];
attribute nodes_attri[NUM]; //存放每个结点的属性
int length=0;
int length1=0;
char newtem[5][10];         //存放临时变量
int length2=0;

//初始化属性值
void InitAttri(attribute &a)
{
    a.chain=0;
    a.codebegin=0;
    a.falsechain=0;
    a.place=0;
    a.truechain=0;
    a.type='$';
    a.symbol=-1;
}

//拉链回填
void BackPatch(int p,int t)
{
    unsigned int i;
    char p1[5];
    char t1[5];
    sprintf(p1,"%d",p);
    while(strcmp(p1,zero)!=0)
    {
        sprintf(t1,"%d",t);
        for (i=0; i<=strlen(quate[p].four); i++)
            p1[i]=quate[p].four[i];
        for (i=0; i<=strlen(t1); i++)
            quate[p].four[i]=t1[i];
    }
}

//合并两条链
int Merge(int p1,int p2)
{
    unsigned int i;
    char p3[5];
    if (p2!=0)
    {
        sprintf(p3,"%d",p1);
        for (i=0; i<=strlen(p3); i++)
            quate[p2].four[i]=p3[i];
        return p2;
    }
    else
        return p1;
}

//产生临时变量，顺序为t1,t2......
char* NewTemp()
{
    char*p;
    int i=0;
    char m[8];
    p=(char*)malloc(8);
    //开辟八个字节空间  char *p=(char *)malloc(8*sizeof(char));
//p就指向一个有8个连续空间的首地址，p+1就是第二个空间的地址。简单总结就是申请空间，有多少空间就有多少地址。

    nt++;//nt在最上方初始化为0
    sprintf(m,"%d",nt);     //将整型转换成字符串
    strcpy(p+1,m);
    p[0]='t';
    while(p[i]!='\0')
        i++;
    return p; //p是返回一个代表t1、t2、t3、t4的字符串
}

//产生四元式
void CreatQua(int l,char o[],char a1[],char a2[],char r[])
{
    unsigned int i;
    quate[nextstate].order=nextstate;
    for (i=0; i<=strlen(o); i++)
        quate[nextstate].one[i]=o[i];
    for (i=0; i<=strlen(a1); i++)
        quate[nextstate].two[i]=a1[i];
    for (i=0; i<=strlen(a2); i++)
        quate[nextstate].three[i]=a2[i];
    for (i=0; i<=strlen(r); i++)
        quate[nextstate].four[i]=r[i];
}

//输出优化前的四元式
void PrintQua()
{
    int i;
    cout<<endl<<"语义制导后生成的四元式如下："<<endl;
    for (i=1; i<nextstate; i++)
    {
        cout<<"\t"<<i<<'.'<<'('<<quate[i].one;
        cout<<','<<quate[i].two<<','<<quate[i].three<<','<<quate[i].four<<')'<<endl;
    }
}

//语义子程序
//index1代表用的产生式的序号、b是一个属性、node_attri暂时存放每个归约产生的右部符号的属性
void Program(int index,attribute &attri,attribute node_attri[])
{
    unsigned int i,j;
    int x,q,q1;
    char rop[3];
    char q2[5];
    char q3[5];
    char q4[5];
    char* n1;
    char* n2;
    char* n3;
    switch(index)
    {
        case 0:         //I->vm(){S}
            InitAttri(attri);
            attri.chain=node_attri[5].chain; //I.chain=S.chain
//            cout<<"I.chain="<<attri.chain;
            length=0;
            break;
        case 1:         //S->C{S}
            InitAttri(attri);
            attri.chain=Merge(node_attri[0].chain,node_attri[2].chain);//S.chain=Merge(C.chain,S.chain)
            length=0;
            break;
        case 2:         //S->T{S}
            InitAttri(attri);
            attri.chain=Merge(node_attri[0].chain,node_attri[2].chain);//S.chain=Merge(T.chain,S.chain)
            length=0;
            break;
        case 3:         //S->K{S}
            InitAttri(attri);
            BackPatch(node_attri[2].chain,node_attri[0].codebegin);//BackPatch(S.chain,K.chain)
            sprintf(q4,"%d",node_attri[0].codebegin);
            CreatQua(nextstate,jump,duanhengxian,duanhengxian,q4);// i.emit(jump,-,-,K.codebegin)
            nextstate++;
            attri.chain=node_attri[0].chain;//S.chain=K.chain
            length=0;
            break;
        case 4:         //S->A
            InitAttri(attri);
            attri.chain=0;//S.chain=0;
            length=0;
            break;
        case 5:         //A->AA
            InitAttri(attri);
            length=0;
            break;
        case 6:         //C->fE
            InitAttri(attri);
            BackPatch(node_attri[1].truechain,nextstate);//BackPatch(E.true,nextstat);
            attri.chain=node_attri[1].falsechain;//C.chain=E.false;
            length=0;
            break;
        case 7:         //T->C{S}e
            InitAttri(attri);
            q=nextstate;
            CreatQua(nextstate,jump,duanhengxian,duanhengxian,zero);//emit(jump,-,-,0);
            nextstate++;                //调用一次CreatQua函数，nextstate加1
            BackPatch(node_attri[0].chain,nextstate);//BackPatch(C.chain,nextstate);
            attri.chain=Merge(q,node_attri[2].chain);//T.chain=Merge(nextstate,S.chain);
            length=0;
            break;
        case 8:         //W->w
            InitAttri(attri);
            attri.codebegin=nextstate;//W.codebegin=nextstate;
            length=0;
            break;
        case 9:         //K->WE
            InitAttri(attri);
            attri.codebegin=node_attri[0].codebegin;//K.codebegin=W.codebegin;
            attri.chain=node_attri[1].falsechain;//K.chain=E.false;
            BackPatch(node_attri[1].truechain,nextstate);//BackPatch(E.true,nextstate);
            length=0;
            break;
        case 10:        //F->r(i=d;
            InitAttri(attri);
            //i.(=,L(d.place).word,-,L(i.place).word)    (d是数字)(i是标识符)
            //emit(=,d.place,-,i.place);
            CreatQua(nextstate,denghao,L[node_attri[4].place].word,duanhengxian,L[node_attri[2].place].word);
            nextstate++;
            attri.place=node_attri[2].place;//F.place=i.place;
            attri.chain=nextstate;//F.chain=nextstate;
            CreatQua(nextstate,jump,duanhengxian,duanhengxian,zero);
            //emit(jump,-,-,0);
            nextstate++;
            attri.codebegin=nextstate;//F.codebegin=nextstate;
            length=0;
            break;
        case 11:        //G->FiRN;
            InitAttri(attri);
            attri.codebegin=node_attri[0].codebegin;//G.codebegin=F.codebegin;
            attri.place=node_attri[0].place;//G.place=F.place;
            q=nextstate;
            q1=q+3;
            sprintf(q2,"%d",q1);
            rop[0]='j';
            rop[1]='\0';
            x=strlen(rop);
            for (i=0; i<=strlen(L[node_attri[2].place].word); i++)
            {
                rop[x]=L[node_attri[2].place].word[i];
                //rop[2]=L[R.place].word里面所有的字母  R能推出的全部是算数运算符(>,<,>=,<=,==,!=)
                x++;
            }
           // emit(jrop,L[i.place].word,L[N.place].word,q2);
            CreatQua(nextstate,rop,L[node_attri[1].place].word,L[node_attri[3].place].word,q2);
            nextstate++;
            attri.chain=nextstate;//G.chain=nextstate;
            CreatQua(nextstate,jump,duanhengxian,duanhengxian,zero);//emit(jump,-,-,0);
            nextstate++;
            length=0;
            break;
        case 12:        //H->Gi=i+d)
            InitAttri(attri);
            attri.codebegin=node_attri[0].codebegin;//H.codebegin=G.codebegin;
            attri.place=node_attri[0].place;//H.place=G.place;
            attri.chain=node_attri[0].chain;//H.chain=G.chain;
            //emit(+,L[i1.place].word,L[d.place].word,L[i2.place].word);
            CreatQua(nextstate,add,L[node_attri[1].place].word,L[node_attri[5].place].word,L[node_attri[3].place].word);
            nextstate++;
            BackPatch(node_attri[0].chain-2,nextstate);//BackPatch(G.chain-2,nextstate);
            length=0;
            break;
        case 13:        //S->H{S}
            InitAttri(attri);
            sprintf(q3,"%d",node_attri[0].codebegin);
            //emit(jump,-,-,q3);
            CreatQua(nextstate,jump,duanhengxian,duanhengxian,q3);
            nextstate++;
            BackPatch(node_attri[2].chain,node_attri[0].codebegin);//BackPatch(S.chain,H.chain);
            attri.chain=node_attri[0].chain;//S.chain=H.chain;
            length=0;
            break;
        case 14:        //N->d
            InitAttri(attri);
            attri.place=node_attri[0].place;//N.place=d.place;
            length=0;
            break;
        case 15:        //S->B;S
            InitAttri(attri);
            attri.chain=node_attri[0].chain;//S.chain=B.chain;
            length=0;
            break;
        case 16:        //S->B
            InitAttri(attri);
            attri.place=node_attri[0].place;//S.chain=B.chain;
            length=0;
             cout<<"S.chain="<<attri.place;
            break;
        case 17:        //E->EhE
            InitAttri(attri);
            attri.codebegin=node_attri[0].codebegin;//E.codebegin=E1.codebegin;
            BackPatch(node_attri[0].falsechain,node_attri[2].codebegin);//BackPatch(E1.false,E2.codebegin)
            attri.truechain=Merge(node_attri[0].truechain,node_attri[2].truechain);//E.true=Merge(E1.true,E2.true);
            attri.falsechain=node_attri[2].falsechain;//E.false=E2.false;
            length=0;
            break;
        case 18:        //E->EkE
            InitAttri(attri);
            attri.codebegin=node_attri[0].codebegin;//E.codebegin=E1.codebegin;
            BackPatch(node_attri[0].truechain,node_attri[2].codebegin);//BackPatch(E1.true,E2.codebegin)
            attri.truechain=node_attri[2].truechain;//E.true=E2.true;
            attri.falsechain=Merge(node_attri[0].falsechain,node_attri[2].falsechain);//E.false=Merge(E1.false,E2.false);
            length=0;
            break;
        case 19:        //E->!E
            InitAttri(attri);
            attri.codebegin=node_attri[1].codebegin;//E.codebegin=E.codebegin;
            attri.truechain=node_attri[1].falsechain;//E.true=E.false;
            attri.falsechain=node_attri[0].truechain;//E.false=E.true;
            length=0;
            break;
        case 20:        //E->(E)
            InitAttri(attri);
            attri.codebegin=node_attri[1].codebegin;//E.codebegin=E.codebegin;
            attri.truechain=node_attri[1].truechain;//E.true=E.true;
            attri.falsechain=node_attri[1].falsechain;//E.false=E.false;
            length=0;
            break;
        case 21:        //E->dRd
            InitAttri(attri);
            attri.codebegin=nextstate;//E.codebegin=nextstate;
            attri.truechain=nextstate;//E.true=nextstate;
            attri.falsechain=nextstate+1;//E.false=nextstate;
            rop[0]='j';
            rop[1]='\0';
            x=strlen(rop);
            for (j=0; j<=strlen(L[node_attri[1].place].word); j++)
            {
                rop[x]=L[node_attri[1].place].word[j];
                x++;
            }
            if (node_attri[0].type==node_attri[2].type)         //两操作数类型一致
            {
                //emit(jrop,L[d1.place].word,L[d2.place].word,0);
                CreatQua(nextstate,rop,L[node_attri[0].place].word,L[node_attri[2].place].word,zero);
                nextstate++;
                //emit(jump,-,-,0);
                CreatQua(nextstate,jump,duanhengxian,duanhengxian,zero);
                nextstate++;
                attri.type=node_attri[0].type;
            }
            else
            {
                cout<<"ERROR:The type of two operands are inconsistent!"<<endl;
                exit(0);
            }
            length=0;
            break;
        case 22:        //E->iRi
            InitAttri(attri);
            attri.codebegin=nextstate;//E.codebegin=nextstate;
            attri.truechain=nextstate;//E.true=nextstate;
            attri.falsechain=nextstate+1;//E.false=nextstate+1;
            rop[0]='j';
            rop[1]='\0';
            x=strlen(rop);
            for (j=0; j<=strlen(L[node_attri[1].place].word); j++)
            {
                rop[x]=L[node_attri[1].place].word[j];
                x++;
            }
            if (node_attri[0].type==node_attri[2].type)         //两操作数类型一致
            {
                //emit(jrop,L[i1.place].word,L[i2.place].word,0);
                CreatQua(nextstate,rop,L[node_attri[0].place].word,L[node_attri[2].place].word,zero);
                nextstate++;
                //emit(jump,-,-,0);
                CreatQua(nextstate,jump,duanhengxian,duanhengxian,zero);
                nextstate++;
                attri.type=node_attri[0].type;
            }
            else
            {
                cout<<"ERROR:The type of two operands are inconsistent!"<<endl;
                exit(0);
            }
            length=0;
            break;
        case 23:        //E->iRd
            InitAttri(attri);
            attri.codebegin=nextstate;//E.codebegin=nextstate;
            attri.truechain=nextstate;//E.true=nextstate;
            attri.falsechain=nextstate+1;//E.false=nextstate+1;
            rop[0]='j';
            rop[1]='\0';
            x=strlen(rop);
            for (j=0; j<=strlen(L[node_attri[1].place].word); j++)
            {
                rop[x]=L[node_attri[1].place].word[j];
                x++;
            }
            if (node_attri[0].type==node_attri[2].type)         //两操作数类型一致
            {
                //emit(jrop,L[i.place].word,L[d.place].word,0);
                //emit(jump,-,-,0);
                CreatQua(nextstate,rop,L[node_attri[0].place].word,L[node_attri[2].place].word,zero);
                nextstate++;
                CreatQua(nextstate,jump,duanhengxian,duanhengxian,zero);
                nextstate++;
                attri.type=node_attri[0].type;
            }
            else
            {
                cout<<"ERROR:The type of two operands are inconsistent!"<<endl;
                exit(0);
            }
            length=0;
            break;
        case 24:        //R->a
            InitAttri(attri);
            attri.place=node_attri[0].place;//R.place=a.place;
            length=0;
            break;
        case 25:        //R->b
            InitAttri(attri);
            attri.place=node_attri[0].place;
            length=0;
            break;
        case 26:        //R->c
            InitAttri(attri);
            attri.place=node_attri[0].place;
            length=0;
            break;
        case 27:        //R->g
            InitAttri(attri);
            attri.place=node_attri[0].place;
            length=0;
            break;
        case 28:        //R-><
            InitAttri(attri);
            attri.place=node_attri[0].place;
            length=0;
            break;
        case 29:        //R->>
            InitAttri(attri);
            attri.place=node_attri[0].place;
            length=0;
            break;
        case 30:        //A->i=D;
            InitAttri(attri);
            //emit(=,L[D.place].word,-,L[i.place].word);
            if(node_attri[2].symbol==0)
                CreatQua(nextstate,denghao,newtem[node_attri[2].place],duanhengxian,L[node_attri[0].place].word);
            else
                CreatQua(nextstate,denghao,L[node_attri[2].place].word,duanhengxian,L[node_attri[0].place].word);
            nextstate++;
            length=0;
            break;
        case 31:        //D->D+D
            InitAttri(attri);
            attri.symbol=0;
            n1=NewTemp();//NewTemp(n1);
            i=0;
            while(n1[i]!='\0')
                i++;
            for (j=0; j<=i; j++)
                newtem[length2][j]=n1[j];
            attri.place=length2;//D.place=n1.place;
            length2++;
            if (node_attri[0].type=='n'&&node_attri[2].type=='n')       //两操作数均为整型
            {
                //emit(+,L[D1.place].word,L[D2.place].word,n1);
                CreatQua(nextstate,add,L[node_attri[0].place].word,L[node_attri[2].place].word,n1);
                nextstate++;
                attri.type='n';
            }
            else if(node_attri[0].type=='t'&&node_attri[2].type=='t')
            {
                CreatQua(nextstate,add1,L[node_attri[0].place].word,L[node_attri[2].place].word,n1);
                nextstate++;
                attri.type='t';
            }
            else
                cout<<"ERROR:The type of two operands are inconsistent!"<<endl;
            length=0;
            break;
        case 32:        //D->D*D
            InitAttri(attri);
            attri.symbol=0;
            n2=NewTemp();
            i=0;
            while(n2[i]!='\0')
                i++;
            for (j=0; j<=i; j++)
                newtem[length2][j]=n2[j];
            attri.place=length2;
            length2++;
            if (node_attri[0].type=='n'&&node_attri[2].type=='n')       //两操作数均为整型
            {
                CreatQua(nextstate,mul,L[node_attri[0].place].word,L[node_attri[2].place].word,n2);
                nextstate++;
                attri.type='n';
            }
            else if(node_attri[0].type=='t'&&node_attri[2].type=='t')
            {
                CreatQua(nextstate,mul1,L[node_attri[0].place].word,L[node_attri[2].place].word,n2);
                nextstate++;
                attri.type='t';
            }
            else
                cout<<"ERROR:The type of two operands are inconsistent!"<<endl;
            length=0;
            break;
        case 33:        //D->-D
            InitAttri(attri);
            attri.symbol=0;
            n3=NewTemp();
            i=0;
            while(n3[i]!='\0')
                i++;
            for (j=0; j<=i; j++)
                newtem[length2][j]=n3[j];
            attri.place=length2;
            length2++;
            CreatQua(nextstate,sub,L[node_attri[1].place].word,duanhengxian,n3);
            nextstate++;
            length=0;
            break;
        case 34:        //D->(D)
            InitAttri(attri);
            attri.place=node_attri[1].place;//D.place=D.place;
            length=0;
            break;
        case 35:        //D->d
            InitAttri(attri);
            attri.symbol=1;//D.symbol=1;
            attri.type=node_attri[0].type;//D.type=d.type;
            attri.place=node_attri[0].place;//D.place=d.place;
            length=0;
            break;
        case 36:        //D->i
            InitAttri(attri);
            attri.symbol=1;//D.symbol=1;
            attri.type=node_attri[0].type;//D.type=d.type;
            attri.place=node_attri[0].place;//D.place=i.place;
            length=0;
            break;
        case 37:        //B->t
            InitAttri(attri);
            attri.place=node_attri[0].place;//B.place=t.place;
            length=0;
            break;
        case 38:        //B->n
            InitAttri(attri);
            attri.place=node_attri[0].place;//B.place=n.place;
            length=0;
            break;
        case 39:        //B->BL
            InitAttri(attri);
            attri.chain=0;//B.chain=0;
            length=0;
            break;
        case 40:        //L->L,i
            InitAttri(attri);
            attri.chain=0;//L.chain=0;
            length=0;
            break;
        case 41:        //L->i
            InitAttri(attri);
            attri.place=node_attri[0].place;//L.place=i.place;
            length=0;
            break;
    }
}

//输出文件的表头
Status PrintStack_table()
{
    FILE *fp;
    if ((fp=fopen("SemanticAnalysis.txt","a+"))==NULL)  //!!写入格式为a+
    {
        printf("CANNOT OPEN!\n");
        exit(0);
    }
    fprintf(fp,"%s %s %s %s %s %s %s","place","codebegin","truechain","falsechain","chain","symbol","type");
    fprintf(fp,"\n");
    fclose(fp);
    return OK;
}

