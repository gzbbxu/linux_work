package com.zk;

public class Test {

	public static void main(String[] args) {
		int a [] = new int[10];
		initData(a);	
		print(a);
		quickSort(a, 0, a.length-1);
		print(a);
	}
	public static void quickSort(int datas[],int start,int end){
		if(start>=end){
			//1个数据 不用排
		}else{
			//找出分界点 把数据分成两半 
			int middle = findMiddle(datas,start,end);
			//每一半 都是用同样的方式排序的
			//前一半
			quickSort(datas, start, middle-1);
			//后一半
			quickSort(datas,middle+1,end);
		}
	}
	public static void print(int datas[]){
		for(int i = 0; i <datas.length;i++){
			System.out.print(" "+ datas[i]);
		}
		System.out.println();
	}
	public static void initData(int datas[]){
		for(int i= 0 ;i<datas.length;i++){
			datas[i] = (int) ((Math.random()*100)+1);
		}
	}
	public static int findMiddle(int datas[],int start,int end){
		int temp = datas[end]; //以最后一个数据作为参照物
		int left = start; //左指针从start 开始
		int right = end-1;
		while(true){
  			
			//1 ,左指针从左边依次找数据，找到第一个比参照物大的数据。
			// left < end 首先要保证下标是正确的
			 while(left<end && datas[left] <=temp){
				 left++;
			 }
			 if(left==end){
				//特殊情况， 参照物的值 是最大值
				 //参照物是最大的值
				 break;
			 }
			 
			//2 ，右指针 从右边依次招数，找到第一个比参照物小的数据
			 while(right>=start && datas[right]>=temp){
				 right--;  
			 }
			 if(right == start-1){
				 //如果参照物是最小的值 这种情况和 64-68行的情况是一样的
			 }
			//3 ，比较左右指针 是否出现交叉  
			if(left<right){
				// 4 如果如果没有交叉 交换左右位置的数据
				int d = datas[left];
				datas[left] = datas[right];
				datas[right] = d;
			}else{
				//5 如果出现交叉， 交换左指针和参照物的值 ，停止查找
				int d = datas[left];
				datas[left] = datas[end];
				datas[end] = d;
				//跳出循环， 此时左指针就是分界点的位置 
				break;
			}
		
		}
		return left;
	}
}
