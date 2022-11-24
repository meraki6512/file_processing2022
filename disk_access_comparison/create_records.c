#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORD_SIZE 200

//
// input parameters: 학생 레코드 수, 레코드 파일
//
int main(int argc, char **argv)
{

	FILE* fp;
	char student[RECORD_SIZE];
	int i = 0;

	if (argc!=3){
		fprintf(stderr, "usage: %s <#records> <record_file_name>\n", argv[0]);
		exit(1);
	}

	// 사용자로부터 입력 받은 레코드 수 만큼의 레코드 파일을 생성하는 코드 구현
	
	// 파일에 '학생 레코드' 저장 방법
	// 1. 레코드의 크기는 무조건 200 바이트를 준수
	// 2. 레코드 파일에서 레코드와 레코드 사이에 어떤 데이터도 들어가면 안됨
	// 3. 레코드에는 임의의 데이터를 저장해도 무방
	// 4. 만약 n개의 레코드를 저장하면 파일 크기는 정확히 200 x n 바이트가 되어야 함


	if ((fp = fopen(argv[2], "w"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[2]);
		exit(1);
	}

	memset(student, 'a', sizeof(student));
	while ( i++ < atoi(argv[1])) {
		if (fwrite(student, sizeof(char), RECORD_SIZE, fp)!=RECORD_SIZE){
			fprintf(stderr, "fwrite error\n");
			exit(1);
		}
	}

	return 0;
}
