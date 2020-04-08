### 共享内存常用api

int **shmget**(key_t key,size_t size,int shmflg)

​		key_t key: 16进制，非0数据 。 描述当前共享内存状态。shared -- private

​	    size_t size ： 大小

​		int shmflg: 状态：独些执行权限 --8进制。0644.

​												IPC_CREATE | IPC_EXCL

​		返回值： 成功0，失败-1，设置errorno

void * **shmat**(int shmid,const void * shmaddr,int shmflg)

​		shmid: shmget 返回值

  		shmaddr： 建议传入地址。默认NULL内核自动分配

​        shmflg: SHM_RND 读写 。 SHM_RDONLY 只读

​		 返回值： 成功映射内存首地址。失败（void*） -1 ,设置errorno

int  **shmdt**(const void * shmaddr) 

​       取消进程与共享内存的关联关系 使用了次函数 ，引用计数会减少



int **shmctl**(int shmid,int cmd,struct shmid_ds * buf)

​    shmid:shmget 返回值

   cmd: 	IPC_RMID : 标记共享内存即将被销毁（引入计数变为 0 ）

​	  当nattch 没有关联的时候，会被干掉。

   返回值： 成功0，失败-1，设置errorno。	

ipcs 查看共享内存。



linux 内核管理共享内存的方法

key值+ 引用计数。

share memory segments 

key         shmid        owner   perms     bytes  nattch   status

0018       5734417    test      644         256       4                          --- 标记删除前

0000      5734417     test    644              256    3           dest       --- 标记删除后

标记删除后，key 值发生变化 0018 变为 0000 （private 状态）

多个进程，shmget 加入进来，没有创建，有则open. 这个时候共享内存是共有(share) 状态。

其中某个进程 执行了， shmctl(IPC_RMID) 操作，key 由 原来的0018 变为 0000（private ）， 这个private 是针对其它没有加入进来的进程来说的**。针对之前没有删除的，依旧是共享的。其它的进程也不能加入进来了**。

### 实现共享内存的步骤：

- 创建共享内存，shmget 函数实现,生成的共享内存块将与某个特定的key(shmget 的第一个参数绑定)

- 映射共享内存 ，shmat 来实现

- 访问共享内存，shmget 。进程1 生成key,进程2 通过shmget ，传入同一个key,进程2执行shmat ，将内存映射到它的空间中。

- 进程通信。共享内存的各个进程实现了内存映射后，便可以利用该区域进行信息交换。**由于内存共享本身没有同步机制，所以参与通信的诸进程需要自己协商处理**。

- 撤销内存映射区。shmdt 来实现。引用计数会减少

- 删除内存共享。最后必须删除内存共享，shctl 函数实现。

  





### shmdt  与 **shmctl**

**当一个进程不再需要共享内存段时，它将调用shmdt()系统调用取消这个段，但是，这并不是从内核真正地删除这个段，而是把相关shmid_ds结构的 shm_nattch域的值减1，当这个值为0时，内核才从物理上删除这个共享段**





**IPC_RMID 命令实际上不从内核删除一个段，而是仅仅把这个段标记为删除，实际的删除发生在最后一个进程离开这个共享段时。**





