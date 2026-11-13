#include <stdio.h>
#include <stdlib.h>

/*

本关任务：有一个演播厅，每天早上8点开门，晚上22点关门，
演播厅每场活动收费固定，不按照活动时间长短来收费，
因此为了收益最大化，需要在开门时间内每天都安排最多场次的活动。
测试输入：
第一行：活动个数n
下列n行：每个活动的开始时间和结束时间

预期输出：
最多活动数
被安排的活动编号

测试输入：
9
1 8:00 9:30
2 8:20 9:10
3 8:40 9:20
4 9:10 10:30
5 10:00 11:00
6 10:30 12:00
7 12:30 14:30
8 14:40 16:30
9 14:00 15:10
预期输出：
5
2 4 6 7 8

*/

typedef struct time
{
    int hour;
    int minute;
}time;

typedef struct activity
{
    int sequence;
    time start;
    time end;
    int select;
}Activity;

int main()
{
    int n;//n个活动
    Activity A[10];
    scanf("%d",&n);
    int i,j;
    for(i=0;i<n;i++)
    {
        scanf("%d",&A[i].sequence);
        scanf("%d:%d ",&A[i].start.hour,&A[i].start.minute);
        scanf("%d:%d",&A[i].end.hour,&A[i].end.minute);
        A[i].select=0;
    }
    Activity temp;
    for(i=0;i<n;i++)//按活动结束最早时间排序
    {
        for(j=0;j<n-i;j++)
            if( A[j].end.hour>A[j+1].end.hour ||(A[j].end.hour==A[j+1].end.hour&&A[j].end.minute>A[j+1].end.minute)  )
            {
                temp=A[j];
                A[j]=A[j+1];
                A[j+1]=temp;
            }
    }
    Activity t={0,0,0,8,0};
    int k=0;
    for(i=0;i<n;i++)
    {
        if(A[i].start.hour>t.end.hour || (A[i].start.hour==t.end.hour&&A[i].start.minute>=t.end.minute)  )
        {
            A[i].select=1;
            k++;
            t=A[i];
        }
    }
    printf("%d\n",k);
//    for(i=0;i<n;i++)
//    {
//        printf("%d ",A[i].sequence);
//        printf("%d:%d ",A[i].start.hour,A[i].start.minute);
//        printf("%d:%d  ",A[i].end.hour,A[i].end.minute);
//        printf("select=%d\n",A[i].select);
//    }

    for(i=0;i<n;i++)
    {
        if(A[i].select==1)
            printf("%d ",A[i].sequence);
    }
    return 0;
}
