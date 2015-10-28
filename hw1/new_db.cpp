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

using namespace std;

int lookup_table [8][TOTAL][TOTAL][TOTAL][TOTAL][TOTAL][TOTAL];
int patterns [][6] = {{0,1,5,6,10,11},{2,3,4,7,8,9},{12,13,14,18,19,23},{15,16,17,20,21,22},{0,1,2,5,6,7},{3,4,8,9,13,14},{12,17,18,19,22,23},{10,11,15,16,20,21}};
char files[8][100]={"../db/0.out","../db/1.out","../db/2.out","../db/3.out","../db/4.out","../db/5.out","../db/6.out","../db/7.out"};
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
    	return find(val) != this->c.cend();
    }
};
class Board{
public:
	int board[TOTAL];
	int step;
	/**
		The position of the i-th non_empty tile on the board is non_empty[i]
	**/
	int non_empty[6] ;
	Board(){
		step = 0;
		memset(board,-1,TOTAL*sizeof(int));
	}

	void init_with_sequence(int * seq,int pi){

		for(int i =0 ; i< 6 ;i++){
			board[patterns[pi][i]] = seq[i];
			non_empty[i] = patterns[pi][i];
		}

	}
	
	void print(){
		for( int i =0 ;i  < TOTAL ; i++ ){
			printf("%02d\t",board[i]);
			if((i+1) %5==0)
				putchar('\n');
		}
		printf("step = %d evaluate = %d\n",step,evaluate());
		for(int i=0;i< 6; i++){
			printf("%d\t",non_empty[i]);
		}
		putchar('\n');
		for(int i=0;i< 6; i++){
			printf("%d\t",board[non_empty[i]]);
		}
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
		memcpy(b->non_empty,this->non_empty,6*sizeof(int));
		int swap_index_tile_id;
		if(b->board[target_index]!=-1){
			for(int x =0 ; x < 6;x++){
				if(non_empty[x]==target_index)
					swap_index_tile_id =x;
			}
			b->board[target_index] = board[source_index];
			b->board[source_index]=board[target_index];
			b->non_empty[tile_id] = target_index;
			b->non_empty[swap_index_tile_id] = source_index;
		}else{
			b->board[target_index] = board[source_index];
			b->non_empty[tile_id] = target_index;
			b->board[source_index] =-1;
		}
		if(b->board[source_index] != TOTAL-1)
			b->step = step+1;
		

		return b; 
	}
	int abs(int x) const{
		if(x >0 )
			return x;
		return -x;
	}
	int evaluate() const{
		int sum =0 ,x1,x2,y1,y2;
		for (int i =0; i< 6 ;i++){
			x1 = non_empty[i]/SIZE;
			y1 = non_empty[i]%SIZE;
			x2 = board[non_empty[i]]/SIZE;
			y2= board[non_empty[i]]%SIZE;
			sum+= abs(x1-x2)+abs(y1-y2);	
		}
		return sum+step;
	}
	bool is_goal(){
		for(int i =0; i< 6; i++){
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

void initialize(){
	puts("Initialization");
	/*for(int i : {0,1,2,3,4,5,6,7}){
		for (int j = 0; j < 25 ;j++){
			for (int k =0; k< 25 ;k++){
				memset(lookup_table[i][j][k],-1,TOTAL*TOTAL*TOTAL*TOTAL*sizeof(int));
			}
		}
	}*/
	for(int i=0;i<8;i++){
		fclose(fopen(files[i],"w"));
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

bool re(int * seq){
	for(int i=0;i<6;i++){
		for(int j=i+1;j<6;j++){
			if(seq[i]==seq[j])
				return true;
		}
	}
	return false;
}

void print_seq(int * seq){
	printf("Dealing with sequence ");
	for(int i =0; i< 6 ;i++){
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
	bool have_next;
	do{
		have_next = gen_next(seq,5);
	}while(re(seq) && have_next);
	return 	have_next;
}
void write_db(int *seq, int pi,int step){
	FILE * fp = fopen(files[pi],"a");
	//fprintf(fp,"%d %d %d %d %d %d %d\n",seq[0],seq[1],seq[2],seq[3],seq[4],seq[5],step);
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
		if(record < 31)
			continue;
		print_seq(seq);
		for(int piindex = 0 ; piindex < 8 ;piindex++){
			start = new Board();
			start->init_with_sequence(seq,piindex);
			open_list.push(start);
			step_size =0;	
			while(!open_list.empty()){
				current = open_list.top();
				open_list.pop();
				if(current->is_goal()){
					printf("Goal Found for pattern %d , step =%d\n",piindex,current->step);
					//current->print();
					break;
				}
				if(step_size<current->step){
					step_size = current->step;
					printf("%d\n",step_size);
				}
				for(int tile_id =0 ;tile_id < 6 ;tile_id++){
					for (int direc =0; direc < 4 ;direc++){
						Board * tmp = current->gen_board(tile_id,direc);
						if(tmp){
							if(!open_list.exist(tmp)){	
								open_list.push(tmp);
							}else
								delete tmp;
						}
					}
				}
				delete current;
			}
			write_db(seq,piindex,current->step);
			while(!open_list.empty()){
				current = open_list.top(); 
				open_list.pop();
				delete current;
			}
		}
	}
}


int main(){
	
	test();

	return 0;
}
