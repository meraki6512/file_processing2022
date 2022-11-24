#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	FILE* fp1, *fp2, *fp3;
	long fSize1, fSize2;
	char* buf1, *buf2;
	size_t read1, read2;

	//arg err handling
	if (argc!=4){
		fprintf(stderr, "Usage: %s filename1 filename2 filename3\n", argv[0]);
		exit(1);
	}

	//fopen
	if ((fp1=fopen(argv[1], "r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	if ((fp2=fopen(argv[2], "r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[2]);
		exit(1);
	}

	if ((fp3=fopen(argv[3], "w"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[3]);
		exit(1);
	}

	//fseek
	fseek(fp1, 0, SEEK_END);
	fSize1 = ftell(fp1);
	rewind(fp1);

	fseek(fp2, 0, SEEK_END);
	fSize2 = ftell(fp2);
	rewind(fp2);

	//malloc
	if ((buf1 = (char*) malloc(sizeof(char) * fSize1))==NULL){
		fprintf(stderr, "malloc error\n");
		exit(1);
	}

	if ((buf2 = (char*) malloc(sizeof(char) * fSize2))==NULL){
		fprintf(stderr, "malloc error\n");
		exit(1);
	}

	//fread
	if ((read1 = fread(buf1, sizeof(char), fSize1,fp1)) != fSize1){
		fprintf(stderr, "fread error\n");
		exit(1);
	}

	if ((read2 = fread(buf2, sizeof(char), fSize2, fp2)) != fSize2){
		fprintf(stderr, "fread error\n");
		exit(1);
	}

	//fputs
	fputs(buf1, fp3);
	fputs(buf2, fp3);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

	return 0;
}
