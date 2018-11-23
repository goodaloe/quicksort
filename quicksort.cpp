/* ID 2015114727	- REPLACE WITH YOUR ID
 * NAME Kim Ho-Jeong	- REPLACE WITH YOUR NAME */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MEASURE_TIME

#ifdef MEASURE_TIME
#include<time.h>
#endif

#define CONTAINER_SIZE	512

struct CONTAINER {
	int key;
	float  fval[CONTAINER_SIZE];
	double dval[CONTAINER_SIZE];
};

void generatecontainer(struct CONTAINER *c, int key)
{
	int i;
	c->key = key;
	for (i = 0; i < CONTAINER_SIZE; i++) {
		c->fval[i] = (float)key;
		c->dval[i] = (double)key;
	}
}


int validate_container_item(const struct CONTAINER *c)
{
	int i;
	i = (rand() % CONTAINER_SIZE);
	if (c->key != (int)c->fval[i]) { return i; }
	else if (c->key != (int)c->dval[i]) { return i; }
	else return -1;
}

void copycontainer(const struct CONTAINER *src, struct CONTAINER *dst)
{
	int i;
	for (i = 0; i < CONTAINER_SIZE; i++) dst->fval[i] = src->fval[i];
	for (i = 0; i < CONTAINER_SIZE; i++) dst->dval[i] = src->dval[i];
	dst->key = src->key;
}

void swap_container(struct CONTAINER *c1, struct CONTAINER *c2)
{
	struct CONTAINER tmp;
	copycontainer(c1, &tmp);
	copycontainer(c2, c1);
	copycontainer(&tmp, c2);
}


void print_container_array(const struct CONTAINER C[], int N)
{
	int i;
	for (i = 0; i < N; i++) fprintf(stdout, "%d\n", C[i].key);
}


/////////////////////////////////////////////////////////////////////
// ***FILL*** if necessary, your functions can be added
/////////////////////////////////////////////////////////////////////

void MakeK(CONTAINER * C, int * K, int N) {
	for (int i = 0; i < N; i++) {
		K[i] = C[i].key;
	}
}
void MakeIndex(int * Index, int  N) {
	for (int i = 0; i < N; i++) {
		Index[i] =i ;
	}
}

void MakeReverseIndex(int* I,int*J,int N){
	for (int i = 0; i < N; i++) {
	J[I[i]] = i;
	}
}

void Swap(int *K,int*J , int a, int b) 
{

	int temp = K[a];
	K[a] = K[b];
	K[b] = temp;

	temp = J[a];
	J[a] = J[b];
	J[b] = temp;
}

int Partition(int *K, int * J, int left, int right)
{
	int pivot;
	pivot = left; // pivot start at left

	int low = left + 1;
	int high = right;

	while (low <= high) // repeat until low meethigh
	{
		while (K[pivot] >= K[low] && low <= right) //find the value that bigger then pivot value 
		{
			low++; // low move right
		}
		while (K[pivot] <= K[high] && high > (left)) // //find the value that smaller then pivot value 
		{
			high--; // high move left
		}
		if (low <= high)// it low and high does not meet  
		{
			Swap(K,J, low, high); //low and high-> swap 
		}

	}

	Swap(K,J, pivot, high); // pivot and high ->Swap 

	return high;  // return changed pivot index 

}
//reference: https ://hongku.tistory.com/149

void QuickSort(int *K, int *J, int left, int right)
{
	if (left < right)
	{
		int pivot = Partition(K, J, left, right); // devide.
		QuickSort(K, J, left, pivot - 1); // sort left
		QuickSort(K, J,  pivot + 1, right); // sort right


	}
}

void convert(CONTAINER * C, int*I, int N) {

	struct CONTAINER *ConvertC;
	ConvertC = (struct CONTAINER*) malloc(sizeof(struct CONTAINER)*N);
	for (int i = 0; i < N; i++) {
		generatecontainer(ConvertC + i, C[I[i]].key);
	}
	for (int i = 0; i < N; i++) {
		swap_container( C+i, ConvertC+i);
	}
	free(ConvertC);
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
int main()
{
	int i, N;
	int val;
	int check;

	struct CONTAINER *C;


	int *K;	
	int *I;	
	int *J;	
	

	fscanf(stdin, "%d", &N);

	C = (struct CONTAINER*) malloc(sizeof(struct CONTAINER)*N);
	for (i = 0; i < N; i++) {
		if (fscanf(stdin, "%d", &val) != 1) {
			fprintf(stderr, "cannot read integer from standard input.\n");
			free(C);
			exit(0);
		}
		else { generatecontainer(C + i, val); }
	}
#ifdef MEASURE_TIME
	clock_t start, end;
	double cpu_time_used;

	start = clock();
#endif

	K = (int*)malloc(sizeof(int)*N);
	I = (int*)malloc(sizeof(int)*N);
	J = (int*)malloc(sizeof(int)*N);

	/////////////////////////////////////////////////////////////////////
	// ***FILL***: do container sorting EFFICIENTLY
	// you may define some functions
	/////////////////////////////////////////////////////////////////////

	// 1. sort keys with index
	//  (output) K: sorted key value array
	//  (output) I: index generated by sorting, such that K[i] == C[I[i]].key
	//  (output) J: inverted index, such that K[J[i]] == C[i].key
	//              (the inverted index set J may not be necessary
	//              depending on your method)

	
	MakeK(C, K, N);
	MakeIndex(I, N);

	QuickSort(K, I, 0, N - 1);
	MakeReverseIndex(I,J, N);


// 2. re-order containers by I or J

	convert(C, I, N);
	printf("\n order by I \n");
	print_container_array(C,N);

	convert(C, J, N);
	printf("\n re-order by J \n");
	print_container_array(C, N);

	/////////////////////////////////////////////////////////////////////
	// print sorted key values
	printf("\n sorted value\n"); 

	for (int i = 0; i < N; i++) {
		printf("%d ", K[i]);
	}
	printf("\n");

	/////////////////////////////////////////////////////////////////////



	// check if the container items are all correct
	for (i = 0, check = 0; i < N; i++) {
		val = validate_container_item(C + i);
		if (val >= 0) {
			fprintf(stderr, "validation failed at C[%d]: %d %.1f %.1lf\n",
				i, C[i].key, C[i].fval[val], C[i].dval[val]);
		}
		else check++;
	}
	if (check != N) {
		fprintf(stderr,
			"Check failed: %d/%d (%.1f%%) container items are not valid\n",
			N - check, N, (double)(N - check) / (double)N);
	}


#ifdef MEASURE_TIME
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	fprintf(stderr, ">> Execution time: %.5f seconds\n", cpu_time_used);
#endif

	free(K);
	free(I);
	free(J);
	free(C);

	return 0;
}

