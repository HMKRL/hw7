#include<stdio.h>
#include<stdlib.h>

int bucket_union();
int new_mapping();
void print_result();

int main() {
	int i, j, rows = 0, curr_row = 0, curr_num = 0, n, number = 0;
	char temp, input_file[11] = "buckets.in\0", *row = malloc(200 * sizeof(char));
	FILE *infp;

	printf("Please input n:");
	scanf("%d",&n);

//open input file, if file not exist, show error message.
	infp = fopen(input_file, "r");
	if(infp == NULL) {
		printf("ERROR:Input file not found.");
		exit(1);
	}

//scan for row lines in bucket.in than create array
	while(fgets(row, 200, infp) != NULL) {
		rows++;
	}
	free(row);
	int **orig_bucket;
	orig_bucket = malloc(rows * sizeof(int*));
	for(i = 0;i < rows;i++) {
		orig_bucket[i] = calloc(40, sizeof(int));
	}

//reset read head
	rewind(infp);

//get number from file"buckets.in" and store into array
	while(1) {
		temp = fgetc(infp);
	
		if(temp == EOF) {
			number = 0;
			break;
		}
	
		if(temp == ' ') {
			orig_bucket[curr_row][curr_num] = number;
			curr_num++;
			number = 0;
		}
		else if(temp == '\n') {
			orig_bucket[curr_row][curr_num] = number;
			curr_row++;
			curr_num = 0;
			number = 0;
		}
		else if((int)temp > 47 && (int)temp < 58) {
			number *= 10;
			number += (int)temp - 48;
		}
	}
//release allocated memory
	for(i = 0;i < rows;i++) {
		free(orig_bucket[i]);
	}
	free(orig_bucket);

	return 0;
}

int bucket_union() {
	
}
