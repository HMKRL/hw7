#include<stdio.h>

int bucket_union();
int new_mapping();
void print_result();

int main()
{
	int n;
	char input_file[] = "buckets.in";
	FILE *infp;
	
	printf("Please input n:");
	scanf("%d",&n);

//open input file, if file not exist, show error message.
	if(infp = fopen(input_file,"r") == NULL){
		printf("ERROR:Input file not found.");
		exit(1);
	}


	
	return 0;
}
