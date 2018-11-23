#define NR_DATA 1000
#include "trap.h"

int main(){
	int i,j,k,ans=0;
	for(i=0;i<NR_DATA;i++){
		for(j=0;j<NR_DATA;j++){
			k=i+j;
			if(k>=1000){
				ans+=k;
			}
		}
	}
	nemu_assert(ans==665667000);
}
