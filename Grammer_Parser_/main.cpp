#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Lexical.h"
#include "Grammer.h"
using namespace std;


int main()
{
    cifa_analysis();  //读取测试代码并对其进行词法分析，将读取到的代码简化
    project_club();           //保存所有的产生式及其项目集
    Get_firstclub(16);     //判断非终结符是否能推空，求出First集
    Getproject_club(grammar); //传入文法及其项目，构造项目集族
    GetForm(form,form_num); //构造Action表和Goto表
    yufa_analysis(simp_sentence,action_form,goto_form);  //语法分析


    return 0;
}
