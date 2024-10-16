#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname; // to store source file name of beautiful.bmp
    FILE *fptr_src_image;  // file pointer to store address of beautiful.bmp
    uint bits_per_pixel; 
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname; // to store file name of secret.txt
    FILE *fptr_secret;  // file pointer to store address of secret.txt
    char extn_secret_file[MAX_FILE_SUFFIX]; // to store the extention of the secret file 
    char secret_data[MAX_SECRET_BUF_SIZE]; 
    long size_secret_file; // to store the size of the secret file 
    long extn_size;

} DecodeInfo;


/* Encoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string( char *magic_string, DecodeInfo *decInfo);

/* Decode  lsb to size*/
Status decode_lsb_to_size( long *size , char* image_buffer );

/* Decode secret file extenstion */
Status decode_secret_file_extn_size( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn( DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data( DecodeInfo *decInfo);

/* Decode function, which does the real encoding */
Status decode_image_to_data(char *data, int size, FILE *fptr_src_image, FILE *fptr_secret);

/* Decode a byte into LSB of image data array */
Status decode_lsb_to_bytes(char *data, char *image_buffer);

#endif