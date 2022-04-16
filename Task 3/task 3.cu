#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>


__device__ int pw_check(char *attempt) {
	char pw1[] = "YD58";
	char pw2[] = "CD25";
	char pw3[] = "DF14";
	char pw4[] = "GF45";


	char *e = attempt;
	char *a = attempt;
	char *g = attempt;
	char *l = attempt;
	char *p1 = pw1;
	char *p2 = pw2;
	char *p3 = pw3;
	char *p4 = pw4;

	while(*e == *p1) { 
		if(*e == '\0') 
		{
			printf("Password: %s\n",pw1);
			break;
		}

		e++;
		p1++;
	}
	
	while(*a == *p2) { 
		if(*a == '\0') 
		{
			printf("Password: %s\n",pw2);
			break;
		}

		a++;
		p2++;
	}

	while(*g == *p3) { 
		if(*g == '\0') 
		{
			printf("Password: %s\n",pw3);
			break;
		}

		g++;
		p3++;
	}

	while(*l == *p4) { 
		if(*l == '\0') 
		{
			printf("Password: %s\n",pw4);
			return 1;
		}

		l++;
		p4++;
	}
	return 0;

}

__global__ void  kernel() {
	char k1,k2;

	char pw[7];
	pw[6] = '\0';

	int i = blockIdx.x+65;
	int j = threadIdx.x+65;
	char firstMatch = i; 
	char secondMatch = j; 

	pw[0] = firstMatch;
	pw[1] = secondMatch;
	for(k1='0'; k1<='9'; k1++){
		for(k2='0'; k2<='9'; k2++){
			
				
					pw[2] = k1;
					pw[3] = k2;
					 
					if(pw_check(pw)) {
					} 
					else {
	     			//printf("tried: %s\n", pw);		  
					}
				
			
		}
	}
}

int time_diff(struct timespec *start, 
	struct timespec *finish, 
	long long int *diff) {
	long long int dsec =  finish->tv_sec - start->tv_sec; 
	long long int dnanosec =  finish->tv_nsec - start->tv_nsec; 
	if(dnanosec < 0 ) {
		dsec--;
		dnanosec += 1000000000; 
	} 
	*diff = dsec * 1000000000 + dnanosec;
	return !(*diff > 0);
}


int main() {

	struct  timespec start, finish;
	long long int time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	kernel <<<26,26>>>();
	cudaThreadSynchronize();

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_diff(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldnanosec or %0.9lfsec\n", time_elapsed, (time_elapsed/1.0e9)); 

	return 0;
}


