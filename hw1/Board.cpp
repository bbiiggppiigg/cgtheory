#include "Board.h"
class Board{
public:
	int board[TOTAL];
	int empty_index;
	int step;
	int last_move;
	Board * prev;
	Board(){
		step = 0;
		prev = NULL;
		last_move = -1;
	}
	void init_sequence(){
		for (int i = 0 ; i< TOTAL; i++){
			this->board[i] = i;
		}
		empty_index = TOTAL-1;
	}

	void init_with_sequence(int * addr){
		for (int i =0; i < TOTAL ;i++){
			this->board[i] = addr[i];
			if(addr[i]==24)
				empty_index = i;
		}
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
	Board * clone(){
		Board * b = new Board();
		memcpy(b->board,this->board,TOTAL*sizeof(int));
		b->empty_index = this->empty_index;
		b->step = this->step;
		b->last_move = this->last_move;
		return b;
	}
	bool update_table(){
		bool updated= false;
		for(int pi : {0,1,2,3}){
			if(!lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]]){
				lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]]
 = step;
				//printf("%d %d %d %d %d %d %d\n",board[patterns[pi][0]],board[patterns[pi][1]],board[patterns[pi][2]],board[patterns[pi][3]],board[patterns[pi][4]],board[patterns[pi][5]],step);
				updated = true;
			}else{
				//printf("no update %d %d %d %d %d %d %d\n",patterns[pi][0],patterns[pi][1],patterns[pi][2],patterns[pi][3],patterns[pi][4],patterns[pi][5],step);
			}
		}
		return updated;
	}
	int evaluate() const {
		int sum = 0;
		for(int pi : {0,1,2,3}){
			sum += lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]];
		}
		return sum+step;
	}
	bool operator<(const Board &other){
		return this->evaluate() < other.evaluate();
	}
	bool is_goal(){
		for (int i =0; i< TOTAL ;i++){
			if(board[i]!=i)
				return false;
		}
		return true;
	}
	void print_history(){
		if(this->prev != NULL){
			this->prev->print_history();
		}
		print();
	}
};