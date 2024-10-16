#include "common.h"

extern int mess_index;
extern int size;

Status do_encoding(EncodeInfo *encInfo) {

    strcpy(messages[mess_index++], "\033[3:37m-------------------  Varifying the data  -------------------\033[0m");

    if ( open_files( encInfo ) == e_failure )
        return e_failure;
            
    if ( check_capacity( encInfo ) == e_failure )
        return e_failure;

    strcpy(messages[mess_index++], "\033[3:37m---------------  Encoding Procedure Started  ---------------\033[0m");
    
    if ( copy_bmp_header( encInfo->fptr_src_image, encInfo->fptr_stego_image ) == e_failure )
        return e_failure;

    if ( encode_magic_string( MAGIC_STRING, encInfo ) == e_failure )
        return e_failure;

    if ( encode_secret_file_extn_size( encInfo->extn_secret_size, encInfo) == e_failure )
        return e_failure; 

    if ( encode_secret_file_extn( encInfo->extn_secret_file , encInfo) == e_failure )
        return e_failure;

    if ( encode_secret_file_size( encInfo->size_secret_file, encInfo) == e_failure )
        return e_failure;

    if ( encode_secret_file_data( encInfo ) == e_failure )
        return e_failure;

    if ( copy_remaining_img_data( encInfo->fptr_src_image, encInfo->fptr_stego_image ) == e_failure )
        return e_failure;

    strcpy(messages[mess_index++], "\033[3:37m--------------  Encoding Procedure Completed  --------------\033[0m");

    return e_success;
}

uint get_image_size_for_bmp( EncodeInfo *encInfo )
{
    uint width, height;
    // Seek to 18th byte
    fseek(encInfo->fptr_src_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, encInfo->fptr_src_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, encInfo->fptr_src_image);

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: %s size is %ld Bytes", encInfo->src_image_fname,(long int)height * width * 3 );

    // Return image capacity
    return width * height * 3;
}

Status open_files(EncodeInfo *encInfo)
{

    strcpy(messages[mess_index++], "INFO: Accessing the files....");
    strcpy(messages[mess_index++], "INFO: Opening all the necessary files....");

    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "fopen: %s", strerror(errno));
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "ERROR: Unable to open file %s", encInfo->src_image_fname);
    	return e_failure;
    } else snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Opened %s", encInfo->src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "fopen: %s", strerror(errno));
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "ERROR: Unable to open file %s", encInfo->secret_fname);
    	return e_failure;
    } else snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Opened %s", encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "fopen: %s", strerror(errno));
        sprintf(messages[mess_index++], "ERROR: Unable to open file %s\n", encInfo->stego_image_fname );
    	return e_failure;
    } else snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Opened %s", encInfo->stego_image_fname);

    strcpy(messages[mess_index++], "INFO: All files successfully opened");

    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) {

    strcpy(messages[mess_index++], "INFO: Varifying file names....");

    /* checking .bmp source file */
    if ( argv[2] != NULL && find_extention( argv[2] ) == e_success ) { 
        encInfo->src_image_fname = argv[2];
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Valid extention file %s found", argv[2] );

        /* checking for .txt text file */
        if ( argv[3] != NULL && find_extention( argv[3] ) == e_success ) {

            encInfo->secret_fname = argv[3];
            strcpy( encInfo->extn_secret_file, ext_after_dot ); //ext_after_dot found in find_extention() function
            encInfo->extn_secret_size = size;
            snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Valid extention file %s found", argv[3] );

        } else {
            strcpy(messages[mess_index++], "ERROR: Secret file with Ivalid extention found");
            strcpy(messages[mess_index++], "ERROR: Failure in Validation");
            return e_failure;
        }

        /* checking for .bmp output image file name*/
        if ( argv[4] != NULL ) {

            if (find_extention( argv[4] ) == e_success) {

                encInfo->stego_image_fname = argv[4];
                snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Valid extention file %s found", argv[4]);

            } else {
                strcpy(messages[mess_index++], "ERROR: Output image file with Ivalid extention found");
                strcpy(messages[mess_index++], "ERROR: Failure in Validation");
                return e_failure;
            }

        } else {
            strcpy(messages[mess_index++], "INFO: Creating output file as default");
            find_extention( argv[2] ); //finding soure file extention
            strcat( default_file_name, ext_after_dot); //merging both name and extention
            encInfo->stego_image_fname = default_file_name; //saving the name
            snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Created %s", encInfo->stego_image_fname );

        }
        strcpy(messages[mess_index++], "INFO: Validation of file names completed");
        return e_success;
    }
    else strcpy(messages[mess_index++], "ERROR: Source image file with Ivalid extention found");

    strcpy(messages[mess_index++], "ERROR: Failure in Validation");
    return e_failure;
}

Status check_capacity(EncodeInfo *encInfo) {

    //find the image size
    encInfo->image_capacity = get_image_size_for_bmp( encInfo );
    encInfo->size_secret_file = get_file_size( encInfo );

    //check the capacity of fit secret messege in the bmp file
    // source image size > 54 + ( sizeof(int) +sizeof(magic string) + sizeof(int) + sizeof(.txt file) ) * 8 
    if ( encInfo->image_capacity > HEADER_DATA + ( ( sizeof( MAGIC_STRING ) * BITS ) + ( sizeof(encInfo->extn_secret_file) * BITS ) + ( sizeof(encInfo->size_secret_file) * BITS ) + (encInfo->size_secret_file * BITS ) ) ) {
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: %s size enough to handel %s data", encInfo->src_image_fname, encInfo->secret_fname );
        return e_success;
    }
    else {
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "ERROR: %s size enough not to handel %s data", encInfo->src_image_fname, encInfo->secret_fname );
        return e_failure;
    }
}

uint get_file_size( EncodeInfo *encInfo ) {

    /* moving curser to EOF */
    fseek(encInfo->fptr_secret, 0, SEEK_END);

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: %s size is %ld Bytes", encInfo->secret_fname, ftell(encInfo->fptr_secret) );

    return ftell(encInfo->fptr_secret);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image) {

    strcpy(messages[mess_index++], "INFO: Copying Image Header....");

    //declare array of size HEADER_DATA
    char *buffer = malloc( HEADER_DATA * sizeof(char) );

    if (buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    //reset the cursor to 0 or rewind the fptr_src_image
    rewind(fptr_src_image);

    //read the first 54bytes from fptr_src_image to buffer
    if (fread(buffer, sizeof(char), HEADER_DATA, fptr_src_image) != HEADER_DATA) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    //print the 54bytes from buffer fptr_to dest_image
    if (fwrite(buffer, sizeof(char), HEADER_DATA, fptr_dest_image) != HEADER_DATA) {
        strcpy(messages[mess_index++], "ERROR: Destination file write permission denied");
        return e_failure;
    }

    strcpy(messages[mess_index++], "INFO: Image header successfully copied");

    free(buffer);
    
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo) {

    strcpy(messages[mess_index++], "INFO: Encodeing Magic String Signature....");

    /* encoding magic string */
    if ( encode_data_to_image( magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image ) == e_failure )
        return e_failure;

    strcpy(messages[mess_index++], "INFO: Magic String Signature successfully encoded");

    return e_success;

}

Status encode_byte_to_lsb(char data, char *image_buffer) {

    data = data + MAGIC_PASSWORD; //additional encription

    // get  the MSB bit from the data 
    for ( j = 0; j < BITS; j++ ) {
        //clear image_buffer's LSB bits
        image_buffer[j] = image_buffer[j] & ( (unsigned int ) ~1 ); 
        // store it to LSB of the image_buffer
        image_buffer[j] = image_buffer[j] | ( 1 & ( data >> ( BITS - j - 1) ) );
    }

}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image) {
    
    char *image_buffer = malloc(MAX_IMAGE_BUF_SIZE * sizeof(char));

    if (image_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    // run loop upto size times
    for ( i = 0; i < size; i++ ) {

        //read the 8 bytes from source image
        if ( fread( image_buffer, sizeof(char), MAX_IMAGE_BUF_SIZE, fptr_src_image) != MAX_IMAGE_BUF_SIZE ) {
            strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
            return e_failure;
        }

        //call byte_to_lsb function*( data[0], image_buffer)
        encode_byte_to_lsb( data[i], image_buffer );

        //write a 8 bytes to stego_image
        if ( fwrite( image_buffer, sizeof(char), MAX_IMAGE_BUF_SIZE, fptr_stego_image ) != MAX_IMAGE_BUF_SIZE ) {
            strcpy(messages[mess_index++], "ERROR: Destination file write permission denied");
            return e_failure;
        }
    }

    free(image_buffer);

    return e_success;
}

Status encode_secret_file_extn_size( long extn_size, EncodeInfo *encInfo) {
    
    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Encoding %s size....", encInfo->extn_secret_file );

    char *image_buffer = malloc(BITS * sizeof(int) * sizeof(char));

    if (image_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    //read the first 4 bytes data from sorce image
    if (fread(image_buffer, sizeof(char), BITS * sizeof(int), encInfo->fptr_src_image) != BITS * sizeof(int)) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    //encode the size of the extention
    encode_size_to_lsb( extn_size, image_buffer );

    //writing 4 byte to the output image
    if (fwrite( image_buffer, sizeof(char), BITS * sizeof(int), encInfo->fptr_stego_image ) != BITS * sizeof(int)) {
        strcpy(messages[mess_index++], "ERROR: Destination file write permission denied");
        return e_failure;
    }

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully encoded %s size", encInfo->extn_secret_file );

    free(image_buffer);
    return e_success;

}

Status encode_size_to_lsb( int data, char* image_buffer ) {

    data = data + MAGIC_PASSWORD; //additional encription
        // get  the MSB bit from the data 
    for ( j = 0; j < BITS * sizeof(int); j++ ) {
        //clear image_buffer's LSB bits
        image_buffer[j] = image_buffer[j] & ( (unsigned int ) ~1 ); 
        // store it to LSB of the image_buffer
        image_buffer[j] = image_buffer[j] | ( 1 & ( data >> ( BITS * sizeof(int) - j - 1) ) );
    }
    
}

Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo) {

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Encoding %s extention....", file_extn );

    /* encoding extention .txt size i.e. 4*/
    if ( encode_data_to_image( file_extn, encInfo->extn_secret_size, encInfo->fptr_src_image, encInfo->fptr_stego_image ) == e_failure )
        return e_failure;

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully encoded %s extention", file_extn );

    return e_success;   
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo) {

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Encoding %s file size....", encInfo->secret_fname );

    char *image_buffer = malloc(BITS * sizeof(int) * sizeof(char));

    if (image_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    //read the first 4 bytes data from sorce image
    if (fread(image_buffer, sizeof(char), BITS * sizeof(int), encInfo->fptr_src_image) != BITS * sizeof(int)) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    /* converting size to lsb*/
    encode_size_to_lsb( file_size, image_buffer );

    //writing 4 byte to the output image
    if (fwrite( image_buffer, sizeof(char), BITS * sizeof(int), encInfo->fptr_stego_image ) != BITS * sizeof(int)) {
        strcpy(messages[mess_index++], "ERROR: Destination file write permission denied");
        return e_failure;
    }

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully encoded %s file size", encInfo->secret_fname );

    free(image_buffer);

    return e_success;  
}

Status encode_secret_file_data(EncodeInfo *encInfo) {

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Encoding %s file Data....", encInfo->secret_fname );

    char *secret_buffer = malloc(encInfo->size_secret_file * sizeof(char) );

    if (secret_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    /* geting cursure back to 0th position*/
    rewind(encInfo->fptr_secret);
    
    /* copying the .txt file data to buffer */
    if (fread( secret_buffer, sizeof(char), encInfo->size_secret_file, encInfo->fptr_secret) != encInfo->size_secret_file) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    /* encoding the data*/
    encode_data_to_image(secret_buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully encoded %s file data", encInfo->secret_fname );

    free(secret_buffer);

    return e_success;  
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) {

    strcpy(messages[mess_index++], "INFO: Copying left over data....");

    char buffer;

    /* copying the left over data*/
    while ( fread( &buffer, sizeof(char), MAX_SECRET_BUF_SIZE, fptr_src) == MAX_SECRET_BUF_SIZE ) {
        fwrite( &buffer, sizeof(char), MAX_SECRET_BUF_SIZE, fptr_dest);
    }

    strcpy(messages[mess_index++], "INFO: Successfully codied remainig left over data");

}