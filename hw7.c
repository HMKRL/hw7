#include<stdio.h>
#include<stdlib.h>

int bucket_union(int **orig_bucket, FILE *infp);
int new_mapping();
void print_result();

int main() {
	int i, j, rows = 0, n;
	char input_file[11] = "buckets.in\0", *row = malloc(200 * sizeof(char));
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
	while(bucket_union(orig_bucket, infp) != rows) {
		rewind(infp);
	}

	for(i = 0;i < rows;i++) {
		for(j = 0;j < 40;j++) {
			if(orig_bucket[i][j] != 0) {
				printf("%d ",orig_bucket[i][j]);
			}
		}
		printf("\n");
	}

//release allocated memory
	for(i = 0;i < rows;i++) {
		free(orig_bucket[i]);
	}
	free(orig_bucket);

	return 0;
}

int bucket_union(int **orig_bucket, FILE *infp) {
	int curr_row = 0, curr_num = 0;
	char temp;
	while(fscanf(infp, "%d%c", &orig_bucket[curr_row][curr_num], &temp) != EOF) {
		curr_num++;
		if(temp == '\r') {
			curr_row++;
			curr_num = 0;
		}
	}
/*
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
*/
	return ++curr_row;
}
