/*****************************************
* MODULE:                                 RGBTOGRAY 
*DESCRIPTION:                             Converts the data captured by btvid driver to grayscale
*ORIGINAL AUTHOR:                         Sam Siewert, CU Boulder*/


/*Function prototypes*/
typedef enum  color { Red = 1, Green = 2, Blue = 3} color;
extern color convC;
void rgbToGray(char*, char*, color convC);

