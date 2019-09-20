#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/time.h>

#include "signaldefine.h"
#include "util.h"

void
MakeNameByTime(OUT char* pcFileName)
{
    time_t     liTime = time(NULL);
    struct tm* stm    = localtime(&liTime);

    struct timeval stv;
    gettimeofday(&stv, NULL);

    sprintf(pcFileName,
            "%04d%02d%02d%02d%02d%02d%03d",
            1900 + stm->tm_year, 1 + stm->tm_mon, stm->tm_mday,
            stm->tm_hour,        stm->tm_min,     stm->tm_sec,
            stv.tv_usec / 1000);
}

int
getPidByName(IN char* pcProgramName)
{
    int iPid = -1; // 负数为未找到

    // 打开路径
    DIR* dir = opendir("/proc");

    if (NULL == dir) {
        return iPid;
    }

    struct dirent* ptr = NULL;

    // 循环读取路径下的每一个文件/文件夹
    while (NULL != (ptr = readdir(dir)))
    {
        // 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
        if ((0 == strcmp(ptr->d_name, ".")) ||
            (0 == strcmp(ptr->d_name, "..")))
        {
            continue;
        }

        if (DT_DIR != ptr->d_type) {
            continue;
        }

        // 大小能装下 cmdline 文件的路径即可
        char caStatusFilePath[50];

        // 生成要读取的文件的路径
        sprintf(caStatusFilePath,
                "/proc/%s/status",
                ptr->d_name);

        // 打开文件
        FILE* fpStatus = fopen(caStatusFilePath, "r");

        if (NULL == fpStatus)
        {
            continue;
        }

        char buf[128] = {0};

        // 大小能装下要识别的命令行文本即可
        char caProgramName[50] = {0};

        if (NULL != fgets(buf,
                          sizeof(buf),
                          fpStatus))
        {
            // * 亦可用于格式中, (即 %*d 和 %*s) 加了星号表示跳过此数据不读入
            // 也就是不把此数据读入参数中
            sscanf(buf, "%*s %s", caProgramName);
        }

        fclose(fpStatus);


        // 如果文件内容满足要求则打印路径的名字（即进程的PID）
        if (0 == strcmp(pcProgramName, caProgramName)) {
            iPid = atoi(ptr->d_name);
            break;
        }

    }

    // 关闭路径
    closedir(dir);

    return iPid;
}

int
SendSysSig(EDefSigType eDST,
           char*       pcProgramName,
           char*       pcSignalContent)
{
    static unsigned short s_KeyCounter = 1;

    int iResult = 0;
    int shmid   = -1;

    do
    {
        if ((NULL == pcProgramName)         ||
            (NULL == pcSignalContent)       ||
            (   0 >= strlen(pcProgramName)) ||
            (   0 >= strlen(pcSignalContent)))
        {
            iResult = -1;
            break;
        }

        int iPID = getPidByName(pcProgramName);

        if (0 > iPID) {
            iResult = -2;
            break;
        }

        // 创建共享内存
        shmid = shmget((key_t)(s_KeyCounter >= 65535) ? // 大于 0 的32位整数
                       (s_KeyCounter = 1) : s_KeyCounter,
                       strlen(pcSignalContent) + 1,       // 共享内存大小，以字节为单位
                       0666 | IPC_CREAT);                 // 所有用户读写权限 且 内存不存在则创建

        if (-1 == shmid)
        {
            iResult = -3;
            break;
        }

        // 将共享内存连接到当前进程的地址空间
        void* shm = shmat(shmid, // 共享内存标识符
                          NULL,  // 指定共享内存出现在进程的内存地址，NULL 让内核决定
                          0);    // SHM_RDONLY 为只读模式，其他为读写模式

        if ((void*)-1 == shm)
        {
            iResult = -4;
            break;
        }

        strcpy((char*)shm, pcSignalContent);

        // 把共享内存从当前进程中分离
        if (-1 == shmdt(shm)) // 共享内存出现在进程的内存地址
        {
            iResult = -5;
            break;
        }

        // union sigval {
        //   int   sival_int;
        //   void* sival_ptr;
        // };
        union sigval val;

        val.sival_int = (short)s_KeyCounter << 16 | (short)(strlen(pcSignalContent) + 1);

        sigqueue(iPID,
                 eDST,
                 val);

    }while(0);

    if (( 0  > iResult) &&
        (-1 != shmid))
    {
        // 删除共享内存
        shmctl(shmid,
               // IPC_STAT 把 shmid_ds 结构中的数据设置为共享内存的当前关联值，
               // 即用共享内存的当前关联值覆盖 shmid_ds 的值
               // IPC_SET  如果进程有足够的权限，
               // 就把共享内存的当前关联值设置为 shmid_ds 结构中给出的值
               // IPC_RMID 删除共享内存段
               IPC_RMID,
               // struct shmid_ds {
               //     uid_t  shm_perm.uid;
               //     uid_t  shm_perm.gid;
               //     mode_t shm_perm.mode;
               // };
               NULL);
    }

    // 无论成败下次都换新数
    s_KeyCounter++;

    return iResult;
}

int
RecvSysSig(siginfo_t*   pifo,
           char*        pcSignalContent,
           unsigned int uiContentSize)
{
    int iResult = 0;

    int   shmid = -1;
    void* shm   = NULL;

    do
    {
        if (NULL == pifo)
        {
            ERROR_PRT("siginfo_t* pifo == NULL");
            iResult = -1;
            break;
        }

        INFO_PRT("si_int: %d\n",    pifo->si_int);
        INFO_PRT("sival_int: %d\n", pifo->si_value.sival_int);
        INFO_PRT("si_ptr: %p\n",    pifo->si_ptr);
        INFO_PRT("sival_ptr: %p\n", pifo->si_value.sival_ptr);

        key_t iKey  = (short)(pifo->si_int  >> 16);
        short iSize = (short)((pifo->si_int << 16) >> 16);

        INFO_PRT("iKey = %d, iSize = %d\n", iKey, iSize);

        if (0 >= iKey)
        {
            ERROR_PRT("iKey = %d\n", iKey);
            iResult = -2;
            break;
        }

        if (iSize > uiContentSize) {
            ERROR_PRT("low capacity buffer, iSize(%d) > uiContentSize(%d)\n",
                      iSize, uiContentSize);
            iResult = -3;
            break;
        }

        // 创建共享内存
        shmid = shmget(iKey,  // 大于 0 的32位整数
                       iSize, // 共享内存大小，以字节为单位
                       0666 | IPC_CREAT); // 所有用户读写权限 且 内存不存在则创建

        if (-1 == shmid)
        {
            ERROR_PRT("shmget failed\n");
            iResult = -4;
            break;
        }

        // 将共享内存连接到当前进程的地址空间
        shm = shmat(shmid, // 共享内存标识符
                    NULL,  // 指定共享内存出现在进程的内存地址，NULL 让内核决定
                    0);    // SHM_RDONLY 为只读模式，其他为读写模式

        if ((void*)-1 == shm)
        {
            ERROR_PRT("shmat failed\n");
            break;
        }

        strcpy(pcSignalContent, (char*)shm);

    }while(0);

    if ((     NULL != shm) &&
        ((void*)-1 != shm))
    {
        // 把共享内存从当前进程中分离
        if (-1 == shmdt(shm)) // 共享内存出现在进程的内存地址
        {
            ERROR_PRT("shmdt failed\n");
        }
    }

    if (-1 != shmid)
    {
        // 删除共享内存
        if (-1 == shmctl(shmid,
                         // IPC_STAT 把 shmid_ds 结构中的数据设置为共享内存的当前关联值，
                         // 即用共享内存的当前关联值覆盖 shmid_ds 的值
                         // IPC_SET  如果进程有足够的权限，
                         // 就把共享内存的当前关联值设置为 shmid_ds 结构中给出的值
                         // IPC_RMID 删除共享内存段
                         IPC_RMID,
                         // struct shmid_ds {
                         //     uid_t  shm_perm.uid;
                         //     uid_t  shm_perm.gid;
                         //     mode_t shm_perm.mode;
                         // };
                         NULL)) // shmid_ds 结构指针
        {
            ERROR_PRT("shmctl(IPC_RMID) failed\n");
        }
    }

    return iResult;
}

void
RegiSysSig(EDefSigType   eDST,
           DEAL_SIG_FUNC pfDSF)
{
    // struct sigaction {
    // void     (*sa_handler)(int);
    // void     (*sa_sigaction)(int, siginfo_t*, void*);
    // sigset_t sa_mask;
    // int      sa_flags;
    // };
    struct sigaction act;

    // sa_flags 成员用于指定信号处理的行为，它可以是以下值的 按位或 组合：
    // SA_RESTART   使被信号打断的系统调用自动重新发起
    // SA_NOCLDSTOP 使父进程在它的子进程暂停或继续运行时不会收到 SIGCHLD 信号
    // SA_NOCLDWAIT 使父进程在它的子进程退出时不会收到 SIGCHLD 信号，这时子进程如果退出也不会成为僵尸进程
    // SA_NODEFER   使对信号的屏蔽无效，即在信号处理函数执行期间仍能发出这个信号
    // SA_RESETHAND 信号处理之后重新设置为默认的处理方式
    // SA_SIGINFO   使用 sa_sigaction 成员而不是 sa_handler 作为信号处理函数
    //              注：回调函数 sa_handler 和 sa_sigaction 只能任选其一
    act.sa_flags     = SA_RESTART | SA_SIGINFO;
    act.sa_sigaction = pfDSF;

    // sa_mask 成员用来指定在信号处理函数执行期间需要被屏蔽的信号集，
    //         特别是当某个信号被处理时，它自身会被自动放入进程的信号掩码，
    //         因此在信号处理函数执行期间这个信号不会再度发生
    // sigemptyset 清空其中所有信号
    sigemptyset(&act.sa_mask);

    // 返回：0 表示成功，-1 表示有错误发生
    sigaction(eDST,  // 更新信息信号
              &act,  // 新处理方式
              NULL); // 原处理方式，可以备份以便恢复
}
