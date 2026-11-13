#include "Grammer.h"
//构造ACTION表和GOTO表,输入是产生的状态集族和状态个数，输出是ACTION表和GOTO表
void GetForm(CLOSURE form[],int n)
{
    int i,j=1,s=1,q;
    int arr,arr1,arr2;
    int p,tem,k;
    char temp[10];
    for (i=0; i<strlen(end_singal); i++)     //储存action表的表头
    {
        action_form[0][j].symbol=end_singal[i];
        j++;
    }
    action_form[0][j].symbol='#';
    for (i=0; i<form_num; i++)            //储存action和GOTO表的表头（列）
        if (Find_again(form[i].order,s-1)==-1)
        {
            action_form[s][0].mark=form[i].order;
            goto_form[s][0].mark=form[i].order;
            s++;
        }
    j=1;
    for (i=1; i<strlen(non_terminal); i++)  //储存GOTO表的表头（行）
    {
        goto_form[0][j].symbol=non_terminal[i];
        j++;
    }
    for (i=1; i<s; i++)                    //对action表进行初始化
        for (j=1; j<=strlen(end_singal)+1; j++)
        {
            action_form[i][j].mark=0;
            action_form[i][j].symbol='0';
        }
    for (i=1; i<s; i++)                    //对GOTO表进行初始化
        for (j=1; j<strlen(non_terminal); j++)
        {
            goto_form[i][j].mark=0;
            goto_form[i][j].symbol='0';
        }
    for (i=0; i<form_num-1; i++)           //构造action表和GOTO表
    {
        arr=First_end(go_to[i].pass_char,end_singal);
        if (arr>=0)           //是终结符
        {
            action_form[go_to[i].start_state+1][arr+1].symbol='s';   //移进
            action_form[go_to[i].start_state+1][arr+1].mark=form[i+1].order;
        }
        else
        {
            arr1=First_end(go_to[i].pass_char,non_terminal);           //找到该非终结符的位置
            goto_form[go_to[i].start_state+1][arr1].mark=form[i+1].order;
        }
    }
    for (i=0; i<form_num; i++)             //找所有状态中的归约项目
    {
        for (j=0; j<form[i].pro_num; j++)  //遍历当前状态中的每一个项目
        {
            s=First_end('.',form[i].closure[j].project_lr1);        //计算小圆点的位置
            if (s==strlen(form[i].closure[j].project_lr1)-1)       //小圆点在产生式的末尾
            {
                for (q=0; q<strlen(form[i].closure[j].project_lr1)-1; q++)      //去掉产生式中的小圆点，以便找到文法的位置
                    temp[q]=form[i].closure[j].project_lr1[q];
                temp[q+1]='\0';
                tem=FindRules(temp,form[i].closure[j].left_lr1);
                memset(temp,0,sizeof(temp));                        //清空数组为下一次计算做准备
                if (tem==0)                 //第零条文法
                    action_form[form[i].order+1][strlen(end_singal)+1].symbol='a';  //表示接受状态
                else
                    for (k=0; k<strlen(form[i].closure[j].fwd_char); k++)
                    {
                        arr2=First_end(form[i].closure[j].fwd_char[k],end_singal);  //找到该向前搜索符在终结符中的位置
                        if (arr2==-1)     //没有找到，说明该向前搜索符是‘#’,在#列对应的行写归约
                        {
                            action_form[form[i].order+1][strlen(end_singal)+1].mark=tem;
                            action_form[form[i].order+1][strlen(end_singal)+1].symbol='r';
                        }
                        else                //在相应的终结符的列添加归约
                        {
                            action_form[form[i].order+1][arr2+1].mark=tem;
                            action_form[form[i].order+1][arr2+1].symbol='r';
                        }
                    }
            }
        }
    }
    //输出action表
    ofstream outfile;
    outfile.open("ACTION.txt",ios::out);
    outfile<<"ACTION:"<<endl;
    outfile<<"\t";
    for (i=1; i<=strlen(end_singal)+1; i++)
        outfile<<setw(6)<<action_form[0][i].symbol<<"\t";
    outfile<<endl;
    for (i=1; i<s; i++)
    {
        outfile<<setw(2)<<action_form[i][0].mark<<"\t";
        //outfile<<action_table[i][0].mark<<"\t";
        for (j=1; j<=strlen(end_singal)+1; j++)
            outfile<<setw(4)<<action_form[i][j].symbol<<setw(0)<<action_form[i][j].mark<<"\t";
        outfile<<endl;
    }
    outfile.close();




    /////////////////////////////////////
     outfile.open("ACTION1.txt",ios::out);
    //outfile<<"ACTION111:"<<endl;
    //outfile<<setw(6)<<"State"<<setw(0)<<"    ";
//    for (i=1; i<=strlen(terminal)+1; i++)
//        outfile<<setw(6)<<action_table[0][i].symbol<<"\t";
//    outfile<<endl;

    for (i=1; i<s; i++)
    {

        //outfile<<setw(2)<<action_table[i][0].mark<<"\t";

        //outfile<<action_table[i][0].mark<<" ";
        outfile<<"{";
        for (j=1; j<=strlen(end_singal); j++)
        {
             //outfile<<setw(4)<<action_table[i][j].symbol<<setw(0)<<action_table[i][j].mark<<",";
            outfile<<setw(0)<<action_form[i][j].mark<<",";
        }
        outfile<<setw(0)<<action_form[i][j].mark<<" ";


            //outfile<<action_table[i][j].symbol<<setw(0)<<action_table[i][j].mark<<" ";
        outfile<<"},";
        outfile<<endl;
    }

    outfile.close();






   // 输出GOTO表
    outfile.open("GOTO.txt",ios::out);
    outfile<<"GOTO"<<endl;
       outfile<<"\t";
    for (i=1; i<strlen(non_terminal); i++)
        outfile<<setw(3)<<goto_form[0][i].symbol<<"\t";
    outfile<<endl;
    for (i=1; i<s; i++)
    {

        outfile<<goto_form[i][0].mark<<"\t";
        for (j=1; j<strlen(non_terminal); j++)
            outfile<<" "<<goto_form[i][j].mark<<"\t";

        outfile<<endl;
    }
    outfile.close();


       // 输出GOTO表
    outfile.open("GOTO1.txt",ios::out);
//    outfile<<"GOTO11111"<<endl;
//       outfile<<"\t";
//    for (i=1; i<strlen(non_terminal); i++)
//        outfile<<setw(3)<<go_to_table[0][i].symbol<<"\t";
//    outfile<<endl;
    for (i=1; i<s; i++)
    {
        outfile<<"{";
//        outfile<<go_to_table[i][0].mark<<"\t";
        for (j=1; j<strlen(non_terminal)-1; j++)
            outfile<<" "<<goto_form[i][j].mark<<",";
        outfile<<" "<<goto_form[i][j].mark<<" ";
        outfile<<"},";
        outfile<<endl;
    }
    outfile.close();

}

//判断当前的状态序号是否出现过
int Find_again(int order,int n)
{
    int arr=-1;
    int i;
    for (i=0; i<n; i++)
        if (order==action_form[i][0].mark)
            arr=i;
    return arr;
}
