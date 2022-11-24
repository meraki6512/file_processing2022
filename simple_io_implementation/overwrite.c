#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	FILE* fp;
	int offset;
	char* data;

	//arg err handling
	if (argc!=4){
		fprintf(stderr, "Usage: %s offset data filename", argv[0]);
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
	fseek(fp, offset, SEEK_SET);

	//fputc
	for (int i=0; data[i]!='\0'; i++){
		fputc((int)data[i], fp);
	}

	fclose(fp);
	return 0;
}
