#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef struct {
	int key; //关键字
	int hit;// 冲突次数
}DataType;
typedef struct 
{
	DataType * data;
	int tableSize; //哈希表长度
	int curSize;//关键字的个数
}HastTable;

void CreateHashTabe(HastTable* H,int m,int p,int hash[],int n)
{
	int i,sum,addr,di,k = 1;
	H ->data = new DataType[m * sizeof(DataType)];		
	if(!H->data)
	{
		printf("create error ");
		return ;
	}
	//初始化哈希表
	for(i =0;i<m;i++)
	{
		H->data[i].key = -1;
		H->data[i].hit = 0;
	}
	//求哈希函数地址并处理冲突
	for(i = 0;i < n; i++ )
	{
		//冲突的次数	
		sum = 0;
		addr = hash[i]%p;
		di = addr;
		//如果不冲突则将元素存储到表中。
		if(H->data[addr].key == -1){
			H->data[addr].key = hash[i];
			H->data[addr].hit = 0;
		}else {
			cout<<"冲突的位置 " << i << "冲突的key<< " << hash[i]<<"di " << di  <<endl; 
			//用线性探测再散列法处理冲突
			do{
				di = (di+k)%m;
				sum = sum+1;
			}while(H->data[di].key !=-1);
			H->data[di].key = hash[i];
			H->data[di].hit = sum;
			cout << "冲突找到位置 "<<di <<endl;
		}
	}
	H->tableSize = m;//哈希表的长度
	H->curSize = n;
}
void DisplayHash(HastTable* H , int m)
{
	int i;
	cout<<"哈希表地址:";
	for(i = 0 ;i < m;i++)
	{
		cout << " " << i;
	}
	cout << endl<<"关键字key:";
	for(i = 0 ;i < m ; i++)
	{
		cout << " " << H->data[i].key;
	}
	cout << endl <<"冲突次数";
	for(i = 0 ;i< m; i++)
	{
		cout << " " << H->data[i].hit;
	}
	cout << endl;

} 
int searchHash(HastTable * h,int key)
{
	int d , dl,m;
	m  = h->tableSize;
	d = dl = key % m; 		
	while(h->data[d].key!=-1)
	{
		if(h->data[d].key == key)	
		{
			//如果找到了，返回存储再哈希表中的位置
			return d;
		}else{
			//线性探测再散列	
			d = (d+1) % m;	
		}
		//没有找到关键字存储再哈希表中的位置
		if(d == dl)	
		{
			break;
		}
	}
	return -1;
}
int  main()
{
	int hash [] = {23,35,12,56,123,39,342,90};
	int n = sizeof(hash)/sizeof(int);
	int m = 11, p =11,pos;
	HastTable table;
	CreateHashTabe(&table,m,p,hash,n);
	DisplayHash(&table,m);
	int searchNum = 1233;
	pos = searchHash(&table,searchNum);
	if(pos ==-1)
	{
		cout << "没有找到"<<endl;
	}else{

		cout << "关键字"<<searchNum<<"在哈希表中的位置 "<<pos<<endl;
	}
	return 0;
}
