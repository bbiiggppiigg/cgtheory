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
	Board * gen_board(int index, int dir){
		int swap_index = -1;
		int empty_index = non_empty[index];
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
		if(swap_index==-1 || board[swap_index]!=-1)
			return NULL;
		
		Board * b = new Board();
		memcpy(b->board,this->board,TOTAL*sizeof(int));
		memcpy(b->non_empty,this->non_empty,6*sizeof(int));
		b->step = step+1;
		b->board[swap_index] = board[empty_index];
		b->board[empty_index]=-1;
		b->non_empty[index] = swap_index;
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
	for(int i : {0,1,2,3,4,5,6,7}){
		for (int j = 0; j < 25 ;j++){
			for (int k =0; k< 25 ;k++){
				memset(lookup_table[i][j][k],-1,TOTAL*TOTAL*TOTAL*TOTAL*sizeof(int));
			}
		}
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
	for(int i =0; i< 6 ;i++){
			printf("%d\t",seq[i]);
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
void test(){
	
	int seq [6] = {0,0,0,0,0,0};
	Board * start ;
	Board * current;
	int record=0;
	initialize();
	while(gen_next_recur(seq)){
		record =0 ;
		for(int piindex = 0 ; piindex < 8 ;piindex++){
			start = new Board();
			start->init_with_sequence(seq,piindex);
			open_list.push(start);
			//printf("evaluate = %d\n",start->evaluate());
			
			while(!open_list.empty()){
				current = open_list.top();
				open_list.pop();
				//printf("%lu\n",open_list.size());
				//current->print();
				if(current->is_goal()){
					puts("Goal Found");
					printf("Goal Found , step =%d\n",current->step);
					//current->print();
					break;
				}
				for(int in_pattern =0 ;in_pattern < 6 ;in_pattern++){
					for (int direc =0; direc < 4 ;direc++){
						Board * tmp = current->gen_board(in_pattern,direc);
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
			/*for(int i =0;i <6 ;i++){
				printf("%d\t",seq[i]);
			}
			printf("%d\n",current->step);*/
			lookup_table[piindex][seq[0]][seq[1]][seq[2]][seq[3]][seq[4]][seq[5]] = current->step;
			while(!open_list.empty()){
				current = open_list.top(); 
				open_list.pop();
				delete current;
			}
		}
	}
	output_db("../db/0.out",0);
	output_db("../db/1.out",1);
	output_db("../db/2.out",2);
	output_db("../db/3.out",3);
	output_db("../db/4.out",4);
	output_db("../db/5.out",5);
	output_db("../db/6.out",6);
	output_db("../db/7.out",7);

}


int main(){
	
	test();

	return 0;
}
