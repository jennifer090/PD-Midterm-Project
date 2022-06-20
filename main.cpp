#include <iostream>
#include <cmath>
using namespace std;

const int SIZE = 4;
struct Center
{
	int x;
	int y;
	int cap;//capacity
	int fee; //contstruntion fee
	int benefit; //紀錄蓋這間物流中心的最大獲利
	int id; //物流中心真實編號 
	
};

struct Store
{
	int x;
	int y;
	int demand; //剩餘的需求量 
	int fee; //建造成本 
	int price; //單位價格 
	int suit; //最適配的物流中心
	int dis; //和物流中心的距離 
	int benefitPer; 
	int copyFee;
	int benefit; //建造這個零售店的純獲利
	int id; //店名
	bool fill; //是否被補滿 
	
	
};


//計算距離的函式 
int distance(Center A,Store B)
{
	return abs(A.x-B.x)+abs(A.y-B.y);
}

int fullPackBenefit(Store B, int distance, int cost)
{
	int total = (B.price-cost*distance)*B.demand-B.fee;
	return total;
}

void sList(Store&, Center*,int,int,int);
bool stop (Store*, int);
void print(int** ans, int*, int*,int,int,int,int );
void storeAndItsCenter(Store* store,Center* center,int storeNum,int centerNum,int cost,Store sortedList[]);
void insertionSort(Store sortedList[],int storeNum);

void multiCenDistribution(int len, int len2, Center* restCenter, Store* restStore,int cost,Center* center, Store* store);
void storeAndItsCenter2(Store* store, Center* center, int storeNum,int centerNum,int cost, Store sortedList2[]);
double partialMaxBenefit(Store store,int far,int cost,int demand);
int min(Center center,Store store);
void insertionSort2(Store sortedList[], int storeNum);
void release(Center* center, Store* store, bool check[] , int** ans,int centerNum,int storeNum);
int main()
{
	int info[SIZE]={0};
	for (int i = 0; i < SIZE; i++)
	    cin>>info[i];
	
	int centerNum =  info[0];	//物流中心的數量 
	int storeNum = info[1];  //零售店的數量
	int cost =  info[2]; //單位補貨成本 
	int multi = info[3]; //單中心或都中心 
	
	Center* center = new Center [centerNum];
	Store* store = new Store[storeNum];
	
	//最終各個店被補獲的狀況 
	int** ans = new int*[storeNum];
	for (int i = 0; i < storeNum; i++)
	{
		ans[i]= new int [centerNum];
		for ( int j = 0; j < centerNum; j++)
		    ans[i][j]=0;
	
	}
	   
	//輸入物流中心的x,y值及id 
	for (int i = 0; i <centerNum; i++)
	{
		cin >> center[i].x >> center[i].y;
		center[i].id = i+1;
	}
		
	//輸入零售店的x,y值
	for (int i = 0; i < storeNum; i++)
	    cin >> store[i].x>>store[i].y;
	//輸入零售店的需求量
	for (int i = 0; i < storeNum; i++)
	    cin>>store[i].demand;
	//最初的需求量(順便紀錄商店的編號) 
	int* demand=new int[storeNum];
	for (int i = 0; i < storeNum; i++)
	{
		demand[i]=store[i].demand;
		store[i].id = i+1;
		store[i].fill = false; //還未補滿 
	}
	    
	//輸入商店每日建造成本
	for (int i = 0; i < storeNum; i++)
	{
		cin>>store[i].fee;
		store[i].copyFee = store[i].fee; 
	}
	    
	//輸入商店單位價格
	for (int i = 0; i < storeNum; i++)
	    cin>>store[i].price;
    
    //物流中心每日建造成本 
    for (int i = 0; i < centerNum; i++)
        cin>>center[i].fee;
	//輸入物流中心的貨量
	for (int i = 0; i < centerNum; i++)
	    cin>>center[i].cap;
	
	//選過的的零售店
	bool* check = new bool[storeNum];
	for ( int i = 0; i < storeNum; i++)
	    check[i]=false;
	
	//出使化中心利潤 
	for (int i = 0; i < centerNum; i++)
	    center[i].benefit=0;
	//演算法 
	int totalCash = 0;
	
	Store* sortedList = new Store[storeNum]; //建立一個store的串列，存所有store的資訊 
	storeAndItsCenter(store, center,storeNum,centerNum,cost,sortedList); //將零售店對應的物流中心及最大利潤算好
	

	
	//使用insertion sort，把benefit由大到小排出來
	
	insertionSort(sortedList,storeNum);
	//for (int i = 0; i < storeNum; i++)
	//{
		//cout<< sortedList[i].id <<" "<<sortedList[i].suit<<" "<<sortedList[i].benefit<<'\n';
	//}
	//選要蓋的物流中心 
	for (int i = 0; i < storeNum; i++)
	{
		 
		if (sortedList[i].benefit <= 0) //如果蓋這間店就算補滿也不會獲利，就可以停了 
		    break;
		int corr =sortedList[i].suit-1;//對應零售店的編號
		
		if (center[corr].cap == 0) //如果零售地沒貨了就繼續看下一個 
		    continue;
		int index = sortedList[i].id-1;
		if (center[corr].cap >= store[index].demand) //如果物流中心可以補滿全部
		{   center[corr].cap-=store[index].demand; //物流中心扣掉捕獲的數量
		    center[corr].benefit+=sortedList[i].benefit;//計入蓋物流中心可以獲得的潛在價值 
			store[index].fill = true; //這個商店被補滿了
			check[index]=true; //這個商店被選了
			store[index].fee = 0;
			ans[index][corr] = store[index].demand;
			
		}
		else if (center[corr].cap < sortedList[i].demand) //如果物流中心不能補滿全部
		{
			int retailBenefit = (sortedList[i].price-cost*distance(center[corr],sortedList[i]))*center[corr].cap-sortedList[i].fee;
			if (retailBenefit > 0)
			{
				ans[index][corr] = center[corr].cap; //將答案輸入 
				store[index].demand-=center[corr].cap; //將零售店的需求扣掉
				center[corr].cap = 0; //需求量變為0 
				store[index].fee = 0; //建造費只要花一次
				center[corr].benefit+=retailBenefit;
				check[index]=true; // 這個商店已經被選了
				
				
			}
		
		}
		 
	
	
	}
	
	//如果是多中心的話，進入多中心函式再一次進行分配
	if (multi==2)
	{
		//蒐集那些需求還有剩的中心及尚未被補滿的商店 
		Center* restCenter = new Center[centerNum];
		Store* restStore = new Store[storeNum];
		int len = 0, len2 = 0;
		for (int i = 0; i < centerNum; i++)
		{
			if (center[i].cap > 0)
			{
				restCenter[len]=center[i];
				len+=1;
			}
			    
		}

		for (int i = 0; i < storeNum; i++)
		{
			if (store[i].fill==false)
			{
				restStore[len2]=store[i];
				len2++;
			}
		}
		
		Store* sortedList2= new Store [len2];
	    storeAndItsCenter2(restStore, restCenter,len2,len,cost,sortedList2);
	    insertionSort2(sortedList2,len2);
	    
	
	    for (int i = 0; i < len2 ; i++)
	    {
		    int corr = sortedList2[i].suit-1;
		    if (center[corr].cap == 0)
		        continue;
		    int index = sortedList2[i].id-1;
		    
		    if (center[corr].cap >= store[index].demand) //如果物流中心可以補滿全部
		    {   
		        
			    int profit = (store[index].price-cost*sortedList2[i].dis)*store[index].demand-store[index].fee;
		        
		        if (profit > 0)
		        {
		        	center[corr].cap-=store[index].demand; //物流中心扣掉捕獲的數量
		            center[corr].benefit+=store[i].benefit;//計入蓋物流中心可以獲得的潛在價值 
			        store[index].fill = true; //這個商店被補滿了
			        check[index]=true; //這個商店被選了
			        store[index].fee = 0;
			        ans[index][corr] += store[index].demand;
				}
			    
		    }
		    else if (center[corr].cap < sortedList[i].demand) //如果物流中心不能補滿全部
		   {
			    int retailBenefit = (store[index].price-cost*store[index].dis)*center[corr].cap-store[index].fee;
			    if (retailBenefit > 0)
			    {
				    ans[index][corr] += center[corr].cap; //將答案輸入 
				    store[index].demand-=center[corr].cap; //將零售店的需求扣掉
				    center[corr].cap = 0; //需求量變為0 
				    store[index].fee = 0; //建造費只要花一次
				    center[corr].benefit+=retailBenefit;
				    check[index]=true; // 這個商店已經被選了
				}
	       } 
	    }
	    for (int i = 0; i < centerNum; i++)
	    {
	    	if (center[i].cap > 0)
	    	{
	    		for (int j = 0; j < storeNum; j++)
	    		{
	    			if (center[i].cap == 0)
	    			    break;
	    			if (store[j].fill==false and center[i].cap > 0)
	    			{
	    				int quantity = min(center[i],store[j]);
	    				int revenue = (store[j].price-cost*distance(center[i],store[j]))*quantity-store[j].fee;
	    				if (revenue > 0)
	    				{
	    					ans[j][i]+=quantity;
	    					store[j].demand-=quantity;
	    					center[i].cap-=quantity;
	    					center[i].benefit+=revenue;
	    					check[j]=true;
	    					if (store[j].demand==0)
	    					    store[j].fill=true;
	    					
						}
					}
	    			
				}
			}
		}
	}
	    
	
	
	//檢查物流中心是否值得蓋 

	int* centerName = new int[centerNum];
	int* storeName =  new int[storeNum];
	//選了n家物流中心
	int totalCenter = 0;
	for (int i = 0; i < centerNum; i++)
	{
		
		if (center[i].benefit > center[i].fee)
	    {
	    	centerName[totalCenter]=center[i].id;
	    	totalCenter+=1;
		}
		else//如果決定不蓋物流中心 
		{
			
			for (int j = 0; j < storeNum; j++)
			{
				if (store[j].suit == i+1 ) //所有選到這間物流中心的零售店都要release
				{
					check[j]=false; //零售店並未被選到 
					store[j].demand += ans[j][i]; //曾經給零售店的需求需求重新放回去
					store[j].fill = false; //商店沒有補滿
					store[j].fee = store[j].copyFee; //零售店未被建造 
				}
			}
			for (int j = 0; j < storeNum; j++)
			    ans[j][i]=0; //將需求量調回成0
		}
	}
	int totalStore = 0;
	//選了m家零售店 
	for (int i = 0; i < storeNum; i++)
	{
		if (check[i]==true)
		{
			
			storeName[totalStore]=i+1;
			totalStore+=1;
		}
	}
	print(ans,storeName,centerName ,centerNum,storeNum,totalStore,totalCenter); 
	
}
void print(int** ans, int* storeName, int* centerName , int centerNum, int storeNum,int totalStore,int totalCenter)
{
	cout<<totalCenter;
	for (int i = 0; i < totalCenter; i++)
	    cout<<" "<<centerName[i];
	cout<<'\n';
	
	cout<<totalStore;
	for (int i = 0; i < totalStore; i++)
	    cout<<" "<<storeName[i];
	cout<<'\n';
	
	for (int i = 0; i < storeNum; i++)
	{
		for (int j = 0; j < centerNum; j++)
	    {
	    	cout<<ans[i][j];
	    	if (j!=centerNum-1)
	    	    cout<<" ";
		}
		cout<<'\n';
	}
	    
	        
		
	
	    
	
	
}






//找到目前每個零售店對應到最適當的物流中心 
void storeAndItsCenter(Store* store, Center* center, int storeNum,int centerNum,int cost, Store sortedList[])
{
	for (int i = 0; i < storeNum; i++) //每個店都找最佳的物流中心 
	{
		int bestCenter = 0, bestBenefit = 0;
		for (int j = 0; j < centerNum; j++)
		{
			int far = distance(center[j],store[i]);
			int sum = fullPackBenefit(store[i], far, cost);
			if (sum > bestBenefit)
			{
				bestBenefit= sum;
				bestCenter = center[j].id;
			}
			else if (sum == bestBenefit)
			{
				if (center[bestCenter-1].fee< center[j].fee) //如果一樣大，挑物流中心容量大的
				    bestCenter=center[j].id; 
				    
			}
		}
		store[i].suit = bestCenter; //每個店補滿的話最佳的物流中心
		store[i].benefit = bestBenefit; //當這個店補滿且扣掉蓋店的成本的純獲利
		sortedList[i] = store[i]; 
	}
}



void insertionSort(Store sortedList[], int storeNum)
{
	for (int i = 1; i < storeNum; i++)
	{
		for (int j = i; j > 0; j--)
		{
			if (sortedList[j].benefit > sortedList[j-1].benefit)
			{
				
				Store temp = sortedList[j];
				sortedList[j]=sortedList[j-1];
				sortedList[j-1]=temp;
			}
			else
			   break;
		}
	}
}





void storeAndItsCenter2(Store* store, Center* center, int storeNum,int centerNum,int cost, Store sortedList2[])
{
	for (int i = 0; i < storeNum; i++) //每個店都找最佳的物流中心 
	{
		int bestCenter = 0, bestBenefit = -10000, distan = 0;
		for (int j = 0; j < centerNum; j++)
		{
			int far = distance(center[j],store[i]);
			int demand = min(center[j],store[i]);
			double sum = partialMaxBenefit(store[i], far, cost,demand);
			
			if (sum > bestBenefit)
			{
				bestBenefit= sum;
				bestCenter = center[j].id;
				distan = far;
			}
			else if (sum == bestBenefit)
			{
				if (center[bestCenter-1].cap < center[j].cap) //如果一樣大，挑物流中心容量大的
				{
					bestCenter=center[j].id;
					distan = far;
				} 
				    
			}
		}
		store[i].suit = bestCenter; //每個店補滿的話最佳的物流中心
		store[i].benefitPer = bestBenefit; //權重 
		store[i].dis = distan; //蓋了的獲利 
		sortedList2[i] = store[i]; //紀錄index 
	}
}

//記算單位獲利 
double partialMaxBenefit(Store store,int far,int cost,int demand)
{
	return static_cast<float>((store.price-cost*demand)-store.fee)/demand;
	
}

int min(Center center,Store store) //取物流中心和零售店剩餘數量的最小值 
{
	return (center.cap < store.demand )? center.cap:store.demand;
}

void insertionSort2(Store sortedList[], int storeNum)
{
	for (int i = 1; i < storeNum; i++)
	{
		for (int j = i; j > 0; j--)
		{
			if (sortedList[j].benefitPer > sortedList[j-1].benefitPer)
			{
				
				Store temp = sortedList[j];
				sortedList[j]=sortedList[j-1];
				sortedList[j-1]=temp;
			}
			else
			   break;
		}
	}
}



void release(Center* center, Store* store, bool check[] , int** ans,int centerNum, int storeNum)
{
	for (int i = 0; i < centerNum; i++)
	{
		
		if (center[i].benefit < center[i].fee)
		{
			for (int j = 0; j < storeNum; j++)
		   	{
				if (store[j].suit == i+1 ) //所有選到這間物流中心的零售店都要release
				{
					check[j]=false; //零售店並未被選到 
					store[j].demand += ans[j][i]; //曾經給零售店的需求需求重新放回去
					store[j].fill = false; //商店沒有補滿
					store[j].fee = store[j].copyFee; //零售店未被建造 
				}
			}
			for (int j = 0; j < storeNum; j++)
			    ans[j][i]=0; //將需求量調回成0
	   }
    }
}
