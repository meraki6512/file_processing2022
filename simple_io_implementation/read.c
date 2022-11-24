#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	FILE* fp;
	int offset;
	int byte;
	int byte_abs;
	char* temp;

	//arg err hanling
	if (argc!=4){
		fprintf(stderr, "Usage: %s offset bytes filename\n", argv[0]);
		exit(1);
	}

	offset = atoi(argv[1]);
	byte = atoi(argv[2]);

	//fopen
	if ((fp = fopen(argv[3], "r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[3]);
		exit(1);
	}

	//fseek & fread
	if (byte!=0){
		fseek(fp, offset+byte+1, SEEK_SET);

		if (byte>0){
			byte_abs = byte;
		}
		else if (byte<0){
			byte_abs = byte * -1;
		}

		temp = (char*)malloc(sizeof(char)*byte_abs);
		fread(temp, sizeof(char), byte_abs, fp);
		printf("%s", temp);
	
		free(temp);
	}

	fclose(fp);
	return 0;
}
	
