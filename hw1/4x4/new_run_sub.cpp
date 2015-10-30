#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <time.h>
#include <sys/time.h>

#define MAX_TESTCASE 100
#define FILENAME "../log/runlog"

using namespace std;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SIZE  4
#define TOTAL SIZE*SIZE
#define PATTERN_SIZE 4
#define PATTERN_NUM 8


int lookup_table [PATTERN_NUM][TOTAL][TOTAL][TOTAL][TOTAL];
int patterns [][PATTERN_SIZE] = {{0,1,4,5},{8,9,12,13},{2,3,6,7},{10,11,14,15},{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};
int testcases[MAX_TESTCASE][TOTAL];
int num_test_case;

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
            if (**first==*val) return first;
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
			if(addr[i]==TOTAL-1)
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
		b->board[swap_index] = TOTAL-1; 
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
		for(int pi =0 ; (pi < PATTERN_NUM/2) ;pi++){
			sum += lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]];
			
		}
		for (int pi = PATTERN_NUM/2; pi < PATTERN_NUM; pi++ ){
			sum2+= lookup_table[pi][board[patterns[pi][0]]][board[patterns[pi][1]]][board[patterns[pi][2]]][board[patterns[pi][3]]];
		}
		//printf ("sum = %d sum2 = %d\n",sum,sum2);
		if(sum==sum2 && sum ==0){
			for (int i =0; i < TOTAL ;i++){
				if(board[i]!=i)
					goal_reached = false;
			}
			//if(!goal_reached)
			//	puts("Un recognized pattern!!!!!");
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


struct compareBoard{
	bool operator()(const Board * lhs, const Board * rhs) const {
		return lhs->evaluate() > rhs->evaluate();
	}
};
MyQueue<Board*, vector<Board*>,compareBoard > * pq = new MyQueue<Board*, vector<Board*>,compareBoard >();
MyQueue<Board*, vector<Board*>,compareBoard > * close_list = new MyQueue<Board*, vector<Board*>,compareBoard >();



void LOG(char * str){
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	FILE * fp = fopen(FILENAME,"a");
	fprintf(fp,"%s\n",str);
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
	bool have_next;
	do{
		have_next = gen_next(seq,PATTERN_SIZE-1);
	}while(re(seq) && have_next);
	return 	have_next;
}

void input_db(){
	int seq [PATTERN_SIZE];

	memset(seq,0,sizeof(int)*PATTERN_SIZE);
	
	char input_file_name [PATTERN_NUM][100];
	
	FILE * fps [PATTERN_NUM]; 
	for(int i= 0; i< PATTERN_NUM; i++){
		sprintf(input_file_name[i],"../db/%d.out",i);
		fps[i] = fopen(input_file_name[i],"r");
	}
	while(gen_next_recur(seq)){
		//print_seq(seq);	
		for(int i =0; i< PATTERN_NUM;i++){
			fscanf(fps[i],"%d",&lookup_table[i][seq[0]][seq[1]][seq[2]][seq[3]]);
		}
		//break;	
	}
	for(int i=0;i <PATTERN_NUM;i++){
		fclose(fps[i]);
	}
}

void intialization(){
	puts("Initialization");
	for(int i =0; i< PATTERN_NUM ;i++){
		memset(lookup_table[i],0,TOTAL*TOTAL*TOTAL*TOTAL*sizeof(int));
	}
	input_db();
	puts("End of Initialization");
}
void load_testcase(){
	FILE * fp = fopen("../testcase/test.in","r");
	fscanf(fp,"%d",&num_test_case);
	for(int j =0; j < num_test_case ;j ++)
		for (int i =0; i < TOTAL ; i++){
			fscanf(fp,"%d",&testcases[j][i]);
		}
	fclose(fp);
	puts("End Loading Testcase");
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
			//getchar();
			//b->print();
		}
		if(b->is_goal()){
			printf("Number of nodes viewed with %d threshold %d\n",i,threshold);
			//sprintf(logstring,"Number of nodes viewed with %d threshold %d\n",i,threshold);
			//LOG(logstring);
			return cutoff;	
		}
		if(b->last_move == 3){
			pq->pop();
			close_list->push(b);
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
	//sprintf(logstring,"Number of nodes viewed with %d threshold %d\n",i,threshold);
	//LOG(logstring);
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
	struct timeval tval_before, tval_after, tval_result;
	gettimeofday(&tval_before, NULL);
	char logstring[1000];
	while (threshold <= 80 && (!goal_reached)){
		//log_threshold(threshold);
		threshold = depth_limited(start->clone(),threshold);
		if(!pq->empty()){
			tmp = pq->top();			
			if(tmp->is_goal()){
				LOG("goal_reached!!");
				puts("Goal Reached!!");
				gettimeofday(&tval_after, NULL);
				tmp->print_history();
				goal_reached = true;
				timersub(&tval_after, &tval_before, &tval_result);
				printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
				sprintf(logstring,"Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
				LOG(logstring);
			}
		}
		while(!pq->empty()){
			tmp = pq->top();
			pq->pop();
			delete tmp;
		}
		while(!close_list->empty()){
			tmp = close_list->top();
			close_list->pop();
			delete tmp;
		}
	}
	printf("threshold = %d\n",threshold);
}


void test(){
	intialization();
	load_testcase();
	for(int i=0;i<num_test_case;i++)
		id_a_star(i);
}

int main(){
	FILE * fp = fopen(FILENAME,"w");
	fclose(fp);
	test();
	return 0;
}
