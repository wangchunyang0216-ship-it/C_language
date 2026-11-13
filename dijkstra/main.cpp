/*
使用此程序需输入以下内容创建图G：
第一步：7 12
第二步：0123456
第三步：依次输入下面的内容，输入完一行就按下换行键

7 12
0 1 2 3 4 5 6
0 1 6
0 2 3
1 2 2
1 3 1
1 4 4
2 3 5
2 5 7
3 4 3
3 5 6
4 5 2
4 6 2
5 6 3
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MaxVerterNum 100		// 顶点数目的最大值
#define INFINITY 65535			// 用65535代表 ∞

typedef char VertexType;		// 顶点的数据类型
typedef int EdgeType;			// 带权图中边上权值的数据类型

/* 邻接矩阵的存储结构 */
typedef struct
{
	int Vexs[MaxVerterNum];					// 顶点表
	EdgeType Edge[MaxVerterNum][MaxVerterNum];		// 邻接矩阵
	int vexNum, arcNum;								// 图当前顶点数和弧数
}MGraph;

/*清除缓冲区的换行符*/
void Clean(void)
{
	while (getchar() != '\n')
		continue;
}

/* 建立无向网图的邻接矩阵表示 */
void CreateMGraph(MGraph* G);

/* 迪杰斯特拉（Dijkstra) 算法*/
typedef int Patharc[MaxVerterNum];			// 用于存储最短路径下标的数组，从源点Vi到顶点Vj之间的最短路径的前驱
typedef int ShortPathTable[MaxVerterNum];	// 用于存储到各点最短路径的权值和
void ShortestPath_Dijkstra(MGraph G, int v0, Patharc path, ShortPathTable D);

/* 输出最短路径 */
/* Dijkstra算法的结果输出 */
void Show_ShortestPath_Dijkstra(Patharc path, ShortPathTable dist, MGraph G, int v0);

int main(void)
{
	MGraph G;
	Patharc path;
	ShortPathTable dist;
	CreateMGraph(&G);
	for (int i = 0; i < G.vexNum; i++) // 输出各点到各点的最短路径序列，不再局限于一个顶点
	{
		ShortestPath_Dijkstra(G, i, path, dist);
		Show_ShortestPath_Dijkstra(path, dist, G, i);
	}
	return 0;
}

/* 建立无向网图的邻接矩阵表示 */
void CreateMGraph(MGraph* G)
{
	int i, j, k, w;
	printf("请输入顶点数和边数：");
	scanf("%d %d", &G->vexNum, &G->arcNum);			// 获取无向图顶点数和边数
	printf("请输入全部顶点信息：\n");
	for (i = 0; i < G->vexNum; i++)					// 读取顶点信息，建立顶点表
		scanf("%d", &G->Vexs[i]);
	for (i = 0; i < G->vexNum; i++)
		for (j = 0; j < G->vexNum; j++)
			G->Edge[i][j] = INFINITY;				// 邻接矩阵初始化
	for (k = 0; k < G->arcNum; k++)					// 读入arcNum条边，建立邻接矩阵
	{
		printf("请输入边（Vi, Vj)上的下标i，下标j和权w：\n");
		scanf("%d %d %d", &i, &j, &w);				// 获取边和权
		G->Edge[i][j] = w;							// 无向图矩阵对称
		G->Edge[j][i] = G->Edge[i][j];
	}
	return;
}

/* 迪杰斯特拉（Dijkstra) 算法*/
void ShortestPath_Dijkstra(MGraph G, int v0, Patharc path, ShortPathTable dist)
{
	int v, w, k, min;
	int final[MaxVerterNum];				/* final[w] = 1表示求得顶点 v0 至 vw的最短路径，即已访问过顶点vw*/
	for (v = 0; v < G.vexNum; v++)
	{
		final[v] = 0;						// 全部顶点初始化为未知最短路径状态
		dist[v] = G.Edge[v0][v];			// 将与v0点有连线的顶点加上权值
		path[v] = -1;						// 初始化路劲数组p为-1
	}
	dist[v0] = 0;							// v0至v0路径为0
	final[v0] = 1;							// v0至v0不需要路径
	/* 开始主循环，每次求得v0到某个顶点v的最短路径*/
	for (v = 1; v < G.vexNum; v++)
	{
		min = INFINITY;						// 当前所知离v0顶点的最近距离
		for (w = 0; w < G.vexNum; w++)		// 寻找离v0最近的顶点
		{
			if (!final[w] && dist[w] < min)
			{
				k = w;
				min = dist[w];				// w顶点离v0顶点更近
			}
		}
		final[k] = 1;						// 将目前找到的最近的顶点置为1
		for (w = 0; w < G.vexNum; w++)		// 修正当前最短路径及距离
		{
			/* 如果经过v顶点的路径比现在这条路径的长度短的话 */
			if (!final[w] && (min + G.Edge[k][w] < dist[w]))
			{
				/* 找到了更短的路径，修改D[w]和P[w] */
				dist[w] = min + G.Edge[k][w];	// 修改当前路径长度
				path[w] = k;
			}
		}
	}
}

/* 输出最短路径 */
/* Dijkstra算法的结果输出 */
void Show_ShortestPath_Dijkstra(Patharc path, ShortPathTable dist, MGraph G, int v)
{
	int w, k;
	printf("V%d到各点的最短路径如下：\n", v);
	for (w = 0; w < G.vexNum; w++)
	{
		if (w != v)
		{
			printf("V%d-V%d weight: %d", v, w, dist[w]);
			k = path[w];
			printf(" path: V%d", w);
			while (k != -1)  // 当 k = -1 ，结束循环并输出源点
			{
				printf(" <- V%d", k);
				k = path[k];
			}
			printf(" <- V%d\n", v);
		}
	}
	printf("\n");
}
