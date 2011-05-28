/*
 * read_texture() - read in an image file in SGI 'libimage' format
 * 	currently its very simple minded and converts all images
 *      to RGBA8 regardless of the input format and returns the
 *	original number of components in the appropriate parameter.
 *    
 *     
 *	the components are converted as follows
 *		L    -> LLL 1.0
 *		LA   -> LLL A
 *		RGB  -> RGB 1.0
 *		RGBA -> RGB A
 *
 * read_image() - read in an image file in SGI 'libimage' format
 *      same as above, but doesn't do any conversion to RGBA8.
 *
 */

unsigned char *
read_texture(char *name, int *width, int *height, int *components);

unsigned char *
read_sgi(char *name, int *width, int *height, int *components);
