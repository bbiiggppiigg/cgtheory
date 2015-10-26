#include <stdio.h>
int lookup_table [4][25][25][25][25][25][25];
int main(){
	lookup_table[0][0][0][0][0][0][0]=1;
	printf("%d\n",lookup_table[0][0][0][0][0][0][0]);
	return 0;
}