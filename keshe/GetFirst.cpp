//#include "morphology.h"
//#include "grammar.h"
//#include "semantic.h"

#include"keshe.h"
#include"stack.h"

//计算一个串的FIRST集合
void FIRST(char strings[],char *first)
{
    int i;
    int x,l;
    if (FindChar(strings[0],terminal)!=-1)  //字符串的第一个就是终结符，那么该串的FIRST集就是该终结符
    {
        x=strlen(first);
        first[x]=strings[0];
        first[x+1]='\0';
    }
    else    //字符串的第一个字符是非终结符，求该符号的FIRST
    {
        x=strlen(first);
        l=FindChar(strings[0],non_terminal); //找到该非终结符在数组中的位置
        for (i=0; i<strlen(nonter_first[l].Fir); i++)
        {
            first[x]=nonter_first[l].Fir[i];
            x++;
        }
        first[x]='\0';
    }
}

//初始化非终结符及其能否推空以及它的FIRST集,传入参数为非终结符的个数
void GetNonter(int n)
{
    int i,j,x,l,m=0,g;
    int ends=1,index,x1;
    for (i=0; i<n; i++)             //初始化
    {
        nonter_first[i].non_termin=non_terminal[i];     //初始化每一个非终结符
        strcpy(nonter_first[i].Fir,"");                 //清空数组
        strcpy(nonter_first[i].temp_noend,"");          //初始化数组
        nonter_first[i].flag=0;                         //针对该文法其非终结符均不能推空
    }
    for (i=0; i<n; i++)
    {
        for (j=0; j<number; j++)
            if (grammar[j].left==nonter_first[i].non_termin)    //找左部相同的产生式
            {
                if (FindChar(grammar[j].right[0],terminal)>=0)  //是终结符
                {
                    if (FindChar(grammar[j].right[0],nonter_first[i].Fir)==-1) //该终结符不在集合中
                    {
                        x=strlen(nonter_first[i].Fir);
                        nonter_first[i].Fir[x]=grammar[j].right[0];
                        nonter_first[i].Fir[x+1]='\0';
                    }
                }
                else    //不是终结符，那么该产生式右部开头的非终结符的FIRST集要加入，先把该非终结符暂存
                    if (nonter_first[i].non_termin!=grammar[j].right[0])    //避免左递归的形式
                    {
                        x=strlen(nonter_first[i].temp_noend);
                        nonter_first[i].temp_noend[x]=grammar[j].right[0];
                        nonter_first[i].temp_noend[x+1]='\0';
                    }
            }
        if (strlen(nonter_first[i].temp_noend)==0)
            nonter_first[i].flag=1;
    }
    //所有还没有求出的FIRST的非终结符求出来
    while(ends==1)
    {
        ends=0;
        for (i=0; i<n; i++)             //遍历每一个非终结符
        {
            if(nonter_first[i].flag!=1) //如果该非终结符没有求完
            {
                for (j=0; j<strlen(nonter_first[i].temp_noend); j++)
                {
                    index=FindChar(nonter_first[i].temp_noend[j],non_terminal);   //找到该非终结符的位置
                    if (nonter_first[index].flag==1)    //该非终结符的FIRST已经求完，加入
                    {
                        x=strlen(nonter_first[i].Fir);
                        for (x1=0; x1<strlen(nonter_first[index].Fir); x1++)
                            if (FindChar(nonter_first[index].Fir[x1],nonter_first[i].Fir)==-1)//不一样加入
                            {
                                nonter_first[i].Fir[x]=nonter_first[index].Fir[x1];
                                x++;
                            }
                        nonter_first[i].Fir[x]='\0';
                        nonter_first[i].temp_noend[j]='*';  //表示该非终结符的FIRST集加入完成
                    }
                }
                for (l=0; l<strlen(nonter_first[i].temp_noend); l++)
                    if (nonter_first[i].temp_noend[l]!='*')
                        m++;
                if (m==0)   //该非终结符的FIRST已经全部求完
                    nonter_first[i].flag=1; //将标志置为1
                m=0;
            }
        }
        for (g=0; g<n; g++)
            if (nonter_first[g].flag==0)
                ends=1;
    }
}
