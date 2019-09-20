#include "stdio.h>
#include "stdlib.h>
#include "unistd.h>
#include "signal.h>
#include "sys/ipc.h>
#include "sys/shm.h>
#include "sys/types.h>

// 基 __SIGRTMIN 础上的前 3 个最好不要用，它们是 linux thread 用的
#define SIGCUSTOM (__SIGRTMIN + 9)

void deal_sig(int       sig, // 信号
             // sigqueue 函数的第三个参数(sigval_t )将拷贝到
             // 此信号处理函数的第二个参数(siginfo_t)中的 si_value 成员变量
             siginfo_t* ifo,
             void*     cxt) // 没有使用
{
   printf("Caught sig: %d\n", sig);
       
   sleep(3);
    
    do
    {
       if (NULL == ifo)
       {
           printf("ifo == NULL");
           break;
       }
       
       printf("si_int: %d\n",   ifo->si_int);
       printf("sival_int: %d\n", ifo->si_value.sival_int);
       printf("si_ptr: %p\n",   ifo->si_ptr);
       printf("sival_ptr: %p\n", ifo->si_value.sival_ptr);
       
       key_t iKey  = (short)(ifo->si_int  >> 16);
       short iSize = (short)((ifo->si_int << 16) >> 16);
       
       printf("iKey = %d, iSize = %d\n", iKey, iSize);

       if (0 == iKey)
       {
           break;
       }
       
       // 创建共享内存
       int shmid = shmget(iKey,  // 大于 0 的32位整数
                          iSize, // 共享内存大小，以字节为单位
                          0666 | IPC_CREAT); // 所有用户读写权限 且 内存不存在则创建

       if (-1 == shmid)
       {
          printf("shmget failed\n");
          break;
       }

       // 将共享内存连接到当前进程的地址空间
       void* shm = shmat(shmid, // 共享内存标识符
                         NULL,  // 指定共享内存出现在进程的内存地址，NULL 让内核决定
                         0);    // SHM_RDONLY 为只读模式，其他为读写模式
       if ((void*)-1 == shm)
       {
          printf("shmat failed\n");
          break;
       }

       printf((char*)shm);
       printf("\n");

       // 把共享内存从当前进程中分离
       if (-1 == shmdt(shm)) // 共享内存出现在进程的内存地址
       {
          printf("shmdt failed\n");
          break;
       }

       // 删除共享内存
       if(-1 == shmctl(shmid,
                       // IPC_STAT 把 shmid_ds 结构中的数据设置为共享内存的当前关联值，
                       // 即用共享内存的当前关联值覆盖 shmid_ds 的值
                       // IPC_SET  如果进程有足够的权限，
                       // 就把共享内存的当前关联值设置为 shmid_ds 结构中给出的值
                       // IPC_RMID 删除共享内存段
                       IPC_RMID,
                       // struct shmid_ds {
                       //    uid_t  shm_perm.uid;
                       //    uid_t  shm_perm.gid;
                       //    mode_t shm_perm.mode;
                       // };
                       NULL)) // shmid_ds 结构指针
       {
          printf("shmctl(IPC_RMID) failed\n");
          break;
       }
              
   }while(0);   
}

int
main(void)
{
    // struct sigaction {
    // void    (*sa_handler)(int);
    // void    (*sa_sigaction)(int, siginfo_t*, void*);
    // sigset_t sa_mask;
    // int     sa_flags;
    // };
    struct sigaction act;
   
    // sa_flags 成员用于指定信号处理的行为，它可以是以下值的 按位或 组合：
    // SA_RESTART  使被信号打断的系统调用自动重新发起
    // SA_NOCLDSTOP 使父进程在它的子进程暂停或继续运行时不会收到 SIGCHLD 信号
    // SA_NOCLDWAIT 使父进程在它的子进程退出时不会收到 SIGCHLD 信号，这时子进程如果退出也不会成为僵尸进程
    // SA_NODEFER  使对信号的屏蔽无效，即在信号处理函数执行期间仍能发出这个信号
    // SA_RESETHAND 信号处理之后重新设置为默认的处理方式
    // SA_SIGINFO   使用 sa_sigaction 成员而不是 sa_handler 作为信号处理函数
   //             注：回调函数 sa_handler 和 sa_sigaction 只能任选其一
   act.sa_flags    = SA_SIGINFO;
   act.sa_sigaction = deal_sig;
    
    // sa_mask 成员用来指定在信号处理函数执行期间需要被屏蔽的信号集，
   //        特别是当某个信号被处理时，它自身会被自动放入进程的信号掩码，
   //        因此在信号处理函数执行期间这个信号不会再度发生
    // sigemptyset 清空其中所有信号
   sigemptyset(&act.sa_mask);
    
    // 返回：0 表示成功，-1 表示有错误发生
   sigaction(SIGCUSTOM, // 信号
             &act,     // 新处理方式
             NULL);    // 原处理方式，可以备份以便恢复
    
   sleep(30);
    
   printf("recvsig will exit");
    
    return EXIT_SUCCESS;
}