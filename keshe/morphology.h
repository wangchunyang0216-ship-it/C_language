#ifndef MORPHOLOGY_H_INCLUDED
#define MORPHOLOGY_H_INCLUDED

#define NUM 1000

typedef struct
{
    char word[10];       //识别出的单词
    char flag;           //简化形式
    char flag1;          //标识符的类型，float、int
}Word_List;
extern Word_List L[NUM];            //存放单词及其简化形式的结构体数组
extern char simp_sentence[NUM];     //存放简化后句子的数组
extern int g;
void Morph_analysis();              //词法分析
int Judge(char identifer[]);        //判断是保留字还是标识符
void SetType();

#endif // MORPHOLOGY_H_INCLUDED
