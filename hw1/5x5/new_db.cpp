#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SIZE  5
#define TOTAL SIZE*SIZE
#define PATTERN_SIZE 6
#define PATTERN_NUM 8

using namespace std;

int patterns [][6] = {{0,1,5,6,10,11},{2,3,4,7,8,9},{12,13,14,18,19,23},{15,16,17,20,21,22},{0,1,2,5,6,7},{3,4,8,9,13,14},{12,17,18,19,22,23},{10,11,15,16,20,21}};
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class MyQueue : public std::priority_queue<T, Container, Compare>
{
public:
    typedef typename
        std::priority_queue<
        T,
        Container,
        Compare>::container_type::const_iterator const_iterator;

    const_iterator find(const T&val) const
    {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first!=last) {
            if (*first==val) return first;
            ++first;
        }
        return last;
    }
    bool exist(const T&val) const{
    	auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first!=last) {
            if (**first==*val) return true;
            ++first;
        }
        return false;
    }
};
class Board{
public:
	int board[TOTAL];
	int step;
	/**
		The position of the i-th non_empty tile on the board is non_empty[i]
	**/
	int non_empty[PATTERN_SIZE] ;
	Board(){
		step = 0;
		memset(board,-1,TOTAL*sizeof(int));
	}

	void init_with_sequence(int * seq,int pi){

		for(int i =0 ; i< PATTERN_SIZE ;i++){
			board[patterns[pi][i]] = seq[i];
			non_empty[i] = patterns[pi][i];
		}

	}
	
	void print(){
		for( int i =0 ;i  < TOTAL ; i++ ){
			printf("%02d\t",board[i]);
			if((i+1) % SIZE==0)
				putchar('\n');
		}
		printf("step = %d evaluate = %d\n",step,evaluate());
		for(int i=0;i< PATTERN_SIZE; i++){
			printf("%d\t",non_empty[i]);
		}
		putchar('\n');
		for(int i=0;i< PATTERN_SIZE; i++){
			printf("%d\t",board[non_empty[i]]);
		}
		putchar('\n');
		putchar('\n');
	}
	/***
		index
		empty_index
		swap_index
	***/
	Board * gen_board(int tile_id, int dir){
		int target_index = -1;
		int source_index = non_empty[tile_id];
		switch(dir){
			case UP:
				if(source_index >= SIZE)
					target_index = source_index-SIZE;
			break;
			case DOWN:
				if(source_index / SIZE < SIZE-1)
					target_index = source_index+SIZE;
			break;
			case LEFT:
				if(source_index % SIZE)
					target_index = source_index-1;
			break;
			case RIGHT:
				if((source_index+1) % SIZE )
				target_index = source_index+1;
			break;
		}
		if(target_index==-1)
			return NULL;
		
		Board * b = new Board();
		memcpy(b->board,this->board,TOTAL*sizeof(int));
		memcpy(b->non_empty,this->non_empty,PATTERN_SIZE*sizeof(int));
		b->step = step;
		if(b->board[source_index] != TOTAL-1 && b->board[target_index] != TOTAL-1)
			b->step++;

		int swap_index_tile_id;
		if(b->board[target_index]!=-1){
			
			/*for(int x =0 ; x < 6;x++){
				if(non_empty[x]==target_index)
					swap_index_tile_id =x;
			}
			b->board[target_index] = board[source_index];
			b->board[source_index]=board[target_index];
			b->non_empty[tile_id] = target_index;
			b->non_empty[swap_index_tile_id] = source_index;*/
			
				delete b;
				return NULL;
			
		}else{
			b->board[target_index] = board[source_index];
			b->non_empty[tile_id] = target_index;
			b->board[source_index] =-1;
		}
		
		

		return b; 
	}
	int abs(int x) const{
		if(x >0 )
			return x;
		return -x;
	}
	int evaluate() const{
		int sum =0 ,x1,x2,y1,y2;
		for (int i =0; i< PATTERN_SIZE ;i++){
			x1 = non_empty[i]/SIZE;
			y1 = non_empty[i]%SIZE;
			x2 = board[non_empty[i]]/SIZE;
			y2= board[non_empty[i]]%SIZE;
			sum+= abs(x1-x2)+abs(y1-y2);	
		}
		return sum+step;
	}
	bool is_goal(){
		for(int i =0; i< PATTERN_SIZE; i++){
			if(non_empty[i] != board[non_empty[i]])
				return false;
		}
		return true;
	}
	
	bool operator<(const Board &other) const{
		return this->evaluate() < other.evaluate();
	}
	bool operator>(const Board &other) const{
		return this->evaluate() > other.evaluate();
	}
	bool operator==(const Board &other) const{
		return memcmp(this->board,other.board,TOTAL*sizeof(int)) ==0 ;
	}
};




void initialize(){
	puts("Initialization");
	char filename[100];
	FILE * fp ;
	for(int i=0;i<PATTERN_NUM;i++){
		sprintf(filename,"../db/%d.out",i);
		fp =  fopen(filename,"w");
		fclose(fp);
	}
	puts("After Initialization");	
}

struct compareBoard{
	bool operator()(const Board * lhs, const Board * rhs) const {
		return lhs->evaluate() > rhs->evaluate();
	}
};
//MyQueue<Board * > open_list; 
MyQueue<Board*, vector<Board*>,compareBoard > open_list;
MyQueue<Board*, vector<Board*>,compareBoard > close_list;
bool re(int * seq){
	for(int i=0;i<PATTERN_SIZE;i++){
		for(int j=i+1;j<PATTERN_SIZE;j++){
			if(seq[i]==seq[j])
				return true;
		}
	}
	return false;
}

void print_seq(int * seq){
	printf("Dealing with sequence ");
	for(int i =0; i< PATTERN_SIZE ;i++){
			printf("%2d ",seq[i]);
	}
	putchar('\n');
}
bool gen_next(int * seq,int x ){
	if(x==-1)
		return false;
	if(seq[x]<TOTAL)
		seq[x]++;
	
	if(seq[x]==TOTAL){
		seq[x]=0;
		return gen_next(seq,x-1);
	}
	return true;
}

bool gen_next_recur(int *seq){
	puts("Generating Sequence");
	bool have_next;
	do{
		have_next = gen_next(seq,PATTERN_SIZE-1);
	}while(re(seq) && have_next);
	puts("End Generating Sequence");
	return 	have_next;
}
void write_db(int *seq, int pi,int step){
	char filename[100];
	sprintf(filename,"../db/%d.out",pi);
	FILE * fp = fopen(filename,"a");
	fprintf(fp,"%d\n",step);
	fclose(fp);
}

void test(){
	
	int seq [6] = {0,0,0,0,0,0};
	Board * start ;
	Board * current;
	int record=0;
	initialize();
	int step_size =0 ;
	while(gen_next_recur(seq)){
		record ++ ;
		//if(record < 20)
		//	continue;
		print_seq(seq);
		for(int piindex = 0 ; piindex < PATTERN_NUM ;piindex++){
			start = new Board();
			start->init_with_sequence(seq,piindex);
			open_list.push(start);
			step_size =0;	
			
			while(!open_list.empty()){
				current = open_list.top();
				open_list.pop();
				close_list.push(current);
				//current->print();
				if(current->is_goal()){
					printf("Goal Found for pattern %d , step =%d\n",piindex,current->step);
					//current->print();
					break;
				}
				if(step_size<current->step){
					step_size = current->step;
					printf("%d\n",step_size);
				}
				for(int tile_id =0 ;tile_id < PATTERN_SIZE ;tile_id++){
					for (int direc =0; direc < 4 ;direc++){
						Board * tmp = current->gen_board(tile_id,direc);
						if(tmp){
							if(!close_list.exist(tmp)&&!open_list.exist(tmp)){	
								open_list.push(tmp);
							}else
								delete tmp;
						}
					}
				}
			}

			write_db(seq,piindex,current->step);
			
			while(!open_list.empty()){
				current = open_list.top(); 
				open_list.pop();
				delete current;
			}
			
			while(!close_list.empty()){
				current = close_list.top();
				close_list.pop();
				delete current;
			}
			//break;
		}
		//break;
	}
}


int main(){
	
	test();

	return 0;
}
