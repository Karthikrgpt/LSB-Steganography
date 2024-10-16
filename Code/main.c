#include "common.h"

int mess_index = 0;
int size = 0;

int main( int argc, char *argv[] ) {

    /* checking for arguments */
    if ( argc >= 3 && argc <= 5 ) {

        /* checking operation type */
        if ( check_operation_type( argv[1] ) == e_encode ) {
            EncodeInfo e_info;
            if ( read_and_validate_encode_args( argv , &e_info ) == e_success ) {
                do_encoding( &e_info );
            }

        /* decode procedure */
        } else {
            DecodeInfo d_info;
            if ( read_and_validate_decode_args( argv , &d_info ) == e_success ) {
                do_decoding( &d_info );
            }
        }
        
        /* printing the messeges */
        loading_anime();

    } else {

        /* if arguments are invalid then promte the messege */
        print_header( e_unsupported );
        printf("ERROR: Invalid operation requested....\nERROR: Encoding - minimum 4 arguments required\nERROR: Decoding - minimum 3 arguments required\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv ) {

    /* checking for encoding */
    if  ( !strcasecmp( argv, "-e") ) {
        print_header( e_encode );
        strcpy(messages[mess_index++], "INFO: Encoding operation requested....");
        return e_encode;

    /* checking for decoding */
    } else if ( !strcasecmp( argv, "-d") ) {
        print_header( e_decode );
        strcpy(messages[mess_index++], "INFO: Decoding operation requested....");
        return e_decode;

    /* if both are not  */
    } else {
        print_header( e_unsupported );
        printf("ERROR: Invalid operation requested....\n");
        exit(0);
    };
}


Status find_extention(char *ext) {

    if ( strrchr( ext , '.') == NULL ) //finding for extention
        return e_failure;

    strcpy( ext_after_dot, strrchr( ext , '.') );

    // Declare a 2D array of file extensions
    char extensions[][10] = { ".jpg", ".jpeg", ".png", ".bmp", ".mp4", ".mp3", ".txt",
                              ".doc", ".docx", ".pdf", ".xls", ".xlsx", ".ppt", ".pptx",
                              ".zip", ".rar", ".html", ".htm", ".css", ".js", ".php",
                              ".exe", ".py", ".c", ".h", ".sql", ".bin", ".log", ".mpeg"};

    // Loop through the extensions array and compare
    for (int i = 0; i < sizeof(extensions) / sizeof(extensions[0]); i++) {
        if (strcmp(ext_after_dot, extensions[i]) == 0) {
            size = strlen(ext_after_dot);  // Assign the length of the extension to 'size'
            return e_success;  // Return success if a match is found
        }
    }

    return e_failure;
}


/* printing header */
void print_header( OperationType comp ) {

    print_line();

    /* checking for operation */
    if ( comp == e_encode ) {
        printf("\t\t\033[3:37mSteganography : Encoding\033[0m\n");
    } else if ( comp == e_decode ) {
        printf("\t\t\033[3:37mSteganography : Decoding\033[0m\n");
    } else {
        printf("\t\t      \033[3:37mSteganography\033[0m\n");
    }
    print_line();
}

/* printing line */
void print_line() {
    for ( i = 0; i < 60; i++ )
        printf("-");
    printf("\n");
}

/* printing the messeges */
void loading_anime() {

    int progress, infoCounter = 0, total_mess = 0;

    char opt[10];

    if ( strstr( messages[0], "Encoding") != NULL ) //finding the operation
        strcpy( opt, "Encoding");
    else strcpy( opt, "Decoding");

    for ( i = 0; i < 50; i++) //finding how many messege need to print
        if ( strlen(messages[i] ) > 0)
            total_mess++;
        else break;

    int progressPerMessage = 44 / total_mess; // 44 is length of the loading anime, if we increse 44,  then length will incresd

    printf("\e[?25l"); //hide cursor

    for ( infoCounter = 0; infoCounter < total_mess; infoCounter++) {

            for ( i = 0; i <= progressPerMessage ; i++) { //creating anime loop

                printf("%s: [", opt );// prints loading messeage

                progress = (infoCounter * progressPerMessage) + i; // steps to print

                for ( j = 0; j < 44; j++) //defines the curser position
                    if (j < progress) 
                        printf("#");
                    else 
                        printf(" ");

                printf("] %d%%", (100 * progress) / 44 ); //printing percentage value
                fflush(stdout); 
                usleep(100000); //delay
                printf("\r\033[K"); //move to starting of line and clear
            }

            if ( strstr(messages[infoCounter], "ERROR") != NULL || strstr(messages[infoCounter], "fopen") != NULL ) { //finding for error messege
                print_red;
                printf("\r%s\n", messages[infoCounter]); // printing messege
                print_default;
            }
            else printf("\r%s\n", messages[infoCounter]); // printing messege
    }

    printf("\e[?25h"); //show cursor
}