#include <stdio.h>
#include <string.h>

int main(){
	int a[100];
	memset(a,-1,100*sizeof(int));
	for(int i =0 ;i < 100 ;i ++){
		printf("%d\n",a[i]);
	}
	return 0;
}
