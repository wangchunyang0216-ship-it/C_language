#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define M 300
//本关任务：编写一个能计算两个n（16<=n<=256）位整数的加减法。
//首先，大整数的位数n由用户从键盘输入；
//接下来请你设计一个数据结构用来保存一个n位的整数，你可以尝试采用顺序表或者链表的方式。
//将两个n位的大整数求和或求差，基本方法可以从低位算起，按位加减，这是需要注意进位和借位的处理。
//加减法的结果可以存储在另一个存储空间中。
//测试输入：
//2222222222222222
//+
//6666666666666666
//预期输出：
//结果:8888888888888888
int main()
{
    int i,j,carry=0,borrow=0;//carry进位，borrow借位
    char op;//op为运算符号
    char a1[M];//输入第一个大整数
    char a2[M];//输入第二个大整数
    char out1[M];
    gets(a1);
    fflush(stdin);
    scanf("%c",&op);
    fflush(stdin);
    //gets(op);
    gets(a2);
    //printf("%d\n",strlen(a1));
    for(i=strlen(a1);i>=0;i--)
    {
        if(op=='+')
            out1[i]='a1[i]'-48+'a2[i]'-48+48;
    }
    printf("%s +",a1);
    printf("%s=%s",a2,out1);
    return 0;
}
