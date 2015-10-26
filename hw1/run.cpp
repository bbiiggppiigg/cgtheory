#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <time.h>
#include "Board.cpp"
#include "MyQueue.cpp"

#define MAX_TESTCASE 100
#define FILENAME "../log/runlog"

using namespace std;

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
	input_db("../db/0.out",0);
	input_db("../db/1.out",1);
	input_db("../db/2.out",2);
	input_db("../db/3.out",3);
	puts("End of Initialization");
}
void load_testcase(){
	FILE * fp = fopen("../testcase/test.in","r");
	for(int j =0; j < 1 ;j ++)
		for (int i =0; i < TOTAL ; i++){
			fscanf(fp,"%d",&testcases[j][i]);
		//	printf("%d\t",testcases[j][i]);
		}
		//putchar('\n');
	fclose(fp);
}



int depth_limited(Board * start,int threshold){
	Board * b , * tmp ;
	pq->push(start);
	int cutoff = 1000000;
	int i =0;
	while(!pq->empty()){
		i++;

		b = pq->top();
		if(pq->size() % 10000 ==0 ){
			printf("%lu\n",pq->size());
		}	
		//printf("step = %d queue size = %d \n",b->step,pq->size());
		if(b->is_goal()){
			//puts("Goal Found");
			printf("Number of nodes viewed with %d threshold %d\n",i,threshold);
			return cutoff;	
		}
		if(b->last_move == 3){
			pq->pop();
			continue;
		}
		b->last_move += 1;
		tmp = b->gen_board(b->last_move);
		tmp->prev = b;
		if(tmp){
			if(tmp->evaluate() > threshold){
				if(tmp->evaluate() < cutoff){
					cutoff = tmp->evaluate();
					//printf("update cutoff %d\n",cutoff);
				}
				continue;
			}
			if(pq->exist(tmp)){
				delete tmp;
				continue;
			}
			pq->push(tmp);
		}
	}
	printf("Number of nodes viewed with %d threshold %d\n",i,threshold);
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
	
	while (threshold < 3000 && (!goal_reached)){
		log_threshold(threshold);
		threshold = depth_limited(start->clone(),threshold);
		if(!pq->empty()){
			tmp = pq->top();			
			if(tmp->is_goal()){
				tmp->print_history();
				goal_reached = true;
				LOG("goal_reached!!");
			
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
