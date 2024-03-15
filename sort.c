#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int m, int r){
	int ln = 0, rn = 0, i = 0, j = 0, k = l;
	int *left = NULL, *right = NULL;

	ln = m - l + 1;
	rn = r - m;

	left = (int *) Alloc(sizeof(int) * ln);
	right = (int *) Alloc(sizeof(int) * rn);

	for (int i = 0; i < ln; i++){
		left[i] = pData[l + i];
	}
	
	for (int i = 0; i < rn; i++){
		right[i] = pData[m + 1 + i];
	}
	
	while (i < ln && j < rn){
		if (left[i] <= right[j]){
			pData[k] = left[i];
			i++;
		}
		else{
			pData[k] = right[j];
			j++;
		}
		k++;
	}

	while (i < ln){
		pData[k] = left[i];
		i++;
		k++;
	}

	while (j < rn){
		pData[k] = right[j];
		j++;
		k++;
	}

	DeAlloc(left);
	DeAlloc(right);
}

void mergeSort(int pData[], int l, int r)
{
	int m = (l + r) / 2;

	if (l < r){
		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);
		merge(pData, l, m, r);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	if (dataSz < 100){
		for (int i = 0; i < dataSz; i++){
			printf("%d ", pData[i]);
		}
		printf("\n");
	}
	else{
		for (int i = 0; i < 100; i++){
			printf("%d ", pData[i]);
		}
		printf("\n\t");

		if (dataSz > 100){
			for (int i = dataSz - 100; i < dataSz; i++){
				printf("%d ", pData[i]);
			}
			printf("\n");
		}
	}
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}