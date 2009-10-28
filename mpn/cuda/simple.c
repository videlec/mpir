/*
 * A very simple program that calls a routine
 * that is executed on the GPU.
 * The routine fills an array of integers with
 * consecutive values 0,1,2,..., size of the array - 1.
 */

#include <stdio.h>
#define SIZEOFARRAY 64 
/*
 * The function fillArray is in a different file
 */
extern void fillArray(int *a,int size);

/* The main program */
int main(int argc,char *argv[])
{
/* Declare the array that will be modified by the GPU */
 int a[SIZEOFARRAY];
 int i;
/* Initialize the array to 0s */
 for(i=0;i < SIZEOFARRAY;i++) {
	a[i]=0;
 }
 /* Print the initial array */
 printf("Initial state of the array:\n");
 for(i = 0;i < SIZEOFARRAY;i++) {
   printf("%d ",a[i]);
 }
 printf("\n");

/* Call the function that will in turn call the function
 * in the GPU that will fill the array
 */
 fillArray(a,SIZEOFARRAY);
 
 /* Now print the array after calling fillArray */
 printf("Final state of the array:\n");
 for(i = 0;i < SIZEOFARRAY;i++) {
   printf("%d ",a[i]);
 }
 printf("\n");
 return 0;
}
