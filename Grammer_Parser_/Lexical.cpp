#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
#include "Lexical.h"

#define path "C:\\Users\\29034\\Desktop\\c_language\\Grammer_Parser_\\test1.txt"
int line=1;           //记录行数，初始值为1

Word_simplelist L[NUM];     //存放单词及其简化形式的结构体数组
char simp_sentence[NUM];

//读取测试代码，并简化
void cifa_analysis()
{
    char str;                  //读入的字符
    char calcution[NUM];      //存放运算符的数组
    char side_part[NUM];       //存放分界符的数组
    char number[NUM];         //存放整数和浮点数的数组
    char Temp_Num[20];        //临时存放整数或浮点数的一部分
    int num1=0,num2=0;        //存放整数或浮点数的长度，超过8位即为错误单词，num1为小数点前面加后面的部分，num2为小数点后面的部分
    int temp_m;               //保留m的值
    char biaoshi[NUM];      //存放标识符的数组
    char ratain_word[NUM];  //存放保留字的数组
    char temp_word[10];       //暂时存放单词
    int m=0,h=0,j=0,l=0,g=0;
    int sum1=0,sum2=0;
    int k,i;
    FILE *fp= fopen(path,"r");
    //if ((fp = fopen("test1.txt","r"))==NULL)   //打开测试代码所在的文件
    if (fp==NULL)   //打开测试代码所在的文件
    {
        printf("寻找test.txt文件错误，在当前文件夹下不存在test.txt文件");
        exit(0);
    }
    fscanf(fp,"%c",&str);                      //先读入第一个字符，进行判断

//    //读取输出的纯文件操作
//    cout<<"读取的纯文件"<<endl;//添加的*句子

    fstream outfile;//对应头文件#include <fstream>
    outfile.open("Simplify.txt",ios::out);
    while(1)
    {
//        //读取输出的纯文件操作
//        cout<<ch;//添加的*句子
        if (str==' '||str=='\n'||str=='\t')
        {
            if (str=='\n')                     //若为回车，行数加一
                line++;
            fscanf(fp,"%c",&str);              //读下一个字符
        }
        //保留字与标识符的识别
        else if (str>='a'&&str<='z')   //为字母
        {
            temp_word[l]=str;                           //存入暂存数组
            L[g].word[l]=str;
            l++;
            while(1)
            {
                fscanf(fp,"%c",&str);
                if(((str>='a'&&str<='z')||(str>='0'&&str<='9'))&&!feof(fp))   //为字母或数字且文件没读完
                {
                    temp_word[l]=str;
                    L[g].word[l]=str;
                    l++;
                }
                else  break;
            }
            temp_word[l]='#';             //一个单词识别完
            L[g].word[l]='\0';
            l++;
            if (Judge(temp_word)==1)      //判断temp_word里面是保留字，简化
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
                else if (strcmp(L[g].word,"begin")==0)
                    L[g].flag='g';
                for (i=0; i<l; i++)
                {
                    ratain_word[sum1]=temp_word[i];
                    sum1++;
                }
                memset(temp_word,0,sizeof(temp_word));  //清空数组
                l=0;
            }
            else     //判断temp_word=0，是标识符
            {
                L[g].flag='i';   //标识符的简化形式
                for (i=0; i<l; i++)
                {
                    biaoshi[sum2]=temp_word[i];
                    sum2++;
                }
                memset(temp_word,0,sizeof(temp_word));
                l=0;
            }
            g++;
        }
        //整数与浮点数的识别
        else if (str<='9'&&str>='0')    //数字
        {
            Temp_Num[num1]=str;
            num1++;//num存放整数或浮点数的长度，超过8位即为错误单词，num1为小数点前面加后面的部分，num2为小数点后面的部分
            while(1)
            {
                fscanf(fp,"%c",&str);
                if ((str>='0'&&str<='9')&&!feof(fp))
                {
                    Temp_Num[num1]=str;
                    num1++;
                }
                else
                    break;
            }
            if (str=='.')              //浮点数
            {
                Temp_Num[num1]=str;
                num1++;
                while(1)
                {
                    fscanf(fp,"%c",&str);
                    if ((str<='9'&&str>='0')&&!feof(fp))
                    {
                        Temp_Num[num1]=str;
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
            else if (str!='.')   //整数
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
            switch(str)
            {       //运算符的判别
                case '+':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    h++;
                    calcution[h]='#';
                    L[g].word[1]='\0';
                    L[g].flag='+';
                    h++;
                    fscanf(fp,"%c",&str); //读入下一个字符，为下个分析做准备
                    g++;
                    break;
                case '-':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    L[g].flag='-';
                    h++;
                    calcution[h]='#';
                    L[g].word[1]='\0';
                    h++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case '*':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    L[g].flag='*';
                    h++;
                    calcution[h]='#';
                    L[g].word[1]='\0';
                    h++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case '/':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    L[g].flag='/';
                    h++;
                    calcution[h]='#';
                    L[g].word[1]='\0';
                    h++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case '=':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    h++;
                    fscanf(fp,"%c",&str);
                    if (str=='=')          //比较符号，等于
                    {
                        calcution[h]=str;
                        L[g].word[1]=str;
                        h++;
                        calcution[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='c';    //代表==
                        h++;
                        fscanf(fp,"%c",&str);

                    }
                    else                 //赋值符号
                    {
                        L[g].word[1]='\0';
                        L[g].flag='=';
                        calcution[h]='#';
                        h++;
                    }
                    g++;
                    break;
                case '!':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    h++;
                    fscanf(fp,"%c",&str);
                    if (str=='=')         //比较符号，不等于
                    {
                        calcution[h]=str;
                        L[g].word[1]=str;
                        h++;
                        calcution[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='g';   //代表！=
                        h++;
                        fscanf(fp,"%c",&str);
                    }
                    else
                    {
                        calcution[h]='#';
                        L[g].word[1]='\0';
                        L[g].flag='!';
                        h++;
                    }
                    g++;
                    break;
                case '>':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    h++;
                    fscanf(fp,"%c",&str);
                    if (str=='=')        //大于等于
                    {
                        calcution[h]=str;
                        L[g].word[1]=str;
                        h++;
                        calcution[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='b';  //代表>=
                        h++;
                        fscanf(fp,"%c",&str);
                    }
                    else                //大于
                    {
                        L[g].word[1]='\0';
                        L[g].flag='>';
                        calcution[h]='#';
                        h++;
                    }
                    g++;
                    break;
                case '<':
                    calcution[h]=str;
                    L[g].word[0]=str;
                    h++;
                    fscanf(fp,"%c",&str);
                    if (str=='=')        //小于等于
                    {
                        calcution[h]=str;
                        L[g].word[1]=str;
                        h++;
                        calcution[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='a';  //代表<=
                        h++;
                        fscanf(fp,"%c",&str);
                    }
                    else
                    {
                        L[g].word[1]='\0';
                        L[g].flag='<';
                        calcution[h]='#';
                        h++;
                    }
                    g++;
                    break;
                case '&':
                    fscanf(fp,"%c",&str);
                    if (str=='&')
                    {
                        calcution[h]='&';
                        L[g].word[0]='&';
                        h++;
                        calcution[h]='&';
                        L[g].word[1]='&';
                        h++;
                        calcution[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='k';  //代表&&
                        h++;
                        fscanf(fp,"%c",&str);
                    }
                    else
                    {
                        outfile<<endl;
                        outfile<<"Line:"<<line<<" "<<"Error:The '&' is not an operator!"<<endl;
                    }
                    g++;
                    break;
                case '|':
                    fscanf(fp,"%c",&str);
                    if (str=='|')
                    {
                        calcution[h]='|';
                        L[g].word[0]='|';
                        h++;
                        calcution[h]='|';
                        L[g].word[1]='|';
                        h++;
                        calcution[h]='#';
                        L[g].word[2]='\0';
                        L[g].flag='h';   //代表||
                        h++;
                        fscanf(fp,"%c",&str);
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
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='(';
                    j++;
                    fscanf(fp,"%c",&str); //读入下一个字符，为下个分析做准备
                    g++;
                    break;
                case ')':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=')';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case ';':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=';';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case ':':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=':';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case '.':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='.';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case '{':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='{';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case '}':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag='}';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                case ',':
                    side_part[j]=str;
                    L[g].word[0]=str;
                    j++;
                    side_part[j]='#';
                    L[g].word[1]='\0';
                    L[g].flag=',';
                    j++;
                    fscanf(fp,"%c",&str);
                    g++;
                    break;
                default:
                    //其他字符
                    outfile<<endl;
                    outfile<<"Line:"<<line<<" "<<"Error:"<<"'"<<str<<"'"<<"is not a legal char"<<endl;
                    fscanf(fp,"%c",&str);
                    break;
            }
        }
        if (feof(fp))   break;
    }

//   //读取输出的纯文件操作
//    cout<<"\n"<<endl;
    cout<<"简化后的字符串："<<endl;
    for (k=0; k<g; k++)
    {
        simp_sentence[k]=L[k].flag;
        outfile<<"word:"<<L[k].word<<"  \t"<<"简化后的字符是:"<<L[k].flag<<endl;
        cout<<L[k].flag;
    }
    cout<<"\n"<<endl;
    simp_sentence[k]='#';
    fclose(fp);
}

//判断是保留字还是标识符，保留字返回1，标识符返回0
int Judge(char ident[])
{
    int i=0,j=0,k=0;
    char ratainword[19][15]= {"begin#","end#","then#","not#","and#","or#","step#","until#","if#","else#","true#","false#","while#","do#","for#","void#","main#","float#","int#"}; //存放保留字的二维数组

    //while(identifer[k]!='#'&&table[i][j]!='#'&&i<11)
     do
    {
        if (ident[k]==ratainword[i][j])          //相同，继续比较
        {   k++;
            j++;   }
        else                                    //不同，与下一个保留字比较
        {   i++;
            k=0;
            j=0;   }
    } while(ident[k]!='#'&&ratainword[i][j]!='#'&&i<19);
    if (ident[k]=='#'&&ratainword[i][j]=='#')    //是保留字
        return 1;
    else
        return 0;
}
