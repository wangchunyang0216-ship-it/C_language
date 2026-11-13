#include <stdio.h>
#include <stdlib.h>
#include"Max_elective.h"
//输入第一行：n,m（课程门数，最大选修门数）
//输入第二行：n门课程的学分数
//输入其他行：课程先修关系（i,j）
//   (i,j)表示i课程是j课程的先修课
//测试输入：
//10,6
//3,1,2,5,2,4,4,1,5,2
//1,2
//1,3
//1,4
//3,5
//3,6
//7,8
//7,9
//7,10
//预期输出：
//最大学分:23

//孩子兄弟表示法
//用二叉链表作树的存储结构，链表中每个结点的两个指针域分别指向其第一个孩子和下一个兄弟结点。

//typedef struct relationship
//{
//    int data;
//    struct relationship* firstchild;
//    struct relationship* nextsibling;
//}REL;


int main()
{
    int n,m;//n,m（课程门数，最大选修门数）
    int S[100];//n门课程的学分数
    //课程先修关系（i,j）  (i,j)表示i课程是j课程的先修课
    //课程先修关系用树形结构表示

    int i,j;
    scanf("%d,%d",&n,&m);
    for(i=0;i<n;i++)
    {
        scanf("%d",&S[i]);
    }
//    printf("%d,%d",n,m);
//    for(i=0;i<n;i++)
//    {
//        printf("%d",S[i]);
//    }

    REL R[10];
    for(i=0;i<8;i++)
    {
        R[i].data=0;
        R[i].firstchild=NULL;
        R[i].nextsibling=NULL;
    }
    for(i=0;i<8;i++)
    {
        if(R[i].firstchild==NULL)
            scanf("%d,%d",&R[i].firstchild->data,&R[i].nextsibling->data);
        else
        {

        }
    }


    return 0;
}
