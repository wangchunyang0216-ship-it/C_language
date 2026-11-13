#include <stdio.h>
#include <stdlib.h>

//测试输入：第一行是项目个数n和投资钱数y，以下各行为每个项目的投资和收益。
//预期输出：最大收益:41万元
//int 0_1bag(S[20]);
struct scheme
{
    int invest;
    int income;
    //int income_lat;
}S[20];

void Write_Print(struct scheme S[20],int n)
{
        for(int i=1;i<=n;i++)
    {
        scanf("%d,%d",&S[i].invest,&S[i].income);
    }
//       for(int i=0;i<=n;i++)
//    {
//        printf("%d,%d\n",S[i].income_pre,S[i].invest);
//    }
}

//int 0_1bag(struct scheme S[20],int n)
//{
//    int i,j;
//    for(i=0;i<n;i++)
//        S[i].income_lat=0;
//    for(j=0;j<y;j++)
//        S[j].
//    for(i=0;i<n;i++)//n种方案
//    {
//        for(j=0;j<y;j++)//y元钱的总投资额即为背包的总容积
//        {
//            if(i==0 || j==0)
//            {
//                S[i].income_lat=0;
//
//            }
//            if()
//
//
//        }
//    }
//}


int main()
{
    int n,y;//项目个数n和投资钱数y
    int i,j;
    int V[20][100];
    int max_income=0;
    scanf("%d,%d",&n,&y);

    //Scheme[20] S;
    Write_Print(S,n);
    //max_income=0_1bag(S,n);
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
                max=V[i-1][j-S[i].invest]+S[i].income;
            V[i][j]=max;
        }
    }
    for(i=0;i<=n;i++)
    {
        for(j=0;j<=y;j++)
        {
            printf("%d ",V[i][j]);
        }
        printf("\n");
    }


    max_income=V[n][y];
    printf("最大收益:%d万元",max_income);
    return 0;
}
