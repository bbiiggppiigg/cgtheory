#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <time.h>

#define MAX_TESTCASE 100
#define FILENAME "../log/runlog"

using namespace std;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SIZE  5
#define TOTAL SIZE*SIZE

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
		printf("Step required is %d , estimate = %d \n",step,this->evaluate());
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
	
	int evaluate() const {
		int sum = 0 ,sum2 =0 ;
		bool goal_reached = true;
		for(int pi : {0,1,2,3}){
			sum += lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]];
			
		}
		for (int pi : {4,5,6,7} ){
			sum2+= lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]][board[patterns[pi][4]]][board[patterns[pi][5]]];
		}
		//printf ("sum = %d sum2 = %d\n",sum,sum2);
		if(sum==sum2 && sum ==0){
			for (int i =0; i < TOTAL ;i++){
				if(board[i]!=i)
					goal_reached = false;
			}
			if(!goal_reached)
				puts("Un recognized pattern!!!!!");
		}
		if(sum < sum2)
			return sum2+step;
		return sum + step;
	}
	bool operator<(const Board &other) const {
		return this->evaluate() < other.evaluate();
	}
	bool operator==(const Board &other) const{
		return memcmp(this->board,other.board,TOTAL*sizeof(int)) ==0 ;
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

int testcases[MAX_TESTCASE][TOTAL];
MyQueue<Board*, vector<Board*>,less<vector<Board*>::value_type> > * pq = new MyQueue<Board*, vector<Board*>,less<vector<Board*>::value_type> >();



void LOG(char * str){
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	FILE * fp = fopen(FILENAME,"a");
	fprintf(fp,"%s : %s\n",asctime (timeinfo),str);
	fclose(fp);
}

void log_threshold(int d){
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	FILE * fp = fopen(FILENAME,"a");
	fprintf(fp,"%s : Threshold = %d\n",asctime (timeinfo),d);
	fclose(fp);
}

void input_db(char * filename ,int index){
puts("Reading File ");
	puts(filename);
	FILE * fp = fopen(filename,"r");
	char str[200];
	int a,b,c,d,e,f,step;
	while(fgets(str,200,fp)){
		sscanf(str,"%d%d%d%d%d%d%d",&a,&b,&c,&d,&e,&f,&step);
		lookup_table[index][a][b][c][d][e][f]=step;
	}
	fclose(fp);
	puts("Done");
}

void intialization(){
	puts("Initialization");
	for(int i : {0,1,2,3,4,5,6,7}){
		for (int j = 0; j < 25 ;j++){
			for (int k =0; k< 25 ;k++){
				memset(lookup_table[i][j][k],0,TOTAL*TOTAL*TOTAL*TOTAL*sizeof(int));
			}
		}
	}
	input_db("../db/0.out",0);
	input_db("../db/1.out",1);
	input_db("../db/2.out",2);
	input_db("../db/3.out",3);
	input_db("../db/4.out",4);
	input_db("../db/5.out",5);
	input_db("../db/6.out",6);
	input_db("../db/7.out",7);
	puts("End of Initialization");
}
void load_testcase(){
	FILE * fp = fopen("../testcase/test.in","r");
	for(int j =0; j < 1 ;j ++)
		for (int i =0; i < TOTAL ; i++){
			fscanf(fp,"%d",&testcases[j][i]);
		}
	fclose(fp);
}



int depth_limited(Board * start,int threshold){
	Board * b , * tmp ;
	pq->push(start);
	int cutoff = 1000000;
	int i =0;
	char logstring[1000];
	int record = 0;
	printf("Depth Limited A* starting with %d\n",threshold);
	while(!pq->empty()){
		b = pq->top();

		if(b->step > record ){
			record = b->step;
			printf("step = %d , cost = %d , pq size = %lu\n",record,b->evaluate(),pq->size());
		}
		if(b->is_goal()){
			printf("Number of nodes viewed with %d threshold %d\n",i,threshold);
			sprintf(logstring,"Number of nodes viewed with %d threshold %d\n",i,threshold);
			LOG(logstring);
			return cutoff;	
		}
		if(b->last_move == 3){
			pq->pop();
			continue;
		}
		b->last_move += 1;
		tmp = b->gen_board(b->last_move);
		if(tmp){
			tmp->prev = b;
			if(tmp->evaluate() > threshold){
				if(tmp->evaluate() < cutoff){
					cutoff = tmp->evaluate();
				}
				delete tmp;
				continue;
			}
			if(pq->exist(tmp)){
				delete tmp;
				continue;
			}
			i++;
			pq->push(tmp);
		}
	}
	printf("Number of nodes viewed with %d threshold %d\n",i,threshold);
	sprintf(logstring,"Number of nodes viewed with %d threshold %d\n",i,threshold);
	LOG(logstring);
	return cutoff;
}

void id_a_star(int t_index){
	Board * start = new Board();
	start->init_with_sequence(testcases[t_index]);
	puts("Initial Position");
	start->print();
	puts("");
	Board * tmp;
	bool goal_reached = false;
	int threshold =0 ;
	
	while (threshold < 80 && (!goal_reached)){
		log_threshold(threshold);
		threshold = depth_limited(start->clone(),threshold);
		if(!pq->empty()){
			tmp = pq->top();			
			if(tmp->is_goal()){
				tmp->print_history();
				goal_reached = true;
				LOG("goal_reached!!");
				puts("Goal Reached!!");
			}
		}
		while(!pq->empty()){
			tmp = pq->top();
			pq->pop();
			delete tmp;
		}
	}
	printf("threshold = %d\n",threshold);
}


void test(){
	intialization();
	load_testcase();
	id_a_star(0);
}

int main(){
	FILE * fp = fopen(FILENAME,"w");
	fclose(fp);
	test();
	return 0;
}
