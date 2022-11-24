#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	FILE* fp;
	int offset;
	int byte, abs_byte;
	long fSize, front, rear;
	char* buf1, *buf2;

	//arg err handling
	if (argc!=4){
		fprintf(stderr, "Usage: %s offset bytes filename\n", argv[0]);
		exit(1);
	}

	offset = atoi(argv[1]);
	byte = atoi(argv[2]);

	if (byte>0){
		abs_byte = byte;
	}
	else if (byte<0){
		abs_byte = byte*-1;
	} 
	else if (byte==0){
		return 0;
	}

	//fopen
	if ((fp = fopen(argv[3], "r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[3]);
		exit(1);
	}

	//fseek & fread
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	rewind(fp);

	front = offset + byte;
	if (front>fSize || front<0)
		return 0;
	rear = fSize - front - abs_byte;
	buf1 = (char*) malloc (sizeof(char) * front);
	buf2 = (char*) malloc (sizeof(char) * rear);

	fseek(fp, 0, SEEK_SET);
	fread(buf1, sizeof(char), front, fp);
	fseek(fp, offset+byte+abs_byte, SEEK_SET);
	fread(buf2, sizeof(char), rear, fp);

	//freopen
	if ((fp = freopen(argv[3], "w", fp))==NULL){
		fprintf(stderr, "freopen error for %s\n", argv[3]);
		exit(1);
	}

	fputs(buf1, fp);
	fputs(buf2, fp);

	free(buf1);
	free(buf2);
	fclose(fp);
	return 0;
}
