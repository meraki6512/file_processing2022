#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	FILE* fp;
	int offset;
	char* data;
	long fSize, cur;
	char* buf;

	//arg err handling
	if (argc!=4){
		fprintf(stderr, "Usage: %s offset data filename\n", argv[0]);
		exit(1);
	}

	offset = atoi(argv[1]);
	data = argv[2];

	//fopen
	if ((fp = fopen(argv[3], "r+"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[3]);
		exit(1);
	}

	//fseek
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	rewind(fp);

	fseek(fp, offset+1, SEEK_SET);
	buf = (char*) malloc (sizeof(char)* fSize);
	fread(buf, sizeof(char), fSize, fp);
	rewind(fp);

	//fputs
	fseek(fp, offset+1, SEEK_SET);
	if (offset+1<fSize){
		fputs(data, fp);
		fputs(buf, fp);
	}
	else if (offset+1==fSize){
		fputs(data, fp);
	}

	free(buf);
	fclose(fp);
	return 0;
}
