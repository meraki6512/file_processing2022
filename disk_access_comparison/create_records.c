#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORD_SIZE 200

//
// input parameters: �л� ���ڵ� ��, ���ڵ� ����
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

	// ����ڷκ��� �Է� ���� ���ڵ� �� ��ŭ�� ���ڵ� ������ �����ϴ� �ڵ� ����
	
	// ���Ͽ� '�л� ���ڵ�' ���� ���
	// 1. ���ڵ��� ũ��� ������ 200 ����Ʈ�� �ؼ�
	// 2. ���ڵ� ���Ͽ��� ���ڵ�� ���ڵ� ���̿� � �����͵� ���� �ȵ�
	// 3. ���ڵ忡�� ������ �����͸� �����ص� ����
	// 4. ���� n���� ���ڵ带 �����ϸ� ���� ũ��� ��Ȯ�� 200 x n ����Ʈ�� �Ǿ�� ��


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
