//#include "morphology.h"
//#include "grammar.h"
//#include "semantic.h"

#include"keshe.h"
#include"stack.h"

//closure闭包函数，输出一个状态的项目集
void GetClosure(char l,char pro[],char forwardchar[],int ord)
{
    int h,x,x1,x2,e,index;
    int i,p,j,m,q,g,p1;
    int k=0,cir=0,sum=0;
    char first[10];
    char temp[10];     //暂时存放一个串
    char temp1[100];   //暂时将涉及到的非终结符储存
    int id[100];       //标识每个存入暂存数组的字符是由于第几条产生式加入的
    strcpy(temp1,"");  //初始化数组
    strcpy(temp,"");
    strcpy(first,"");
    state[ord].closure[k].left_lr1=l;                   //先将闭包函数的核心的项目的左部符号储存
    for (i=0; i<=strlen(pro); i++)                      //把项目的右部储存
        state[ord].closure[k].project_lr1[i]=pro[i];
    for (i=0; i<strlen(forwardchar); i++)               //将核心的向前搜索符存入
        state[ord].closure[k].fwd_char[i]=forwardchar[i];
    state[ord].closure[k].fwd_char[i]='\0';
    //第一个产生式存入，sum表示该集合现在存的产生式数量
    sum++;
    p=FindChar('.',state[ord].closure[k].project_lr1);      //找到小圆点存放的位置,若下一个不是非终结符直接结束
    if ((FindChar(state[ord].closure[k].project_lr1[p+1],terminal)==-1)&&(p<=strlen(state[ord].closure[k].project_lr1)))//是非终结符
    {
        //将该非终结符存入暂存数组
        x=strlen(temp1);
        temp1[x]=state[ord].closure[k].project_lr1[p+1];
        id[x]=k;
        temp1[x+1]='\0';
        //计算x,决定是否进入循环
        x=strlen(temp1);
        while(x!=0)
        {
            p=FindChar('.',state[ord].closure[cir].project_lr1);
            for (i=0; i<number; i++)                //遍历产生式
            {
                if ((grammar[i].left==temp1[cir]))  //找到与当前非终结符左部相等的产生式
                {
                    k++;                            //要存入下一个
                    state[ord].closure[k].left_lr1=grammar[i].left;     //存左部
                    for (j=0; j<=grammar[i].length+1; j++)              //存右部
                        state[ord].closure[k].project_lr1[j]=grammar[i].project[0][j];  //计算向前搜索符号
                    if(p+2==strlen(state[ord].closure[id[cir]].project_lr1))        //该非终结符的后边是空
                        for (m=0; m<strlen(state[ord].closure[id[cir]].fwd_char); m++)
                            state[ord].closure[k].fwd_char[m]=state[ord].closure[id[cir]].fwd_char[m];
                    else            //给非终结符后边不空，其向前搜索符就是该非终结符后边的内容与当前的非终结符组成的串的FIRST集
                    {
                        q=strlen(temp);             //构造临时串
                        for (g=p+2; g<strlen(state[ord].closure[id[cir]].project_lr1); g++)
                        {
                            temp[q]=state[ord].closure[id[cir]].project_lr1[g];
                            q++;
                        }
                        for (h=0; h<strlen(state[ord].closure[id[cir]].fwd_char); h++)
                        {
                            temp[q]=state[ord].closure[id[cir]].fwd_char[h];
                            q++;
                        }
                        temp[q]='\0';
                        FIRST(temp,first);                  //计算串的FIRST集
                        for (e=0; e<strlen(first); e++)     //该串的FIRST集为该项目的向前搜索符
                            state[ord].closure[k].fwd_char[e]=first[e];
                        memset(temp,0,sizeof(temp));        //清空数组为下一次计算做准备
                        memset(first,0,sizeof(first));
                    }
                    if (FindProject(state[ord].closure[k],sum,ord)==-1) //不在已经产生的产生式中(不包括向前搜索符)
                    {
                        sum++;          //存入完成，sum++
                        p1=FindChar('.',state[ord].closure[k].project_lr1);
                        if(FindChar(state[ord].closure[k].project_lr1[p1+1],terminal)==-1) //是非终结符
                        {
                            x=strlen(temp1);
                            temp1[x]=state[ord].closure[k].project_lr1[p1+1];
                            id[x]=k;
                            temp1[x+1]='\0';
                        }
                    }
                    else
                    {
                        index=FindProject(state[ord].closure[k],sum,ord); //将新产生的产生式的向前搜索符加到原本已存在的相同的产生式中
                        x1=strlen(state[ord].closure[index].fwd_char);
                        for (x2=0; x2<strlen(state[ord].closure[k].fwd_char); x2++)
                            if (FindChar(state[ord].closure[k].fwd_char[x2],state[ord].closure[index].fwd_char)==-1) //当前字符不在原本的向前搜索符中
                            {
                                state[ord].closure[index].fwd_char[x1]=state[ord].closure[k].fwd_char[x2];
                                x1++;
                            }
                        state[ord].closure[index].fwd_char[x1]='\0';
                        k--;
                    }
                }
            }
            x=0;                                //先把x清零
            temp1[cir]='*';                     //表示该终结符已经计算完成
            for (i=0; i<strlen(temp1); i++)     //重新计算x
                if (temp1[i]!='*')
                    x++;
            cir++;
        }
    }
    state[ord].pro_num=sum; //把状态下的项目的数量保存
}
