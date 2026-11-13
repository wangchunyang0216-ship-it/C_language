#include<stdio.h>
#include<stdlib.h>
#include"BTreeBook.h"
//#include <iostream>
//
//using namespace std;
//#include<cstring.h>


int main() {
	BTree T;
	T = NULL;
	result r;
    system("color F0");

    while(1)
    {
        system("cls");
        printf(" |----------------------------------------------------|\n");
        printf(" |                                                    |\n");
        printf(" |           |  基于B树的图书管理信息系统  |          |\n");
        printf(" |                                                    |\n");
        printf(" |     111：初始化（第一次使用请先初始化！！！）      |\n");
        printf(" |     1：采编入库                 2：清除库存        |\n");
        printf(" |     3：查找图书                 4：输出B树的状态   |\n");
        printf(" |     5：借阅图书                 6：归还图书        |\n");
        printf(" |     7：退出                                        |\n");
        printf(" |                                                    |\n");
        printf(" |                                                    |\n");
        printf(" |请选择进行功能序号                                  |\n");
        printf(" |                                                    |\n");
        printf(" |注：第一次使用请先输入111！！！                     |\n");
        printf(" |----------------------------------------------------|\n");
        char bn[30], bw[20], bp[30];
        int a;
        scanf("%d",&a);
        if(a==111)
        {
            InitBTree(T);
            cout<<"B树已创建完成，下面输出已创建完成的B树"<<endl;
            PrintBTree(T);

            printf(" 初始化完成!\n");

            printf("**************************************************************************************\n");

            system("pause");
            continue;
        }
        else if(a==1)
        {
            system("cls");
			printf("请输入书的编号\n");
			int key01;
			scanf("%d", &key01);
			InsearchBTree(T, key01, r);
			if (r.tag == 1) {
				printf("此书本已存在\n");
				ShowBook(T, key01);
				int n1;
				printf("请输入需要添加此书本的数量\n");
				scanf("%d", &n1);
				addBook(T, key01, n1);
			}
			else {
				printf("此书本不存在，\n");
				//char bn[30], bw[20], bp[30];
				printf("请按顺序输入此书的书名、作者（用空格隔开）\n");
				scanf("%s%s", &bn,&bw);
				//cin>>bn>>bw>>bp;
                printf("请输入需要添加此书本的数量\n");
				int n2;
				scanf("%d", &n2);
				InsertBook(T, key01, r, bn, bw, n2);
			}
			printf("添加完成!\n");
			ShowBook(T, key01);
			PrintBTree(T);
			printf("**************************************************************************************\n");
            system("pause");
           continue;

        }
        else if(a==2)
        {
            system("cls");
			printf("请输入书的编号\n");
			int key02;
			scanf("%d", &key02);
			InsearchBTree(T, key02, r);
			if (r.tag == 0)
				printf("此书本不存在！\n");
			else {
				ShowBook(T, key02);
				printf("提示：清除后不可恢复！请确认是否清除：\n");
				printf("  1：确认清除       0：取消  \n");
				int a;
				scanf("%d", &a);
				if (a == 1) {
					DeleteBook(T, key02);
					printf("清除成功!\n");
				}
				else
					printf("已取消\n");
			}
			PrintBTree(T);
			printf("**************************************************************************************\n");
            //Output_system(B);
            system("pause");
            continue;
        }
        else if(a==3)
        {
            system("cls");
			printf("请输入书的编号\n");
			int key05;
			scanf("%d", &key05);
			InsearchBTree(T, key05, r);
			if (r.tag == 0)
				printf("此书本不存在！\n");
			else {
				ShowBookAll(T, key05);

			}
			printf("**************************************************************************************\n");
			system("pause");
            continue;



        }
        else if(a==4)
        {
            system("cls");
			PrintBTree(T);
			printf("**************************************************************************************\n");
            system("pause");
            continue;




        }
        else if(a==5)
        {




            system("cls");
 			printf("请输入书的编号\n");
			int key03;
			scanf("%d", &key03);
			InsearchBTree(T, key03, r);
			if(r.tag==0)
				printf("此书本不存在！\n");
			else {
				ShowBook(T, key03);
				if (examineBook(r.pt, r.i))
					printf("此书本现库存不足，暂时无法借阅！\n");
				else {
					printf("请确认是否借书：\n");
					printf("1：确认   0：取消  \n");
					int a;
					scanf("%d", &a);
					if (a == 1) {
						printf("请输入借阅证号：\n");
						int q;
						scanf("%d", &q);
						printf("请输入姓名：\n");
						char name[20];
						scanf("%s", name, sizeof(name));
						BorrowBook(r, q, name);
						ShowBook(T, key03);
						printf("借书成功!\n");
					}
					else
						printf("已取消\n");
				}
			}

			printf("**************************************************************************************\n");
            system("pause");
            continue;


        }
        else if(a==6)
        {

            system("cls");
			printf("请输入书的编号\n");
			int key04;
			scanf("%d", &key04);
			InsearchBTree(T, key04, r);
			if (r.tag == 0)
				printf("此书本不存在！\n");
			else {
				ShowBook(T, key04);
				printf("请确认是否还书：\n");
				printf("1：确认   0：取消  \n");
				int b;
				scanf("%d", &b);
				if ( b == 1) {
					printf("请输入借阅证号：\n");
					int q;
					scanf("%d", &q);
					printf("请输入姓名：\n");
					char name[20];
					scanf("%s", name, sizeof(name));
					if (ReturnBook(r, q, name)) {
						ShowBook(T, key04);
						printf("还书成功!\n");
					}
					else
						printf("还书失败!查无对应的借阅者！\n");
				}
				else
					printf("已取消\n");
			}
			printf("**************************************************************************************\n");
			system("pause");
            continue;




        }
        else if(a==7)
        {
            system("cls");
            printf("\n\n\n\n\n\n\n\n* * * * 图书管理系统使用完毕 欢迎您下次继续使用 * * * *\n\n\n\n\n");
            break;


            continue;
        }
        else
        {
            system("cls");
            printf("您输入的数字有误，请重新输入！");
            system("pause");
        }

    }








//	int flag = 1;
//	while (flag)
//	{
//		int i;
//		scanf("%d", &i);
//		switch (i)
//		{
//		case 1:
//            system("cls");
//			printf("请输入书的编号\n");
//			int key01;
//			scanf("%d", &key01);
//			InsearchBTree(T, key01, r);
//			if (r.tag == 1) {
//				printf("此书本已存在\n");
//				ShowBook(T, key01);
//				int n1;
//				printf("请输入需要添加此书本的数量\n");
//				scanf("%d", &n1);
//				addBook(T, key01, n1);
//			}
//			else {
//				printf("此书本不存在，\n");
//				char bn[30], bw[20], bp[30];
//				printf("请按顺序输入此书的书名、作者、出版社（用空格隔开）\n");
//				scanf("%s%s%s", bn,30,bw,20,bp,30);
//                printf("请输入需要添加此书本的数量\n");
//				int n2;
//				scanf("%d", &n2);
//				InsertBook(T, key01, r, bn, bw, bp, n2);
//			}
//			printf("添加完成!\n");
//			ShowBook(T, key01);
//			PrintBTree(T);
//			printf("**************************************************************************************\n");break;
//		case 2:
//			printf("请输入书的编号\n");
//			int key02;
//			scanf("%d", &key02);
//			InsearchBTree(T, key02, r);
//			if (r.tag == 0)
//				printf("此书本不存在！\n");
//			else {
//				ShowBook(T, key02);
//				printf("提示：清除后不可恢复！请确认是否清除：\n");
//				printf("  1：确认清除       0：取消  \n");
//				int a;
//				scanf("%d", &a);
//				if (a == 1) {
//					DeleteBook(T, key02);
//					printf("清除成功!\n");
//				}
//				else
//					printf("已取消\n");
//			}
//			PrintBTree(T);
//			printf("**************************************************************************************\n");break;
//		case 3:
//			printf("请输入书的编号\n");
//			int key03;
//			scanf("%d", &key03);
//			InsearchBTree(T, key03, r);
//			if(r.tag==0)
//				printf("此书本不存在！\n");
//			else {
//				ShowBook(T, key03);
//				if (examineBook(r.pt, r.i))
//					printf("此书本现库存不足，暂时无法借阅！\n");
//				else {
//					printf("请确认是否借书：\n");
//					printf("1：确认   0：取消  \n");
//					int a;
//					scanf("%d", &a);
//					if (a == 1) {
//						printf("请输入借阅证号：\n");
//						int q;
//						scanf("%d", &q);
//						printf("请输入姓名：\n");
//						char name[20];
//						scanf("%s", name, sizeof(name));
//						BorrowBook(r, q, name);
//						ShowBook(T, key03);
//						printf("借书成功!\n");
//					}
//					else
//						printf("已取消\n");
//				}
//			}
//
//			printf("**************************************************************************************\n");break;
//		case 4:
//			printf("请输入书的编号\n");
//			int key04;
//			scanf("%d", &key04);
//			InsearchBTree(T, key04, r);
//			if (r.tag == 0)
//				printf("此书本不存在！\n");
//			else {
//				ShowBook(T, key04);
//				printf("请确认是否还书：\n");
//				printf("1：确认   0：取消  \n");
//				int b;
//				scanf("%d", &b);
//				if ( b == 1) {
//					printf("请输入借阅证号：\n");
//					int q;
//					scanf("%d", &q);
//					printf("请输入姓名：\n");
//					char name[20];
//					scanf("%s", name, sizeof(name));
//					if (ReturnBook(r, q, name)) {
//						ShowBook(T, key04);
//						printf("还书成功!\n");
//					}
//					else
//						printf("还书失败!查无对应的借阅者！\n");
//				}
//				else
//					printf("已取消\n");
//			}
//			printf("**************************************************************************************\n");break;
//		case 5:
//            system("cls");
//			printf("请输入书的编号\n");
//			int key05;
//			scanf("%d", &key05);
//			InsearchBTree(T, key05, r);
//			if (r.tag == 0)
//				printf("此书本不存在！\n");
//			else {
//				ShowBookAll(T, key05);
//
//			}
//			printf("**************************************************************************************\n");break;
//		case 6:
//			PrintBTree(T);
//			printf("**************************************************************************************\n");break;
//		case 0:flag = 0;break;
//		default:
//			printf("输入错误，请重新输入!\n");
//			printf("**************************************************************************************\n");break;
//		}
//	}
	return 1;
}
