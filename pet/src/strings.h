// ====================================
//
// Common static petscii 'strings' used
// within the game. Longer text is stored
// in the on-disk strings.dat file, but
// these are things like on screen labels
// actions, etc.
//
// ====================================

// Functions for our 0xFF terminated PETSCII string arrays

void string_put(unsigned char x, unsigned char y, char *text);
unsigned short string_len(char *text);

// Simple strings used in cputsxy()

#define STRING_ENGINE_NAME				"olderscrolls"
#define STRING_SIMPLE_SCREEN			"screen error!\n"
#define STRING_SIMPLE_40COL				"only 40x25 screens are supported"
#define STRING_SIMPLE_OK				"ok"
#define STRING_SIMPLE_ERROR				"error"
#define STRING_DATAFILE_ERROR			"error: opening datafile"
#define STRING_ANY_KEY					"press any key!"
#define STRING_FILE_NOT_FOUND			"error: file not found"
#define STRING_DEVICE_NOT_FOUND	 		"error: device not found"
#define STRING_FILEHANDLE_ERROR 		"error: filehandle"
#define STRING_FILENAME_ERROR			"error: filename"
#define STRING_DATAFILE_READ_ERROR		"error: reading datafile"
#define STRING_DATAFILE_ITEM_ERROR		"error: datafile item not found"
#define STRING_DATAFILE_OK				"ok: datafile read"