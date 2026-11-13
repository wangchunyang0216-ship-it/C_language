#include "Grammer.h"

//保存所有的产生式及其项目
void project_club()
{
    int i,j;
    for (i=0; i<number; i++)  //number为产生式的数量
    {
        grammar[i].left=product_left[i];    //存放左部非终结符
        grammar[i].length=sum[i];
        for (j=0; j<grammar[i].length; j++)
            grammar[i].right[j]=terminal_r[i][j];
        GetProject(grammar[i].right,grammar[i].length,i);
    }
}

//构造每一条产生式的项目，输入是产生式和右部长度以及产生式的序号
void GetProject(char product[],int n,int order)
{
    int i,j,k=0;
    for (i=0; i<n+1; i++)   //把"."存入项目数组
        grammar[order].project[i][i]='.';
    for (i=0; i<n+1; i++)   //将产生式项目中的其他部分存入数组
    {
        for (j=0; j<n+1; j++)
            if (grammar[order].project[i][j]!='.')
            {
                grammar[order].project[i][j]=product[k];
                k++;
            }
            else;
        grammar[order].project[i][j]='\0'; //结束标记
        k=0;
    }
}

//构造项目集族，输入是文法及其项目
void Getproject_club(RULE grammar[])
{
    int i,j=0,k=0,k1,k2,k3;    //j表示状态数组的下标
    int index;
    int n=0,n1=0;
    int p,e,x;
    int i1,i2,i3,i4,i5,i6;
    int sign[NUM1];
    memset(sign,0,sizeof(sign));
    int temp_j=0;
    char temp[10];        //临时存放一个串
    strcpy(temp,"");
    int tem,m=0,l=0,m1=0,m2;   //表示第tem条文法，m表示产生式在当前状态的下标位置,l表示GO函数中的下标
    char forwardchar[]= {'#','\0'};
    for (i=0; i<NUM; i++)      //初始化结束标记
        form[i].ends=0;
    //构造第零个状态,左部符号为文法的第零条文法的左部，项目为第零条文法的圆点在第一个的项目，状态数组的下标为0
    GetClosure(grammar[0].left,grammar[0].project[0],forwardchar,j);
    form[j].order=n;          //给状态编号赋值
    n1++;                      //记录状态数包括重复的状态
    while(temp_j<=n1)
    {
        while(form[temp_j].ends==0&&m<form[temp_j].pro_num)                 //遍历该状态下的所有产生式
        {
            p=First_end('.',form[temp_j].closure[m].project_lr1);             //计算当前产生式的小圆点位置
            if (p<strlen(form[temp_j].closure[m].project_lr1)-1&&sign[m]==0) //当前的小圆点位置不在末尾
            {
                go_to[l].start_state=form[temp_j].order;
                go_to[l].pass_char=form[temp_j].closure[m].project_lr1[p+1]; //保存GO函数中的内容
                j++;
                x=strlen(temp);
                for (e=0; e<strlen(form[temp_j].closure[m].project_lr1); e++)
                    if (form[temp_j].closure[m].project_lr1[e]!='.')
                    {
                        temp[x]=form[temp_j].closure[m].project_lr1[e];
                        x++;
                    }
                temp[x]='\0';
                tem=FindRules(temp,form[temp_j].closure[m].left_lr1);   //定位文法的位置
                strcpy(temp,"");                                         //清空临时数组为下一次作准备
                form[j].closure[k].left_lr1=form[temp_j].closure[m].left_lr1;
                for (k1=0; k1<strlen(form[temp_j].closure[m].project_lr1); k1++) //存右部
                    form[j].closure[k].project_lr1[k1]=grammar[tem].project[p+1][k1];
                form[j].closure[k].project_lr1[k1]='\0';
                for (k2=0; k2<strlen(form[temp_j].closure[m].fwd_char); k2++)
                    form[j].closure[k].fwd_char[k2]=form[temp_j].closure[m].fwd_char[k2];
                form[j].closure[k].fwd_char[k2]='\0';
                k++;
                form[j].pro_num++;
                for (m1=0; m1<form[temp_j].pro_num; m1++)                   //遍历该状态的所有项目
                {
                    m2=First_end('.',form[temp_j].closure[m1].project_lr1);  //计算小圆点的位置
                    if (form[temp_j].closure[m1].project_lr1[m2+1]==go_to[l].pass_char&&m1!=m) //在这一个状态中找到均要过该符号的所有产生式
                    {
                        x=strlen(temp);
                        for (e=0; e<strlen(form[temp_j].closure[m1].project_lr1); e++)
                            if (form[temp_j].closure[m1].project_lr1[e]!='.')
                            {
                                temp[x]=form[temp_j].closure[m1].project_lr1[e];
                                x++;
                            }
                        temp[x]='\0';
                        tem=FindRules(temp,form[temp_j].closure[m1].left_lr1); //定位文法的位置
                        strcpy(temp,"");                                        //清空临时数组为下一次作准备
                        form[j].closure[k].left_lr1=form[temp_j].closure[m1].left_lr1;
                        for (k1=0; k1<strlen(form[temp_j].closure[m1].project_lr1); k1++) //存右部
                            form[j].closure[k].project_lr1[k1]=grammar[tem].project[m2+1][k1];
                        form[j].closure[k].project_lr1[k1]='\0';
                        for (k2=0; k2<strlen(form[temp_j].closure[m1].fwd_char); k2++)
                            form[j].closure[k].fwd_char[k2]=form[temp_j].closure[m1].fwd_char[k2];
                        form[j].closure[k].fwd_char[k2]='\0';
                        k++;
                        sign[m1]=1;
                        form[j].pro_num++;
                    }
                }
                k=0;
                for (k3=0; k3<form[j].pro_num; k3++) //计算每一个的闭包
                {
                    m2=First_end('.',form[j].closure[k3].project_lr1); //计算小圆点的位置
                    x=strlen(temp);
                    for (e=0; e<strlen(form[j].closure[k3].project_lr1); e++)
                        if (form[j].closure[k3].project_lr1[e]!='.')
                        {
                            temp[x]=form[j].closure[k3].project_lr1[e];
                            x++;
                        }
                    temp[x]='\0';
                    tem=FindRules(temp,form[j].closure[k3].left_lr1); //定位文法的位置
                    strcpy(temp,""); //清空临时数组为下一次作准备
                    j++;
                    GetClosure(grammar[tem].left,grammar[tem].project[m2],form[j-1].closure[k3].fwd_char,j);
                    i1=form[j-1].pro_num; //将该产生的新状态并入到前一个状态
                    for (i2=0; i2<form[j].pro_num; i2++)
                    {
                        index=FindProject(form[j].closure[i2],form[j-1].pro_num,j-1);
                        if (index==-1) //该产生式不在该状态中
                        {
                            form[j-1].closure[i1].left_lr1=form[j].closure[i2].left_lr1;  //并左部
                            for (i3=0; i3<=strlen(form[j].closure[i2].project_lr1); i3++)  //并右部
                                form[j-1].closure[i1].project_lr1[i3]=form[j].closure[i2].project_lr1[i3];
                            for (i4=0; i4<=strlen(form[j].closure[i2].fwd_char); i4++)     //并向前搜索符
                                form[j-1].closure[i1].fwd_char[i4]=form[j].closure[i2].fwd_char[i4];
                            i1++;
                            form[j-1].pro_num++;
                        }
                        else
                            if (strcmp(form[j-1].closure[index].fwd_char,form[j].closure[i2].fwd_char)) //如果向前搜索符不相等，将向前搜索符并入
                            {
                                i5=strlen(form[j-1].closure[index].fwd_char);
                                for (i6=0; i6<strlen(form[j].closure[i2].fwd_char); i6++)
                                    if(First_end(form[j].closure[i2].fwd_char[i6],form[j-1].closure[index].fwd_char)==-1)
                                    {
                                        form[j-1].closure[index].fwd_char[i5]=form[j].closure[i2].fwd_char[i6];
                                        i5++;
                                    }
                                form[j-1].closure[index].fwd_char[i5]='\0';  //并入完成
                            }
                    }
                    form[j].pro_num=0;
                    j=j-1;
                }
                if (FindState(form[j],j)==-1)  //是新的状态,保存该状态
                {
                    n++;
                    form[j].order=n;
                    n1++;
                    form[j].ends=0;            //表明该状态还未扩展
                    l++;
                }
                else   //不是新的状态
                {
                    form[j].order=FindState(form[j],j);
                    form[j].ends=form[FindState(form[j],j)].ends;
                    n1++;
                    l++;
                }
            }
            m++; //直接判断下一个
        }
        memset(sign,0,sizeof(sign));
        form[temp_j].ends=1;            //表明该状态的项目已经全部扩展完成
        for (i=temp_j; i<n1; i++)        //一旦该temp_j被标识为扩展完成，以后和该状态标号相同的均标识为被扩展
            if (form[temp_j].order==form[i].order)
                form[i].ends=1;
        temp_j++;
        m=0;
    }
    form_num=n1;
}

//判断当前所得状态是否与已存在状态有相同,传入的参数为一个新状态以及目前已经存在的状态个数
int FindState(CLOSURE newstate,int n)
{
    int index=-1;
    int i=0,j;
    while(i<n)
    {
        j=0;
        if (form[i].pro_num==newstate.pro_num) //新状态与当前所处的状态的项目数量相同
        {
            while(j<form[i].pro_num)
                if ((form[i].closure[j].left_lr1==newstate.closure[j].left_lr1)&&(strcmp(form[i].closure[j].project_lr1,newstate.closure[j].project_lr1)==0)&&(strcmp(form[i].closure[j].fwd_char,newstate.closure[j].fwd_char)==0))
                    j++;
                else
                    goto jia;
            return form[i].order;
        }
        else
jia:
            i++;
    }
    return index;
}

//在文法中匹配产生式的位置
int FindRules(char strings[],char l)
{
    int i;
    int index=-1;
    for (i=0; i<number; i++)
        if ((strcmp(strings,grammar[i].right)==0)&&(l==grammar[i].left))
            index=i;
    return index;
}

//判断新项目是否已存在在该状态中
int FindProject(PROJECT newpro,int n,int ord)
{
    int index=-1;
    int i;
    for (i=0; i<n; i++)
        if ((form[ord].closure[i].left_lr1==newpro.left_lr1)&&(strcmp(form[ord].closure[i].project_lr1,newpro.project_lr1)==0))
            index=i;
    return index;
}

//在一个字符串中找字符，找到返回其位置，找不到返回-1
int First_end(char c,char lists[])
{
    int i;
    int index=-1;
    for (i=0; i<strlen(lists); i++)
        if (c==lists[i])
            index=i;
    return index;
}

