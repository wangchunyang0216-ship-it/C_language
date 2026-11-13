#ifndef MAX_ELECTIVE_H_INCLUDED
#define MAX_ELECTIVE_H_INCLUDED


//typedef struct CSNode
//{
//	ElemType data;
//	struct CSNode* firstchild, * nextsibling;
//}CSNode,*CSTree;

typedef struct relationship
{
    int data;
    struct relationship* firstchild;
    struct relationship* nextsibling;
}REL;



#endif // MAX_ELECTIVE_H_INCLUDED
