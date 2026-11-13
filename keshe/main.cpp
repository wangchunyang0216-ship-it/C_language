#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "morphology.h"
//#include "grammar.h"
//#include "semantic.h"


#include"keshe.h"
#include"stack.h"
#include "optimize.h"

using namespace std;

int main()
{
    Morph_analysis();  //读取测试代码并对其进行词法分析，将读取到的代码简化
    Table();           //保存所有的产生式及其项目集
    GetNonter(16);     //判断非终结符是否能推空，求出First集
    GetState(grammar); //传入文法及其项目，构造项目集族
    GetAction(state,state_num); //构造Action表和Goto表
    PrintStack_table();
    Analysis(simp_sentence,action_table,go_to_table);  //语法分析
    PrintQua();
    FindEntrance();
    Optimize();
    RefreshLabel(quate_optimize);
    return 0;
}
