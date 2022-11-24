#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define RECORD_SIZE 200
#define SUFFLE_NUM	1000

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);

//
// input parameters: 레코드 파일
//
int main(int argc, char **argv)
{
	int *read_order_list;
	int num_of_records;
	FILE* fp;
	long size;
	struct timeval start, end;

	if (argc!=2){
		fprintf(stderr, "usage: %s <record_file_name>\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	num_of_records = size/RECORD_SIZE;
	char student[num_of_records][RECORD_SIZE];
	read_order_list = (int*)malloc(sizeof(int)*num_of_records);
	
	// 이 함수를 실행하면 'read_order_list' 배열에 읽어야 할 레코드 번호들이 순서대로
	// 나열되어 저장됨. 'num_of_records'는 레코드 파일에 저장되어 있는 전체 레코드의 수를 의미함.
	GenRecordSequence(read_order_list, num_of_records);

	// 'read_order_list'를 이용하여 레코드 파일로부터 전체 레코드를 random 하게 읽어들이고,
	// 이때 걸리는 시간을 측정하는 코드 구현

	gettimeofday(&start, NULL);

	for (int i=0; i<num_of_records; i++){
		fseek(fp, read_order_list[i] * RECORD_SIZE - 1, SEEK_SET);
		if (fread(student[i], sizeof(char), RECORD_SIZE, fp) != RECORD_SIZE){
			fprintf(stderr, "fread error\n");
			exit(1);
		}
	}

	gettimeofday(&end, NULL);
	printf("%ld usec\n", end.tv_usec - start.tv_usec);

	free(read_order_list);
	return 0;
}

void GenRecordSequence(int *list, int n)
{
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++) {
		list[i] = i;
	}

	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}

	return;
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;

	return;
}
