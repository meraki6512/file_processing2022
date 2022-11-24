#include <stdio.h>		// �ʿ��� header file �߰� ����
#include <stdlib.h>
#include <string.h>
#include "student.h"

/* prototypes & comments */
//
// �Լ� readRecord()�� �л� ���ڵ� ���Ͽ��� �־��� rrn�� �ش��ϴ� ���ڵ带 �о 
// recordbuf�� �����ϰ�, ���� unpack() �Լ��� ȣ���Ͽ� �л� Ÿ���� ������ ���ڵ���
// �� �ʵ尪�� �����Ѵ�. �����ϸ� 1�� �׷��� ������ 0�� �����Ѵ�.
// unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� record���� �� field�� �����ϴ� ���� �Ѵ�.
//
int readRecord(FILE *fp, STUDENT *s, int rrn);
void unpack(const char *recordbuf, STUDENT *s);

//
// �Լ� writeRecord()�� �л� ���ڵ� ���Ͽ� �־��� rrn�� �ش��ϴ� ��ġ�� recordbuf�� 
// ����Ǿ� �ִ� ���ڵ带 �����Ѵ�. ������ pack() �Լ��� ȣ���Ͽ� recordbuf�� �����͸� ä�� �ִ´�.
// ���������� �����ϸ� '1'��, �׷��� ������ '0'�� �����Ѵ�.
//
int writeRecord(FILE *fp, const STUDENT *s, int rrn);
void pack(char *recordbuf, const STUDENT *s);

//
// �Լ� appendRecord()�� �л� ���ڵ� ���Ͽ� ���ο� ���ڵ带 append�Ѵ�.
// ���ڵ� ���Ͽ� ���ڵ尡 �ϳ��� �������� �ʴ� ��� (ù ��° append)�� header ���ڵ带
// ���Ͽ� �����ϰ� ù ��° ���ڵ带 �����Ѵ�. 
// �翬�� ���ڵ带 append�� �� ������ header ���ڵ忡 ���� ������ �ڵ���� �Ѵ�.
// �Լ� appendRecord()�� ���������� writeRecord() �Լ��� ȣ���Ͽ� ���ڵ� ������ �ذ��Ѵ�.
// ���������� �����ϸ� '1'��, �׷��� ������ '0'�� �����Ѵ�.
//
int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email);

//
// �л� ���ڵ� ���Ͽ��� �˻� Ű���� �����ϴ� ���ڵ尡 �����ϴ����� sequential search ����� 
// ���� ã�Ƴ���, �̸� �����ϴ� ��� ���ڵ��� ������ ����Ѵ�. �˻� Ű�� �л� ���ڵ带 �����ϴ�
// � �ʵ嵵 �����ϴ�. ���������� readRecord() �Լ��� ȣ���Ͽ� sequential search�� �����Ѵ�.
// �˻� ����� ����� �� �ݵ�� printRecord() �Լ��� ����Ѵ�. (�ݵ�� ���Ѿ� �ϸ�, �׷���
// �ʴ� ��� ä�� ���α׷����� �ڵ������� Ʋ�� ������ �ν���)
//
void searchRecord(FILE *fp, enum FIELD f, char *keyval);
void printRecord(const STUDENT *s);

//
// ���ڵ��� �ʵ���� enum FIELD Ÿ���� ������ ��ȯ���� �ش�.
// ���� ���, ����ڰ� ������ ��ɾ��� ���ڷ� "NAME"�̶�� �ʵ���� ����Ͽ��ٸ� 
// ���α׷� ���ο��� �̸� NAME(=1)���� ��ȯ�� �ʿ伺�� ������, �̶� �� �Լ��� �̿��Ѵ�.
//
enum FIELD getFieldID(char *fieldname);





char fname[256];

/* functions */	// ��� file processing operation�� C library�� ����� ��

void main(int argc, char *argv[])
{
	FILE *fp;
	char temp[37], key[6], value[31];
	char* ptr;
	int size;

	if (argc < 4){
		fprintf(stderr, "Usage: %s -option[a/s] record_file_name ... \n", argv[0]);
		exit(1);
	}

	//file open
	strcpy(fname, argv[2]);
	if ((fp = fopen(argv[2], "a+"))==NULL){ 
		fprintf(stderr, "fopen() error for %s\n", argv[2]);
		exit(1);
	}

	if (!strcmp(argv[1], "-a")){ //append
		if (argc != 8){
			fprintf(stderr, "Usage: %s -a record_file_name \"field_value1\" \"filed_value2\" \"filed_value3\" \"filed_value4\" \"filed_value5\"\n", argv[0]);
			exit(1);
		}
		size = strlen(argv[3]);
		if (size>8 || size<1){
			fprintf(stderr, "sizeof id value must be same or less than 8\n");
			exit(1);
		}
		size = strlen(argv[4]);
		if (size>10 || size<1){
			fprintf(stderr, "sizeof name value must be same or less than 10\n");
			exit(1);	
		} 
		size = strlen(argv[5]);
		if (size>12 || size<1){
			fprintf(stderr, "sizeof dept value must be same or less than 12\n");
			exit(1);	
		} 
		size = strlen(argv[6]);
		if (size>30 || size<1){
			fprintf(stderr, "sizeof address value must be same or less than 30\n");
			exit(1);	
		} 
		size = strlen(argv[7]);
		if (size>20 || size<1){
			fprintf(stderr, "sizeof email value must be same or less than 20\n");
			exit(1);	
		}
		if (!appendRecord(fp, argv[3], argv[4], argv[5], argv[6], argv[7])){
			fprintf(stderr, "appendRecord() failed\n");
			exit(1);
		}
	}
	else if (!strcmp(argv[1], "-s")){ //search
		if (argc != 4){
			fprintf(stderr, "Usage: %s -a record_file_name \"field_name=field_value\"\n", argv[0]);
			exit(1);
		}
		strcpy(temp, argv[3]);
		ptr = strtok(temp, "=");
		strcpy(key, ptr);
		ptr = strtok(NULL, "=");
		strcpy(value, ptr);
		searchRecord(fp, getFieldID(key), value);
	}
	else{
		fprintf(stderr, "INPUT ERROR: option must be -a or -s\n");
		exit(1);
	}

	//file close
	fclose(fp);

}

void pack(char *recordbuf, const STUDENT *s){

	int size;
	size = strlen(s->id) + strlen(s->name) + strlen(s->dept) + strlen(s->addr) + strlen(s->email) + 5;

	strcpy(recordbuf, s->id);
	strcat(recordbuf, "#");
	
	strcat(recordbuf, s->name);
	strcat(recordbuf, "#");

	strcat(recordbuf, s->dept);
	strcat(recordbuf, "#");

	strcat(recordbuf, s->addr);
	strcat(recordbuf, "#");

	strcat(recordbuf, s->email);
	strcat(recordbuf, "#");

	//memset 0
	while (size<RECORD_SIZE){
		strcat(recordbuf, "0");
		size++;
	}

}

int writeRecord(FILE *fp, const STUDENT *s, int rrn){

	char recordbuf[RECORD_SIZE+1];

	pack(recordbuf, s);
	
	fseek(fp, HEADER_SIZE + rrn * RECORD_SIZE, SEEK_SET);

	if (fwrite(recordbuf, RECORD_SIZE, 1, fp)<0){
		fprintf(stderr, "fwrite error\n");
		return 0;
	}

	return 1;

}

int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email){

	char header[5] = "0000";
	char full_header[HEADER_SIZE+1] = {0,};
	char buf[BUFFER_SIZE+1], full_buf[HEADER_SIZE+BUFFER_SIZE+1] = {0,};
	int record_num, size, reserved = 4;

	const STUDENT s;
	strcpy((void*)s.id, id);
	strcpy((void*)s.name, name);
	strcpy((void*)s.dept, dept);
	strcpy((void*)s.addr, addr);
	strcpy((void*)s.email, email);

	//read header
	fseek(fp, 0, SEEK_SET);
	fread(header, 4, 1, fp);
	rewind(fp);
	
	record_num = atoi(header) + 1;
	sprintf(header, "%d", record_num);
	
	//make header (to (over)write)
	if ((size = 4 - strlen(header))<0){
		fprintf(stderr, "overflowed\n");
		return 0;
	}
	while (size--){
		strcat(full_header, "0");
	}
	strcat(full_header, header);
	while (reserved--){
		strcat(full_header, "0");
	}
	
	if (record_num==1){ //write
		if (fwrite(full_header, HEADER_SIZE, 1, fp)<0){
			fprintf(stderr, "fwrite header failed\n");
			return 0;
		}
	}
	else{ //overwrite

		fseek(fp, 0, SEEK_END);
		int len = ftell(fp) - HEADER_SIZE;

		fseek(fp, HEADER_SIZE, SEEK_SET);
		fread(buf, len, 1, fp);

		strcpy(full_buf, full_header);
		strcat(full_buf, buf);

		fclose(fp);
		fopen(fname, "w");
		fwrite(full_buf, HEADER_SIZE + len, 1, fp);
		
		fclose(fp);
		fopen(fname, "a+");
	}

	return writeRecord(fp, &s, record_num-1);
}

enum FIELD getFieldID(char *fieldname){
	
	enum FIELD f;

	if (!strcmp(fieldname, "ID"))
		f = 0;
	else if (!strcmp(fieldname, "NAME"))
		f = 1;
	else if (!strcmp(fieldname, "DEPT"))
		f = 2;
	else if (!strcmp(fieldname, "ADDR"))
		f = 3;
	else if (!strcmp(fieldname, "EMAIL"))
		f = 4;
	else{
		fprintf(stderr, "FIELD can only be ID, NAME, DEPT, ADDR, EMAIL\n");
		exit(1);
	}

	return f;

}

void printRecord(const STUDENT *s)
{
	printf("%s | %s | %s | %s | %s\n", s->id, s->name, s->dept, s->addr, s->email);
}

void unpack(const char *recordbuf, STUDENT *s){

	char buf[RECORD_SIZE+1];
	char* ptr;

	strcpy(buf, recordbuf);
	ptr = strtok(buf, "#");
	strcpy(s->id, ptr);

	ptr = strtok(NULL, "#");
	strcpy(s->name, ptr);

	ptr = strtok(NULL, "#");
	strcpy(s->dept, ptr);

	ptr = strtok(NULL, "#");
	strcpy(s->addr, ptr);

	ptr = strtok(NULL, "#");
	strcpy(s->email, ptr);
}

int readRecord(FILE *fp, STUDENT *s, int rrn){

	char recordbuf[RECORD_SIZE+1];
	fseek(fp, HEADER_SIZE + rrn * RECORD_SIZE, SEEK_SET);
	
	if (fread(recordbuf, RECORD_SIZE, 1, fp)==RECORD_SIZE){
		fprintf(stderr, "fread error\n");
		return 0;
	}

	unpack(recordbuf, s);

	return 1;
}

void searchRecord(FILE *fp, enum FIELD f, char *keyval){

	char header[HEADER_SIZE+1];
	int record_num, i, cmp;
	STUDENT s;

	fseek(fp, 0, SEEK_SET);
	fread(header, 4, 1, fp);
	rewind(fp);

	if ((record_num = atoi(header))<1){
		printf("No records in record file\n");
		exit(1);
	}
	

	for (i=0; i<record_num; i++){

		if (readRecord(fp, &s, i)){

			switch(f){
				case 0:
					cmp = strcmp(keyval, s.id);
					break;
				case 1:
					cmp = strcmp(keyval, s.name);
					break;
				case 2:
					cmp = strcmp(keyval, s.dept);
					break;
				case 3:
					cmp = strcmp(keyval, s.addr);
					break;
				case 4:
					cmp = strcmp(keyval, s.email);
					break;
			}

			if (!cmp)
				printRecord(&s);
		}
	}
}


