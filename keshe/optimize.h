#ifndef OPTIMIZE_H_INCLUDED
#define OPTIMIZE_H_INCLUDED

#define NUM3 100

//块的结构体
typedef struct
{
    int entry;      //该块入口语句的编号
    int out;        //出口语句的编号
    int reduce_num; //减少的四元式的条数
}block;
extern block B[NUM3];   //保存块的结构体数组
extern int block_num;   //块的数量
extern int node_num;    //当前生成的结点数量

//图的结构体
#define INFINITY 100000     //最大值
#define MAX_VERTEX_NUM 20   //最大顶点个数
typedef int InfoType;
typedef int VRType;
typedef struct
{
    int node_id;        //结点编号
    char below[5];      //结点的下方信息
    char right[5][5];   //结点的右方信息
}node;
typedef node VertexType;
typedef struct ArcCell
{
  VRType adj;           //顶点关系类型，对无权图用0或1
                        //表示是否相邻，对于带权图则为权值类型
  InfoType *info;       //该弧相关信息的指针
}ArcCell,AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef struct
{
   VertexType vexs[MAX_VERTEX_NUM]; //顶点向量
   AdjMatrix arcs;                  //邻接矩阵
}MGraph;
extern MGraph M;
extern int right_num[MAX_VERTEX_NUM];
extern Qua quate_optimize[NUM];     //存放优化后四元式的结构体数组

void FindEntrance();
int FindNode(char* findnode,MGraph M);
int FindNode1(char* n,MGraph M);
int FindNode2(char*n1,char* n2,char* ope,MGraph M);
void CreatDAG(block &b);
int FindKinds(int n,MGraph M,int kinds[]);
void CreatQua_optimize(int l,char o[],char a1[],char a2[],char r[]);
void Optimize();
void RefreshLabel(Qua quate_optimize[]);


#endif // OPTIMIZE_H_INCLUDED

