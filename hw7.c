#include<stdio.h>
#include<stdlib.h>

struct mapping {
	int value;
	struct mapping *next;
};

struct m_index {
	struct mapping *map;
	struct m_index *next;
};

typedef struct mapping Map;
typedef struct m_index Mi;

int bucket_union(int**, FILE*);
int new_mapping(Mi*, int**, const int);
void print_result();

int main() {
	int i, j, rows = 0, n;
	char *row = malloc(200 * sizeof(char));
	FILE *infp;
	Mi *firstMi = NULL;

	printf("Please input n:");
	scanf("%d",&n);

//open input file, if file not exist, show error message.
	infp = fopen("buckets.in", "r");
	if(infp == NULL) {
		fprintf(stderr, "%s\n", "ERROR:Input file not found.");
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

//free allocated memory
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
		if(temp == '\r' || temp == '\n') {
			curr_row++;
			curr_num = 0;
		}
	}

	return ++curr_row;
}

int new_mapping(Mi *firstMi, int **orig_bucket, const int n) {
	int curr_row = 0, curr_num = 0, count = 0;
	Mi *curMi = firstMi;
	Map *curMap;
	while(orig_bucket[curr_row][0] != 0) {
		if(curMi != NULL) {
			curr_num = 0;
			count = 0;
			curMap = curMi -> map;
			while(orig_bucket[curr_row][curr_num] != 0 || curMap != NULL) {
				if(orig_bucket[curr_row][curr_num] != curMap -> value) {
					if(orig_bucket[curr_row][curr_num] > curMap -> value) {
						count++;
						curMap = curMap -> next;
					}
					else {
						count++;
						curr_num++;
					}
				}
				else {
					count++;
					curMap = curMap -> next;
					curr_num++;
				}
			}
			while(orig_bucket[curr_row][curr_num] != 0) {
				count++;
				curr_num++;
			}
			if((count) <= n) {
//save current row into new mapping
				curr_row++;
			}
			else {
//scan next bucket
				curMi = curMi -> next;
			}
		}
	}
}
