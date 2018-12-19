#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <sys/types.h>  

//userspace std::
using namespace std;

#define CLIP(X) ( (X) > 128 ? 255 : 0)
//#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)

int bmp_read(unsigned char *image, int, int, char *);
int bmp_write(unsigned char *image, int, int, char *);
int bmp_write_2bytes(unsigned char *image, int, int, char *);
int bmp_print(unsigned char *image, int, int);
int generate_test_img( char *);
int test_2bytes();

// for one bit
int bmp_write_bit( unsigned char *image, int, int, char * );


int main(int argc, char *argv[]) {
    unsigned char *image;
    int xsize = 128;
    int ysize = 128;
	char imageName[128] = "source";
	char outputFileName[128] = "output.txt";
	int mode = 0;

	if( argc == 6 ) {
		xsize = stoi( argv[1] );
		ysize = stoi( argv[2] );
		memcpy( imageName, argv[3] );
		memcpy( outputFileName, argv[4] );
		mode = stoi( argv[5] );

	} else {
		printf( "Not enough parameters\n" );
		printf( "./BMPtoRGB xSize ySize SourcePicNameWithoutBMP OutputFileName mode\n" );
		printf( "Mode:\n" );
		printf( "0 to RGB888, 3bytes\n" );
		printf( "1 to RGB565, 2bytes\n" );
		printf( "2 to Gray, 1bit per pixel\n" );
		printf( "3 to print original RGB to screen\n" );
	} // end if

    image = (unsigned char *)malloc((size_t)xsize * ysize * 3);
    if (image == NULL)  {
		printf( "Can't open image %s.bmp\n", imageName );
      return -1;
	} // end if
	if( bmp_read( image, xsize, ysize, imageName ) == 0) {
		//bmp_print( image, xsize, ysize );
		switch (mode) {
		case 0:
			printf( "Output RGB 3bytes\n" );
			bmp_write( image, xsize, ysize, outputFileName );
			break;
		case 1:
			printf( "Output RGB 2bytes\n" );
			bmp_write_2bytes(image, xsize, ysize, outputFileName);
			break;
		case 2:
			printf( "Output Gray 1bit per pixel\n" );
			bmp_write_bit(image, xsize, ysize, outputFileName);
			break;
		case 3:
			printf( "Print RGB, for testing, maybe not correct.\n" );
			bmp_print( image, xsize, ysize );
			break;
		default:
			printf( "Wrong mode" );
			break;

		} // end switch
		

		//bmp_write( image, xsize, ysize, "splash.txt" );
		//bmp_write_bit(image, xsize, ysize, "bit_72.txt");
	} // end if
	//generate_test_img("testimg.txt");
    /*  
    for(int y = 0; y != ysize; ++y) {
      for(int x = 0; x != xsize; ++x) {
        // set (R,G,B) = (255,0,0)
        // R
        *(image + 3 * (y * xsize + x) + 2) = 255;
        // G
        *(image + 3 * (y * xsize + x) + 1) = 0;
        // B
        *(image + 3 * (y * xsize + x) + 0) = 0;
      }
    }
    */
    // bmp_write(image, xsize, ysize, "onlyRed");
    
    free(image);
	system("pause");
}

int bmp_read(unsigned char *image, int xsize, int ysize, char *filename) {
    char fname_bmp[128];
    sprintf_s( fname_bmp, "%s.bmp", filename);
    
    FILE *fp;
	errno_t err;
    err = fopen_s(&fp, fname_bmp, "rb");
	if( err != 0 ) {
		printf( "Can't open %s\n", fname_bmp );
      return -1;
	} // end if
      
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fp);
    fread(image, sizeof(unsigned char), (size_t)(long)xsize * ysize * 3, fp);
    
    fclose(fp);
    return 0;
}
int bmp_write(unsigned char *image, int xsize, int ysize, char *filename) {

	char name[128];
    sprintf_s( name, "%s", filename);
    
    FILE *fp;
	errno_t err;
    err = fopen_s(&fp, name, "w");
	if( err != 0 )  {
		printf( "Can't open %s\n", name );
      return -1;
	} // end if
      
	fprintf_s(fp, "Copy and paste the following values to where you want\n");
	fprintf_s(fp, "-------------------------------------------------------------------\n");

	int w = 0;
	for(int y = ysize - 1; y >= 0; --y) {
      for(int x = 0; x != xsize; ++x) {
	
        // set (R,G,B) = (255,0,0)
        // B
        fprintf_s( fp, "0x%02x, ", *(image + 3 * (y * xsize + x) + 0));
        // G
        fprintf_s( fp, "0x%02x, ", *(image + 3 * (y * xsize + x) + 1));
        // R
        fprintf_s( fp, "0x%02x, ", *(image + 3 * (y * xsize + x) + 2));

		w++;
		if( w % 3 == 0 )
	      fprintf_s( fp, "\n");
	  } // end inner for
	  
    } // end outer for
	printf( "Output values to %s DONE\n", name );
	fclose(fp);

	return 0;
} 


int bmp_write_2bytes(unsigned char *image, int xsize, int ysize, char *filename) {

	char name[128];
    sprintf_s( name, "%s", filename);
    
    FILE *fp;
	errno_t err;
    err = fopen_s(&fp, name, "w");
	if( err != 0 )  {
		printf( "Can't open %s\n", name );
      return -1;
	} // end if
      
	fprintf_s(fp, "Copy and paste the following values to where you want\n");
	fprintf_s(fp, "-------------------------------------------------------------------\n");

	int w = 0;
	for(int y = ysize - 1; y >= 0; --y) {
      for(int x = 0; x != xsize; ++x) {
	
				unsigned char byteH = 0x00;
				unsigned char byteL = 0x00;
				unsigned char byteR = 0x00;
				unsigned char byteG = 0x00;
				unsigned char byteB = 0x00;
				byteB = (*(image + 3 * (y * xsize + x) + 2) + 1) / 8;
				if( byteB != 0 ) byteB -= 1;
	//			printf( "%d\n",  (*(image + 3 * (y * xsize + x) + 2) + 1) / 8);
				byteG = (*(image + 3 * (y * xsize + x) + 1) + 1) / 4;
				if( byteG != 0 ) byteG -= 1;
				byteR = (*(image + 3 * (y * xsize + x) + 0) + 1) / 8;
				if( byteR != 0 ) byteR -= 1;
		//		printf("R=%d, G=%d, B=%d\n ", byteR, byteG, byteB);

				byteH |= (byteG >> 3);
				byteH |= (byteB << 3);

				byteL |= (byteR);
				byteL |= (byteG << 5);

				// B
        //printf("0x%02x, ", *(image + 3 * (y * xsize + x) + 0));
        // G
        //printf("0x%02x, ", *(image + 3 * (y * xsize + x) + 1));
        // R
        //printf("0x%02x, ", *(image + 3 * (y * xsize + x) + 2));
				fprintf_s( fp, "0x%02x, 0x%02x, ", byteH, byteL);
		//printf( "[0x%02x, 0x%02x]\n", byteH, byteL );
				

		w++;
		if( w % 3 == 0 )
	      fprintf_s( fp, "\n");
	  } 
	  
    }
	printf( "Output values to %s DONE\n", name );
	fclose(fp);

	return 0;
} // end function bmp_write_2bytes

int bmp_print(unsigned char *image, int xsize, int ysize) {


	int w = 0;
	for(int y = ysize - 1; y >= 0; --y) {
      for(int x = 0; x != xsize; ++x) {
		//for(int y =  0; y < 5; ++y) {
			//for(int x = 0; x != 5; ++x) {
				unsigned char byteH = 0x00;
				unsigned char byteL = 0x00;
				unsigned char byteR = 0x00;
				unsigned char byteG = 0x00;
				unsigned char byteB = 0x00;
				byteR = (*(image + 3 * (y * xsize + x) + 2) + 1) / 8 - 1;
				byteG = (*(image + 3 * (y * xsize + x) + 1) + 1) / 4 - 1;
				byteB = (*(image + 3 * (y * xsize + x) + 0) + 1) / 8 - 1;
				printf("R=%d, G=%d, B=%d\n ", byteR, byteG, byteB);

				byteH |= (byteG >> 3);
				byteH |= (byteB << 3);

				byteL |= (byteR);
				byteL |= (byteG << 2);

				// B
        printf("0x%02x, ", *(image + 3 * (y * xsize + x) + 0));
        // G
        printf("0x%02x, ", *(image + 3 * (y * xsize + x) + 1));
        // R
        printf("0x%02x, ", *(image + 3 * (y * xsize + x) + 2));
				printf( "[0x%02x, 0x%02x]\n", byteH, byteL );
				
        // set (R,G,B) = (255,0,0)
        // B
        //printf("0x%x, ", *(image + 3 * (y * xsize + x) + 0));
        // G
        //printf("0x%x, ", *(image + 3 * (y * xsize + x) + 1));
        // R
        //printf("0x%x, ", *(image + 3 * (y * xsize + x) + 2));
		w++;
		if( w >= 3 )
	      printf("\n");
	  } 
	  
    }
	return 0;
} 

int generate_test_img( char *filename) {

	char name[128];
    sprintf_s( name, "%s", filename);
    
    FILE *fp;
	errno_t err;
    err = fopen_s(&fp, name, "w");
	if( err != 0 )  {
		printf( "Can't open %s\n", name );
      return -1;
	} // end if
      
	fprintf_s(fp, "Copy and paste the following values to where you want\n");
	fprintf_s(fp, "-------------------------------------------------------------------\n");

	int w = 0;
	for(int y = 250 - 1; y >= 0; --y) {
      for(int x = 0; x != 250; ++x) {
        // set (R,G,B) = (255,0,0)

		if( y >= 0 && y <= 100 ) {
        // B
			fprintf_s( fp, "0x%02x, ", 0);
        // G
			fprintf_s( fp, "0x%02x, ", 0);
        // R
			fprintf_s( fp, "0x%02x, ", 255);
		} else if( y > 100 && y < 200 ) {
			// B
			fprintf_s( fp, "0x%02x, ", 255);
        // G
			fprintf_s( fp, "0x%02x, ", 0);
        // R
			fprintf_s( fp, "0x%02x, ", 0);
		} else {
			// B
			fprintf_s( fp, "0x%02x, ", 0);
        // G
			fprintf_s( fp, "0x%02x, ", 255);
        // R
			fprintf_s( fp, "0x%02x, ", 0);
		} // end if
		w++;
		if( w % 3 == 0 )
	      fprintf_s( fp, "\n");
	  } 
	  
    }
	printf( "Output values to %s DONE\n", name );
	fclose(fp);

	return 0;
} // end function generate_test_img

int test_2bytes(){

	unsigned char byteH = 0x00;
	unsigned char byteL = 0x00;
	unsigned char byteR = 0x00;
	unsigned char byteG = 0x00;
	unsigned char byteB = 0x00;
	//byteR =  / 8;

	return 0;
} // end function test_2bytes()


int bmp_write_bit( unsigned char *image, int xsize, int ysize, char * filename ) {

	char name[128];
    sprintf_s( name, "%s", filename);
    
    FILE *fp;
	errno_t err;
    err = fopen_s(&fp, name, "w");
	if( err != 0 )  {
		printf( "Can't open %s\n", name );
      return -1;
	} // end if
      
	fprintf_s(fp, "Copy and paste the following values to your file\n");
	fprintf_s(fp, "-------------------------------------------------------------------\n");

	int w = 1;
	unsigned char value = 0;
	int debug_count = 0;
	int frameBufferCounter = 0;
	int shift_count = 0;
	unsigned char mask1 = 1, mask0 = 0;
	for(int y = ysize - 1; y >= 0; --y) {
      for(int x = 0; x < xsize; ++x) {
		  
		unsigned char byteY = RGB2Y(*(image + 3 * (y * xsize + x) + 2), 
                                                 *(image + 3 * (y * xsize + x) + 1), 
                                                 *(image + 3 * (y * xsize + x) + 0));
												 
                //frameBuffer[ /*x + y * s*/frameBufferCounter ] = 
                // bit operaction
		//  unsigned char byteY = *(image +  (y * xsize + x));
		  printf("%d,", *(image + 3 * (y * xsize + x)));
                if( byteY > 128 ) {
                    // 1
                    value = value | mask1;
                } else {
                    // value = value | mask0;
                } // end if
                shift_count++;
                if( shift_count >= 8 ) {
                    shift_count = 0;
                    //mask1 = 1;
                    debug_count++;
               //     frameBuffer[ /*x + y * s*/frameBufferCounter++ ] = value;
					fprintf_s( fp, "0x%02x, ", value);
                    value = 0;
					w++;
                } else {
                    value = value << 1;
                } // end if
		/*
				if( y==0 )  {
			printf("%d, %d, %d\n", *(image + 3 * (y * xsize + x) + 2), 
                                                 *(image + 3 * (y * xsize + x) + 1), 
                                                 *(image + 3 * (y * xsize + x) + 0));
		}
		*/
		if( w % 8 == 0 ) {
			fprintf_s( fp, "\n");
			printf("\n");
			w = 1;
		}
	  } 
	  
    }
	printf( "Output values to %s DONE\n", name );
	fclose(fp);
	return 0;
} // end function bmp_write_bit