#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Double the size of the char array if it fills up using realloc
int main(int argc,char** argcv){
	FILE* file;
	file = fopen(argcv[1],"rb");
	if(file == NULL){
		printf("File specified can not be found.\nMake sure it is the first argument passed\n");
	}
	char* current = (char*)malloc(sizeof(char)*100);
	unsigned char next;
	int max = 100;
	while(fgetc(file) != EOF){
		fseek(file,-1,SEEK_CUR);
		next = fgetc(file);
		if(next >= 32 && next <= 126){
			if(*current == '\0'){
				*current = next;
			}else{
				if(strlen(current) > max){
					current = realloc(current,max*2);
					max = max*2;
				}
				strncat(current,&next,1);
			}

		}else if(*current != '\0'){
			if(strlen(current) >= 4){
				printf("%s\n",current);
			}
			current = (char*)calloc(100,sizeof(char));
		}
	}
	free(current);
	fclose(file);
	return 0;

}
