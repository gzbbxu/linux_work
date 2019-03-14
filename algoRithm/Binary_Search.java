package com.zk;

public class Binary_Search {
	public static void main(String[] args) {
		int array[] = new int[100000];
		for(int i=0;i<array.length;i++){
			array[i] = i+1;
		}
		
		binary_search(array,array.length,1332);
	}
	public static int binary_search(int [] array,int length,int key){
		int low =0;
		int hight = length-1;
		int times = 0;
		while(low<=hight){
			times++;
			int middle = (low+hight)/2;
			if(array[middle]>key){
				hight = middle-1;
			}else if(array[middle]<key){
				low = middle+1;
			}else if(array[middle]==key){
				System.out.println("找了 "+times+"次找到了");
				return key;
			}
		}
		System.out.println("找了 "+times+"次没找到了");
		return 0;
	}
}
