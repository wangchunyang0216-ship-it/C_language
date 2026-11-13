#include <stdio.h>
#include <stdlib.h>

//测试输入：第一行是项目个数n和投资钱数y，以下各行为每个项目的投资和收益。
//预期输出：最大收益:41万元
//          投资项目有:10号 7号 6号 4号 1号
//int 0_1bag(S[20]);
struct scheme
{
    int invest;//背包容积
    int income;//背包价值
    //int temp;//记录路径
}S[30];

void Write_Print(struct scheme S[30],int n)
{
        for(int i=1;i<=n;i++)
    {
        scanf("%d,%d",&S[i].invest,&S[i].income);
    }
}


int main()
{
    int n,y;//项目个数n和投资钱数y
    int i,j;
    int V[200][1000];
    int a[100];//记录路径
    int max_income=0;
    scanf("%d,%d",&n,&y);
    Write_Print(S,n);
    for(i=0;i<=n;i++)
        V[i][0]=0;
    for(j=0;j<=y;j++)
        V[0][j]=0;
    for(i=1;i<=n;i++)
        for(j=1;j<=y;j++)
    {
        V[i][j]=V[i-1][j];
        if(S[i].invest<=j)
        {
            int max=0;
            if(V[i][j]>V[i-1][j-S[i].invest]+S[i].income)
                max=V[i][j];
            else
            {
               max=V[i-1][j-S[i].invest]+S[i].income;
//               S[i].temp=1;
//               a[k]=i;
//               k++;
            }

            V[i][j]=max;
        }
    }
    //int max_k=k-1;
    max_income=V[n][y];
    printf("最大收益:%d万元\n",max_income);
    //逆向构造最优解
    j=y;
    for(i=n;i>0;i--)
    {
        if(V[i][j]>V[i-1][j])
        {
            a[i]=1;
            j-=S[i].invest;//减去装入的每个背包的容积
        }
        else
            a[i]=0;
    }
    //输出
    printf("投资项目有:");
    for(i=n;i>0;i--)
    {
        if(a[i]==1)
            printf("%d号 ",i);
    }
    return 0;
}
