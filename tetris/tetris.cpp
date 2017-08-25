#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#define SPEED 1000
typedef struct{
	int x;
	int y;
}coordinate_t;
void display(int[][20],coordinate_t*,int=-1,int* = NULL);
void initialData();
void updataMA(coordinate_t*,int);
void rotationMA(coordinate_t*,int,int,int);
int postionMA(coordinate_t*,int,int[10][20] = NULL);
//data///////////////////////
int HoldData[7][4]={
	{41,43,45,47},
	{3,5,43,45},
	{4,42,44,46},
	{2,42,44,46},
	{6,42,44,46},
	{2,4,44,46},
	{4,6,42,44}
};
int InitialData[7][4][2]={
	{{3,0},{4,0},{5,0},{6,0}},
	{{4,-1},{5,-1},{4,0},{5,0}},
	{{4,-1},{3,0},{4,0},{5,0}},
	{{3,-1},{3,0},{4,0},{5,0}},
	{{5,-1},{3,0},{4,0},{5,0}},
	{{3,-1},{4,-1},{4,0},{5,0}},
	{{4,-1},{5,-1},{3,0},{4,0}}
};
int RotationData[2][7][4][4][2]={
	{
		{
			{{-2,-1},{-2,0},{-2,1},{-2,2}},		//0
			{{2,-1},{1,-1},{0,-1},{-1,-1}},
			{{2,1},{2,0},{2,-1},{2,-2}},
			{{-2,1},{-1,1},{0,1},{1,1}}
		},{
			{{-1,-1},{0,-1},{-1,0},{0,0}},		//1
			{{-1,-1},{0,-1},{-1,0},{0,0}},
			{{-1,-1},{0,-1},{-1,0},{0,0}},
			{{-1,-1},{0,-1},{-1,0},{0,0}}
		},{
			{{-2,0},{-1,1},{-1,0},{-1,-1}},		//2
			{{0,2},{1,1},{0,1},{-1,1}},
			{{2,0},{1,-1},{1,0},{1,1}},
			{{0,-2},{-1,-1},{0,-1},{1,-1}}
		},{
			{{-2,1},{-1,1},{-1,0},{-1,-1}},		//3
			{{1,2},{1,1},{0,1},{-1,1}},
			{{2,-1},{1,-1},{1,0},{1,1}},
			{{-1,-2},{-1,-1},{0,-1},{1,-1}}
		},{
			{{-2,-1},{-1,1},{-1,0},{-1,-1}},		//4
			{{-1,2},{1,1},{0,1},{-1,1}},
			{{2,1},{1,-1},{1,0},{1,1}},
			{{1,-2},{-1,-1},{0,-1},{1,-1}}
		},{
			{{-2,1},{-2,0},{-1,0},{-1,-1}},		//5
			{{1,2},{0,2},{0,1},{-1,1}},
			{{2,-1},{2,0},{1,0},{1,1}},
			{{-1,-2},{0,-2},{0,-1},{1,-1}}
		},{
			{{-1,0},{-1,-1},{0,1},{0,0}},		//6
			{{0,1},{-1,1},{1,0},{0,0}},
			{{1,0},{1,1},{0,-1},{0,0}},
			{{0,-1},{1,-1},{-1,0},{0,0}}
		}
	},{
		{
			{{-1,2},{-1,1},{-1,0},{-1,-1}},		//0
			{{-1,-2},{0,-2},{1,-2},{2,-2}},
			{{1,-2},{1,-1},{1,0},{1,1}},
			{{1,2},{0,2},{-1,2},{-2,2}}
		},{
			{{-1,-1},{0,-1},{-1,0},{0,0}},		//1
			{{-1,-1},{0,-1},{-1,0},{0,0}},
			{{-1,-1},{0,-1},{-1,0},{0,0}},
			{{-1,-1},{0,-1},{-1,0},{0,0}}
		},{
			{{0,0},{-1,-1},{-1,0},{-1,1}},		//2
			{{0,0},{-1,1},{0,1},{1,1}},
			{{0,0},{1,1},{1,0},{1,-1}},
			{{0,0},{1,-1},{0,-1},{-1,-1}}
		},{
			{{0,-1},{-1,-1},{-1,0},{-1,1}},		//3
			{{-1,0},{-1,1},{0,1},{1,1}},
			{{0,1},{1,1},{1,0},{1,-1}},
			{{1,0},{1,-1},{0,-1},{-1,-1}}
		},{
			{{0,1},{-1,-1},{-1,0},{-1,1}},		//4
			{{1,0},{-1,1},{0,1},{1,1}},
			{{0,-1},{1,1},{1,0},{1,-1}},
			{{-1,0},{1,-1},{0,-1},{-1,-1}}
		},{
			{{0,-1},{0,0},{-1,0},{-1,1}},		//5
			{{-1,0},{0,0},{0,1},{1,1}},
			{{0,1},{0,0},{1,0},{1,-1}},
			{{1,0},{0,0},{0,-1},{-1,-1}}
		},{
			{{1,0},{1,1},{0,-1},{0,0}},		//6
			{{0,-1},{1,-1},{-1,0},{0,0}},
			{{-1,0},{-1,-1},{0,1},{0,0}},
			{{0,1},{-1,1},{1,0},{0,0}}
		}
	}
};
/////////////////////////////
int score=0;
int level=1;
int main(){
	int CurrentArray[10][20]={0};
	coordinate_t MoveArray[4];
	int Hold=-1,NowRotation=0;
	int NextArray[14],NextPointer=0,NowBlock;
	for(int i=0;i<7;i++){
		NextArray[i]=rand()%7;
		for(int j=0;j<i;j++)
			if(NextArray[i]==NextArray[j]){
				NextArray[i]=rand()%7;
				j=-1;
			}
		NextArray[i+7]=NextArray[i];
	}
	NowBlock=NextArray[NextPointer];
	updataMA(MoveArray,NextArray[NextPointer]);
	NextPointer++;
	
	//等待按鈕事件 
	int pr,clo,temp;		//控制用的變數 
	while(1){
		display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
		clo=clock();
		while(clock()-clo<SPEED){
			if(kbhit()&&(pr=getch())){
				//方向鍵(上下左右) 
				if(pr==72){
					if(postionMA(MoveArray,1)&&
					   !CurrentArray[MoveArray[0].x][MoveArray[0].y-1]&&
					   !CurrentArray[MoveArray[1].x][MoveArray[1].y-1]&&
					   !CurrentArray[MoveArray[2].x][MoveArray[2].y-1]&&
					   !CurrentArray[MoveArray[3].x][MoveArray[3].y-1]){
						for(int i=0;i<4;i++)
							MoveArray[i].y--;
						display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
					}
				}
				else if(pr==80){
					if(postionMA(MoveArray,2)&&
					   !CurrentArray[MoveArray[0].x][MoveArray[0].y+1]&&
					   !CurrentArray[MoveArray[1].x][MoveArray[1].y+1]&&
					   !CurrentArray[MoveArray[2].x][MoveArray[2].y+1]&&
					   !CurrentArray[MoveArray[3].x][MoveArray[3].y+1]){
						for(int i=0;i<4;i++)
							MoveArray[i].y++;
						display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
					}
				}
				else if(pr==75){
					if(postionMA(MoveArray,4|64,CurrentArray)/*&&
					   (!CurrentArray[MoveArray[0].x-1][MoveArray[0].y]||MoveArray[0].y<0)&&
					   (!CurrentArray[MoveArray[1].x-1][MoveArray[1].y]||MoveArray[1].y<0)&&
					   (!CurrentArray[MoveArray[2].x-1][MoveArray[2].y]||MoveArray[2].y<0)&&
					   (!CurrentArray[MoveArray[3].x-1][MoveArray[3].y]||MoveArray[3].y<0)*/){
						for(int i=0;i<4;i++)
							MoveArray[i].x--;
						display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
					}
				}
				else if(pr==77){
					if(postionMA(MoveArray,8|128,CurrentArray)/*&&
					   (!CurrentArray[MoveArray[0].x+1][MoveArray[0].y]||MoveArray[0].y<0)&&
					   (!CurrentArray[MoveArray[1].x+1][MoveArray[1].y]||MoveArray[1].y<0)&&
					   (!CurrentArray[MoveArray[2].x+1][MoveArray[2].y]||MoveArray[2].y<0)&&
					   (!CurrentArray[MoveArray[3].x+1][MoveArray[3].y]||MoveArray[3].y<0)*/){
						for(int i=0;i<4;i++)
							MoveArray[i].x++;
						display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
					}
				}
				// 
				else if(pr==32){
					while(postionMA(MoveArray,2)&&
					!CurrentArray[MoveArray[0].x][MoveArray[0].y+1]&&
					!CurrentArray[MoveArray[1].x][MoveArray[1].y+1]&&
					!CurrentArray[MoveArray[2].x][MoveArray[2].y+1]&&
					!CurrentArray[MoveArray[3].x][MoveArray[3].y+1]){
						MoveArray[0].y++;
						MoveArray[1].y++;
						MoveArray[2].y++;
						MoveArray[3].y++;
					}
					clo-=SPEED;
				}else if(pr==99){
					temp=Hold;
					Hold=NowBlock;
					if(temp==-1){
						NowBlock=NextArray[NextPointer];
						NextArray[NextPointer]=rand()%7;
						for(int i=0;i<NextPointer;i++)
							if(NextArray[i]==NextArray[NextPointer]){
								NextArray[NextPointer]=rand()%7;
								i=-1;
							}
						NextArray[NextPointer+7]=NextArray[NextPointer];
						NextPointer=(NextPointer+1)%7;
					}
					else
						NowBlock=temp;
					NowRotation=0;
					updataMA(MoveArray,NowBlock);
					display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
				}else if(pr==122){
					rotationMA(MoveArray,0,NowBlock,NowRotation);
					NowRotation=(NowRotation+1)%4;
					display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
				}else if(pr==120){
					rotationMA(MoveArray,1,NowBlock,NowRotation);
					NowRotation=(NowRotation+3)%4;
					display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
				}
				else if(pr>=48&&pr<=55){
					Hold=pr-49;
					display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
				}
			}
			_sleep(50);
		}
		if(postionMA(MoveArray,2)&&
			!CurrentArray[MoveArray[0].x][MoveArray[0].y+1]&&
			!CurrentArray[MoveArray[1].x][MoveArray[1].y+1]&&
			!CurrentArray[MoveArray[2].x][MoveArray[2].y+1]&&
			!CurrentArray[MoveArray[3].x][MoveArray[3].y+1]){
				for(int i=0;i<4;i++)
					MoveArray[i].y++;
		}
		else{
			display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
			_sleep(200);
			CurrentArray[MoveArray[0].x][MoveArray[0].y]=1;
			CurrentArray[MoveArray[1].x][MoveArray[1].y]=1;
			CurrentArray[MoveArray[2].x][MoveArray[2].y]=1;
			CurrentArray[MoveArray[3].x][MoveArray[3].y]=1;
			for(int i=0;i<4;i++){
				for(int j=0;j<10;j++){
					if(CurrentArray[j][MoveArray[i].y]==0)
						break;
					if(j<9)
						continue;
					for(int k=MoveArray[i].y;k>0;k--)
						for(int l=0;l<10;l++)
							CurrentArray[l][k]=CurrentArray[l][k-1];
					for(int l=0;l<10;l++)
						CurrentArray[l][0]=0;
					i--;
					score+=10;
					level=1+score/100;
				}
			}
			NowBlock=NextArray[NextPointer];
			NowRotation=0;
			updataMA(MoveArray,NextArray[NextPointer]);
			NextArray[NextPointer]=rand()%7;
			for(int i=0;i<NextPointer;i++)
				if(NextArray[i]==NextArray[NextPointer]){
					NextArray[NextPointer]=rand()%7;
					i=-1;
				}
			NextArray[NextPointer+7]=NextArray[NextPointer];
			NextPointer=(NextPointer+1)%7;
		}
		//display(CurrentArray,MoveArray,Hold,&NextArray[NextPointer]);
	}
	return 0;
}

void display(int CurrentArray[][20],coordinate_t *MoveArray,int Hold,int *NextArray){
	int ti=0,x,y,z;
	char ot[1000];
	coordinate_t flash[4];
	system("cls");
	//顯示已存在方塊 
	for(y=0;y<20;y++){
		for(x=0;x<10;x++)	//為HOLD留10格空位 
			ot[ti++]=' ';
		for(x=0;x<10;x++){
			ot[ti++]=-95;
			if(CurrentArray[x][y])
				ot[ti++]=-67;
			else{
				/*
				do{
					z=rand()%193;
				}while(z==0||z==1||z==45||z==46||z==47||z==62||z==67||z==68||z==79||z==176||z>95&&z<130);
				ot[ti++]=-1*z;
				*/
				ot[ti++]=-68;
			}
		}
		for(x=0;x<9;x++)	//為NEXT留9格空位 
			ot[ti++]=' ';
		ot[ti++]='\n';
	}
	//計算投影方塊 
	for(x=0;x<4;x++){
		flash[x].x=MoveArray[x].x;
		flash[x].y=MoveArray[x].y;
	}
	while(postionMA(flash,2)&&
	!CurrentArray[flash[0].x][flash[0].y+1]&&
	!CurrentArray[flash[1].x][flash[1].y+1]&&
	!CurrentArray[flash[2].x][flash[2].y+1]&&
	!CurrentArray[flash[3].x][flash[3].y+1]){
		flash[0].y++;
		flash[1].y++;
		flash[2].y++;
		flash[3].y++;
	}
	//顯示投影方塊
	for(x=0;x<4;x++){
		/*
		do{
			y=rand()%193;
		}while(y==0||y==1||y==45||y==46||y==47||y==62||y==67||y==68||y==79||y==176||y>95&&y<130);
		ot[flash[x].y*40+flash[x].x*2+11]=-1*y;
		*/
		ot[flash[x].y*40+flash[x].x*2+11]=-70;
	}
	//顯示移動中方塊(每行40字元+HOLD10字元+'-95'字元+每個字2字元(*2)) 
	for(x=0;x<4;x++){
		y=MoveArray[x].y*40+MoveArray[x].x*2+11;
		if(y>=0)
			ot[y]=-67;
	}
	/*
	ot[MoveArray[0].y*40+MoveArray[0].x*2+11]=-67;
	ot[MoveArray[1].y*40+MoveArray[1].x*2+11]=-67;
	ot[MoveArray[2].y*40+MoveArray[2].x*2+11]=-67;
	ot[MoveArray[3].y*40+MoveArray[3].x*2+11]=-67;
	*/
	//顯示HOLD與NEXT 
	//HOLD
	for(x=0;x<4&&Hold!=-1;x++){
		ot[HoldData[Hold][x]+80]=-95;
		ot[HoldData[Hold][x]+81]=-67;
	}
	//NEXT
	for(x=0;x<5&&NextArray!=NULL;x++){
		for(y=0;y<4;y++){
			ot[HoldData[NextArray[x]][y]+110+x*120]=-95;
			ot[HoldData[NextArray[x]][y]+111+x*120]=-67;
		}
	}
	//文字 
	char tempstr[10];
	strncpy(&ot[1],"H O L D",7);
	strncpy(&ot[32],"N E X T",7);
	strncpy(&ot[671],"SCORE",5);
	sprintf(tempstr,"%d",score);
	strncpy(&ot[712],tempstr,strlen(tempstr));
	strncpy(&ot[751],"LEVEL",5);
	sprintf(tempstr,"%d",level);
	strncpy(&ot[792],tempstr,strlen(tempstr));
	//加上結束字元再顯示出來 
	ot[ti++]='\0';
	printf("%s",ot);
}

void initialData(){
	int i=100;
	_sleep(1000);
}

void updataMA(coordinate_t* MoveArray,int block){
	MoveArray[0].x=InitialData[block][0][0];
	MoveArray[0].y=InitialData[block][0][1];
	MoveArray[1].x=InitialData[block][1][0];
	MoveArray[1].y=InitialData[block][1][1];
	MoveArray[2].x=InitialData[block][2][0];
	MoveArray[2].y=InitialData[block][2][1];
	MoveArray[3].x=InitialData[block][3][0];
	MoveArray[3].y=InitialData[block][3][1];
}

void rotationMA(coordinate_t* MoveArray,int direction,int NowBlock,int NowRotation){
	MoveArray[0].x=MoveArray[3].x+RotationData[direction][NowBlock][NowRotation][0][0];
	MoveArray[0].y=MoveArray[3].y+RotationData[direction][NowBlock][NowRotation][0][1];
	MoveArray[1].x=MoveArray[3].x+RotationData[direction][NowBlock][NowRotation][1][0];
	MoveArray[1].y=MoveArray[3].y+RotationData[direction][NowBlock][NowRotation][1][1];
	MoveArray[2].x=MoveArray[3].x+RotationData[direction][NowBlock][NowRotation][2][0];
	MoveArray[2].y=MoveArray[3].y+RotationData[direction][NowBlock][NowRotation][2][1];
	MoveArray[3].x=MoveArray[3].x+RotationData[direction][NowBlock][NowRotation][3][0];
	MoveArray[3].y=MoveArray[3].y+RotationData[direction][NowBlock][NowRotation][3][1];
}

int postionMA(coordinate_t* MoveArray,int p,int CurrentArray[10][20]){
	return
		(!(p&1)||(MoveArray[0].y>0&&MoveArray[1].y>0&&MoveArray[2].y>0&&MoveArray[3].y>0))&&
		(!(p&2)||(MoveArray[0].y<19&&MoveArray[1].y<19&&MoveArray[2].y<19&&MoveArray[3].y<19))&&
		(!(p&4)||(MoveArray[0].x>0&&MoveArray[1].x>0&&MoveArray[2].x>0&&MoveArray[3].x>0))&&
		(!(p&8)||(MoveArray[0].x<9&&MoveArray[1].x<9&&MoveArray[2].x<9&&MoveArray[3].x<9))&&
		(!(p&240)||
		(!CurrentArray[MoveArray[0].x+p&64-p&128][MoveArray[0].y-p&16+p&32]||(p&192)&&MoveArray[0].y<0)&&
		(!CurrentArray[MoveArray[1].x+p&64-p&128][MoveArray[1].y-p&16+p&32]||(p&192)&&MoveArray[1].y<0)&&
		(!CurrentArray[MoveArray[2].x+p&64-p&128][MoveArray[2].y-p&16+p&32]||(p&192)&&MoveArray[2].y<0)&&
		(!CurrentArray[MoveArray[3].x+p&64-p&128][MoveArray[3].y-p&16+p&32]||(p&192)&&MoveArray[3].y<0));
}
//72 80 75 77
