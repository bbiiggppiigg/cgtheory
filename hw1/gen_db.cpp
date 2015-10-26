#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <queue>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SIZE  5
#define TOTAL SIZE*SIZE

using namespace std;

int lookup_table [4][TOTAL][TOTAL][TOTAL][TOTAL][TOTAL][TOTAL];
int patterns [][6] = {{0,1,5,6,10,11},{2,3,4,7,8,9},{12,13,14,18,19,23},{15,16,17,20,21,22}};

class Board{
public:
	int board[SIZE*SIZE];
	int empty_index;
	int step;
	Board(){
		step = 0;
	}
	bool init_sequence(){
		for (int i = 0 ; i< TOTAL; i++){
			this->board[i] = i;
		}
		empty_index = TOTAL-1;
		return true;
	}
	
	void print(){
		for(int i =0; i< SIZE ;i ++){
			for( int j =0; j < SIZE ; j++){
				printf("%2d\t",board[i*SIZE+j]);
			}
			putchar('\n');
		}
		printf("Step required is %d\n",step);
	}

	void print_line(){
		for( int i =0 ;i  < TOTAL ; i++ ){
			printf("%02d\t",board[i]);
		}
		printf("%d\n",step);
	}
	Board * gen_board(int dir){
		int swap_index = -1;
		switch(dir){
			case UP:
				if(empty_index >= SIZE)
					swap_index = empty_index-SIZE;
			break;
			case DOWN:
				if(empty_index / SIZE < SIZE-1)
					swap_index = empty_index+SIZE;
			break;
			case LEFT:
				if(empty_index % SIZE)
					swap_index = empty_index-1;
			break;
			case RIGHT:
				if((empty_index+1) % SIZE )
				swap_index = empty_index+1;
			break;
		}
		if(swap_index==-1)
			return NULL;
		
		Board * b = new Board();
		memcpy(b->board,this->board,TOTAL*sizeof(int));
		b->board[empty_index] = b->board[swap_index];
		b->board[swap_index] = 24; 
		b->empty_index = swap_index;
		b->step = step+1;
		return b; 
	}
	
	bool update_table(){
		bool updated= false;
		for(int pi : {0,1,2,3}){
			if(-1==lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]]){
				lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]]
 = step;
				//printf("%d %d %d %d %d %d %d\n",board[patterns[pi][0]],board[patterns[pi][1]],board[patterns[pi][2]],board[patterns[pi][3]],board[patterns[pi][4]],board[patterns[pi][5]],step);
				updated = true;
			}else{
				//printf("not updated %d %d %d %d %d %d %d\n",board[patterns[pi][0]],board[patterns[pi][1]],board[patterns[pi][2]],board[patterns[pi][3]],board[patterns[pi][4]],board[patterns[pi][5]],step);
			}
		}
		return updated;
	}
};
queue<Board * > open_list; 

void output_db(char * filename,  int index){
	puts("Writing File ");
	puts(filename);
	FILE * fp = fopen(filename,"w");
	for (int a =0 ; a < TOTAL ; a++){
		for (int b =0 ; b < TOTAL ; b++){
			for (int c =0 ; c < TOTAL ; c++){
				for (int d =0 ; d < TOTAL ; d++){
					for (int e =0 ; e < TOTAL ; e++){
						for (int f =0 ; f < TOTAL ; f++){
							if(-1 != lookup_table[index][a][b][c][d][e][f])
								fprintf(fp,"%d %d %d %d %d %d %d\n",a,b,c,d,e,f,lookup_table[index][a][b][c][d][e][f]);
						}
					}
				}
			}
		}		
	}
	fclose(fp);
	puts("Done");

}
void test(){
	puts("Initialization");
	for(int i : {0,1,2,3}){
		for (int j = 0; j < 25 ;j++){
			for (int k =0; k< 25 ;k++){
				memset(lookup_table[i][j][k],-1,TOTAL*TOTAL*TOTAL*TOTAL*sizeof(int));
			}
		}
	}
	Board * start = new Board();
	start->init_sequence();
	open_list.push(start);
	puts("After Initialization");
	Board * tmp;
	Board * tt;
	while(!open_list.empty()){
		tmp = open_list.front();
		open_list.pop();
		if(open_list.size()%10000 ==0)
			printf("%lu\n",open_list.size());
		
		if(tmp->update_table()){
			for(int dir = 0 ; dir < 4 ; dir++){
				tt = tmp->gen_board(dir);
				if(tt){
					open_list.push(tt);
				}
			}
		}
		delete tmp;
	}
	
	output_db("../db/0.out",0);
	output_db("../db/1.out",1);
	output_db("../db/2.out",2);
	output_db("../db/3.out",3);
	
}


int main(){
	
	test();

	return 0;
}
