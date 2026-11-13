#include "Grammer.h"

RULE grammar[NUM1];         //储存产生式的结构体数组
CLOSURE form[NUM];         //储存状态的结构体数组
NON_TER nonter_first[30];   //储存非终结符及其属性的结构体数组
GO go_to[NUM];
ACT action_form[NUM][NUM]; //action表
ACT goto_form[NUM][NUM];  //goto表

int form_num;
int number=42;              //产生式的数量
char end_singal[]= {'v','m','(',')','{','}','f','e','w','r','j','=','d','<','i','h','k','!',
                  'a','b','c','g','>','*','-',';',',','t','n','+','\0'
                 };         //终结符的集合
char product_left[]= {'I','S','S','S','S','A','C','T','W','K','F','G','H',    //13
                        'S','N','S','S','E','E','E','E','E','E','E','R',        //13
                        'R','R','R','R','R','A','D','D','D','D','D','D',        //12
                        'B','B','B','L','L'                                     //5
                       };   //每个产生式的左部
char non_terminal[]= {'I','S','A','C','T','W','K','F','G','H','N','E','R','D','B','L','\0'};//文法中所有的非终结符
char terminal_r[][10]= {"vm(){S}","C{S}","T{S}","K{S}","A","AA","fE","C{S}e","w",           //9
                        "WE","r(i=d;","FiRN;","Gi=i+d)","H{S}","d","B;S",                   //7
                        "B","EhE","EkE","!E","(E)","dRd","iRi","iRd","a","b","c","g","<",">","i=D;",//15
                        "D+D","D*D","-D","(D)","d","i","t","n","BL","L,i","i"               //11
                       };   //每个产生式的右部
int sum[]= {7,4,4,4,1,2,2,5,1,2,6,5,7,4,1,3,1,3,3,
            2,3,3,3,3,1,1,1,1,1,1,4,3,3,2,3,1,1,1,1,
            2,3,1};         //产生式右部的字符个数



int random1[NUM];
//按照action表和goto表分析句子，输入为要分析的测试函数和action表和goto表
void yufa_analysis(char simp_sentence[],ACT action_form[][NUM],ACT goto_form[][NUM])
{
    int i,i1,i2,j=0,k;
    char ch,ch1;
    int x;
    int index,index1,index2;
    SqStack_c char_stack;
    SqStack_s state_stack;

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

    while(1)
    {
        cout<<setw(2)<<buzhou<<"\t";
        buzhou++;

        i=GetTop_state(state_stack); //获取状态栈的栈顶
        if (simp_sentence[j]=='#')
            index=strlen(end_singal);
        else
            index=First_end(simp_sentence[j],end_singal);  //找到字符在数组中的位置

        if(action_form[i+1][index+1].symbol=='s')      //表明这时的操作为移进
        {
            Push_state(state_stack,action_form[i+1][index+1].mark); //状态移进
            PrintStack_state(state_stack);
            Push_char(char_stack,simp_sentence[j]);     //输入字符串的第一个字符移进
            PrintStack_char(char_stack);

            cout<<"移近"<<action_form[i+1][index+1].symbol<<action_form[i+1][index+1].mark
            <<"\t      "<<endl;

            j++;
        }
        else if(action_form[i+1][index+1].symbol=='r') //表明这时的操作为归约
        {
            index1=action_form[i+1][index+1].mark;     //用的产生式的序号
            for (k=0; k<grammar[index1].length; k++)    //状态栈和符号栈出相应数目的
            {



                Pop_state(state_stack,x);
                PrintStack_state(state_stack);
                Pop_char(char_stack,ch);
                PrintStack_char(char_stack);

                cout<<"规约r"<<action_form[i+1][index+1].mark
                <<"\t      "<<endl;
                cout<<setw(2)<<buzhou<<"\t";
                buzhou++;

            }

            Push_char(char_stack,grammar[index1].left); //归约的文法左部入栈
            i2=GetTop_state(state_stack);               //获取状态栈的栈顶
            ch1=GetTop_char(char_stack);                //获取符号栈的栈顶
            i1=First_end(ch1,non_terminal);
            index2=goto_form[i2+1][i1].mark;          //查找GOTO表
            if(index2!=-1)
            {
                Push_state(state_stack,index2);
                PrintStack_state(state_stack);





            }
            else
            {
                cout<<"ERROR"<<endl;
                break;
            }
            PrintStack_char(char_stack);

            cout<<"    "
            <<"\tGO(s"<<index2<<")"<<endl;
        }
        else if (action_form[i+1][index+1].symbol=='a')
        {
            cout<<"The test is legal!"<<endl;
            break;
        }
        else
        {
            cout<<"ERROR"<<endl;
            break;
        }
    }

// //调试
// for(i=0;i<20;i++){
//        cout<<random1[i]<<" ";
//    }

}
