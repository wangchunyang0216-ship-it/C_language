#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char c[20];
}Str;
void Split(char* buf,char* arg1[]);
int main()
{




    char* arg1[10];

    char* buf="ls -l -o -t";
    Split(buf,arg1);


    return 0;
}



void Split(char* buf,char* arg1[]){
    char* argv[10];
    char c[20];
    Str S[10];

        char b[30];
//    for(int i=0;i<strlen(buf);i++){
//        b[i]=buf[i];
//    }

    for(int i=0;i<10;i++){

        memset(S[i].c,'\0',sizeof(S[i].c));//初始化
    }

    char temp[30];
    strcpy(b, buf);
//    b[i+1]='\0';
    printf("b=%s,b's length is %d\n",b,strlen(b));
    puts(b);
    int count=0;
    int count_sum=0;
   // buf="ls -l -o";
   printf("buf='%s',buf's length is %d\n",buf,strlen(buf));
//   for(int i=0;i<strlen(buf);i++){
//    if (buf[i]==' ')
//    {
//        count++;
//        count_sum=count;
//        memset(c,'\0',sizeof(c));
//    }
//    c[i]=buf[i];
//    argv[count]=c;
//   }
    for(int i=0;i<strlen(buf);i++){
        if(buf[i]==' ')
            count_sum++;
    }
    for(int count_1=0;count_1<count_sum;count_1++)
        {

        for(int i=0;i<strlen(b);i++)
            {

            if (b[i]==' ')
                {
                    //b的空格前面给c
                    int a=0;
    //                for(int aa=0;aa<i;aa++){
    //
    //                    c[a]=b[i];
    //                    a++;
    //                }
                    memset(c,'\0',sizeof(c));
                    strncpy(c,b,i);
                    strncpy(S[count].c,b,i);
                    //strncpy_s(str1, str2, 3);//将前3个字符复制到str1中
                    printf("c=%s\n",c);
                    printf("S[%d].c=%s\n",count,c);


                    //将前面那一半c赋值给argv[i]
                    argv[count]=c;
                    arg1[count]=S[count].c;
                    //strcpy(&argv[count],c);
                    count++;
                    //b的空格后面给b，b重新赋值
                    int x=0;

                    char xin[20];
                    memset(xin,'\0',sizeof(xin));
                    for(int bb=i+1;bb<strlen(b);bb++){
                        //temp[x]=
                        xin[x]=b[bb];
                        x++;
                    }
                    xin[x]='\0';
                    printf("新的char数组xin【】： %s\n",xin);
                    strcpy(b,xin);
                    printf("b:");
                    puts(b);
                    printf("b's length is %d\n",strlen(b));


                    break;

                }//if
            //c[i]=buf[i];
           // argv[count]=c;//!!原来
           // strcpy(argv[count],c);
            //count++;
            //memset(b,'\0',sizeof(b));
           }//b

    }//count

    arg1[count]=b;
    arg1[count+1]=NULL;

   //count_sum=count;

    printf("b='%s',b's length is %d\n",b,strlen(b));
     for(count=0;count<count_sum+2;count++)
        {
      //  printf("argv[%d]=%s\n",count,argv[count]);
        printf("arg1[%d]=%s\n",count,arg1[count]);


        }

}
