#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <queue>
#include <stack>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SIZE  5
#define TOTAL SIZE*SIZE
#define MAX_TESTCASE 100

using namespace std;

int lookup_table [4][TOTAL][TOTAL][TOTAL][TOTAL][TOTAL][TOTAL];
int patterns [][6] = {{0,1,5,6,10,11},{2,3,4,7,8,9},{12,13,14,18,19,23},{15,16,17,20,21,22}};
int testcases[MAX_TESTCASE][TOTAL];


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
	int empty_index;
	int step;
	int last_move = -1;
	Board(){
		step = 0;
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
};


void input_db(char * filename ,int index){
puts("Reading File ");
	puts(filename);
	FILE * fp = fopen(filename,"r");
	char str[200];
	int a,b,c,d,e,f,step;
	while(fgets(str,200,fp)){
		//puts(str);
		sscanf(str,"%d%d%d%d%d%d%d",&a,&b,&c,&d,&e,&f,&step);
		lookup_table[index][a][b][c][d][e][f]=step;
		//printf("%d %d %d %d %d %d %d\n",a,b,c,d,e,f,step);
	}
	fclose(fp);
	puts("Done");
}
void intialization(){
	puts("Initialization");
	for(int i : {0,1,2,3}){
		for (int j = 0; j < 25 ;j++){
			for (int k =0; k< 25 ;k++){
				memset(lookup_table[i][j][k],0,TOTAL*TOTAL*TOTAL*TOTAL*sizeof(int));
			}
		}
	}
	input_db("0.out",0);
	input_db("1.out",1);
	input_db("2.out",2);
	input_db("3.out",3);
	puts("End of Initialization");
}
void load_testcase(){
	FILE * fp = fopen("testcase/test.in","r");
	for(int j =0; j < 1 ;j ++)
		for (int i =0; i < TOTAL ; i++){
			fscanf(fp,"%d",&testcases[j][i]);
		//	printf("%d\t",testcases[j][i]);
		}
		//putchar('\n');
	fclose(fp);
}



MyQueue<Board*, vector<Board*>,less<vector<Board*>::value_type> > * pq;
int depth_limited(Board * start,int threshold){
	Board * b , * tmp ;
	pq->push(start);
	int cutoff = 1000000;
	
	while(!pq->empty()){
		b = pq->top();
		if(pq->size() % 10000 ==0 ){
			printf("%d\n",pq->size());
		}	
		//printf("step = %d queue size = %d \n",b->step,pq->size());
		if(b->is_goal()){
			puts("Goal Found");
			return cutoff;	
		}
		if(b->last_move == 3){
			pq->pop();
			continue;
		}
		b->last_move += 1;
		tmp = b->gen_board(b->last_move);
		if(tmp){
			if(tmp->evaluate() > threshold){
				if(tmp->evaluate() < cutoff){
					cutoff = tmp->evaluate();
					//printf("update cutoff %d\n",cutoff);
				}
				continue;
			}
			if(pq->exist(tmp)){

				continue;
			}
			pq->push(tmp);
		}
	}
	
	return cutoff;
}
void id_a_star(int t_index){
	Board * start = new Board();
	start->init_with_sequence(testcases[t_index]);
	start->print();
	Board * tmp;
	bool goal_reached = false;
	int threshold =0 ;
	while (threshold < 3000 && (!goal_reached)){
		printf("Threshold = %d\n",threshold);
		pq = new MyQueue<Board*, vector<Board*>,less<vector<Board*>::value_type> >();
		threshold = depth_limited(start->clone(),threshold);
		if(!pq->empty()){
			tmp = pq->top();			
			if(tmp->is_goal()){
				tmp->print();
				goal_reached = true;
			}

		}
		while(!pq->empty()){
			tmp = pq->top();
			pq->pop();
			delete tmp;
		}

		delete pq;
	}
	printf("threshold = %d\n",threshold);
}

void test(){
	
	intialization();
	load_testcase();
	id_a_star(0);

}


int main(){
	
	test();

	return 0;
}
