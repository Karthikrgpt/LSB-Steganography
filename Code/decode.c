#include "common.h"

extern int mess_index;
extern int size;
int flag_user_defined_name = 1;

Status do_decoding(DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "\033[3:37m-------------------  Varifying the data  -------------------\033[0m");

    if ( open_files_decode( decInfo ) == e_failure )
        return e_failure;

    strcpy(messages[mess_index++], "\033[3:37m---------------  Decoding Procedure Started  ---------------\033[0m");

    if ( decode_magic_string( MAGIC_STRING, decInfo ) == e_failure) 
        return e_failure;

    if ( decode_secret_file_extn_size( decInfo ) == e_failure )
        return e_failure;

    if ( decode_secret_file_extn( decInfo ) == e_failure )
        return e_failure;

    if ( decode_secret_file_size( decInfo ) == e_failure )
        return e_failure;

    if ( decode_secret_file_data( decInfo ) == e_failure )
        return e_failure;

    strcpy(messages[mess_index++], "\033[3:37m--------------  Decoding Procedure Completed  --------------\033[0m");

    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Varifying file names....");

    /* checking for .bmp stego file*/
    if ( argv[2] != NULL && find_extention( argv[2] ) == e_success ) { 

        decInfo->src_image_fname = argv[2];
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Valid extention file %s found", argv[2] );

        /* checking for .txt output file */
        if ( argv[3] != NULL ) {

            if ( find_extention( argv[3] ) == e_success ) {

                decInfo->secret_fname = argv[3];
                strcpy( decInfo->extn_secret_file, ext_after_dot );
                snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Valid extention file %s found", argv[3]);
                decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
                // Do Error handling
                if (decInfo->fptr_secret == NULL) {
                    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "fopen: %s", strerror(errno));
                    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "ERROR: Unable to open file %s", decInfo->secret_fname);
                	return e_failure;
                } else snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Opened %s", decInfo->secret_fname);

                flag_user_defined_name = 0; // to not provide additional defalut name

            } else {
                strcpy(messages[mess_index++], "ERROR: Source image file with Ivalid extention found" );
                strcpy(messages[mess_index++], "ERROR: Failure in Validation");
                return e_failure;
            }

        }
        strcpy(messages[mess_index++], "INFO: Validation of file names completed");
        return e_success;
    }
    else strcpy(messages[mess_index++], "ERROR: .bmp source image file not found");

    strcpy(messages[mess_index++], "ERROR: Failure in Validation");
    return e_failure;
}

Status open_files_decode(DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Accessing the files....");
    strcpy(messages[mess_index++], "INFO: Opening all the necessary files....");

    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "fopen: %s", strerror(errno));
        snprintf(messages[mess_index++], sizeof(messages[mess_index]), "ERROR: Unable to open file %s", decInfo->src_image_fname);
    	return e_failure;
    } else snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Opened %s", decInfo->src_image_fname);

    strcpy(messages[mess_index++], "INFO: All files successfully opened");

    return e_success;
}

Status decode_magic_string( char *magic_string, DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Decoding Magic string signature....");

    char *buffer = malloc( HEADER_DATA * sizeof(char) );

    if (buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    /* reading 54 header data*/
    if (fread(buffer, sizeof(char), HEADER_DATA, decInfo->fptr_src_image) != HEADER_DATA) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    int magic_size = sizeof(MAGIC_STRING) - 1; //finding size of magic string

    char *secret_buffer = malloc(magic_size * sizeof(char)); // dynamicaly allocating the memmory for secret_buffer

    if (secret_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    /* decoding the magic string*/
    if ( decode_image_to_data( secret_buffer, magic_size, decInfo->fptr_src_image, decInfo->fptr_secret ) == e_failure )
        return e_failure;

    /* camparing the decoded string and magic string*/
    if ( strcmp( MAGIC_STRING, secret_buffer ) != 0 ){
        strcpy(messages[mess_index++], "ERROR: Failed to decode magic string signature");
        return e_failure;
    }

    strcpy(messages[mess_index++], "INFO: Magic string signature successfully decoded");

    free(buffer);

    return e_success;

}

Status decode_image_to_data(char *data, int size, FILE *fptr_src_image, FILE *fptr_secret) {
 
    unsigned char *image_buffer = malloc(MAX_IMAGE_BUF_SIZE * sizeof(unsigned char));

    if (image_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    for (j = 0; j < size; j++) {

        /* reading 8 bytes of data*/
        if ( fread(image_buffer, sizeof(char), MAX_IMAGE_BUF_SIZE, fptr_src_image) != MAX_IMAGE_BUF_SIZE ) {
            strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
            return e_failure;
        }

        /* decoing the 8 bytes of data */
        decode_lsb_to_bytes( &data[j], image_buffer );

        data[j] = data[j] - MAGIC_PASSWORD; //additional decription

    }

    free(image_buffer);

    return e_success;
}

Status decode_lsb_to_bytes(char *data, char *image_buffer) {

    /* decoding 8 bytes of data to 1 byte character */
        for (uint i = 0; i < BITS; i++) {

            // geting lsb
            image_buffer[i] &= 0x01;
            // moving 1 bit left at each step
            (*data) <<= 1;
            // copying to image
            (*data) |= image_buffer[i];
        }

}

Status decode_secret_file_extn_size( DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Decoding secret file extention size.....");

    char *image_buffer = malloc(BITS * sizeof(int) * sizeof(char));

    if (image_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    //read the first 4 bytes data from sorce image
    if (fread(image_buffer, sizeof(char), BITS * sizeof(int), decInfo->fptr_src_image) != BITS * sizeof(int)) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    //encode the size of the extention
    decode_lsb_to_size( &decInfo->extn_size, image_buffer );

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully decoded extention size i.e. %ld Bytes", decInfo->extn_size );

    free(image_buffer);

    return e_success;

}

Status decode_lsb_to_size( long *size , char* image_buffer ) {

    unsigned long data = 0;

    for ( j = 0; j < BITS * sizeof(int); j++) {
        // Shift data left by 1 to make space for the new bit
        data = data << 1;
    
        // Get the bit from image_buffer[j] and add it to the LSB of data
        data = data | (image_buffer[j] & 1);  // Since each element is either 0 or 1
    }

    data = data - MAGIC_PASSWORD; //additional decription

    *size = data;
}

Status decode_secret_file_extn( DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Decoding secret file extention....");

    char *buffer = malloc( MAX_FILE_SUFFIX * sizeof(char) );

    if (buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    /* decoding .txt fiel extention*/
    if ( decode_image_to_data( buffer , decInfo->extn_size, decInfo->fptr_src_image, decInfo->fptr_secret ) == e_failure )
        return e_failure;

    for ( i = decInfo->extn_size; i < MAX_FILE_SUFFIX; i++ ){
        buffer[i] = '\0'; // replacing remaing with NULL character
    }

    if ( find_extention( buffer ) == e_success ) {

        /* checking if file is already creaded or not */
        if ( flag_user_defined_name ) {

            /* creating default output file*/
            strcat( default_file_name, ext_after_dot); //merging both name and extention 
            decInfo->secret_fname = default_file_name; //saving the name
            strcpy( decInfo->extn_secret_file, ext_after_dot ); //saving extention of file
            snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Creating output file by default: stego%s", decInfo->extn_secret_file );
            snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully created %s file", decInfo->secret_fname );

            /* opening the file with hidden extention*/
            decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");

            /* error handling*/
            if (decInfo->fptr_secret == NULL) {
                snprintf(messages[mess_index++], sizeof(messages[mess_index]), "fopen: %s", strerror(errno));
                snprintf(messages[mess_index++], sizeof(messages[mess_index]), "ERROR: Unable to open file %s", decInfo->secret_fname);
            	return e_failure;
            } else snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Opened %s", decInfo->secret_fname);

        } else {

            snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: File with extention %s provided", decInfo->extn_secret_file );
            if ( !strcmp( ext_after_dot, decInfo->extn_secret_file ) ) {
                strcpy(messages[mess_index++], "INFO: Hidden extention matched with user defined extention");
            } else {
                strcpy(messages[mess_index++], "ERROR: Failed to decode the extention ");
                return e_failure;  
            }
        }
    } else {
        strcpy(messages[mess_index++], "ERROR: Failed to decode the extention ");
        return e_failure;
    }

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully decoded %s extention", decInfo->extn_secret_file );

    free(buffer);

    return e_success;   
}

Status decode_secret_file_size( DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Decoding secret file size.....");

    char *image_buffer = malloc(BITS * sizeof(int) * sizeof(char));

    if (image_buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    //read the first 4 bytes data from sorce image
    if (fread(image_buffer, sizeof(char), BITS * sizeof(int), decInfo->fptr_src_image) != BITS * sizeof(int)) {
        strcpy(messages[mess_index++], "ERROR: Source file read permission denied");
        return e_failure;
    }

    //encode the size of the extention
    decode_lsb_to_size( &decInfo->size_secret_file, image_buffer );

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Successfully decoded secret file size i.e. %ld Bytes", decInfo->size_secret_file );

    free(image_buffer);

    return e_success;

}

Status decode_secret_file_data( DecodeInfo *decInfo) {

    strcpy(messages[mess_index++], "INFO: Decoding secret file data....");

    char *buffer = malloc(decInfo->size_secret_file * sizeof(char) );

    if (buffer == NULL) {
        strcpy(messages[mess_index++], "ERROR: Memory allocation failed!");
        return e_failure;
    }

    /* decoding secret file data*/
    if ( decode_image_to_data( buffer ,decInfo->size_secret_file, decInfo->fptr_src_image, decInfo->fptr_secret ) == e_failure )
        return e_failure;

    strcpy(messages[mess_index++], "INFO: Successfully decoded the secrect file data");

    /* writing to the output .txt file*/
    if ( fwrite( buffer, sizeof(char), decInfo->size_secret_file, decInfo->fptr_secret) != decInfo->size_secret_file ) {
        strcpy(messages[mess_index++], "ERROR: Failed to decode secret file data");
        return e_failure;
    }

    snprintf(messages[mess_index++], sizeof(messages[mess_index]), "INFO: Decoded data is copied to file %s", decInfo->secret_fname );

    free(buffer);

    return e_success;      
       
}