//Please edit the value assigned to the variable "size" in line #50 accordingly for time calculation

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>

void shellSort(int theList[], int size);
void InsertSort(int theList[], int counter, int size,  int half);
void quickSort_parallel(int* array, int lenArray, int numThreads);
void quickSort_parallel_internal(int* array, int left, int right, int cutoff);

void shellSort(int theList[], int size)
{
	int gap;
	int count = 0;
	int temp = 0;
	int counter = 0;
	for(gap = size/2; gap > 0; gap = gap/2)
	{
		#pragma omp parallel for shared( theList, size, gap, counter)  default(none)
		for( counter = 0; counter < gap; counter++)
		{
			InsertSort(theList, counter, size, gap);
		}
	}
}
void InsertSort(int theList[], int counter, int size,  int half){

	int temp = 0;
	int count = 0;

	for (int f = half+counter; f<size; f = f+half)
	{
		count = f;
		while(count > counter && theList[count-half] > theList[count])
		{
			temp = theList[count];
			theList[count] = theList[count-half];
			theList[count-half] = temp;
			count = count - half;
		}
	}
}


void quickSort_parallel(int* array, int lenArray, int numThreads){

	int cutoff = 1000;

	#pragma omp parallel num_threads(numThreads)
	{	
		#pragma omp single nowait
		{
			quickSort_parallel_internal(array, 0, lenArray-1, cutoff);	
		}
	}	

}



void quickSort_parallel_internal(int* array, int left, int right, int cutoff) 
{
	
	int i = left, j = right;
	int tmp;
	int pivot = array[(left + right) / 2];

	
	{
	  	/* PARTITION PART */
		while (i <= j) {
			while (array[i] < pivot)
				i++;
			while (array[j] > pivot)
				j--;
			if (i <= j) {
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
				i++;
				j--;
			}
		}

	}


	if ( ((right-left)<cutoff) ){
		if (left < j){ quickSort_parallel_internal(array, left, j, cutoff); }			
		if (i < right){ quickSort_parallel_internal(array, i, right, cutoff); }

	}else{
		#pragma omp task 	
		{ quickSort_parallel_internal(array, left, j, cutoff); }
		#pragma omp task 	
		{ quickSort_parallel_internal(array, i, right, cutoff); }		
	}

}

int main()
{

	//OpenMP over shellsort
	
	int size = 100000;
	int theList[size];	
	int counter = 0;
	int count;
	srand((time(NULL)));

	#pragma omp parallel for num_threads(counter)
	for(counter = 0; counter < size; counter++)
	{
		theList[counter] = (rand() % 900000)+1;

	}

	double startShellTime = clock();


	shellSort(theList, size);
	double endShellTime = clock();


	printf("Time taken by Shell sort for %d elements using OpenMP: %f miliseconds\n", size, (endShellTime - startShellTime));

//x------------------x--------------------------x---------------------x----------------------------x

//OpenMP over bubblesort

	int temp_bubble;


	double startBubbleTime = clock();
#pragma omp parallel for num_threads(counter)
	for (int counter = 0; counter < size; counter++)
	{
	#pragma omp parallel for num_threads(count)
		for (int count = counter + 1; count < size; count++)
		{
			if (theList[count] > theList[counter])
			{
				temp_bubble = theList[count];
				theList[count] = theList[counter];
				theList[counter] = temp_bubble;
			}
		}
	}


	double endBubbleTime = clock();

	printf("Time taken by Bubble sort for %d elements using OpenMP: %lf miliseconds\n", size, (endBubbleTime - startBubbleTime));



//x------------------x--------------------------x---------------------x----------------------------x

//OpenMP over QuickSort

	#pragma omp parallel for num_threads(counter)
	for(counter = 0; counter < size; counter++)
	{
		theList[counter] = (rand() % 900000)+1;
	}

	double startQuickTime = clock();


	quickSort_parallel(theList, size, 8);
	double endQuickTime = clock();


	printf("Time taken by Quick sort for %d elements using OpenMP: %f miliseconds\n", size, (endQuickTime - startQuickTime));

	return 0;

}