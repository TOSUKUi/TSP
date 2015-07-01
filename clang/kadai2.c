#include<stdio.h>
#include<stdlib.h>

typedef struct City{
  int numCity;
  int **distanceMatrix;
}City;

typedef struct Arc{
  int edge[2];
  int cost;
}Arc;

FILE *func_InputFile(char *);
City func_makeDistanceMatrix(FILE *);
int func_getMinimumNextPath(int,City,int*,int*);
int *func_nearestAddition(int *,int *,City);
void func_printArray(int *,int);
void func_printCost(int *,int,City);
void func_insert(int[],int,int,int);

int main(int argc,char *argv[]){
  FILE *fpin;
  char graph_Name[20];
  City city;
  int i;
  int *route;
  int *unusedNode;

  /*------------------
    ファイルインプット
   -------------------*/
  //printf("グラフのファイル名を入力してください.\nファイル名 = ");
  //scanf("%s",graph_Name);
  fpin = func_InputFile(argv[1]);
  printf("%s\n",argv[1]);
  /*-------------------
    都市距離行列の作成
    -------------------*/
  city = func_makeDistanceMatrix(fpin);
  /*-----------------------
    NearestAdditionによるpath生成
   ------------------------*/
  unusedNode = (int *)calloc(city.numCity,sizeof(int));
  
  route = (int *)calloc(city.numCity,sizeof(int)); 
  for(i=0;i<city.numCity;i++)
    unusedNode[i] = 1;
  route[0] = 0; //最初に0番目の都市を集合Tに代入
  unusedNode[0] = 0; //0はもう使っているのでunusedではない
  route = func_nearestAddition(route,unusedNode,city);

  /*-----------------
    コンソール出力
    -----------------*/
  printf("Nearest Additionによる経路生成結果\n");
  func_printArray(route,city.numCity);
  func_printCost(route,city.numCity,city);

  
  free(unusedNode);
  free(route);
  return 0;
}

FILE *func_InputFile(char *input){
  FILE *fp;
  if((fp = fopen(input,"r")) == NULL){
    printf("fuck off!!\n");
    exit(EXIT_FAILURE); //エラーなんで落ちます
  }
  return fp;
}

City func_makeDistanceMatrix(FILE *fp){
  City city;
  int i,j;
  int val;
  char c;
  i = 0;
  /*都市数の取得*/
  while(fscanf(fp,"%d%c",&val,&c)!=EOF){
    i++;
    if(c == '\n'){
      city.numCity = i;
      break;
    }    
  }
  /*都市行列*/
  city.distanceMatrix = (int **)malloc(sizeof(int *)*city.numCity);
  for(i=0;i<city.numCity;i++)
    city.distanceMatrix[i] = (int *)malloc(sizeof(int)*city.numCity);
  if(fseek(fp,0,SEEK_SET))
    exit;
  i=j=0;
  while(fscanf(fp,"%d%c",&val,&c)!=EOF){
    city.distanceMatrix[i][j]=val;
    i++;
    if(c == 10){
      j++;      
      i=0;
    }    
  }
  return city;
}

int *func_nearestAddition(int *route,int *unusedNode,City city){
  int i,j,k;
  int miniNodeThanCurrent;
  int temp;
  int sum_unusedNode=1;
  Arc minArc;
  int flag;
  int index;
  int count=0;
  //unusedNodeの全ての要素が0になったらループ終了
  while(sum_unusedNode){
    sum_unusedNode = 0;
    minArc.cost = 30;
    flag=0;
    if(count > 0){
      for(j=1;j<city.numCity;j++){
	//あるj番目のNodeがunusedNodeでなかったら(つまりすでにpathの中に入っていたら)そのNodeから見て最も近いunusedNodeを選択し、それをArcの大小比較をし、一番短かった場合保存する
	if(!unusedNode[j]){
	  if((miniNodeThanCurrent = func_getMinimumNextPath(j,city,route,unusedNode)) >= 0){
	    
	    //仮に、選ばれた辺が最もコストが低かった場合、そこを選ぶ
	    if(city.distanceMatrix[j][miniNodeThanCurrent] < minArc.cost){
	      printf("miniNodeThanCurrent = %d From %d\n",miniNodeThanCurrent,j);
	      minArc.edge[0] = j;
	      minArc.edge[1] = miniNodeThanCurrent;
	      minArc.cost = city.distanceMatrix[j][miniNodeThanCurrent];	    	   
	      
	    }
	  }
	}
      }
      index = func_getIndex(route,minArc.edge[0],city.numCity);
      printf("index = %d\n",index);
      func_insert(route,city.numCity,minArc.edge[1],index);
      func_printArray(route,city.numCity);
      printf("\n");
      unusedNode[minArc.edge[1]] = 0;      
    }else{    
      miniNodeThanCurrent = func_getMinimumNextPath(0,city,route,unusedNode);
      route[1] = miniNodeThanCurrent;
      unusedNode[miniNodeThanCurrent]=0;
      count++;
      func_printArray(route,city.numCity);
      printf("\n");                
    }
    //unusedNOdoの合計を算出し、全て0かどうかを判断する
    for(i=0;i<city.numCity;i++)
      sum_unusedNode += unusedNode[i];
  }
  printf("Nearest Addition end\n");
  return route;
}
int func_getMinimumNextPath(int current,City city,int *route,int *unusedNode){
  int min = 30;
  int minimumNextPath;
  int i,j;
  int flag=0;
  for(i=0;i<city.numCity;i++){
    if(unusedNode[i])
      if(city.distanceMatrix[route[current]][i] < min){
	min = city.distanceMatrix[route[current]][i];
	minimumNextPath = i;
	flag++;	
      }    
  }
  if(flag){    
    return minimumNextPath;
  }else{
    return -1;
  }
}

void func_printArray(int *array,int length){
  int i;  
  for(i=0;i<length;i++)
    printf("%d -> ",array[i]);
}

void func_printCost(int *array,int length,City city){
  int i;
  int sum = 0;
  for(i=0;i+1<length;i++)
    sum += city.distanceMatrix[array[i]][array[i+1]]; 
  sum+=city.distanceMatrix[array[length-1]][array[0]];
  printf("Cost = %d\n",sum);
}

void func_insert(int array[],int length,int val,int index){
  int i;
  int *temp;
  temp = (int *)malloc(sizeof(int)*length);  
  for(i=index;i+1<length;i++)
    temp[i+1] = array[i];
  for(i=index+1;i<length;i++)
    array[i] = temp[i];  
  array[index] = val;
  free(temp);
}

int func_getIndex(int array[],int val,int length){
  int i=0;
  for(i = 0;i < length;i++)
    if(array[i] == val)
      return i;
  return -1;
  
}
