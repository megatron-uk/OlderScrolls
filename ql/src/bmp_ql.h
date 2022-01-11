/*
 bmp.h - C prototypes for the simple Windows BMP file loader
 based on the example at:
 https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/ 
*/

#define BMP_FILE_SIG_OFFSET	0x0000 // Should always be 'BM'
#define BMP_FILE_SIZE_OFFSET	0x0002 // Size of file, including headers
#define DATA_OFFSET_OFFSET 	0x000A // How many bytes from 0x0000 the data section starts
#define WIDTH_OFFSET 		0x0012 // Where we can find the x-axis pixel size
#define HEIGHT_OFFSET 		0x0016 // Where we can find the y-axis pixel size
#define BITS_PER_PIXEL_OFFSET	0x001C // Where we can find the bits-per-pixel number
#define COMPRESS_OFFSET		0x001E // Where we can find the compression flag
#define COLOUR_NUM_OFFSET	0x002E // Where we can find the numbers of colours used in the image
#define COLOUR_PRI_OFFSET	0x0032 // Where we can find the number of the 'important' colour ???
#define PALETTE_OFFSET		0x0036 // Where the colour palette starts, for <=8bpp images.
#define HEADER_SIZE 		14
#define INFO_HEADER_SIZE 	40
#define BMP_1BPP			1
#define BMP_8BPP			8	
#define BMP_16BPP			16
#define BMP_UNCOMPRESSED	0
#define BMP_VERBOSE			0 // Enable BMP specific debug/verbose output
#define BMP_OK				0 // BMP loaded and decode okay
#define BMP_ERR_NOFILE		-1 // Cannot find file
#define BMP_ERR_SIZE		-2 // Height/Width outside bounds
#define BMP_ERR_MEM			-3 // Unable to allocate memory
#define BMP_ERR_BPP			-4 // Unsupported colour depth/BPP
#define BMP_ERR_READ		-5 // Error reading or seeking within file
#define BMP_ERR_COMPRESSED	-6 // We dont support comrpessed BMP files
#define BMP_ERR_FONT_WIDTH	-7 // We dont support fonts of this width
#define BMP_ERR_FONT_HEIGHT	-8 // We dont support fonts of this height
#define BMP_ERR_NODATA		-9 // No data received from readImage

#define BMP_FONT_MAX_WIDTH	8
#define BMP_FONT_MAX_HEIGHT	8


// ============================
//
// BMP image data structure
//
// ============================
typedef struct bmpdata {
unsigned int 	width;			// X resolution in pixels
unsigned int 	height;			// Y resolution in pixels
unsigned char	compressed;		// If the data is compressed or not (usually RLE)
unsigned short	bpp;			// Bits per pixel
unsigned short 	bytespp;		// Bytes per pixel
unsigned int	offset;			// Offset from header to data section, in bytes
unsigned int 	row_padded;		// Size of a row without padding
unsigned int 	row_unpadded;	// SIze of a row, padded to a multiple of 4 bytes
unsigned int 	size;			// Size of the pixel data, in bytes
unsigned int	n_pixels;		// Number of pixels
unsigned char	*pixels;		// Pointer to raw pixels - in font mode each byte is a single character
} bmpdata_t;

// ============================
//
// Font data structure
//
// 96 characters, 
// each character is 1 byte wide, and up to 8 (max) rows high 
// each row is 1bpp
// total of 6144 bytes per 96 character font
//
//=============================
typedef struct fontdata {
unsigned char	width;			// Width of each character, in pixels
unsigned char	height;			// Height of each character, in pixels
unsigned char	ascii_start;	// ASCII number of symbol 0
unsigned char	n_symbols;		// Total number of symbols
unsigned char	unknown_symbol;	// Which symbol do we map to unknown/missing symbols?
unsigned char 	symbol[96][BMP_FONT_MAX_HEIGHT]; 
} fontdata_t;

void bmp_Destroy(bmpdata_t *bmpdata);
int bmp_ReadFont(FILE *bmp_image, bmpdata_t *bmpdata, fontdata_t *fontdata, unsigned char header, unsigned char data, unsigned char font_width, unsigned char font_height);
int bmp_ReadImage(FILE *bmp_image, bmpdata_t *bmpdata, unsigned char header, unsigned char data);