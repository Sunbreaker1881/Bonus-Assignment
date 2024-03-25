// Evan Suarez
// Bonus Assignment
// 3/24/24

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
    extraMemoryAllocated += sz;
    size_t* ret = malloc(sizeof(size_t) + sz);
    if (!ret) {
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

void DeAlloc(void* ptr)
{
    if (!ptr) {
        printf("Error: Attempting to deallocate null pointer\n");
        return;
    }
    size_t* pSz = (size_t*)ptr - 1;
    extraMemoryAllocated -= *pSz;
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
    return ((size_t*)ptr)[-1];
}

// Selection Sort Implementation
void selectionSort(int* pData, int n)
{
    int i, j, minIndex, temp;
    for (i = 0; i < n - 1; i++)
    {
        minIndex = i;
        for (j = i + 1; j < n; j++)
        {
            if (pData[j] < pData[minIndex])
                minIndex = j;
        }
        temp = pData[minIndex];
        pData[minIndex] = pData[i];
        pData[i] = temp;
    }
}

// Parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
    FILE* inFile = fopen(inputFileName,"r");
    if (!inFile) {
        printf("Error: Could not open file %s\n", inputFileName);
        return 0;
    }
    int dataSz = 0;
    *ppData = NULL;
    
    if (fscanf(inFile,"%d\n",&dataSz) != 1) {
        printf("Error: Unable to read data size from file %s\n", inputFileName);
        fclose(inFile);
        return 0;
    }
    
    *ppData = (int *)Alloc(sizeof(int) * dataSz);
    if (!(*ppData)) {
        fclose(inFile);
        return 0;
    }

    for (int i = 0; i < dataSz; i++)
    {
        if (fscanf(inFile, "%d", &((*ppData)[i])) != 1) {
            printf("Error: Unable to read data from file %s\n", inputFileName);
            fclose(inFile);
            DeAlloc(*ppData);
            return 0;
        }
    }
    
    fclose(inFile);
    
    return dataSz;
}

// Deallocate memory for the data arrays
void deallocateData(int *pData)
{
    DeAlloc(pData);
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i=0;i<100;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");
    
    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
    
    for (i=0;i<3;++i)
    {
        int *pDataSrc, *pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);
        
        if (dataSz <= 0)
            continue;
        
        pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
    
        printf("---------------------------\n");
        printf("Dataset Size : %d\n",dataSz);
        printf("---------------------------\n");
        
        printf("Selection Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        selectionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
        printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
        printArray(pDataCopy, dataSz);
        
        deallocateData(pDataCopy);
        deallocateData(pDataSrc);
    }
    printf("this work\n");
    return 0;
}
