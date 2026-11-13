#include <stdio.h>
#include <stdlib.h>
int S[100];//n个整数集合S
int A[100];
int y1=0;
int n,y;//整数个数n和y
/*
解的元组大小及取值范围
本次子集和问题的解采用固定的n元组s[1~n]，元素的取值范围为{0，1}，0代表数字没有被选中，1代表选中，n元组中所有s[i]取1的数字之和等于y。
约束条件
子集和问题的约束条件为所有s[i]取值为1的数字之和小于等于y。

平台会对你编写的代码进行测试：
输出所有和为y的元组
输入第一行：整数个数n和y
输入第二行：n个整数
输出：所有满足条件的解元组
测试输入：
6 60
10 20 30 40 50 60
预期输出：
1 1 1 0 0 0
1 0 0 0 1 0
0 1 0 1 0 0
0 0 0 0 0 1
*/
void com(int k);//递归
int main()
{
    int i,j;
    scanf("%d %d",&n,&y);
    for(i=0;i<n;i++)
    {
        scanf("%d",&S[i]);
    }
    //com(0);//递归
    int k=0;
//    int c[10];
//    for(k=1;k<=n;k++)
//        c[k]=0;
//    k=0;
    i=0;
    while(k>=0)
    {
//        while(i<n)
//        {
            A[k]=1;
            y1+=S[k];
            if(y1==y)
            {
                for(int j=0;j<n-1;j++)
                    printf("%d ",A[j]);
                printf("%d\n",A[n-1]);
            }
            else if(y1<y)
            {
                k++;
            }

            //i++;
//        }
        else //if(y1>y)
        {
            A[k]=0;
        y1-=S[k];
        k--;
        }

    }
    return 0;
}

void com(int k)//递归
{
    if(k==n) return;
    A[k]=1;
    y1+=S[k];
    if(y1==y)//如果是合法解，即子集和恰好等于y（60）
    {
        for(int j=0;j<n-1;j++)
            printf("%d ",A[j]);
        printf("%d",A[n-1]);
        printf("\n");
    }
    else if(y1<y)//如果是部分解，即子集小于y（60）
        com(k+1);
     //回溯
    A[k]=0;
    y1-=S[k];
    com(k+1);
    return;
}
