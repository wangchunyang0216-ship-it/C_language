#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
//#include "morphology.h"
//#include "grammar.h"


#include"keshe.h"
#include"stack.h"

int line=1;           //记录行数，初始值为1
int g=0;              //该源程序的单词个数
Word_List L[NUM];     //存放单词及其简化形式的结构体数组
char simp_sentence[NUM];

//读取测试代码，并简化
void Morph_analysis()
{
    char ch;                  //读入的字符
    char operation[NUM];      //存放运算符的数组
    char boundary[NUM];       //存放分界符的数组
    char number[NUM];         //存放整数和浮点数的数组
    char Temp_Num[20];        //临时存放整数或浮点数的一部分
    int num1=0,num2=0;        //存放整数或浮点数的长度，超过8位即为错误单词，num1为小数点前面加后面的部分，num2为小数点后面的部分
    int temp_m;               //保留m的值
    char identifer[NUM];      //存放标识符的数组
    char reserved_word[NUM];  //存放保留字的数组
    char temp_word[10];       //暂时存放单词
    int m=0,h=0,j=0,l=0;
    int sum1=0,sum2=0;
    int k,i;
    for (i=0; i<NUM; i++)
        L[i].flag1=0;
    FILE *fp;
    if ((fp = fopen("lasttest.txt","r"))==NULL)   //打开测试代码所在的文件
    {
        printf("寻找test.txt文件错误，在当前文件夹下不存在test.txt文件\n");
        exit(0);
    }
    fscanf(fp,"%c",&ch);                      //先读入第一个字符，进行判断
    fstream outfile;
    outfile.open("Simplify.txt",ios::out);//ios::out 只写,文件不存在则创建，存在则打开并截断原内容
    while(1)
    {
        if (ch==' '||ch=='\n'||ch=='\t')
        {
            if (ch=='\n')                     //若为回车，行数加一
                line++;
            fscanf(fp,"%c",&ch);              //读下一个字符
        }
        //保留字与标识符的识别
        else if ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))   //为字母
        {
            temp_word[l]=ch;                           //存入暂存数组
            L[g].word[l]=ch;
            l++;
            while(1)
            {
                fscanf(fp,"%c",&ch);
                if(((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||(ch>='0'&&ch<='9'))&&!feof(fp))   //为字母或数字且文件没读完
                {
                    temp_word[l]=ch;
                    L[g].word[l]=ch;
                    l++;
                }
                else  break;
            }
            temp_word[l]='#';             //一个单词识别完
            L[g].word[l]='\0';
            l++;
            if (Judge(temp_word)==1)      //保留字，简化
            {
                if (strcmp(L[g].word,"if")==0)
                    L[g].flag='f';
                else if (strcmp(L[g].word,"void")==0)
                    L[g].flag='v';
                else if (strcmp(L[g].word,"main")==0)
                    L[g].flag='m';
                else if (strcmp(L[g].word,"else")==0)
                    L[g].flag='e';
                else if (strcmp(L[g].word,"do")==0)
                    L[g].flag='o';
                else if (strcmp(L[g].word,"while")==0)
                    L[g].flag='w';
                else if (strcmp(L[g].word,"for")==0)
                    L[g].flag='r';
                else if (strcmp(L[g].word,"int")==0)
                    L[g].flag='n';
                else if (strcmp(L[g].word,"float")==0)
                    L[g].flag='t';
                else if (strcmp(L[g].word,"true")==0)
                    L[g].flag='u';
                else if (strcmp(L[g].word,"false")==0)
                    L[g].flag='l';
                for (i=0; i<l; i++)
                {
                    reserved_word[sum1]=temp_word[i];
                    sum1++;
                }
                memset(temp_word,0,sizeof(temp_word));  //清空数组
                l=0;
            }
            else     //标识符
            {
                L[g].flag='i';   //标识符的简化形式
                for (i=0; i<l; i++)
                {
                    identifer[sum2]=temp_word[i];
                    sum2++;
                }
                memset(temp_word,0,sizeof(temp_word));
                l=0;
            }
            g++;
        }
        //整数与浮点数的识别
        else if (ch<='9'&&ch>='0')    //数字
        {
            Temp_Num[num1]=ch;
            num1++;
            while(1)
            {
                fscanf(fp,"%c",&ch);
                if ((ch>='0'&&ch<='9')&&!feof(fp))
                {
                    Temp_Num[num1]=ch;
                    num1++;
                }
                else
                    break;
            }
            if (ch=='.')              //浮点数
            {
                Temp_Num[num1]=ch;
                num1++;
                while(1)
                {
                    fscanf(fp,"%c",&ch);
                    if ((ch<='9'&&ch>='0')&&!feof(fp))
                    {
                        Temp_Num[num1]=ch;
                        num1++;
                        num2++;       //小数点后的部分
                    }
                    else
                        break;
                }
                if ((num1-num2-1)<=8&&num2<=8&&num2!=0)  //当前的浮点数合法
                {
                    for (i=0; i<num1; i++)
                        L[g].word[i]=Temp_Num[i];
                    L[g].word[i]='\0';
                    L[g].flag='d';                       //数字简化
                    temp_m=m;                            //保留当前m的值
                    for (i=temp_m; i<temp_m+num1; i++)
                    {
                        number[i]=Temp_Num[i-temp_m];
                        m++;
                    }
                    number[m]='#';
                    m++;
                    num1=0;                              //为下一个数的读入做准备
                }
                else      //小数点后不是数字，错误
                {
                    num1=0;
                    outfile<<"Line:"<<line<<" "<<"Error:This is an illegal number of float"<<endl;
                }
            }
            else if (ch!='.')   //整数
            {
                if(num1<=8)
                {
                    for (i=0; i<num1; i++)
                        L[g].word[i]=Temp_Num[i];
                    L[g].word[i]='\0';
                    L[g].flag='d';                 //数字简化
                    temp_m=m;                      //保留当前m的值
                    for (i=temp_m; i<temp_m+num1; i++)      //将暂存数组中合法的单词赋值给最后存结果的数组
                    {
                        number[i]=Temp_Num[i-temp_m];
                        m++;
                    }
                    number[m]='#';
                    m++;
                    num1=0;                        //为下一个数的读入做准备
                }
                else      //不合法整数
                {
                    num1=0;
                    outfile<<"Line:"<<line<<" "<<"Error:This is an illegal number of int"<<endl;
                }
            }
            g++;
        }
        else
        {
            switch(ch)
            {       //运算符的判别
                case '+':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    h++;
                    operation[h]='#';
                    L[g].word[1]='\0';
                    L[g].flag='+';
                    h++;
                    fscanf(fp,"%c",&ch); //读入下一个字符，为下个分析做准备
                    g++;
                    break;
                case '-':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    L[g].flag='-';
                    h++;
                    operation[h]='#';
                    L[g].word[1]='\0';
                    h++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case '*':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    L[g].flag='*';
                    h++;
                    operation[h]='#';
                    L[g].word[1]='\0';
                    h++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case '/':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    L[g].flag='/';
                    h++;
                    operation[h]='#';
                    L[g].word[1]='\0';
                    h++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case '=':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    h++;
                    fscanf(fp,"%c",&ch);
                    if (ch=='=')          //比较符号，等于
                    {
                        operation[h]=ch;
                        L[g].word[1]=ch;
                        h++;
                        operation[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='c';    //代表==
                        h++;
                        fscanf(fp,"%c",&ch);

                    }
                    else                 //赋值符号
                    {
                        L[g].word[1]='\0';
                        L[g].flag='=';
                        operation[h]='#';
                        h++;
                    }
                    g++;
                    break;
                case '!':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    h++;
                    fscanf(fp,"%c",&ch);
                    if (ch=='=')         //比较符号，不等于
                    {
                        operation[h]=ch;
                        L[g].word[1]=ch;
                        h++;
                        operation[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='g';   //代表！=
                        h++;
                        fscanf(fp,"%c",&ch);
                    }
                    else
                    {
                        operation[h]='#';
                        L[g].word[1]='\0';
                        L[g].flag='!';
                        h++;
                    }
                    g++;
                    break;
                case '>':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    h++;
                    fscanf(fp,"%c",&ch);
                    if (ch=='=')        //大于等于
                    {
                        operation[h]=ch;
                        L[g].word[1]=ch;
                        h++;
                        operation[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='b';  //代表>=
                        h++;
                        fscanf(fp,"%c",&ch);
                    }
                    else                //大于
                    {
                        L[g].word[1]='\0';
                        L[g].flag='>';
                        operation[h]='#';
                        h++;
                    }
                    g++;
                    break;
                case '<':
                    operation[h]=ch;
                    L[g].word[0]=ch;
                    h++;
                    fscanf(fp,"%c",&ch);
                    if (ch=='=')        //小于等于
                    {
                        operation[h]=ch;
                        L[g].word[1]=ch;
                        h++;
                        operation[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='a';  //代表<=
                        h++;
                        fscanf(fp,"%c",&ch);
                    }
                    else
                    {
                        L[g].word[1]='\0';
                        L[g].flag='<';
                        operation[h]='#';
                        h++;
                    }
                    g++;
                    break;
                case '&':
                    fscanf(fp,"%c",&ch);
                    if (ch=='&')
                    {
                        operation[h]='&';
                        L[g].word[0]='&';
                        h++;
                        operation[h]='&';
                        L[g].word[1]='&';
                        h++;
                        operation[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='k';  //代表&&
                        h++;
                        fscanf(fp,"%c",&ch);
                    }
                    else
                    {
                        outfile<<endl;
                        outfile<<"Line:"<<line<<" "<<"Error:The '&' is not an operator!"<<endl;
                    }
                    g++;
                    break;
                case '|':
                    fscanf(fp,"%c",&ch);
                    if (ch=='|')
                    {
                        operation[h]='|';
                        L[g].word[0]='|';
                        h++;
                        operation[h]='|';
                        L[g].word[1]='|';
                        h++;
                        operation[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='h';   //代表||
                        h++;
                        fscanf(fp,"%c",&ch);
                    }
                    else
                    {
                        outfile<<endl;
                        outfile<<"Line:"<<line<<" "<<"Error:The '|' is not an operator!"<<endl;
                    }
                    g++;
                    break;
                //分界符
                case '(':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='(';
                    j++;
                    fscanf(fp,"%c",&ch); //读入下一个字符，为下个分析做准备
                    g++;
                    break;
                case ')':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=')';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case ';':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=';';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case ':':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=':';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case '.':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='.';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case '{':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='{';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case '}':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='}';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                case ',':
                    boundary[j]=ch;
                    L[g].word[0]=ch;
                    j++;
                    boundary[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=',';
                    j++;
                    fscanf(fp,"%c",&ch);
                    g++;
                    break;
                default:
                    //其他字符
                    outfile<<endl;
                    outfile<<"Line:"<<line<<" "<<"Error:"<<"'"<<ch<<"'"<<"is not a legal char"<<endl;
                    fscanf(fp,"%c",&ch);
                    break;
            }
        }
        if (feof(fp)) break;
    }
        SetType();
        outfile<<"运算符:"<<endl;
        for (k=0; k<h; k++)
            if (operation[k]!='#')
                outfile<<operation[k];
            else
                outfile<<' ';
        outfile<<"\n"<<endl;
        outfile<<"常量:"<<endl;
        for (k=0; k<m; k++)
            if (number[k]!='#')
                outfile<<number[k];
            else
                outfile<<' ';
        outfile<<"\n"<<endl;
        outfile<<"分界符:"<<endl;
        for (k=0; k<j; k++)
            if (boundary[k]!='#')
                outfile<<boundary[k];
            else
                outfile<<' ';
        outfile<<"\n"<<endl;
        outfile<<"标识符:"<<endl;
        for (k=0; k<sum2; k++)
            if (identifer[k]!='#')
                outfile<<identifer[k];
            else
                outfile<<' ';
        outfile<<"\n"<<endl;
        outfile<<"保留字:"<<endl;
        for (k=0; k<sum1; k++)
        {
            if (reserved_word[k]!='#')
                outfile<<reserved_word[k];
            else
                outfile<<' ';
        }
        outfile<<"\n"<<endl;
        outfile<<"测试代码:"<<endl;

        cout<<"测试代码:"<<endl;
        for (k=0; k<g; k++)
        {
            simp_sentence[k]=L[k].flag;
            outfile<<L[k].word<<" ";
            cout<<L[k].word<<" ";
        }
        outfile<<"\n"<<endl;
        outfile<<"简化后的代码:"<<endl;
         cout<<"\n"<<endl;
        cout<<"简化后的代码:"<<endl;
        for (k=0; k<g; k++)
        {
            simp_sentence[k]=L[k].flag;
            outfile<<L[k].flag<<" ";
            cout<<L[k].flag;
        }
        outfile<<"\n"<<endl;
        cout<<"\n"<<endl;
        simp_sentence[k]='#';
        fclose(fp);
    for (k=0; k<g; k++)
    {
        simp_sentence[k]=L[k].flag;
        outfile<<"word:"<<L[k].word<<"  \t"<<"简化后的字符是:"<<L[k].flag<<endl;
    }
    simp_sentence[k]='#';
    fclose(fp);
}


//对源程序中的单词的类型赋值
void SetType()
{
    int i,j,i1;
    for (i=0; i<g; i++)//g=0该源程序的单词个数
        L[i].flag1='$';
    for (i=0; i<g; i++)
    {
        if (L[i].flag=='n') //类型为int
        {
            j=i+1;
            while(L[j].flag!=';') //遇到分号类型定义结束
                if (L[j].flag=='i')
                {
                    L[j].flag1='n';
                    for (i1=0; i1<g; i1++)
                        if (strcmp(L[i1].word,L[j].word)==0)
                            L[i1].flag1=L[j].flag1;
                    j++;
                }
                else
                    j++;
        }
        else if (L[i].flag=='t') //类型为float
        {
            j=i+1;
            while(L[j].flag!=';')
                if (L[j].flag=='i')
                {
                    L[j].flag1='t';
                    for (i1=0; i1<g; i1++)
                        if (strcmp(L[i1].word,L[j].word)==0)
                            L[i1].flag1=L[j].flag1;
                    j++;
                }
                else
                    j++;
        }
        else if(L[i].flag=='d')
        {
            if (FindChar('.',L[i].word)>=0) //该数为浮点数
                L[i].flag1='t';
            else //该数为整数
                L[i].flag1='n';
        }
    }
}

//判断是保留字还是标识符，保留字返回1，标识符返回0
int Judge(char identifer[])
{
    int i=0,j=0,k=0;
    char table[11][15]= {"void#","main#","int#","float#","if#","else#","true#","false#","while#","do#","for#"}; //存放保留字的二维数组
    while(identifer[k]!='#'&&table[i][j]!='#'&&i<11)
    {
        if (identifer[k]==table[i][j])          //相同，继续比较
        {   k++; j++;   }
        else                                    //不同，与下一个保留字比较
        {   i++; k=0; j=0;   }
    }
    if (identifer[k]=='#'&&table[i][j]=='#')    //是保留字
        return 1;
    else
        return 0;
}
