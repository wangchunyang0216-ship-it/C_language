#include<iostream>
#include<queue>

#include<string.h>//C语言
#include<cstring>//c++

using namespace std;
#define INF 0x3f3f3f3f
#define MAXN 21
struct Node		//队列结点类型
{
    int no;			    //结点编号
    int i;			    //表示当前结点属于解空间的第i层（根节点的层次为0），即准备为人员i分配任务
    int x[MAXN];		//x[i]为人员i分配的任务编号
    bool worker[MAXN];	//worker[i]=true表示任务i已经分配
    int cost;			//已经分配任务所需要的成本
    int lb;			    //下界
    bool operator<(const Node& s) const	//重载<关系函数>
    {
        return lb > s.lb;
    }
};
//问题表示
int n = 4;
int c[MAXN][MAXN] = { {0},{0,9,2,7,8},{0,6,4,3,7},
    {0,5,8,1,8},{0,7,6,9,4} };
//下标0的元素不用
int bestx[MAXN];		//最优分配方案
int mincost = INF;		//最小成本
int total = 1;			//结点个数累计
void bound(Node& e)     //求结点e的限界值
{
    int minsum = 0;
    for (int i1 = e.i + 1;i1 <= n;i1++)      //寻找每一行的最小值
    {                                        //如果找到e这个节点，如果说他选择了某个任务的话，就不能选择那一列的值了
        int minc = INF;
        for (int j1 = 1;j1 <= n;j1++)
            if (e.worker[j1] == false && c[i1][j1] < minc)
                minc = c[i1][j1];
        minsum += minc;
    }
    e.lb = e.cost + minsum;          //e.cost代表选择的结点成本+剩下的最小的成本
    //cout << e.lb << " ";           //可以自行选择输出不输出
}

void bfs()
{
    int j;
    Node e, e1;                  //e,e1相当于两个儿，帮忙运进队列的
    priority_queue<Node> qu;
    memset(e.x, 0, sizeof(e.x));               //解向量
    memset(e.worker, 0, sizeof(e.worker));     //任务是否分配
    e.i = 0;                      //根节点也是虚结点
    e.cost = 0;
    bound(e);
    e.no = total++;
    qu.push(e);
    while (!qu.empty())
    {
        e = qu.top();
        qu.pop();
        if (e.i == n)
        {
            if (e.cost < mincost)
            {
                mincost = e.cost;
                for (j = 1;j <= n;j++)
                    bestx[j] = e.x[j];
            }
        }
        e1.i = e.i + 1;         //相当于在根节点的情况下开始拓展进行下一个节点
        for (j = 1;j <= n;j++)
        {
            if (e.worker[j])     //查看任务j是否分配
                continue;
            for(int i1 = 1;i1 <= n;i1++)
                e1.x[i1] = e.x[i1];         //相当于对e1初始化（1）
            e1.x[e1.i] = j;
            for (int i2 = 1;i2 <= n;i2++)
                e1.worker[i2] = e.worker[i2];     //相当于对e1初始化（2)  :::(1)(2)就相当于创建了一个新的结点并且对他初始化
            e1.worker[j] = true;           //这个代表的是它的第几个任务被选择
            e1.cost = e.cost + c[e1.i][j];
            bound(e1);
            e1.no = total++;
            if (e1.lb <= mincost)     //剪枝
                qu.push(e1);
        }
    }
}
int main()
{
    bfs();
    cout << "最优方案：" << endl;
    for (int k = 1;k <= n;k++)
    {
        cout << "第" << k << "个人员分配第" << bestx[k] << "个任务" << endl;
    }
    cout << "总成本" << mincost;
    return 0;
}
