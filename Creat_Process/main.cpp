#include <iostream>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S_pid=0;



//代码 4-1 PCB 数据结构
#define KSTACKSIZE 1024
enum proc_state{UNUSED=0,RUNNABLE,RUNNING,BLOCKED,STOPED};
//runnable 就绪
struct task_struct{
    TrapFrame *tf; //系统寄存器信息
    char kstack[KSTACKSIZE]; //栈
    pid_t pid;
    char pname[20]; //进程名
    enum proc_state state; //进程状态
    ListHead linklist;
};


//代码 4-2 与进程 PCB 相关的几个数据结构
#define NR_PROC 64
struct task_struct task[NR_PROC]; //进程表
extern struct task_struct *current; //当前进程
ListHead RunableList; //就绪队列
int nextpid=0;

//算法 4-1 进程表初始化
void task_init(){
    for(i=0; i<NR_PROC; i++){
        task[i].state = UNUSED; //当前系统中没有进程
    }
}

//算法 4-2 创建 0 号进程
idle_init(){

    task_struct p;

    //使用 task[0]，用于表示内核代码执行流代表的进程。
    task[0].state=RUNNING;
    p=&task[0];
    p->pid=nextpid++; //nextpid 为全局变量，初始化为 0，用于记录进程号
    memcpy(p->pname,"idle",5);
    current=p; //系统启动时，0 号进程为当前进程
}

//算法 4-3 进程创建

struct task_struct *alloc_proc() {
    // Allocate a new PCB
    struct task_struct *new_proc = (struct task_struct *)malloc(sizeof(struct task_struct));
    if (new_proc == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    return new_proc;
}

int kthread_create(void(*proc)(void), const char *name)
{
    task_struct p;

    p=alloc_proc(); //申请空白 PCB
    p=init_pcb(p);//初始化 PCB 信息
    //初始化创建进程操作
    p.pid=1;
    //ListHead RunableList; //就绪队列
    list_add_after(RunableList,p);//将新进程插入就绪队列
    return p.pid;

}


}
A.1 p=alloc_proc()； //申请空白 PCB
A.2 init_pcb(p); //初始化 PCB 信息
A.2.1 初始化标识信息，将系统分配的标识符填入 PCB 中
A.2.2 初始化处理机状态信息，使程序计数器指向程序的入口地址，使栈指针指向
栈顶等；
A.2.3 初始化处理机控制信息，将进程状态设置为就绪态；
A.3 将新进程插入就绪队列
A.4 算法结束，返回 PCB

//算法 4-4 初始化 PCB
struct task_struct init_pcb(struct task_struct *p，void(*proc)(void),const char *name){
    p->pid =nextpid++; //定义进程号
    p->state=RUNNABLE; //设置进程状态
    tf=(TrapFrame*)(memset(p->kstack,0,KSTACKSIZE)+KSTACKSIZE)-1; //初始寄存器组
    p->tf=tf;
    tf->eflags=(uint32_t)(0x01<<9)；
    tf->cs=(uint32_t)KSEL(SEG_KCODE);
    tf->ds=(uint32_t)KSEL(SEG_KDATA);
    tf->gs=(uint32_t)KSEL(SEG_KDATA);
    tf->eip=(uint32_t)proc;
    tf->esp=(uint32_t)tf;
    tf->irq=0x03e8;
    memcpy(p->pname,name,7); //设置进程名字
    list_init(&p->linklist);
    return p;
}





int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
