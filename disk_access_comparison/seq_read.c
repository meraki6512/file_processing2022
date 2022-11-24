#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define RECORD_SIZE 200

//
// input parameters: 레코드 파일
//

int main(int argc, char **argv){

	struct timeval start, end;
	FILE* fp;
	long size;
	int record_num;

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

	record_num = size/RECORD_SIZE;
	char student[record_num][RECORD_SIZE];

	// 레코드 파일로부터 전체 레코드를 순차적으로 읽어들이고, 이때
	// 걸리는 시간을 측정하는 코드 구현

	gettimeofday(&start, NULL);

	for (int i=0; i<record_num; i++){
		if (fread(student[i], sizeof(char), RECORD_SIZE, fp)!=RECORD_SIZE){
			fprintf(stderr, "fread error\n");
			exit(1);
		}
	}

	gettimeofday(&end, NULL);
	
	printf("%ld usec\n", end.tv_usec-start.tv_usec);
	
	return 0;
}
