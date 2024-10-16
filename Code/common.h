#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5
#define BITS 8
#define HEADER_DATA 54

#include "types.h"
#include "encode.h"
#include "decode.h"

/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "-7$y2W~#8b@V!n9)p"
#define MAGIC_PASSWORD '`'

#define print_red printf("\033[0;31m")
#define print_default printf("\033[0m")

/* globle declaretion */
char messages[50][100];
char ext_after_dot[MAX_FILE_SUFFIX];
static char default_file_name[20] = "stego";
static unsigned int i = 0, j = 0;

/* basic function prototype */
void loading_anime();
//void print_mess( char *mess );
void print_line();
void print_header( OperationType comp );

Status find_extention( char *ext );

#endif
