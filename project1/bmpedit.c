#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma pack (1)


typedef struct{
	unsigned short identifier;
	unsigned int fileSize;
	unsigned short reserve1;
	unsigned short reserve2;
	unsigned int pixelStart;
}FileHeader;


typedef struct{
	unsigned int headerSize;
	unsigned int imgWidth;
	unsigned int imgHeight;
	unsigned short numClrPanels;
	unsigned short bitsPerPixel;
	unsigned int cmpressScheme;
	unsigned int imgSize;
	unsigned int hRes;
	unsigned int vRes;
	unsigned int numClrPal;
	unsigned int numImpClr;
}DIBHeader;

typedef struct{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}Pixel;



static unsigned short fileTypeID = 0x4d42;


int main(int argc,char** argv){
	int transformType = -1;
	FileHeader fileHeader = {0};
	DIBHeader dibHeader = {0};
	
	if((strcmp(argv[1],"-grayscale"))!=0 &&  (strcmp(argv[1],"-invert"))!=0){
		printf("Please use either -grayscale or -invert before specifying the file name\n");
		return -1;
	}else{
		if(strcmp(argv[1],"-grayscale")==0){
			transformType = 0;
		}
		if(strcmp(argv[1],"-invert")==0){
			transformType = 1;
		}
	}
	

	FILE* image = fopen(argv[2],"rb+");
	if(image == NULL){
		printf("Can't find file specified, make sure you have typed in the name & path correctly\n");
	}

	//Check IF Magic ID = 'BM'
	fseek(image,0,SEEK_SET);	
	fread(&fileHeader,sizeof(fileHeader),1,image);
	if(fileHeader.identifier != fileTypeID){
		fclose(image);
		printf("File Type is not supported by program, exiting now\n");
		return -1;
	}
	
	fread(&dibHeader,sizeof(dibHeader),1,image);
	
	if(dibHeader.headerSize != 40){
		printf("File Type not supported, exiting now\n");
		fclose(image);
		return -1;
	}

	fseek(image,fileHeader.pixelStart,SEEK_SET);	
	if(dibHeader.bitsPerPixel != 24){
		printf("Bits Per Pixel value incorrect/not supported, exiting now\n");
		fclose(image);
		return -1;
	}
	if(transformType == 1){
		Pixel current;
		int x,y;
		for(y= 0;y<dibHeader.imgHeight;y++){
			for(x=0;x<dibHeader.imgWidth;x++){
				fread(&current,sizeof(current),1,image);
				current.b = ~current.b;
				current.g = ~current.g;
				current.r = ~current.r;
				fseek(image,-3,SEEK_CUR);
				fwrite(&current,sizeof(current),1,image);
			}
			if((dibHeader.imgWidth*3)%4!=0){
				int skip = 4-((dibHeader.imgWidth*3)%4);
				fseek(image,skip,SEEK_CUR);
			}
		}

	}else if(transformType == 0){
		printf("Grayscale\n");
		int x,y;
		Pixel current;
		for(y = 0;y<dibHeader.imgHeight;y++){
			for(x=0;x<dibHeader.imgWidth;x++){
				fread(&current,sizeof(current),1,image);
				double blue = (double)current.b/255;
				double green = (double)current.g/255;
				double red = (double)current.r/255;
				double Y = 0.2126*red+0.7152*green+0.0722*blue;
				if(Y <= 0.00313308){
					double transform = 12.92*Y;
					blue = transform;
					green = transform;
					red = transform;
				}else{
					double power = 1.0/2.4;
					double transform = 1.055*pow(Y,power)-0.055;
					blue = transform;
					green = transform;
					red = transform;
					if(y==0&&x==0){
						printf("%f",transform);
					}
				}
				current.b = (unsigned char)(blue*255.0);
				current.g = (unsigned char)(green*255.0);
				current.r = (unsigned char)(red*255.0);
				fseek(image,-3,SEEK_CUR);
				fwrite(&current,sizeof(current),1,image);
			}
			if((dibHeader.imgWidth*3)%4!=0){
				int skip = 4-((dibHeader.imgWidth*3)%4);
				fseek(image,skip,SEEK_CUR);
			}
		}		
	}
	
	
	fclose(image);
	return 0;
}
