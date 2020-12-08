#include<stdio.h>

const int amas[] = {0,5,7}, bmas[]={1,6,9};


long int scalar_product(const int* a, const int* b, size_t size)
{
	long int sum = 0;
	for(size_t i = 0; i < size; i++ ){
		sum += (long int) a[i] * b[i];
	}
	return sum;
}


int main (int argc, char** argv)
{
	size_t size = sizeof(amas)/sizeof(*amas);
	printf("result = %li\n", scalar_product(amas, bmas, size) );
	return 0;
}
