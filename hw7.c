#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

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
void store_into_bucket(Mi*, FILE*, FILE*, int**, int, int);

int main() {
	int i, rows = 0, n;
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
	int curr_row = 0, curr_num = 0, count = 0;
	FILE *m_out = fopen("mapping.out", "w"), *b_out = fopen("buckets.out", "w");
	bool need_new = false;

	Mi *firstMi = malloc(sizeof(Mi));
	firstMi -> map = calloc(1, sizeof(Map));
	firstMi -> next = NULL;
	firstMi -> map -> next = NULL;

	Mi *curMi = firstMi;
	Map *curMap, *tempMap;

	while(curr_row < rows) {
		count = 0;
		curr_num = 0;
		curMap = curMi -> map;
		if(!need_new) {
			while(orig_bucket[curr_row][curr_num] != 0) {
				if(curMap -> next != NULL) {
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
				else {
					if(orig_bucket[curr_row][curr_num] != curMap -> value) {
						if(orig_bucket[curr_row][curr_num] > curMap -> value) {
							count++;
							break;
						}
						else {
							count++;
							curr_num++;
						}
					}
					else {
						count++;
						curr_num++;
						break;
					}
				}
			}
			while(orig_bucket[curr_row][curr_num] != 0) {
				count++;
				curr_num++;
			}
			tempMap = curMap;
			while(tempMap -> value != 0) {
				if(tempMap -> next == NULL) {
					count++;
					break;
				}
				tempMap = tempMap -> next;
				count++;
			}
			if((count) <= n) {//save current row into new mapping
				store_into_bucket(curMi, m_out, b_out, orig_bucket, curr_row, n);
				curr_row++;
				curMi = firstMi;
				curr_num = 0;
			}
			else if(curMi -> next != NULL){//scan next bucket
				curMi = curMi -> next;
				curr_num = 0;
			}
			else {
				need_new = true;
			}
		}
		else {
			need_new = false;
			curMi -> next = malloc(sizeof(Mi));
			curMi = curMi -> next;
			curMi -> map = calloc(1, sizeof(Map));
			curMi -> map -> next = NULL;
			curMi -> next = NULL;
			store_into_bucket(curMi, m_out, b_out, orig_bucket, curr_row, n);
			curr_row++;
			curMi = firstMi;
		}
	}

	curMi = firstMi;
	while(curMi -> next != NULL) {
		curMap = curMi -> map;
		while(curMap -> next != NULL) {
			printf("%d ", curMap -> value);
			curMap = curMap -> next;
		}
		printf("\n");
		curMi = curMi -> next;
	}

	return 0;
}

void store_into_bucket(Mi *curMi, FILE *m_out, FILE *b_out, int **orig_bucket, int curr_row, int n) {
	int i = 0;
	Map *tempMap, *curMap = curMi -> map;
	while(orig_bucket[curr_row][i] != 0) {
		if(orig_bucket[curr_row][i] < curMap -> value) {
			tempMap = calloc(1, sizeof(Map));
			tempMap -> value = orig_bucket[curr_row][i];
			tempMap -> next = curMap;
			curMap = tempMap;
			i++;
		}
		else if(orig_bucket[curr_row][i] == curMap -> value) {
			i++;
		}
		else if(orig_bucket[curr_row][i] > curMap -> value) {
			if(curMap -> next != NULL) {
				if(orig_bucket[curr_row][i] < curMap -> next -> value) {
					tempMap = calloc(1, sizeof(Map));
					tempMap -> value = orig_bucket[curr_row][i];
					tempMap -> next = curMap -> next;
					curMap -> next = tempMap;
					curMap = curMap -> next;
					i++;
				}
				else {
					curMap = curMap -> next;
				}
			}
			else if(curMap -> value == 0) {
				curMap -> value = orig_bucket[curr_row][i];
				i++;
			}
			else {
				curMap -> next = calloc(1, sizeof(Map));
				curMap = curMap -> next;
				curMap -> value = orig_bucket[curr_row][i];
				curMap -> next = NULL;
				i++;
			}
		}
	}
}
