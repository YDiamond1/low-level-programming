#include<stdio.h>

int is_prime(unsigned long number)
{	unsigned long i;
	if(number == 2) return 1;
	if(number <= 1 || number%2 == 0 ) return 0;
	for(i=3; i*i < number; i+=2){
		if(number%i==0) return 0;
	}
	return 1;

}

int main(int argc, char** argv){

	unsigned long number;
	puts("Input your number:");
	scanf("%lu", &number);
	printf("Your number is: %lu.",number);
	int is = is_prime(number);
	printf("It is %s. \n", is? "prime":"not prime");
	return 0;
}


