#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
using namespace std;
#include "keshe.h"
#include "stack.h"
#include "optimize.h"

char jump1[5]= {'j','u','m','p','\0'};
char valid1[2]= {'-','\0'};             //代表空
char equ1[2]= {'=','\0'};
char sub1[2]= {'-','\0'};

block B[NUM3];      //保存块的结构体数组
int block_num=0;
int node_num=0;     //结点编号从0开始
int right_num[MAX_VERTEX_NUM];
MGraph M;
int kinds[2];
int x1=1;
Qua quate_optimize[NUM];    //储存优化后的四元式

//找入口语句并分块，删除不可达的语句
void FindEntrance()
{
    int i,index,j;
    quate[1].entrance=1;            //第一条四元式为入口语句
    for (i=1; i<nextstate; i++)     //遍历四元式找入口语句
    {
        if (quate[i].one[0]=='j')    //该四元式是条件转移或无条件转移语句
        {
            index=atoi(quate[i].four);
            quate[index].entrance=1;
        }
        if ((quate[i-1].one[0]=='j')&&(strcmp(quate[i-1].one,"jump")!=0)) //该语句跟在转移语句之后
            quate[i].entrance=1;
    }
    for (i=1; i<nextstate; i++)
    {
        if (quate[i].entrance==1)
        {
            B[block_num].entry=i;           //该块的入口语句编号为i
            for (j=i+1; j<nextstate; j++)
            {
                if(quate[j].entrance==1)    //到下一个入口语句，不包括下一个入口语句
                    goto out1;
                else if ((quate[j].one[0]=='j')||(strcmp(quate[j].four,"0")==0)) //到转移语句或停语句，包括该语句
                    goto out2;
            }
out1:
            B[block_num].out=j-1;
            block_num++;
            continue;
out2:
            B[block_num].out=j;
            block_num++;
        }
    }
    for (i=1; i<nextstate; i++)
    {
        if (quate[i].entrance==1)   //入口语句一定可达
            quate[i].sym=1;
        else                        //不是入口语句
            for (j=0; j<block_num; j++)
                if (quate[i].order>B[j].entry&&quate[i].order<=B[j].out) //该语句在某一个块内
                {
                    quate[i].sym=1;
                    break;
                }
    }
    for (i=1; i<nextstate; i++)     //为四元式的类型赋值
    {
        if (quate[i].one[0]=='=')    //赋值语句
            quate[i].kind=0;
        else if (quate[i].one[0]=='+'||quate[i].one[0]=='-'||quate[i].one[0]=='*') //数据运算
        {
            if (quate[i].three[0]=='&')  //单目运算
                quate[i].kind=1;
            else                        //双目运算
                quate[i].kind=2;
        }
        else if (strcmp(quate[i].one,"jump")==0) //无条件转移
            quate[i].kind=3;
        else                                    //条件转移
            quate[i].kind=4;
    }
}

//在已生成的DAG图中查看结点是否存在，存在返回结点编号，否则返回-1，传入参数为被查找的结点和DAG图
int FindNode(char* findnode,MGraph M)
{
    int i,j,index=-1;
    for (i=0; i<node_num; i++)
    {
        if (strcmp(findnode,M.vexs[i].below)==0) //新结点和图中某一结点的下方信息一致
        {
            index=M.vexs[i].node_id;
            break;
        }
        for (j=0; j<right_num[i]; j++)          //新结点和某一结点的右方信息一致
            if (strcmp(findnode,M.vexs[i].right[j])==0)
            {
                index=M.vexs[i].node_id;
                break;
            }
    }
    return index;
}

//在DAG图中查找是否存在相同的单目运算符的结点，存在则返回结点编号，否则返回-1
int FindNode1(char* n,MGraph M)
{
    int index=-1,i,index1;
    index1=FindNode(n,M);       //先找到位置
    for (i=0; i<node_num; i++)
    {
        if (M.arcs[index1][i].adj==1&&(strcmp("-",M.vexs[index1+1].below)==0))
            index=i;
    }
    return index;
}

//在DAG图中查找是否存在相同的双目运算符的结点，存在则返回结点编号，否则返回-1
int FindNode2(char*n1,char* n2,char* ope,MGraph M)
{
    int index=-1,i;
    int index1,index2;
    index1=FindNode(n1,M);
    index2=FindNode(n2,M);
    for (i=0; i<node_num; i++)
    {
        if (M.arcs[index1][i].adj==1&&M.arcs[index2][i].adj==1&&strcmp(M.vexs[i].below,ope)==0)
            index=i;
    }
    return index;
}

//构造一个基本块的DAG图，传入的参数为一个基本块
void CreatDAG(block &b)
{
    int j,j2;
    int i,j1,value,value1,index6,index7,index8;
    int index,index1,index2=1,index3=1,index4=1,index5=1;
    char p1[5];
    char p2[5];
    int l;
    node_num=0;         //初始化
    memset(right_num,0,sizeof(right_num));
    for (i=0; i<MAX_VERTEX_NUM; i++)
    {
        for (j=0; j<MAX_VERTEX_NUM; j++)
            M.arcs[i][j].adj=0;
    }
    for (i=b.entry; i<=b.out; i++)
    {
        if (quate[i].kind==0)       //赋值语句的四元式(=,B,-,A)
        {
            index=FindNode(quate[i].two,M);
            if (index==-1)          //表明该B结点是新结点
            {
                strcpy(M.vexs[node_num].below,quate[i].two);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                M.vexs[node_num].node_id=node_num;              //编号
                node_num++;                                     //整个图的结点数加1
label1:
                index1=FindNode(quate[i].four,M);             //查看A结点是否存在
                if (index1==-1)                                 //A不存在
                {
                    l=0;
                    while(quate[i].four[l]!='\0')
                        l++;
                    for (j2=0; j2<=l; j2++)
                        M.vexs[node_num-1].right[right_num[node_num-1]][j2]=quate[i].four[j2];
                    right_num[node_num-1]++;                    //该结点右方信息个数加1
                }
                else                //该结点已经存在
                {
                    index2=1;
                    for (j=0; j<node_num; j++)          //查看该结点是否为叶子结点
                        if (M.arcs[j][index1].adj==1)   //该结点不是叶子结点
                            index2=0;
                    if (index2==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                    {
                        for (j1=0; j1<right_num[index1]; j1++)
                            if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                strcpy(M.vexs[index1].right[j1],"");
                        l=0;
                        while(quate[i].four[l]!='\0')
                            l++;
                        for (j2=0; j2<=l; j2++)
                            M.vexs[node_num-1].right[right_num[node_num-1]][j2]=quate[i].four[j2];
                        right_num[node_num-1]++;
                    }
                    else
                    {
                        l=0;
                        while(quate[i].four[l]!='\0')
                            l++;
                        for (j2=0; j2<=l; j2++)
                            M.vexs[node_num-1].right[right_num[node_num-1]][j2]=quate[i].four[j2];
                        right_num[node_num-1]++;
                    }
                }
            }
            else    //已经存在且结点编号为index
            {
                index1=FindNode(quate[i].four,M); //查看结点是否存在
                if (index1==-1)                     //不存在
                {
                    l=0;
                    while(quate[i].four[l]!='\0')
                        l++;
                    for (j2=0; j2<=l; j2++)
                        M.vexs[index].right[right_num[index]][j2]=quate[i].four[j2];
                    right_num[index]++;             //该结点右方信息个数加1
                }
                else                                //该变量结点已经存在
                {
                    index2=1;
                    for (j=1; j<node_num; j++)          //查看该结点是否为叶子结点
                        if (M.arcs[j][index1].adj==1)   //该结点不是叶子结点
                            index2=0;
                    if (index2==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                    {
                        for (j1=0; j1<right_num[index1]; j1++)
                        {
                            if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                strcpy(M.vexs[index1].right[j1],"");
                        }
                        l=0;
                        while(quate[i].four[l]!='\0')
                            l++;
                        for (j2=0; j2<=l; j2++)
                            M.vexs[index].right[right_num[index]][j2]=quate[i].four[j2];
                        right_num[index]++;
                    }
                    else
                    {
                        l=0;
                        while(quate[i].four[l]!='\0')
                            l++;
                        for (j2=0; j2<=l; j2++)
                            M.vexs[index].right[right_num[index]][j2]=quate[i].four[j2];
                        right_num[index]++;
                    }
                }
            }
        }
        else if (quate[i].kind==1)  //为单目运算的四元式(op,B,-,A)
        {
            index=FindNode(quate[i].two,M);
            if (index==-1)      //该四元式的变量B不存在
            {
                strcpy(M.vexs[node_num].below,quate[i].two);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                M.vexs[node_num].node_id=node_num;              //编号
                node_num++;                                     //整个图的结点数加1
                for (j=0; j<strlen(M.vexs[node_num-1].below); j++)  //判断该结点是否为常数
                    if (M.vexs[node_num-1].below[j]>'9'||M.vexs[node_num-1].below[j]<'0') //因为上边node_num加了1，但目前要计算的结点是node_num-1的结点
                        index2=0;                                   //该结点不是常数
                for (j1=0; j1<node_num; j1++)               //查看该结点是否为叶子结点
                    if (M.arcs[j1][node_num-1].adj==1)      //该结点不是叶子结点
                        index3=0;
                if (index2==1&&index3==1)                   //是叶子结点且为常数
                {
                    value=atoi(M.vexs[node_num-1].below);   //将字符数组转换为整型进行运算
                    value=0-value;
                    sprintf(p1,"%d",value);                 //将整型转换为字符串，作为结点的下方信息
                    if (M.vexs[node_num-1].node_id==node_num-1) //NODE(B)是处理当前四元式产生的新结点
                        node_num--;
                    if (FindNode(p1,M)==-1)                 //新常数为新的结点
                    {
                        strcpy(M.vexs[node_num].below,p1);  //为该结点的下方信息赋值（下方信息为char类型的数组）
                        M.vexs[node_num].node_id=node_num;  //为结点编号赋值
                        node_num++;                         //整个图的结点数加1
                    }
                }
                else
                {
                    index4=FindNode1(quate[i].two,M);      //找到相同的单目运算
                    if (index4==-1)                         //没有相同的结点存在
                    {
                        strcpy(M.vexs[node_num].below,"-"); //为该结点的下方信息赋值（下方信息为char类型的数组）
                        M.vexs[node_num].node_id=node_num;  //为结点编号赋值
                        M.arcs[node_num-1][node_num].adj=1; //矩阵中边赋值
                        node_num++;                         //整个图的结点数加1
                        goto label1;
                    }
                    else
                    {
                        index1=FindNode(quate[i].four,M); //查看A结点是否存在
                        if (index1==-1)                     //A不存在
                        {
                            l=0;
                            while(quate[i].four[l]!='\0')
                                l++;
                            for (j2=0; j2<=l; j2++)
                                M.vexs[index4].right[right_num[index4]][j2]=quate[i].four[j2];
                            right_num[index4]++;            //该结点右方信息个数加1
                        }
                        else        //该变量结点已经存在
                        {
                            for (j=1; j<node_num; j++)          //查看该结点是否为叶子结点
                                if (M.arcs[j][index1].adj==1)   //该结点不是叶子结点
                                    index2=0;
                            if (index2==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                            {
                                for (j1=0; j1<right_num[index1]; j1++)
                                    if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                        strcpy(M.vexs[index1].right[j1],"");
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index4].right[right_num[index4]][j2]=quate[i].four[j2];
                                right_num[index4]++;
                            }
                            else
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index4].right[right_num[index4]][j2]=quate[i].four[j2];
                                right_num[index4]++;
                            }
                        }
                    }
                }
            }
            else
            {
                for (j=0; j<strlen(M.vexs[index].below); j++)       //判断该结点是否为常数
                    if (M.vexs[index].below[j]>'9'||M.vexs[index].below[j]<'0')
                        index2=0;                   //该结点不是常数
                for (j1=0; j1<node_num; j1++)       //查看该结点是否为叶子结点
                    if (M.arcs[j1][index].adj==1)   //该结点不是叶子结点
                        index3=0;
                if (index2==1&&index3==1)           //是叶子结点且为常数
                {
                    value=atoi(M.vexs[index].below);//将字符数组转换为整型进行运算
                    value=0-value;
                    sprintf(p1,"%d",value);         //将整型转换为字符串，作为结点的下方信息
                    index5=FindNode(p1,M);
                    if (index5==-1)                 //新常数为新的结点
                    {
                        strcpy(M.vexs[node_num].below,p1);  //为该结点的下方信息赋值（下方信息为char类型的数组）
                        M.vexs[node_num].node_id=node_num;  //为结点编号赋值
                        node_num++;                         //整个图的结点数加1
                        goto label1;
                    }
                    else
                    {
                        index1=FindNode(quate[i].four,M); //查看A结点是否存在
                        if (index1==-1)                     //A不存在
                        {
                            l=0;
                            while(quate[i].four[l]!='\0')
                                l++;
                            for (j2=0; j2<=l; j2++)
                                M.vexs[index5].right[right_num[index5]][j2]=quate[i].four[j2];
                            right_num[index5]++;            //该结点右方信息个数加1
                        }
                        else         //该变量结点已经存在
                        {
                            for (j=1; j<node_num; j++)          //查看该结点是否为叶子结点
                                if (M.arcs[j][index1].adj==1)   //该结点不是叶子结点
                                    index2=0;
                            if (index2==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                            {
                                for (j1=0; j1<right_num[index1]; j1++)
                                    if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                        strcpy(M.vexs[index1].right[j1],"");
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index5].right[right_num[index5]][j2]=quate[i].four[j2];
                                right_num[index5]++;
                            }
                            else
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index5].right[right_num[index5]][j2]=quate[i].four[j2];
                                right_num[index5]++;
                            }
                        }
                    }
                }
                else
                {
                    index4=FindNode1(quate[i].two,M);      //找到相同的单目运算
                    if (index4==-1)                         //没有相同的结点存在
                    {
                        strcpy(M.vexs[node_num].below,"-"); //为该结点的下方信息赋值（下方信息为char类型的数组）
                        M.vexs[node_num].node_id=node_num;  //为结点编号赋值
                        M.arcs[node_num-1][node_num].adj=1; //矩阵中边赋值
                        node_num++;                         //整个图的结点数加1
                        goto label1;
                    }
                    else
                    {
                        index1=FindNode(quate[i].four,M); //查看A结点是否存在
                        if (index1==-1)                     //A不存在
                        {
                            l=0;
                            while(quate[i].four[l]!='\0')
                                l++;
                            for (j2=0; j2<=l; j2++)
                                M.vexs[index4].right[right_num[index4]][j2]=quate[i].four[j2];
                            right_num[index4]++;            //该结点右方信息个数加1
                        }
                        else        //该变量结点已经存在
                        {
                            index2=1;
                            for (j=1; j<node_num; j++)          //查看该结点是否为叶子结点
                                if (M.arcs[j][index1].adj==1)   //该结点不是叶子结点
                                    index2=0;
                            if (index2==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                            {
                                for (j1=0; j1<right_num[index1]; j1++)
                                    if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                        strcpy(M.vexs[index1].right[j1],"");
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index4].right[right_num[index4]][j2]=quate[i].four[j2];
                                right_num[index4]++;
                            }
                            else
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index4].right[right_num[index4]][j2]=quate[i].four[j2];
                                right_num[index4]++;
                            }
                        }
                    }
                }
            }
        }
        else if (quate[i].kind==2)      //双目运算(op,B,C,A)
        {
            index=FindNode(quate[i].two,M);
            if(index==-1)               //该四元式的的B结点没有定义过，生成新结点
            {
                strcpy(M.vexs[node_num].below,quate[i].two);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                node_num++;                                     //整个图的结点数加1
                index1=FindNode(quate[i].three,M);
                if (index1==-1)
                {
                    strcpy(M.vexs[node_num].below,quate[i].three);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    node_num++;                                     //整个图的结点数加1
                    for (j=0; j<strlen(M.vexs[node_num-2].below); j++)
                        if (M.vexs[node_num-2].below[j]>'9'||M.vexs[node_num-2].below[j]<'0')
                            index2=0;               //该结点不是常数
                    for (j1=1; j1<node_num; j1++)               //查看该结点是否为叶子结点
                        if (M.arcs[j1][node_num-2].adj==1)      //该结点不是叶子结点
                            index3=0;
                    for (j=0; j<strlen(M.vexs[node_num-1].below); j++)
                        if (M.vexs[node_num-1].below[j]>'9'||M.vexs[node_num-1].below[j]<'0')
                            index4=0;               //该结点不是常数
                    for (j1=1; j1<node_num; j1++)               //查看该结点是否为叶子结点
                        if (M.arcs[j1][node_num-1].adj==1)      //该结点不是叶子结点
                            index5=0;
                    if (index2==1&&index3==1&&index4==1&&index5==1)     //B和C都是标记为常数的叶子结点
                    {
                        value=atoi(M.vexs[node_num-2].below);           //将字符数组转换为整型进行运算
                        value1=atoi(M.vexs[node_num-1].below);
                        if (strcmp(quate[i].one,"+")==0)
                            value=value+value1;
                        else if(strcmp(quate[i].one,"*")==0)
                            value=value*value1;
                        sprintf(p2,"%d",value);                         //将整型转换为字符串，作为结点的下方信息
                        if (M.vexs[node_num-1].node_id==node_num-1)     //NODE(B)是处理当前四元式产生的新结点
                            node_num--;
                        if (M.vexs[node_num-1].node_id==node_num-1)     //NODE(C)是处理当前四元式产生的新结点
                            node_num--;
                        index6=FindNode(p2,M);
                        if (index6==-1)             //产生的新常数是新结点
                        {
                            strcpy(M.vexs[node_num].below,p2);          //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;          //为结点编号赋值
                            node_num++;                                 //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index1=FindNode(quate[i].four,M);     //查看A结点是否存在
                            if (index1==-1)                         //A不存在
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;                //该结点右方信息个数加1
                            }
                            else            //该变量结点已经存在
                            {
                                index2=1;
                                for (j=1; j<node_num; j++)              //查看该结点是否为叶子结点
                                    if (M.arcs[j][index1].adj==1)       //该结点不是叶子结点
                                        index2=0;
                                if (index2==0)                          //不是叶子结点，要将已存在结点右方的对应信息删除
                                {
                                    for (j1=0; j1<right_num[index1]; j1++)
                                        if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index1].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                    else
                    {
                        index7=FindNode(quate[i].two,M);
                        index8=FindNode(quate[i].three,M);
                        index6=FindNode2(quate[i].two,quate[i].three,quate[i].one,M);
                        if (index6==-1)             //没有这样的结点存在，需要新生成一个结点
                        {
                            strcpy(M.vexs[node_num].below,quate[i].one);     //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                            M.arcs[index7][node_num].adj=1;
                            M.arcs[index8][node_num].adj=1;
                            node_num++;                                     //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index=FindNode(quate[i].four,M);      //查看该语句第四区段的变量结点是否存在
                            if (index==-1)                          //赋值到的那个变量是新结点（不存在）
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;                //该结点右方信息个数加1
                            }
                            else            //该变量结点已经存在
                            {
                                index1=1;
                                for (j=1; j<node_num; j++)          //查看该结点是否为叶子结点
                                    if (M.arcs[j][index].adj==1)    //该结点不是叶子结点
                                        index1=0;
                                if (index1==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                                {
                                    for (j1=0; j1<right_num[index]; j1++)
                                        if (strcmp(M.vexs[index].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                }
                else    //C结点被生成过
                {
                    for (j=0; j<strlen(M.vexs[node_num-1].below); j++)
                        if (M.vexs[node_num-1].below[j]>'9'||M.vexs[node_num-1].below[j]<'0')
                            index2=0;       //该结点不是常数
                    for (j1=1; j1<node_num; j1++)               //查看该结点是否为叶子结点
                        if (M.arcs[j1][node_num-1].adj==1)      //该结点不是叶子结点
                            index3=0;
                    for (j=0; j<strlen(M.vexs[index1].below); j++)
                        if (M.vexs[index1].below[j]>'9'||M.vexs[index1].below[j]<'0')
                            index4=0;       //该结点不是常数
                    for (j1=1; j1<node_num; j1++)               //查看该结点是否为叶子结点
                        if (M.arcs[j1][index].adj==1)           //该结点不是叶子结点
                            index5=0;
                    if (index2==1&&index3==1&&index4==1&&index5==1) //B和C都是标记为常数的叶子结点
                    {
                        value=atoi(M.vexs[node_num-1].below);       //将字符数组转换为整型进行运算
                        value1=atoi(M.vexs[index1].below);
                        if (strcmp(quate[i].one,"+")==0)
                            value=value+value1;
                        else if(strcmp(quate[i].one,"*")==0)
                            value=value*value1;
                        sprintf(p2,"%d",value);         //将整型转换为字符串，作为结点的下方信息
                        if (M.vexs[node_num-1].node_id==node_num-1)     //NODE(B)是处理当前四元式产生的新结点
                            node_num--;
                        index6=FindNode(p2,M);
                        if (index6==-1)                 //产生的新常数是新结点
                        {
                            strcpy(M.vexs[node_num].below,p2);          //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;          //为结点编号赋值
                            node_num++;                                 //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index1=FindNode(quate[i].four,M);     //查看A结点是否存在
                            if (index1==-1)                         //A不存在
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;                //该结点右方信息个数加1
                            }
                            else            //该变量结点已经存在
                            {
                                index2=1;
                                for (j=1; j<node_num; j++)              //查看该结点是否为叶子结点
                                    if (M.arcs[j][index1].adj==1)       //该结点不是叶子结点
                                        index2=0;
                                if (index2==0)                          //不是叶子结点，要将已存在结点右方的对应信息删除
                                {
                                    for (j1=0; j1<right_num[index1]; j1++)
                                        if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index1].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                    else
                    {
                        index7=FindNode(quate[i].two,M);
                        index8=FindNode(quate[i].three,M);
                        index6=FindNode2(quate[i].two,quate[i].three,quate[i].one,M);
                        if (index6==-1)             //没有这样的结点存在，需要新生成一个结点
                        {
                            strcpy(M.vexs[node_num].below,quate[i].one);         //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;                  //为结点编号赋值
                            M.arcs[index7][node_num].adj=1;
                            M.arcs[index8][node_num].adj=1;
                            node_num++;                                         //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index=FindNode(quate[i].four,M);  //查看该语句第四区段的变量结点是否存在
                            if (index==-1)                      //赋值到的那个变量是新结点（不存在）
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;            //该结点右方信息个数加1
                            }
                            else            //该变量结点已经存在
                            {
                                index1=1;
                                for (j=1; j<node_num; j++)          //查看该结点是否为叶子结点
                                    if (M.arcs[j][index].adj==1)    //该结点不是叶子结点
                                        index1=0;
                                if (index1==0)                      //不是叶子结点，要将已存在结点右方的对应信息删除
                                {
                                    for (j1=0; j1<right_num[index]; j1++)
                                        if (strcmp(M.vexs[index].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                }
            }
            else  //B结点被定义过
            {
                index1=FindNode(quate[i].three,M);
                if (index1==-1)
                {
                    strcpy(M.vexs[node_num].below,quate[i].three);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    node_num++;                                     //整个图的结点数加1
                    for (j=0; j<strlen(M.vexs[node_num-2].below); j++)
                        if (M.vexs[node_num-2].below[j]>'9'||M.vexs[node_num-2].below[j]<'0')
                            index2=0;               //该结点不是常数
                    for (j1=1; j1<node_num; j1++)               //查看该结点是否为叶子结点
                        if (M.arcs[j1][node_num-2].adj==1)      //该结点不是叶子结点
                            index3=0;
                    for (j=0; j<strlen(M.vexs[node_num-1].below); j++)
                        if (M.vexs[node_num-1].below[j]>'9'||M.vexs[node_num-1].below[j]<'0')
                            index4=0;           //该结点不是常数
                    for (j1=1; j1<node_num; j1++)               //查看该结点是否为叶子结点
                        if (M.arcs[j1][node_num-1].adj==1)      //该结点不是叶子结点
                            index5=0;
                    if (index2==1&&index3==1&&index4==1&&index5==1) //B和C都是标记为常数的叶子结点
                    {
                        value=atoi(M.vexs[node_num-2].below);       //将字符数组转换为整型进行运算
                        value1=atoi(M.vexs[node_num-1].below);
                        if (strcmp(quate[i].one,"+")==0)
                            value=value+value1;
                        else if(strcmp(quate[i].one,"*")==0)
                            value=value*value1;
                        sprintf(p2,"%d",value);         //将整型转换为字符串，作为结点的下方信息
                        if (M.vexs[node_num-1].node_id==node_num-1)     //NODE(C)是处理当前四元式产生的新结点
                            node_num--;
                        index6=FindNode(p2,M);
                        if (index6==-1)                 //产生的新常数是新结点
                        {
                            strcpy(M.vexs[node_num].below,p2);          //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;          //为结点编号赋值
                            node_num++;                                 //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index1=FindNode(quate[i].four,M);     //查看A结点是否存在
                            if (index1==-1)                         //A不存在
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];

                                right_num[index6]++;    //该结点右方信息个数加1
                            }
                            else        //该变量结点已经存在
                            {
                                index2=1;
                                for (j=1; j<node_num; j++)
                                    if (M.arcs[j][index1].adj==1)
                                        index2=0;
                                if (index2==0)
                                {
                                    for (j1=0; j1<right_num[index1]; j1++)
                                        if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index1].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                    else
                    {
                        index7=FindNode(quate[i].two,M);
                        index8=FindNode(quate[i].three,M);
                        index6=FindNode2(quate[i].two,quate[i].three,quate[i].one,M);
                        if (index6==-1)         //没有这样的结点存在，需要新生成一个结点
                        {
                            strcpy(M.vexs[node_num].below,quate[i].one); //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;          //为结点编号赋值
                            M.arcs[index7][node_num].adj=1;
                            M.arcs[index8][node_num].adj=1;
                            node_num++;                                 //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index=FindNode(quate[i].four,M);  //查看该语句第四区段的变量结点是否存在
                            if (index==-1)                      //赋值到的那个变量是新结点（不存在）
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;            //该结点右方信息个数加1
                            }
                            else         //该变量结点已经存在
                            {
                                index1=1;
                                for (j=1; j<node_num; j++)
                                    if (M.arcs[j][index].adj==1)
                                        index1=0;
                                if (index1==0)
                                {
                                    for (j1=0; j1<right_num[index]; j1++)
                                        if (strcmp(M.vexs[index].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                }
                else //C结点被生成过
                {
                    for (j=0; j<strlen(M.vexs[index].below); j++)
                        if (M.vexs[index].below[j]>'9'||M.vexs[index].below[j]<'0')
                            index2=0; //该结点不是常数
                    for (j1=1; j1<node_num; j1++)
                        if (M.arcs[j1][index].adj==1)
                            index3=0;
                    for (j=0; j<strlen(M.vexs[index1].below); j++)
                        if (M.vexs[index1].below[j]>'9'||M.vexs[index1].below[j]<'0')
                            index4=0;       //该结点不是常数
                    for (j1=1; j1<node_num; j1++)
                        if (M.arcs[j1][index1].adj==1)
                            index5=0;
                    if (index2==1&&index3==1&&index4==1&&index5==1)
                    {
                        value=atoi(M.vexs[index1].below);       //将字符数组转换为整型进行运算
                        value1=atoi(M.vexs[index].below);
                        if (strcmp(quate[i].one,"+")==0)
                            value=value+value1;
                        else if(strcmp(quate[i].one,"*")==0)
                            value=value*value1;
                        sprintf(p2,"%d",value);                 //将整型转换为字符串，作为结点的下方信息
                        index6=FindNode(p2,M);
                        if (index6==-1)                         //产生的新常数是新结点
                        {
                            strcpy(M.vexs[node_num].below,p2);  //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;  //为结点编号赋值
                            node_num++;                         //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index1=FindNode(quate[i].four,M); //查看A结点是否存在
                            if (index1==-1)                     //A不存在
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;            //该结点右方信息个数加1
                            }
                            else        //该变量结点已经存在
                            {
                                index2=1;
                                for (j=1; j<node_num; j++)
                                    if (M.arcs[j][index1].adj==1)
                                        index2=0;
                                if (index2==0)
                                {
                                    for (j1=0; j1<right_num[index1]; j1++)
                                        if (strcmp(M.vexs[index1].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index1].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                    else
                    {
                        index7=FindNode(quate[i].two,M);
                        index8=FindNode(quate[i].three,M);
                        index6=FindNode2(quate[i].two,quate[i].three,quate[i].one,M);
                        if (index6==-1)         //没有这样的结点存在，需要新生成一个结点
                        {
                            strcpy(M.vexs[node_num].below,quate[i].one);     //为该结点的下方信息赋值（下方信息为char类型的数组）
                            M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                            M.arcs[index7][node_num].adj=1;
                            M.arcs[index8][node_num].adj=1;
                            node_num++;                                     //整个图的结点数加1
                            goto label1;
                        }
                        else
                        {
                            index=FindNode(quate[i].four,M);      //查看该语句第四区段的变量结点是否存在
                            if (index==-1)                          //赋值到的那个变量是新结点（不存在）
                            {
                                l=0;
                                while(quate[i].four[l]!='\0')
                                    l++;
                                for (j2=0; j2<=l; j2++)
                                    M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                right_num[index6]++;                 //该结点右方信息个数加1
                            }
                            else        //该变量结点已经存在
                            {
                                index1=1;
                                for (j=1; j<node_num; j++)
                                    if (M.arcs[j][index].adj==1)
                                        index1=0;
                                if (index1==0)
                                {
                                    for (j1=0; j1<right_num[index]; j1++)
                                        if (strcmp(M.vexs[index].right[j1],quate[i].four)==0)
                                            strcpy(M.vexs[index].right[j1],"");
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                                else
                                {
                                    l=0;
                                    while(quate[i].four[l]!='\0')
                                        l++;
                                    for (j2=0; j2<=l; j2++)
                                        M.vexs[index6].right[right_num[index6]][j2]=quate[i].four[j2];
                                    right_num[index6]++;
                                }
                            }
                        }
                    }
                }

            }
        }
        else if (quate[i].kind==3)  //无条件转移语句(jump,-,-,s)
        {
            M.vexs[node_num].node_id=node_num;          //为结点编号赋值
            l=0;
            while(quate[i].four[l]!='\0')
                l++;
            for (j2=0; j2<=l; j2++)
                M.vexs[node_num].right[right_num[node_num]][j2]=quate[i].four[j2];
            strcpy(M.vexs[node_num].below,"");
            right_num[node_num]++;
            node_num++;                                 //整个图的结点数加1
        }
        else if (quate[i].kind==4) //条件转移语句(jrop,B,C,s)
        {
            index=FindNode(quate[i].two,M);
            if (index==-1)          //B结点没有被定义过
            {
                strcpy(M.vexs[node_num].below,quate[i].two);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                node_num++;                                     //整个图的结点数加1
                index1=FindNode(quate[i].three,M);
                if (index1==-1)
                {
                    strcpy(M.vexs[node_num].below,quate[i].three);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    node_num++;                                     //整个图的结点数加1
                    strcpy(M.vexs[node_num].below,quate[i].one);     //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    M.arcs[node_num-2][node_num].adj=1;
                    M.arcs[node_num-1][node_num].adj=1;
                    node_num++;                                     //整个图的结点数加1
                    goto label1;
                }
                else
                {
                    strcpy(M.vexs[node_num].below,quate[i].one);     //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    M.arcs[index1][node_num].adj=1;
                    M.arcs[node_num-1][node_num].adj=1;
                    node_num++;                                     //整个图的结点数加1
                    goto label1;
                }
            }
            else
            {
                index1=FindNode(quate[i].three,M);
                if (index1==-1)
                {
                    strcpy(M.vexs[node_num].below,quate[i].three);   //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    node_num++;                                     //整个图的结点数加1
                    strcpy(M.vexs[node_num].below,quate[i].one);     //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    M.arcs[index][node_num].adj=1;
                    M.arcs[node_num-1][node_num].adj=1;
                    node_num++;                                     //整个图的结点数加1
                    goto label1;
                }
                else
                {
                    strcpy(M.vexs[node_num].below,quate[i].one);     //为该结点的下方信息赋值（下方信息为char类型的数组）
                    M.vexs[node_num].node_id=node_num;              //为结点编号赋值
                    M.arcs[index1][node_num].adj=1;
                    M.arcs[index][node_num].adj=1;
                    node_num++;                                     //整个图的结点数加1
                    goto label1;
                };
            }
        }
    }
}

//在DAG图中查找一个编号为n的结点是否有后继结点，返回后继结点的个数，传入参数是要查找的结点编号、DAG图以及储存后继结点编号的数组。
int FindKinds(int n,MGraph M,int kinds[])
{
    int i,x=0;
    memset(kinds,0,sizeof(kinds)); //初始化
    for (i=0; i<node_num; i++)
        if (M.arcs[i][n].adj==1)
        {
            kinds[x]=i;
            x++;
        }
    return x;
}

//产生优化后的四元式
void CreatQua_optimize(int l,char o[],char a1[],char a2[],char r[])
{
    unsigned int i;
    quate_optimize[x1].order=l;
    for (i=0; i<=strlen(o); i++)
        quate_optimize[x1].one[i]=o[i];
    for (i=0; i<=strlen(a1); i++)
        quate_optimize[x1].two[i]=a1[i];
    for (i=0; i<=strlen(a2); i++)
        quate_optimize[x1].three[i]=a2[i];
    for (i=0; i<=strlen(r); i++)
        quate_optimize[x1].four[i]=r[i];
}

//对图进行局部优化
void Optimize()
{
    unsigned int j;
    int i,i1;
    int x=1,index,index1;
    for (i1=0; i1<block_num; i1++)
    {
        CreatDAG(B[i1]);        //生成基本块的DAG图
        for (i=0; i<node_num; i++)
        {
            if (right_num[i]!=0)
            {
                if (FindKinds(i,M,kinds)==0) //该结点没有后继结点
                {
                    if (strcmp(M.vexs[i].below,"")==0) //该结点没有下方信息是无条件转移语句
                    {
                        CreatQua_optimize(x,jump1,valid1,valid1,M.vexs[i].right[0]);
                        x++;
                        x1++;
                    }
                    else //该结点存在下方信息是赋值语句
                    {
                        CreatQua_optimize(x,equ1,M.vexs[i].below,valid1,M.vexs[i].right[0]);
                        x++;
                        x1++;
                    }
                }
                else if (FindKinds(i,M,kinds)==1) //该结点有一个后继结点,是单目运算
                {
                    CreatQua_optimize(x,sub1,M.vexs[kinds[0]].below,valid1,M.vexs[i].right[0]);
                    x++;
                    x1++;
                }
                else if (FindKinds(i,M,kinds)==2) //有两个后继结点
                {
                    if (M.vexs[i].below[0]=='j') //是条件转移语句
                    {
                        CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].below,M.vexs[kinds[1]].below,M.vexs[i].right[0]);
                        x++;
                        x1++;
                    }
                    else //是双目运算语句
                    {
                        index=1;
                        for (j=0; j<strlen(M.vexs[kinds[0]].below); j++)
                            if ((M.vexs[kinds[0]].below[j]>'9'||M.vexs[kinds[0]].below[j]>'9')&&M.vexs[kinds[0]].below[j]!='.')
                                index=0; //不是常数
                        index1=1;
                        for (j=0; j<strlen(M.vexs[kinds[1]].below); j++)
                            if ((M.vexs[kinds[1]].below[j]>'9'||M.vexs[kinds[1]].below[j]>'9')&&M.vexs[kinds[1]].below[j]!='.')
                                index1=0; //不是常数
                        if (right_num[kinds[0]]==0&&right_num[kinds[1]]==0)
                        {
                            CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].below,M.vexs[kinds[1]].below,M.vexs[i].right[0]);
                            x++;
                            x1++;
                        }
                        else if (right_num[kinds[0]]!=0&&right_num[kinds[1]]==0)
                        {
                            if (index==0)
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].right[0],M.vexs[kinds[1]].below,M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                            else
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].below,M.vexs[kinds[1]].below,M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                        }
                        else if (right_num[kinds[0]]==0&&right_num[kinds[1]]!=0)
                        {
                            if (index1==0)
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].below,M.vexs[kinds[1]].right[0],M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                            else
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].below,M.vexs[kinds[1]].below,M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                        }
                        else if (right_num[kinds[0]]!=0&&right_num[kinds[1]]!=0)
                        {
                            if (index==0&&index1==0) //两个都不是常数
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].right[0],M.vexs[kinds[1]].right[0],M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                            else if (index==0&&index1==1)
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].right[0],M.vexs[kinds[1]].below,M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                            else if (index==1&&index1==0)
                            {
                                CreatQua_optimize(x,M.vexs[i].below,M.vexs[kinds[0]].below,M.vexs[kinds[1]].right[0],M.vexs[i].right[0]);
                                x++;
                                x1++;
                            }
                        }
                    }
                }
            }
        }
        B[i1].reduce_num=B[i1].out-B[i1].entry+1-(x-1);
        x=1;
    }
}

//为优化后的产生式重新编号并对对应的跳转语句的第四区段赋值，传入的参数为优化后的四元式数组
void  RefreshLabel(Qua quate_optimize[])
{
    int i,j,untouch=0,j1;
    int p,p1,index;
    char p2[5];
    for (i=1; i<x1; i++)        //为四元式的编号赋值
        quate_optimize[i].order=i;
    for (i=1; i<nextstate; i++)
        if (quate[i].sym==0)    //该语句不可达
        {
            untouch++;          //不可达的语句数量
            index=i;
        }
    for (i=1; i<x1; i++)        //为所有优化之后的四元式的第四区段重新赋值
    {
        if (quate_optimize[i].one[0]=='j'&&quate_optimize[i].four[0]!='0') //是跳转语句
        {
            p=atoi(quate_optimize[i].four); //将第四区段转化为数字
            for (j=0; j<block_num; j++)
                if (p>=B[j].entry&&p<=B[j].out) //现在的p在该分区中
                {
                    if (p<index)
                        p1=p;
                    else
                        p1=p-untouch;
                    for (j1=0; j1<j; j1++)
                        p1=p1-B[j1].reduce_num;
                }
            sprintf(p2,"%d",p1);
            strcpy(quate_optimize[i].four,p2);
        }
    }
    cout<<endl<<"四元式（优化后）："<<endl;
    for (i=1; i<x1; i++)
    {
        cout<<"\t"<<quate_optimize[i].order<<'.'<<'('<<quate_optimize[i].one;
        cout<<','<<quate_optimize[i].two<<','<<quate_optimize[i].three<<','<<quate_optimize[i].four<<')'<<endl;
    }
}
