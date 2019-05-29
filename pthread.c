//Please edit the value assigned to the variable "size" in line #7 accordingly for time calculation

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define size 100000

#define SWAP(x,y) do {\
    __typeof__(x) tmp = x;\
    x = y;\
    y = tmp;\
} while(0)

void *bubbleSort()
{

	printf("\n\nBubble sort, sorts %d elements in descending order now..\n\n", size);
	int counter;
	srand((time(NULL)));
	int *theList = (int*)malloc(sizeof(int)*size);

	for(counter = 0; counter < size; counter++)
	{
		theList[counter] = (rand() % 900000)+1;

	}

	int temp_bubble;

	for (int counter = 0; counter < size; counter++)
	{
		for (int count = counter + 1; count < size; count++)
		{
			if (theList[count] > theList[counter])
			{
				temp_bubble = *(theList + count);
				*(theList + count) = *(theList + counter);
				*(theList + counter) = temp_bubble;
			}
		}
	}

	free(theList);
}

void *shellSort()
{

	printf("\n\nShell sort, sorts %d elements in ascending order now..\n\n", size);
	int counter;
	srand((time(NULL)));
	int *theList = (int*)malloc(sizeof(int)*size);

	for(counter = 0; counter < size; counter++)
	{
		theList[counter] = (rand() % 9000000)+1;

	}

	int temp_shell;
	int count;
	int gap;

	for(gap = size / 2; gap > 0; gap /= 2)
	{

		for (counter = gap; counter < size; counter++)
		{

			temp_shell = *(theList + counter);
			count = counter - gap;

			{

				while (count >= 0 && *(theList + count) > temp_shell)
				{

					*(theList + (count+gap)) = *(theList + count);
					count = count - gap;

				}

				*(theList + (count+gap)) = temp_shell;

			}

		}
	}
	free(theList);
}


int partition(int *array, int left, int right, int pivot)
{
    int pivotValue = array[pivot];
    SWAP(array[pivot], array[right]);
    int storeIndex = left;
    for (int i=left ; i<right ; i++)
    {
        if (array[i] <= pivotValue)
        {
            SWAP(array[i], array[storeIndex]);
            storeIndex++;
        }
    }
    SWAP(array[storeIndex], array[right]);
    return storeIndex;
}

void quicksort(int *array, int left, int right)
{
     if (right > left)
     {
        int pivotIndex = left + (right - left)/2;
        pivotIndex = partition(array, left, right, pivotIndex);
        quicksort(array, left, pivotIndex-1);
        quicksort(array, pivotIndex+1, right);
     }
}

struct qsort_starter
{
    int *array;
    int left;
    int right;
    int depth;
};

void parallel_quicksort(int *array, int left, int right, int depth);

void* quicksort_thread(void *init)
{
    struct qsort_starter *start = init;
    parallel_quicksort(start->array, start->left, start->right, start->depth);
    return NULL;
}


void parallel_quicksort(int *array, int left, int right, int depth)
{

    if (right > left)
    {
        int pivotIndex = left + (right - left)/2;
        pivotIndex = partition(array, left, right, pivotIndex);
        
        if (depth-- > 0)
        {
            struct qsort_starter arg = {array, left, pivotIndex-1, depth};
            pthread_t thread;
            int ret = pthread_create(&thread, NULL, quicksort_thread, &arg);
            parallel_quicksort(array, pivotIndex+1, right, depth);
            pthread_join(thread, NULL);
        }
        else
        {
            quicksort(array, left, pivotIndex-1);
            quicksort(array, pivotIndex+1, right);
        }
    }
}


int main()
{
	pthread_t shellThread;
	pthread_t bubbleThread;
	int counter;

	printf ("List being sorted in descending order...\n\n");

	double startShellTime = clock();

	pthread_create(&shellThread, NULL, &shellSort, (void *)NULL);
	pthread_join(shellThread, NULL);

	double endShellTime = clock();

	printf("Time taken by shell sort using pThreads: %lf miliseconds \n", (endShellTime - startShellTime) );

	double startBubbleTime = clock();

	pthread_create(&bubbleThread, NULL, &bubbleSort, (void *)NULL);
	pthread_join(bubbleThread, NULL);

	double endBubbleTime = clock();

	printf("Time taken by bubble sort using pThreads: %lf miliseconds \n", (endBubbleTime - startBubbleTime));

	int depth = 4;
	srand((time(NULL)));

	int *theList = (int*)malloc(sizeof(int)*size);

	for(counter = 0; counter < size; counter++)
	{
		theList[counter] = (rand() % 9000000)+1;
	}

	
	double startQuickTime = clock();
	printf("\n\n\nQuickSort, sorts %d elements in ascending order now..\n", size );
	parallel_quicksort(theList, 0, size-1, depth);
	double endQuickTime = clock();

	printf("\n\nTime taken by Quick sort using  elements using pThreads: %lf miliseconds\n",  endQuickTime - startQuickTime);

}
