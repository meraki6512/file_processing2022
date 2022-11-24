#include <stdio.h>
#include <stdlib.h>
#define READ_SIZE 10


int main(int argc, char* argv[]){

	FILE* org_fp;
	FILE* cp_fp;
	char temp[READ_SIZE];
	size_t rw_size;

	//arg err handling
	if (argc!=3){
		fprintf(stderr, "Usage: %s orgFname cpFname\n", argv[0]);
		exit(1);
	}

	//fopen
	if ((org_fp = fopen(argv[1], "r"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	if ((cp_fp = fopen(argv[2], "w"))==NULL){
		fprintf(stderr, "fopen error for %s\n", argv[2]);
		exit(1);
	}

	while (!feof(org_fp)){
		//fread
		if ((rw_size = fread(temp, sizeof(char), READ_SIZE, org_fp))<0){
			fprintf(stderr, "fread error\n");
			exit(1);
		}

		//fwrite
		if (rw_size>0){
			if (fwrite(temp, sizeof(char), rw_size, cp_fp) != rw_size){
				fprintf(stderr, "fwrite error\n");
				exit(1);
			}
		}
	}

	fclose(org_fp);
	fclose(cp_fp);

	return 0;

}
