//#include "morphology.h"
//#include "grammar.h"
//#include "semantic.h"

#include"keshe.h"
#include"stack.h"

RULE grammar[NUM1];         //储存产生式的结构体数组
CLOSURE state[NUM2];         //储存状态的结构体数组
NON_TER nonter_first[30];   //储存非终结符及其属性的结构体数组
GO go_to[NUM2];
ACT action_table[NUM2][NUM2]; //action表
ACT go_to_table[NUM2][NUM2];  //goto表

int number=42;              //产生式的数量
char terminal[]= {'v','m','(',')','{','}','f','e','w','r','j','=','d','<','i','h','k','!',
                  'a','b','c','g','>','*','-',';',',','t','n','+','\0'
                 };         //终结符的集合
char non_terminal_l[]= {'I','S','S','S','S','A','C','T','W','K','F','G','H',    //13
                        'S','N','S','S','E','E','E','E','E','E','E','R',        //13
                        'R','R','R','R','R','A','D','D','D','D','D','D',        //12
                        'B','B','B','L','L'                                     //5
                       };   //每个产生式的左部
char non_terminal[]= {'I','S','A','C','T','W','K','F','G','H','N','E','R','D','B','L','\0'};//文法中所有的非终结符
char terminal_r[][10]= {"vm(){S}","C{S}","T{S}","K{S}","A","AA","fE","C{S}e","w",           //9
                        "WE","r(i=d;","FiRN;","Gi=i+d)","H{S}","d","B;S",                   //7
                        "B","EhE","EkE","!E","(E)","dRd","iRi","iRd","a","b",
                        "c","g","<",">","i=D;",//15
                        "D+D","D*D","-D","(D)","d","i","t","n","BL","L,i","i"               //11
                       };   //每个产生式的右部
int sum[]= {7,4,4,4,1,2,2,5,1,2,6,5,7,4,1,3,1,3,3,
            2,3,3,3,3,1,1,1,1,1,1,4,3,3,2,3,1,1,1,1,
            2,3,1};         //产生式右部的字符个数

//按照action表和goto表分析句子，输入为要分析的测试函数和action表和goto表
void Analysis(char simp_sentence[],ACT action_table[][NUM2],ACT go_to_table[][NUM2])
{
    int i,i1,i2,i3,j=0,k;
    char ch,ch1;
    int x;
    int index,index1,index2;
    attribute a,a1,a2;

    SqStack_c char_stack;
    SqStack_s state_stack;
    SqStack semant_stack;           //语义栈

    int buzhou=1;
    cout<<"步骤\t\t\t\t状态栈\t\t\t\t\t符号栈\t      \tACTION\tGOTO\n";
    InitStack_state(state_stack);   //初始化一个状态空栈
    Push_state(state_stack,0);      //将0号状态入栈
    cout<<setw(2)<<buzhou<<"\t";
        buzhou++;
    PrintStack_state(state_stack);

    InitStack_char(char_stack);     //初始化一个符号空栈
    Push_char(char_stack,'#');      //'#'入符号栈
    PrintStack_char(char_stack);

    cout<<" "<<endl;

    InitStack(semant_stack);        //初始化语义空栈
    InitAttri(a);                   //初始化属性值
    Push(semant_stack,a);
    PrintStack_semantic(semant_stack);
    while(1)
    {
         cout<<setw(2)<<buzhou<<"\t";
        buzhou++;

        i=GetTop_state(state_stack); //获取状态栈的栈顶
        if (simp_sentence[j]=='#')
            index=strlen(terminal);
        else
            index=FindChar(simp_sentence[j],terminal);  //找到字符在数组中的位置
        if(action_table[i+1][index+1].symbol=='s')      //表明这时的操作为移进
        {
            Push_state(state_stack,action_table[i+1][index+1].mark); //状态移进
            PrintStack_state(state_stack);
            Push_char(char_stack,simp_sentence[j]);     //输入字符串的第一个字符移进
            PrintStack_char(char_stack);

            cout<<"移近"<<action_table[i+1][index+1].symbol<<action_table[i+1][index+1].mark
            <<"\t      "<<endl;

            InitAttri(a1);
            a1.place=j;
            a1.type=L[j].flag1;  //Word_List L[NUM];存放单词及其简化形式的结构体数组
            Push(semant_stack,a1);
            nodes_attri[length1]=a1;  //attribute nodes_attri[NUM]; //存放每个结点的属性
            PrintStack_semantic(semant_stack);
            length1++;

            j++;
        }
        else if(action_table[i+1][index+1].symbol=='r') //表明这时的操作为归约
        {
            index1=action_table[i+1][index+1].mark;     //用的产生式的序号
            for (i3=length1-grammar[index1].length;i3<length1;i3++)//length文法右部的符号长度
            {
                node_attri[length]=nodes_attri[i3];//extern attribute node_attri[40];    //暂时存放每个归约产生的右部符号的属性
                length++;
            }
            Program(index1,a1,node_attri);  //index1代表用的产生式的序号、a1是一个属性、node_attri暂时存放每个归约产生的右部符号的属性
            for (k=0; k<grammar[index1].length; k++)    //状态栈和符号栈出相应数目的
            {
                Pop_state(state_stack,x);
                PrintStack_state(state_stack);
                Pop_char(char_stack,ch);
                PrintStack_char(char_stack);

                cout<<"规约r"<<action_table[i+1][index+1].mark
                <<"\t      "<<endl;
                cout<<setw(2)<<buzhou<<"\t";
                buzhou++;

                Pop(semant_stack,a2); //语义栈出栈
                PrintStack_semantic(semant_stack);
            }
            Push_char(char_stack,grammar[index1].left); //归约的文法左部入栈
            Push(semant_stack,a1); //规约左部的属性入语义栈 ,上面从Program进阶来的b属性进语意栈
            PrintStack_semantic(semant_stack);//3
            nodes_attri[length1-grammar[index1].length]=a1;
            length1=length1-grammar[index1].length+1;
            i2=GetTop_state(state_stack);               //获取状态栈的栈顶
            ch1=GetTop_char(char_stack);                //获取符号栈的栈顶
            i1=FindChar(ch1,non_terminal);
            index2=go_to_table[i2+1][i1].mark;          //查找GOTO表
            if(index2!=-1)
            {
                Push_state(state_stack,index2);
                PrintStack_state(state_stack);//1
            }
            else
            {
                cout<<"ERROR"<<endl;
                break;
            }
            PrintStack_char(char_stack);//2

            cout<<"    "
            <<"\tGO(s"<<index2<<")"<<endl;

        }
        else if (action_table[i+1][index+1].symbol=='a')
        {
            int x_1=GetLength_state(state_stack)-1;
            int x_2=GetLength_char(char_stack)-1;
            int x2;
            for(int x1=0;x1<x_1;x1++){
                Pop_state(state_stack,x2);
            }
            PrintStack_state(state_stack);
            char xin;
            for(int x2=0;x2<x_2;x2++){
                Pop_char(char_stack,xin);
            }
            PrintStack_char(char_stack);
            cout<<"规约r0"
                <<"\t      "<<endl;

            cout<<"\n句子合法，程序运行成功！"<<endl;
            break;
        }
        else
        {
            cout<<"ERROR"<<endl;
            break;
        }
    }
}
