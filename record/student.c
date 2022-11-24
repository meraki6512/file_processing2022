#include <stdio.h>		// 필요한 header file 추가 가능
#include <stdlib.h>
#include <string.h>
#include "student.h"

/* prototypes & comments */
//
// 함수 readRecord()는 학생 레코드 파일에서 주어진 rrn에 해당하는 레코드를 읽어서 
// recordbuf에 저장하고, 이후 unpack() 함수를 호출하여 학생 타입의 변수에 레코드의
// 각 필드값을 저장한다. 성공하면 1을 그렇지 않으면 0을 리턴한다.
// unpack() 함수는 recordbuf에 저장되어 있는 record에서 각 field를 추출하는 일을 한다.
//
int readRecord(FILE *fp, STUDENT *s, int rrn);
void unpack(const char *recordbuf, STUDENT *s);

//
// 함수 writeRecord()는 학생 레코드 파일에 주어진 rrn에 해당하는 위치에 recordbuf에 
// 저장되어 있는 레코드를 저장한다. 이전에 pack() 함수를 호출하여 recordbuf에 데이터를 채워 넣는다.
// 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int writeRecord(FILE *fp, const STUDENT *s, int rrn);
void pack(char *recordbuf, const STUDENT *s);

//
// 함수 appendRecord()는 학생 레코드 파일에 새로운 레코드를 append한다.
// 레코드 파일에 레코드가 하나도 존재하지 않는 경우 (첫 번째 append)는 header 레코드를
// 파일에 생성하고 첫 번째 레코드를 저장한다. 
// 당연히 레코드를 append를 할 때마다 header 레코드에 대한 수정이 뒤따라야 한다.
// 함수 appendRecord()는 내부적으로 writeRecord() 함수를 호출하여 레코드 저장을 해결한다.
// 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email);

//
// 학생 레코드 파일에서 검색 키값을 만족하는 레코드가 존재하는지를 sequential search 기법을 
// 통해 찾아내고, 이를 만족하는 모든 레코드의 내용을 출력한다. 검색 키는 학생 레코드를 구성하는
// 어떤 필드도 가능하다. 내부적으로 readRecord() 함수를 호출하여 sequential search를 수행한다.
// 검색 결과를 출력할 때 반드시 printRecord() 함수를 사용한다. (반드시 지켜야 하며, 그렇지
// 않는 경우 채점 프로그램에서 자동적으로 틀린 것으로 인식함)
//
void searchRecord(FILE *fp, enum FIELD f, char *keyval);
void printRecord(const STUDENT *s);

//
// 레코드의 필드명을 enum FIELD 타입의 값으로 변환시켜 준다.
// 예를 들면, 사용자가 수행한 명령어의 인자로 "NAME"이라는 필드명을 사용하였다면 
// 프로그램 내부에서 이를 NAME(=1)으로 변환할 필요성이 있으며, 이때 이 함수를 이용한다.
//
enum FIELD getFieldID(char *fieldname);





char fname[256];

/* functions */	// 모든 file processing operation은 C library를 사용할 것

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


