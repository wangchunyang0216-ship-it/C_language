#include <iostream>

using namespace std;

#include<stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>


#define TRUE 1
#define FLASE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -2
//Status是函数返回值类型，其值是函数结果状态代码
typedef int Status;
typedef int KeyType;

#define L 1000
#define m 3//B-树的阶，暂设为3

int n;//输入n组图书数据






typedef struct book
{
    //书号、书名、著者、现存量和总库存量
    int Number;//书号
    char title[L];//书名
    char author[L];//著者
    int existing;//现存量
    int totalexist;//总存量
}Book;

typedef struct BTNode
{
    int keynum;//节点中关键字的个数，即节点的大小
    struct BTNode *parent;//指向双亲节点
    int key[m+1];//关键字向量，0号单元未用
    struct BTNode *ptr[m+1];//子树指针向量
    int *recptr[m+1];//记录指针向量，0号单元未用

    BTNode(){
        keynum=0;
        parent=NULL;
        for(int i=0;i<m+1;i++)
        {
            ptr[i]=NULL;
        }
    }

}BTNode,*BTree;//B-树的节点和B-树的类型

typedef struct
{
    BTNode *pt;//指向能找到的节点
    int i;//1~m,在节点中的关键字序号
    int tag;//1表示查找成功，0表示查找失败
}result;//B-树的查找结果类型



void Input_system(Book B[]);
void Output_system(Book B[]);





//int Search(BTree T,KeyType key)//为了找到关键字下标K[i]的i
//{
//    BTree p,q;
//    p=new BTree;
//    p=T;
//    q=new BTree;
//    int i=1;
//    while()
//
//}

//int Search(BTree p,int k){  //在p->key[1..p->keynum]找k
//    int i=1;
//    while(i<=p->keynum&&k>p->key[i]) i++;
//    return i;
//}
//
//Result SearchBTree(BTree T,KeyType key)
//{
//    //在m阶B-树上查找关键字key，返回结果（pt，i，tag）
//    //若查找成功，则特征值tag=1，指针pt所指节点的第i个关键字等于key
//    //否则特征值tag=0，等于key的关键字应插入在指针pt所指节点中的第i个和第i+1个关键字关键字之间
//    BTree p,q;
//    p=new BTree;
//    int found,i;
//    p=T;//p指向待查节点
//    q=NULL;//q指向p的双亲
//    found=FALSE;
//    i=0;
//    while(p&&!found)
//    {
//        i=Search(p,key);
//        //在p->K[1..keynum]中查找i，使得p->K[i]<=key<p->K[i+1]
//        if(i>0 && p->K[i]==key)
//            found=TRUE;//找到待查关键字
//        else
//        {
//            q=p;
//            p=p->ptr[i];
//        }
//    }
//    if(found) return (p,i,1);//查找成功
//    else return (q,i,0);//查找不成功，返回key的插入位置信息
//
//}
//
//Status InsertBTree(BTree &T,KeyType key,BTree q,int i)
//{
//    //在m阶B-树T上节点*q的K[i]与K[i+1]之间插入关键字key
//    //若引起节点过大，则沿双亲链进行必要的节点分裂调整，使T仍是m阶B-树
//    KeyType x;
//    int finished;
//    int s;
//    x=key;
//    ap=NULL;
//    finished=FALSE;
//    while(q&&!finished)
//    {
//        Insert(q,i,x,ap);//将x和ap分别插入q->key[i+1]和q->ptr[i+1]
//        if(q->keynum<m)//插入完成
//            finished=TRUE;
//        else//分裂节点
//        {
//            s=(m+1)/2;
//            split(q,s,ap);
//            x=q->K[s];
//            //将q->K[s+1..m],q->ptr[s..m]和q->recptr[s+1..m]移入新节点*ap
//            q=q->parent;
//            if(q)
//                i=Search(q,x);//在双亲节点*q中查找x的插入位置
//
//        }
//    }
//    if(!finished)//T是空树（参数q初值为NULL）或者根节点已分裂为节点*q和*ap
//        NewRooT(T,q,x,ap);//生成含信息（T，x，ap）的新的跟节点*T,原T和ap为子树指针
//    return OK;
//
//}

int Search(BTree p,int k){  //在p->key[1..p->keynum]找k
    int i=1;
    while(i<=p->keynum&&k>p->key[i]) i++;
    return i;
}
void SearchBTree(BTree t,int k,result &r){
    //在m阶B树t上查找关键字k，用r返回(pt,i,tag).
    //若查找成功，则标记tag=1，指针pt所指结点中第i个关键字等于k；
    //否则tag=0，若要插入关键字为k的记录，应位于pt结点中第i-1个和第i个关键字之间
    int i=0,found=0;
    BTree p=t,q=NULL;//初始，p指向根节点，p将用于指向待查找结点，q指向其双亲
    while(p!=NULL&&0==found){
        i=Search(p,k);
        if(i<=p->keynum&&p->key[i]==k) found = 1 ;
        else{q=p;p=p->ptr[i-1];}//指针下移
    }
    if(1==found){//查找成功，返回k的位置p及i
        r={p,i,1};
    }
    else{//查找不成功，返回k的插入位置q及i
        r={q,i,0};
    }
}
void split(BTree &q,int s,BTree &ap){//将q结点分裂成两个结点，前一半保留在原结点，后一半移入ap所指新结点
    int i,j,n=q->keynum;
    ap=(BTNode*)malloc(sizeof(BTNode));//生成新结点
    ap->ptr[0]=q->ptr[s];
    for(i=s+1,j=1;i<=n;i++,j++){    //后一半移入ap结点
        ap->key[j]=q->key[i];
        ap->ptr[j]=q->ptr[i];
    }
    ap->keynum=n-s;
    ap->parent=q->parent;
    for(i=0;i<=n-s;i++)
        if(ap->ptr[i]!=NULL) ap->ptr[i]->parent=ap;
    q->keynum=s-1;
}
void newRoot(BTree &t,BTree p,int x,BTree ap){//生成新的根结点
    t=(BTNode*)malloc(sizeof(BTNode));
    t->keynum=1;t->ptr[0]=p;t->ptr[1]=ap;t->key[1]=x;
    if(p!=NULL) p->parent = t;
    if(ap!=NULL) ap->parent=t;
        t->parent = NULL;
}
void Insert(BTree &q,int i,int x,BTree ap){//关键字x和新结点指针ap分别插到q->key[i]和q->ptr[i]
    int j,n=q->keynum;
    for(j=n;j>=i;j--){
        q->key[j+1]=q->key[j];
        q->ptr[j+1]=q->ptr[j];
    }
    q->key[i]=x;q->ptr[i]=ap;
    if(ap!=NULL) ap->parent=q;
    q->keynum++;
}
void InsertBTree(BTree &t,int k,BTree q,int i){
    //在B树中q结点的key[i-1]和key[i]之间插入关键字k
    //若插入后结点关键字个数等于b树的阶，则沿着双亲指针链进行结点分裂，使得t仍是m阶B树
    int x,s,finished = 0,needNewRoot = 0;
    BTree ap;
    if(NULL==q) newRoot(t,NULL,k,NULL);
    else{
        x=k;ap=NULL;
        while(0==needNewRoot&&0==finished){
            Insert(q,i,x,ap);//x和ap分别插到q->key[i]和q->ptr[i]
            if(q->keynum<m) finished=1;//插入完成
            else{
                s = (m+1)/2;split(q,s,ap);x=q->key[s];
                if(q->parent!=NULL){
                    q=q->parent;i=Search(q,x);//在双亲结点中查找x的插入位置
                }
                else needNewRoot=1;
            }
        }
        if(1==needNewRoot)//t是空树或者根结点已经分裂成为q和ap结点
            newRoot(t,q,x,ap);
    }
}




void Remove(BTree &p,int i)
{
    int j,n=p->keynum;
    for(j=i;j<n;j++){
        p->key[j]=p->key[j+1];
        p->ptr[j]=p->ptr[j+1];
    }
    p->keynum--;
}
void Successor(BTree &p,int i){//由后继最下层非终端结点的最小关键字代替结点中关键字key[i]
    BTree child = p->ptr[i];
    while(child->ptr[0]!=NULL) child=child->ptr[0];
    p->key[i]=child->key[1];
    p=child;
}
void Restore(BTree &p, int i,BTree &T) {//对B树进行调整
    int j;
    BTree ap=p->parent;
    if(ap==NULL) //若调整后出现空的根结点，则删除该根结点，树高减1
    {
            T=p; //根结点下移
            p=p->parent;
            return ;
    }
    BTree lc,rc,pr;
    int finished = 0 ,r=0;
    while(!finished)
    {
        r=0;
        while(ap->ptr[r]!=p) r++; //确定p在ap子树中的位置
        if(r==0)
        {
            r++;
            lc=NULL,rc=ap->ptr[r];
        }
        else if(r==ap->keynum)
        {
            rc=NULL;lc=ap->ptr[r-1];
        }
        else
        {
             lc=ap->ptr[r-1];rc=ap->ptr[r+1];
        }
        if(r>0&&lc!=NULL&&(lc->keynum>(m-1)/2))//向左兄弟借关键字
        {
            p->keynum++;
            for(j=p->keynum;j>1;j--)//结点关键字右移
            {
                 p->key[j]=p->key[j-1];
                 p->ptr[j]=p->ptr[j-1];
            }
            p->key[1]=ap->key[r];//父亲插入到结点
            p->ptr[1]=p->ptr[0];
            p->ptr[0]=lc->ptr[lc->keynum];
            if(NULL!=p->ptr[0])//修改p中的子女的父结点为p
            {
                p->ptr[0]->parent=p;
            }
            ap->key[r]=lc->key[lc->keynum];//左兄弟上移到父亲位置
            lc->keynum--;
            finished=1;
            break;
        }
        else if(ap->keynum>r&&rc!=NULL&&(rc->keynum>(m-1)/2)) //向右兄弟借关键字
        {
            p->keynum++;
            p->key[p->keynum]=ap->key[r]; //父亲插入到结点
            p->ptr[p->keynum]=rc->ptr[0];
            if(NULL!=p->ptr[p->keynum]) //修改p中的子女的父结点为p
                p->ptr[p->keynum]->parent=p;
            ap->key[r]=rc->key[1]; //右兄弟上移到父亲位置
            rc->ptr[0]=rc->ptr[1];
            for(j=1;j<rc->keynum;j++)  //右兄弟结点关键字左移
            {
                rc->key[j]=rc->key[j+1];
                rc->ptr[j]=rc->ptr[j+1];
            }
            rc->keynum--;
            finished=1;
            break;
        }
        r=0;
        while(ap->ptr[r]!=p) //重新确定p在ap子树的位置
            r++;
        if(r>0&&(ap->ptr[r-1]->keynum<=(m-1)/2)) //与左兄弟合并
        //if(r>0) //与左兄弟合并
        {
            lc=ap->ptr[r-1];
            p->keynum++;
            for(j=p->keynum;j>1;j--) //将p结点关键字和指针右移1位
            {
                p->key[j]=p->key[j-1];
                p->ptr[j]=p->ptr[j-1];
            }
            p->key[1]=ap->key[r]; //父结点的关键字与p合并
            p->ptr[1]=p->ptr[0]; //从左兄弟右移一个指针
            ap->ptr[r]=lc;
            for(j=1;j<=lc->keynum+p->keynum;j++) //将结点p中关键字和指针移到p左兄弟中
            {
                lc->key[lc->keynum+j]=p->key[j];
                lc->ptr[lc->keynum+j]=p->ptr[j];
            }
            if(p->ptr[0]) //修改p中的子女的父结点为lc
            {
                for(j=1;j<=p->keynum;j++)
                if(p->ptr[p->keynum+j])   p->ptr[p->keynum+j]->parent=lc;
            }
            lc->keynum=lc->keynum+p->keynum;  //合并后关键字的个数
            for(j=r;j<ap->keynum;j++)//将父结点中关键字和指针左移
            {
                ap->key[j]=ap->key[j+1];
                ap->ptr[j]=ap->ptr[j+1];
            }
            ap->keynum--;
            pr=p;free(pr);
            pr=NULL;
            p=lc;
        }
        else //与右兄弟合并
        {
            rc=ap->ptr[r+1];
            if(r==0)
                r++;
            p->keynum++;
            p->key[p->keynum]=ap->key[r]; //父结点的关键字与p合并
            p->ptr[p->keynum]=rc->ptr[0]; //从右兄弟左移一个指针
            rc->keynum=p->keynum+rc->keynum;//合并后关键字的个数
            ap->ptr[r-1]=rc;
            for(j=1;j<=(rc->keynum-p->keynum);j++)//将p右兄弟关键字和指针右移
            {
                rc->key[p->keynum+j]=rc->key[j];
                rc->ptr[p->keynum+j]=rc->ptr[j];
            }
            for(j=1;j<=p->keynum;j++)//将结点p中关键字和指针移到p右兄弟
            {
                rc->key[j]=p->key[j];
                rc->ptr[j]=p->ptr[j];
            }
            rc->ptr[0]=p->ptr[0]; //修改p中的子女的父结点为rc
            if(p->ptr[0])
            {
                for(j=1;j<=p->keynum;j++)
                if(p->ptr[p->keynum+j])    p->ptr[p->keynum+j]->parent=rc;
            }
            for(j=r;j<ap->keynum;j++)//将父结点中关键字和指针左移
            {
                ap->key[j]=ap->key[j+1];
                ap->ptr[j]=ap->ptr[j+1];
            }
            ap->keynum--;//父结点的关键字个数减1
            pr=p;
            free(pr);
            pr=NULL;
            p=rc;
        }
        ap=ap->parent;
        if(p->parent->keynum>=(m-1)/2||(NULL==ap&&p->parent->keynum>0))
            finished=1;
        else if(ap==NULL) //若调整后出现空的根结点，则删除该根结点，树高减1
        {
            pr=T;
            T=p; //根结点下移
            free(pr);
            pr=NULL;
            finished=1;
        }
        p=p->parent;
    }
}
void DeleteBTree(BTree &p,int i,BTree &T){//删除B树上p结点的第i个关键字
    if(p->ptr[i]!=NULL){//若不是在最下层非终端结点
        Successor(p,i);//在Ai子树中找出最下层非终端结点的最小关键字替代ki
        DeleteBTree(p,1,T);//转换为删除最下层非终端结点的最小关键字
    }else{//若是最下层非终端结点
        Remove(p,i);
        if(p->keynum<(m-1)/2)//删除后关键字个数小于(m-1)/2
            Restore(p,i,T);//调整B树
    }
}

void show_Btree(BTree &p)
{
    if(p==NULL) {puts("B tree does not exist");return ;}
    bool have_child = false ;
    printf("[");
    for(int i=1;i<=p->keynum;i++)
    {
        if(i==1) ;
        else printf(" ");
        printf("%d",p->key[i]);
    }
    printf("]");
    for(int i=0;i<=p->keynum;i++)
    {
        if(p->ptr[i]!=NULL)
        {
            if(i==0) printf("<");
            else printf(",");
            show_Btree(p->ptr[i]);
            have_child = true ;
        }
    }
    if(have_child) printf(">");
}

void show_Btree_quanbudongxi(BTree &p)//输出全部东西
{
    int a;
    if(p==NULL) {puts("B tree does not exist");return ;}
    for(int i=1;i<=p->keynum;i++)
    {
        printf("%d ",p->key[i]);
        a=p->key[i];
        cout<<B[a].title<<' '<<B[a].author<<' '<<B[a].existing<<' '<<B[a].totalexist<<endl;
    }
    for(int i=0;i<=p->keynum;i++)
    {
        if(p->ptr[i]!=NULL)
        {
            show_Btree_quanbudongxi(p->ptr[i]);
        }
    }
}


void show_Btree2(BTree &p,int deep)
{
    if(p==NULL) {return ;}
    int i ;
    for(i = 0 ; i < p->keynum ;i++)
    {
        show_Btree2(p->ptr[i],deep+1);
        for(int i=0;i<deep;i++)
        {
            printf("\t");
        }
        printf("%d\n",p->key[i+1]);
    }
    show_Btree2(p->ptr[i],deep+1);

}
//void Destory(BTree &t)
//{
//    int i = 0 ;
//    if(t!=NULL)
//    {
//        while(i<t->keynum)
//        {
//            Destory(t->ptr[i]);
//            free(t->ptr[i]);
//            i++;
//        }
//    }
//    free(t);
//    t=NULL;
//}
//void creat_btree()//建一个空树
//{
//    T = new BTNode ;
//    T->keynum=0;
//    puts("New success");
//}
void CreatBTree(BTree &T,Book B[])//构建图书系统B的索引
{
    result p ;
    int j;
    int a[100];
    for(int k=0;k<100;k++)
    {
        if(B[k]!=NULL)
        {
            a[g]=B[k];
            g++;
        }
    }
    for(j=0;j<n;j++)
    {
        SearchBTree(T,B[j].Number,p);
        if(p.tag==0)
        {
            InsertBTree(T,B[j].Number,p.pt,p.i);
            //puts("Insert success");
            //show_Btree(T);
            //puts("");
        }
    }
    show_Btree(T);
}

void insert_keytype(BTree &T,Book B[])//插入关键字
{
    cout<<"输入要插入的关键字(图书书号)"<<endl;
    KeyType temp;
    cin>>temp;
    result p ;
    SearchBTree(T,temp,p);
    if(p.tag==0)
    {
        InsertBTree(T,temp,p.pt,p.i);
        cout<<"插入成功！"<<endl;
        show_Btree(T);
        puts("");
    }
    else cout<<"该书号在图书系统已存在"<<endl;
}


//void Insearchshuzu(Book B[],int c)//在数组B中进行查找
//{
//    for(int i=0;i<n;i++)
//    {
//        if(c==B[i].Number)
//        {
//            cout<<"输出查找图书的书号,书名，作者，现存量，总存量\n";//输入字符串以\n结尾
//            cout<<B[i].Number<<' '<<B[i].title<<' '<<B[i].author<<' '<<B[i].existing<<' '<<B[i].totalexist<<endl;
//            break;
//        }
//    }
//}



void find_keytype(BTree &T,Book B[])
{
    puts("请输入要查找的书号\n");
    KeyType temp;
    scanf("%d",&temp);
    result p ;
    SearchBTree(T,temp,p);
    if(p.tag)
    {
        puts("查找成功！\n");
        //Insearchshuzu(B,temp);
        //cout<<p.pt->recptr[p.i]<<endl;
    }
    else puts("查找失败！\n");
}
void delete_keytype(BTree &T)
{
    cout<<"输入要删除的关键字（图书的书号）"<<endl;
    KeyType temp;
    cin>>temp;
    result p ;
    SearchBTree(T,temp,p);
    if(p.tag)
    {
        DeleteBTree(p.pt,p.i,T);
        cout<<"删除成功！"<<endl;
        show_Btree(T);
        puts("");
    }
    else cout<<"该关键字在B树中不存在！"<<endl;
}


void chushihua(Book B[])
{
    for(int i=0;i<100;i++)
        B[i]=NULL;
}


int main()
{
    Book B[100];
    chushihua(B);
//    int i=0;
//
//    cout<<"要输入几组图书数据？"<<endl;
//    cin>>n;
//    cout<<"具体输入"<<n<<"组数据"<<endl;
//    for(i=0;i<n;i++)//输入
//    {
//        cout<<"请依次输入图书的书号,书名，作者，现存量，总存量\n";//输入字符串以\n结尾
//        cin>>B[i].Number>>B[i].title>>B[i].author>>B[i].existing>>B[i].totalexist;
//    }
//       for(i=0;i<n;i++)//输出
//    {
//        //cout<<"请依次输入图书的书号,书名，作者，现存量，总存量\n";
//        cout<<B[i].Number<<' '<<B[i].title<<' '<<B[i].author<<' '<<B[i].existing<<' '<<B[i].totalexist<<endl;
//    }
    BTree T;
    T=new BTNode;


    int a;
    while(1)
    {
        printf("******************************************************\n\n\n");
        printf("         欢 迎 进 入 图 书 管 理 系 统          \n\n\n");
        printf("       输 入 菜 单 选 项 前 的 序 号 进 入 功 能      \n\n\n");
        printf("******************************************************\n\n\n");
        printf("*****************图书管理系统功能菜单*****************\n\n");
        printf("第一使用请先录入图书信息！！！！\n\n");
        printf("--------------------------------------------\n");
        printf("*1.首次录入图书信息*    *2.显示图书系统信息    *\n\n");
        printf("*3.构建B树搜索树，查询    *    *4.增加图书信息    *\n\n");
        printf("*5.删除图书信息    *    *6.借阅             *\n\n");
        printf("*7.归还            *    *8.显示图书借阅系统*\n");
        printf("*9.退出系统        *\n");
        printf("--------------------------------------------\n\n\n");
        scanf("%d",&a);
        if(a==1)
        {
           system("cls");
           printf("\n\n\n* * * * 图 书 信 息 录 入 * * * *\n\n\n");
           Input_system(B);
           //Output_system(B);
           continue;
        }
        else if(a==2)
        {
            system("cls");
            printf("\n\n\n* * * * 显 示 图 书 系 统 信 息* * * *\n\n\n");
            //Output_system(B);
            continue;
        }
        else if(a==3)
        {
            system("cls");
           // printf("\n\n\n* * * * 1.构建B树搜索树\t2.查询\t3.返回上一级菜单栏 * * * *\n\n\n");
            int order1;//选择功能
            while(1)
            {
                printf("\n\n\n* * * * 1.构建B树搜索树\t2.查询\t3.返回上一级菜单栏 * * * *\n\n\n");
                cin>>order1;
                if(order1==1)
                {
                    CreatBTree(T,B);continue;
                }
                else if(order1==2)
                {
                    find_keytype(T,B);continue;
                }
                else if(order1==3)
                        break;
                else
                    cout<<"输入错误请重新输入"<<endl;
            }
            continue;
        }
        else if(a==4)
        {
            system("cls");
            printf("\n\n\n* * * * 增 加 图 书 信 息 * * * *\n\n\n");
            insert_keytype(T,B);
            continue;
        }
        else if(a==5)
        {
            system("cls");
            printf("\n\n\n* * * * 删 除 图 书 信 息 * * * *\n\n\n");
            delete_keytype(T);
            continue;
        }
        else if(a==6)
        {
            system("cls");
            printf("\n\n\n* * * * 借 阅 * * * *\n\n\n");

            continue;
        }
        else if(a==7)
        {
            system("cls");
            printf("\n\n\n* * * * 归 还 * * * *\n\n\n");


            continue;
        }
        else if(a==8)
        {
            system("cls");
            printf("\n\n\n* * * * 显 示 图 书 借 阅 系 统 * * * *\n\n\n");
            continue;
        }
        else if(a==9)
        {
            system("cls");
            printf("\n\n\n\n\n\n\n\n* * * * 图书管理系统使用完毕 欢迎您下次继续使用 * * * *\n\n\n\n\n");
            break;
        }
        else
        {
            system("cls");
            printf("您输入的数字有误，请重新输入！");
        }

    }

    return 0;
}

void Input_system(Book B[])
{
    int i=0;
    cout<<"要输入几组图书数据？"<<endl;
    cin>>n;
    cout<<"具体输入"<<n<<"组数据"<<endl;
    for(i=0;i<n;i++)//输入
    {
        int a;
        cout<<"请先输入图书的书号"<<endl;
        cin>>a;
        B[a].Number=a;
        cout<<"请在依次输入书号为"<<a<<"的书名，作者，现存量，总存量\n";//输入字符串以\n结尾
        cin>>B[a].title>>B[a].author>>B[a].existing>>B[a].totalexist;
    }
}

//void Output_system(Book B[])
//{
//    for(int i=0;i<n;i++)//输出
//    {
//        //cout<<"请依次输入图书的书号,书名，作者，现存量，总存量\n";
//        cout<<B[i].Number<<' '<<B[i].title<<' '<<B[i].author<<' '<<B[i].existing<<' '<<B[i].totalexist<<endl;
//    }
//}
