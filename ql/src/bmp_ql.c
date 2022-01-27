/*
 bmp.c - a simple Windows BMP file loader
 based on the example at:
 https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <math.h>

#ifndef _BMP_H
#include "bmp_ql.h"
#define _BMP_H
#endif
#ifndef _UTIL_H
#include "../common/utils.h"
#define _UTIL_H
#endif

int div_ceil(int x, int y) {
    return x / y + (x % y > 0);
}

int bmp_ReadImage(FILE *bmp_image, bmpdata_t *bmpdata, unsigned char header, unsigned char data){
	/* 
		NOTE - This QL implementation is specific for the OlderScrolls rpg engine and ONLY
		supports 1bpp bitmap images. Any other format will return an error.
	
		bmp_image 	== open file handle to your bmp file
		bmpdata 	== a bmpdata_t struct
		header		== 1/0 to enable bmp header parsing
		data		== 1/0 to enable bmp pixel extraction (cannot do this without first parsing the header)
	
		Example use:
	
		FILE *f;
		bmpdata_t bmp = NULL;
		bmp = (bmpdata_t *) malloc(sizeof(bmpdata_t));
		bmp->pixels = NULL;
		
		f = fopen("file.bmp", "rb");
		bmp_ReadImage(f, bmp, 1, 1);
		bmp_Destroy(bmp);
		fclose(f);
	*/
	
	unsigned char *bmp_ptr;		// Represents which row of pixels we are reading at any time
	unsigned int i;				// A loop counter
	int	status;					// Generic status for calls from fread/fseek etc.

	if (header){
		// Seek to dataoffset position in header
		status = fseek(bmp_image, 0, 1);
		status = fseek(bmp_image, DATA_OFFSET_OFFSET, SEEK_SET);

		// Read data offset value
		status = fread(&bmpdata->offset, 4, 1, bmp_image);

		// offset is a little-endian 32bit, and Sinclair QL is big-endian, so swap it
		bmpdata->offset = swap_int32(bmpdata->offset);
		
		// Seek to image width/columns position in header
		status = fseek(bmp_image, WIDTH_OFFSET, SEEK_SET);

		// Read width value
		status = fread(&bmpdata->width, 4, 1, bmp_image);

		// width is a little-endian 32bit, and Sinclair QL is big-endian, so swap it
		bmpdata->width = swap_int32(bmpdata->width);
		
		// Seek to image height/rows position in header
		status = fseek(bmp_image, HEIGHT_OFFSET, SEEK_SET);

		// Read height value
		status = fread(&bmpdata->height, 4, 1, bmp_image);

		// Height is a little-endian 32bit,, and Sinclair QL is big-endian so swap it
		bmpdata->height = swap_int32(bmpdata->height);
		
		// Seek to bpp location in header
		status = fseek(bmp_image, BITS_PER_PIXEL_OFFSET, SEEK_SET);

		// Read bpp value
		status = fread(&bmpdata->bpp, 2, 1, bmp_image);

		// BPP is a little-endian 16bit, and Sinclair QL is big-endian, so swap it
		bmpdata->bpp = swap_int16(bmpdata->bpp);
		
		// Seek to compression field
		status = fseek(bmp_image, COMPRESS_OFFSET, SEEK_SET);
		
		// Read compression value
		status = fread(&bmpdata->compressed, sizeof(bmpdata->compressed), 1, bmp_image);
				
		// compression is a little-endian 32bit,, and Sinclair QL is big-endian so swap it
		bmpdata->compressed = swap_int32(bmpdata->compressed);
		if (bmpdata->compressed != BMP_UNCOMPRESSED){
			return BMP_ERR_COMPRESSED;
		}
				
		// Calculate the bytes needed to store a single pixel
		bmpdata->bytespp = (unsigned char) (bmpdata->bpp >> 3);
		
		// Rows are stored bottom-up
		
		if (bmpdata->bpp == BMP_1BPP){
			
			// Using quotient and remainder without libm
			
			// Each row is padded to be a multiple of 4 bytes == 32 pixels. 
			// We calculate the padded row size in bytes
			
			if (bmpdata->width % 4 != 0){
				bmpdata->row_padded = ((bmpdata->width / 4) + 1) / 8;
			} else {
				bmpdata->row_padded = (bmpdata->width / 4) / 8;
			}
			
			if (bmpdata->width % 8 != 0){
				bmpdata->row_unpadded = (bmpdata->width / 8) + 1;
			} else {
				bmpdata->row_unpadded = (bmpdata->width / 8);
			}

			if ((bmpdata->width * bmpdata->height) % 8 != 0){
				bmpdata->size = ((bmpdata->width * bmpdata->height) / 8) + 1;	
			} else {
				bmpdata->size = ((bmpdata->width * bmpdata->height) / 8);
			}
			bmpdata->n_pixels = bmpdata->size * 8;
		} else if (bmpdata->bpp == BMP_4BPP){
			
			// Each row is padded to be a multiple of 4 bytes == 8 pixels. 
			// We calculate the padded row size in bytes
			
			// Using quotient and remainder without libm
			if (bmpdata->width % 4 != 0){
				bmpdata->row_padded = ((bmpdata->width / 2) + 1);
			} else {
				bmpdata->row_padded = (bmpdata->width / 2);
			}
			
			if (bmpdata->width % 8 != 0){
				bmpdata->row_unpadded = (bmpdata->width / 2) + 1;
			} else {
				bmpdata->row_unpadded = (bmpdata->width / 2);
			}

			if ((bmpdata->width * bmpdata->height) % 2 != 0){
				bmpdata->size = ((bmpdata->width * bmpdata->height) / 2) + 1;	
			} else {
				bmpdata->size = ((bmpdata->width * bmpdata->height) / 2);
			}
			bmpdata->n_pixels = bmpdata->size * 2;
		} else {
			return BMP_ERR_BPP;
		}
	}
	
	if (data){
		
		// First verify if we've actually read the header section...
		if (bmpdata->offset <= 0){
			return BMP_ERR_READ;
		}
		
		// Allocate the total size of the pixel data in bytes		
		if (bmpdata->bpp == BMP_1BPP){
			bmpdata->pixels = (unsigned char*) calloc(bmpdata->size, 1);
		} else {
			bmpdata->pixels = (unsigned char*) calloc(bmpdata->n_pixels, bmpdata->bytespp);
		} 
		if (bmpdata->pixels == NULL){
			return BMP_ERR_MEM;
		}
	
		// Set the pixel buffer pointer to point to the very end of the buffer, minus the space for one row
		// We have to read the BMP data **backwards** into the buffer, as it is stored in the file bottom to top
		bmp_ptr = bmpdata->pixels + ((bmpdata->height - 1) * bmpdata->row_unpadded);
		
		// Seek to start of data section in file
		status = fseek(bmp_image, bmpdata->offset, SEEK_SET);
		
		// For every row in the image...
		for (i = 0; i < bmpdata->height; i++){		
			
			status = fread(bmp_ptr, 1, bmpdata->row_unpadded, bmp_image);
			if (status < 1){
				free(bmpdata->pixels);
				return BMP_ERR_READ;	
			}
			
			// Update pixel buffer position to the next row which we'll read next loop (from bottom to top)
			bmp_ptr -= bmpdata->row_unpadded;
			
			// Seek to next set of pixels for the next row if row_unpadded < row_padded
			if (status != bmpdata->row_unpadded){
				// Seek the number of bytes left in this row
				status = fseek(bmp_image, (bmpdata->row_padded - bmpdata->row_unpadded), SEEK_CUR);
				if (status != 0){
					free(bmpdata->pixels);
					return BMP_ERR_READ;
				}
			}
			// Else... the fread() already left us at the next row	
		}
		
		return BMP_OK;
	}
	return BMP_OK;
}

int bmp_ReadFont(FILE *bmp_image, bmpdata_t *bmpdata, fontdata_t *fontdata, unsigned char header, unsigned char data, unsigned char font_width, unsigned char font_height){
	// Read a font from disk - really a wrapper around the bitmap reader
	unsigned int h, w;
	unsigned char status = 0;
	unsigned int width_chars = 0;
	unsigned int height_chars = 0;
	unsigned int row_bytepos = 0;
	unsigned int bytepos = 0;		// position in bmp image data of the top left pixel of the current symbol
	unsigned int heightpos = 0;		// vertical offsetof into bmp image data
	unsigned int pos = 0;			// character/symbol position (0, 1, 2, ... 32)
	
	status = bmp_ReadImage(bmp_image, bmpdata, header, data);
	if (status == BMP_OK){
		fontdata->width = font_width;
		fontdata->height = font_height;
		if (data != 0){
			// Process BMP pixels to planar font array
			width_chars = bmpdata->width / font_width;
			height_chars = bmpdata->height / font_height;
			if (bmpdata->bpp == BMP_1BPP){			
				// For each WxH character in the bitmap image,
				// slice it by each row of bits and store each row as
				// a single byte for that row.
				// Since this is a 1bpp image, only store a single plane
				// of bytes.

				// For every row of symbols
				for(h = 0; h < height_chars; h++){
					// FOr every symbol in the row
					for(w = 0; w < width_chars; w++){
						// For every row of pixels in a symbol
						for(heightpos = 0; heightpos < font_height; heightpos++){
							row_bytepos = bytepos + (bmpdata->row_unpadded * heightpos);
							fontdata->symbol[pos][heightpos] = (unsigned char) bmpdata->pixels[row_bytepos];
						}
						// Jump to next symbol in row
						bytepos += 1;
						// Increment symbol number
						pos++;
					}
					// At end of every row of symbols, step by the height of one symbol row
					// so that we're at the top left pixel of the first symbol of the new row
					bytepos += (bmpdata->row_unpadded * (font_height - 1));
				}
				fontdata->n_symbols = pos;
				return BMP_OK;
			} else {
				// Unsupported bpp for font
				return BMP_ERR_BPP;
			}
			// No data returned from bmp data reader
		} else {
			return BMP_ERR_NODATA;
		}
	} else {
		// Error returned from parsing bmp header reader
		return status;	
	}	
}

void bmp_Destroy(bmpdata_t *bmpdata){
	// Destroy a bmpdata structure and free any memory allocated
	
	if (bmpdata->pixels != NULL){
		free(bmpdata->pixels);	
	}
	free(bmpdata);	
}

void bmp_Print(bmpdata_t *bmpdata){

	printf("Resolution: %u x %u\n", bmpdata->width, bmpdata->height);
	printf("Padded row size: %u bytes\n", bmpdata->row_padded);
	printf("Unpadded row size: %u bytes\n", bmpdata->row_unpadded);
	printf("Colour depth: %dbpp\n", bmpdata->bpp);
	printf("Storage size: %u bytes\n", bmpdata->size);
	printf("Pixels: %u\n", bmpdata->n_pixels);
	printf("Pixel data @ %x\n", (unsigned int) &bmpdata->pixels);
}