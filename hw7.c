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
int new_mapping(int**, const int, int);
void print_result(Mi*, FILE*, FILE*, int**, int, int);

int main() {
	int i, j, rows = 0, n;
	char *row = malloc(200 * sizeof(char));
	FILE *infp;

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

	int **orig_bucket;
	orig_bucket = malloc(rows * sizeof(int*));
	for(i = 0;i < rows;i++) {
		orig_bucket[i] = calloc(100, sizeof(int));
	}

//reset read head
	rewind(infp);

//get number from file"buckets.in" and store into array
	while(bucket_union(orig_bucket, infp) != rows) {
		rewind(infp);
	}

	for(i = 0;i < rows;i++) {
		for(j = 0;j < 100;j++) {
			if(orig_bucket[i][j] != 0) {
				printf("%d ",orig_bucket[i][j]);
			}
		}
		printf("\n");
	}

	new_mapping(orig_bucket, n, rows);

//free allocated memory
	for(i = 0;i < rows;i++) {
		free(orig_bucket[i]);
	}
	free(orig_bucket);
	free(row);

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

int new_mapping(int **orig_bucket, const int n, int rows) {
	int curr_row = 0, curr_num = 0, count = 0, i = 0;
	FILE *m_out = fopen("mapping.out", "w"), *b_out = fopen("buckets.out", "w");

	Mi *firstMi = malloc(sizeof(Mi));
	firstMi -> map = malloc(sizeof(Map));
	firstMi -> next = NULL;

	Mi *curMi = firstMi;
	Map *curMap;

	while(i < rows) {
		if(curMi -> next != NULL) {
			curr_num = 0;
			count = 0;
			curMap = curMi -> map;
			while(orig_bucket[curr_row][curr_num] != 0 && curMap -> next != NULL) {
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
			if((count) <= n) {//save current row into new mapping
				print_result(curMi, m_out, b_out, orig_bucket, curr_row, n);
				curr_row++;
				curMi = firstMi;
			}
			else {//scan next bucket
				curMi = curMi -> next;
			}
		}
		else {
			print_result(curMi, m_out, b_out, orig_bucket, curr_row, n);
			curMi -> next = malloc(sizeof(Mi));
			curMi = curMi -> next;
			curMi -> map = malloc(sizeof(Map));
			curMi -> next = NULL;
			curMi = firstMi;
		}
		i++;
	}
	

	return 0;
}

void print_result(Mi *curMi, FILE *m_out, FILE *b_out, int **orig_bucket, int curr_row, int n) {
	int i = 0;
	Map *tempMap;
	while(orig_bucket[curr_row][i] != 0) {
		if(orig_bucket[curr_row][i] < (curMi -> map) -> value) {
			tempMap = malloc(sizeof(Map));
			tempMap -> value = orig_bucket[curr_row][i];
			tempMap -> next = curMi -> map;
			curMi -> map = tempMap;
			i++;
		}
		else if(orig_bucket[curr_row][i] == (curMi -> map) -> value) {
			i++;
		}
		else if(orig_bucket[curr_row][i] > (curMi -> map) -> value) {
			if((curMi -> map) -> next != NULL) {
				if(orig_bucket[curr_row][i] < (curMi -> map) -> next -> value) {
					tempMap = malloc(sizeof(Map));
					tempMap -> value = orig_bucket[curr_row][i];
					tempMap -> next = (curMi -> map) -> next;
					(curMi -> map) -> next = tempMap;
					curMi -> map = tempMap;
					i++;
				}
				else {
					curMi -> map = (curMi -> map) -> next;
				}
			}
			else {
				(curMi -> map) -> next = malloc(sizeof(Map));
				curMi -> map = (curMi -> map) -> next;
				(curMi -> map) -> value = orig_bucket[curr_row][i];
				(curMi -> map) -> next = NULL;
				i++;
			}
		}
	}
}
