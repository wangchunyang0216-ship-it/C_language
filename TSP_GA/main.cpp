/*1 旅行商问题
旅行商问题（Traveling Salesman Problem, TSP），又译为旅行推销员问题、
货担郎问题，简称为TSP问题，是最基本的路线问题。
假设有n个可直达的城市，一销售商从其中的某一城市出发，
不重复地走完其余n-1个城市并回到原出发点，在所有可能的路径中求出路径长度最短的一条。


遗传算法（Genetic Algorithm）
解决TSP问题


回溯
*/
#include <iostream>

using namespace std;
//#define max_vertex 4
#define MaxInt 32767//表示极大值，即∞
//typedef struct Undirected_Graph
//{
//    int V[max_vertex];//顶点集
//    int A[max_vertex][max_vertex];//邻接矩阵
//    int vexnum,arcnum;//图的当前点数和边数
//}UnGraph;//无向图的邻接矩阵
#include<stdio.h>

#define MVNum 100//最大顶点数

typedef struct//邻接矩阵定义的结构体
{
    int vexs[MVNum];//顶点表（顶点数组）
    int arcs[MVNum][MVNum];//邻接矩阵
    int vexnum,arcnum;//图的当前点数和边数
}AMGrape;
bool visited[MVNum];//访问标志数组，其初值为“false”

int LocateVex1(AMGrape &G1,int v)//位置函数
{
    int i;
    for(i=0;i<G1.vexnum;i++)
    {
        if(G1.vexs[i]==v)
            return i;
    }
    return -1;
}

int CreateUDN(AMGrape &G1)//采用邻接矩阵表示法，创建不带权无向图G1
{
    int i,j,k,v1,v2,w;
    cout<<"请输入总顶点数，总边数"<<endl;
    cin>>G1.vexnum>>G1.arcnum;//输入总顶点数，总边数
    cout<<"请依次输入顶点的信息"<<endl;
    for(i=0;i<G1.vexnum;i++)//依次输入点的信息
    {
        cin>>G1.vexs[i];
        visited[i]=false;
    }
    for(i=0;i<G1.vexnum;i++)//初始化邻接矩阵，边的对应邻接矩阵的点设为0
        for(j=0;j<G1.vexnum;j++)
            G1.arcs[i][j]=MaxInt;
    cout<<"请依次输入每一条边依附的两个顶点，及每条边的权值"<<endl;
    for(k=0;k<G1.arcnum;k++)//构建邻接矩阵
    {
        cin>>v1>>v2>>w;//输入每一条边依附的顶点
        i=LocateVex1(G1,v1);//确定v1，和v2在G中的位置，即顶点数组的下标
        j=LocateVex1(G1,v2);//v2下标
//        i=v1;
//        j=v2;
        G1.arcs[i][j]=w;//边<v1，v2>对应邻接矩阵矩阵的点设置为1
        G1.arcs[j][i]=G1.arcs[i][j];//置<v1,v2>的对称边<v2,v1>的对应邻接矩阵的点为1
    }
    cout<<"输出创建好的邻接矩阵"<<endl;
    //输出创建好的邻接矩阵
    for(i=0;i<G1.vexnum;i++)
    {
        for(j=0;j<G1.vexnum;j++)
            cout<<G1.arcs[i][j]<<'\t';
        cout<<'\n';
    }

    return 1;
}
int main()
{
    int i,j;
    AMGrape G1;
    CreateUDN(G1);
    int c[10];
    int k;
    int path_length=MaxInt;
    int bestx[10];
    for(k=1;k<=G1.vexnum;k++)
    {
        c[k]=0;
    }
    k=1;
    while(k>=1)
    {
        while(c[k]<=G1.vexnum-1)
        {
            c[k]=c[k]+1;
            if(i==G1.vexnum && G1.arcs[i][0]!=MaxInt)//遍历到最后一个顶点可以回到第一个
            {
                path_length=path_length+G1.arcs[i][0];
            }
            else if(G1.arcs[i][0]!=MaxInt)
            {
                k=k+1;
            }
        }
        c[k]=0;
        k=k-1;
    }
    //scanf("%d %d",&G.vexnum,&G.arcnum);
    return 0;
}
