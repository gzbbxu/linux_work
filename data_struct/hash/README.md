### hash数据结构概述
1 ,hash表是种数据结构。它可以提供快速的插入操作和查找操作。它不以关键字的比较为基础。采用直接寻址技术。
  不管哈希表中有多少数据，插入和删除
  只需要接近常量O(1)的时间级。
### hash 表的缺点 
 1,hash是基于数组的，数组创建之后难于扩展某些哈希表被基本填满时，性能下降的非常严重，所以程序员必须
 要清楚表中要存储多少个数据（或者准备号顶起地把数据转移到更大的哈希表中，这是个费时的操作）
### 求余数
 1,该方法是最为简单的一种方法。他是以表 长 m来除关键字，其余数作为散列地址
 即：h(key) = key%m;
 2,该方法的关键是取m.选取m应时的散列函数值尽可能与关键字的各位相关.m最好为素数。m小于表的长度。
 例如
 	int array[] = {12,3,5,7,8,11,22,6,23,55,45};
	h(key) = key%m;长度为11 ，m就取11
	列1			列2 		冲突1		冲突2		冲突3
	0	        11	        22			55	
	1			12          23          45
	2
	3			 3
	4
	5			 5
	6
	7			 7
	8
	9            9
	10
	11
## 哈希的构造方法
### 直接取地址法
	取关键字或关键字的某个线性函数为哈希地址，即:
	H(key) = key 或者 H（key） = a*key+b;
	其中a,b为常数，（这中哈希函数叫做自身函数）
### 平方取中法
	具体方法，先通过求关键字的平方值扩大相近数的差别，然后根据表长度取中间的几位数作为散列函数值。
	又因为一个乘积的中间几位数和乘数的每一位都相关，
	所以由此产生的散列地址较为均匀（此种情况可以考虑平方取中法）
	例如：
	将一组关键字（0100,0110,1010,1001,0111）平方后得（0010000,0012100,1020100,1002001,0012321）
	若表长度为1000，则可取中间的三位数作为散列地址集。（100,121,201,020,123）.
### 折叠法
	将关键字分割成位数相同的几部分（最后一部分位数可以不同）然后取这几部分叠加和（舍去进位）作为哈希地址。
	例如：
	每一种图书都有一个国际标准的图书编号(ISBN),它是一个10位的十进制数字，若要以它为关键字建立一个哈希表
	,当馆藏书类不到10000 时，可采用折叠法构造一个四位数的哈希函数。
	如编号为:1-442-20586-4
    5864 + 4220 + 04 = 10088 ; H(key) = 0088;
### 随机数法
	暂不考虑
### 把关键字变为整数
	对于非整数的关键字key,如字符串key = "abc" ,可以将字符串中的每个字符在ascii
	中对应的整数值，然后相加，再用上面讲到的方法计算哈希地址即可。
	key = "abc";
	sum = 97 + 98 + 99 = 204;
	用余数法等方法对sum进行hash 地址计算。
### 处理冲突的方法
	1,开放地址法。当冲突发生时，使用某种探查技术再散列表中形成一个探查序列。沿此序列逐个查找，直到找到
	给定的关键字，或者碰到一个开放的地址(既该地址单元为空)为止（若要插入，再探查到开放的地址，则可将待
	插入的新节点存入该地址单元）
	Hi = (H(key) + di)%m ; di = 1,2,3,4.. k(k<m-1)
	其中H（key） 为哈希函数，m为哈希表表长，di为增量序列。
		1) 线性探测再散列
		   di = 1,2,3,4,5,...m-1(k <= m-1)
		2) 二次探测再散列
		   di = 1*1，-1*1，2*2，-2*2，3*3，。。。+-(k*2) (k<=m/2)
	3,再哈希法。
		不考虑
	4,链地址法.
	   1) 将所有关键字同义词结点链接再同一个单链表中。若选定的散列表长度为m,
	   则可将散列表定义一个由m个头指针组成的指针数组T[0...m-1];
	   2) 凡是散列地址为i的节点。均插入到T[i]为头指针的单链表中。T中各分量的
	   初值均为空指针。
