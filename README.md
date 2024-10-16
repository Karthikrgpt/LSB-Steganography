------------------------------------------------------------------------------------------------------------
                                                SETGANOGRAPHY
------------------------------------------------------------------------------------------------------------
Created by "Karthik R."
Last updated on 10th October 2024
------------------------------------------------------------------------------------------------------------

Description :-

  LSB (Least Significant Bit) Steganography is a technique used to hide secret information within digital
images, audio files, or videos. It operates by modifying the least significant bit (the smallest unit of 
information) in each byte of the cover media. Because the LSB is the part of the data that causes the least 
visible or audible change, it’s an ideal place for concealing data without affecting the quality of the media 
noticeably.
 
Here’s a breakdown to its key features :-

1. Encoding:

    -> The -e command is used to encoded secret data (such as text or another file) into a cover file (like an 
       image, audio, or video). The process involves altering the least significant bits (LSBs) or some other 
       part of the media to hide the information.
    -> Usage Example:
        -> If you want to embed a secret message into an image file:

                gcc *.c -o output && ./output -e beautiful.bmp secret.txt 
                                        or
                gcc *.c -o output && ./output -e beautiful.bmp secret.txt output.bmp                        

        -> -e: Encode command to hide data.
        -> beautiful.bmp: The cover file where data will be hidden (e.g., an image).
        -> secret.txt: The secret data to be embedded (e.g., a text file).
        -> output.bmp: The resulting file that contains the hidden message and mentioning the file name is user 
            choice and it is not nessery.
    -> output:
   ------------------------------------------------------------
                ------------------------------------------------------------
                                Steganography : Encoding
                ------------------------------------------------------------
                INFO: Encoding operation requested....
                INFO: Varifying file names....
                INFO: Valid source file beautiful.bmp found
                INFO: Valid source file secret.txt found
                INFO: Creating output file as default
                INFO: Created stego.bmp
                INFO: Validation of file names completed
                -------------------  Varifying the data  -------------------
                INFO: Accessing the files....
                INFO: Opening all the necessary files....
                INFO: Opened beautiful.bmp
                INFO: Opened secret.txt
                INFO: Opened stego.bmp
                INFO: All files successfully opened
                INFO: beautiful.bmp size is 2359296 Bytes
                INFO: secret.txt size is 58 Bytes
                INFO: beautiful.bmp size enough to handel secret.txt data
                ---------------  Encoding Procedure Started  ---------------
                INFO: Copying Image Header....
                INFO: Image header successfully copied
                INFO: Encodeing Magic String Signature....
                INFO: Magic String Signature successfully encoded
                INFO: Encoding .txt size....
                INFO: Successfully encoded .txt size
                INFO: Encoding .txt extention....
                INFO: Successfully encoded .txt extention
                INFO: Encoding secret.txt file size....
                INFO: Successfully encoded secret.txt file size
                INFO: Encoding secret.txt file Data....
                INFO: Successfully encoded secret.txt file data
                INFO: Copying left over data....
                INFO: Successfully codied remainig left over data
                --------------  Encoding Procedure Completed  --------------

2. Decoding:

    -> The -d command is used to decode the hidden data from a steganographically modified file. This process 
       reads the bits that were altered during the encoding process to retrieve the original hidden message.
    -> Usage Example:
        -> If you want to extract a hidden message from a stego image:

                    gcc *.c -o output && ./output -d stego.bmp sectret_file.txt
        -> d: Decode command to extract the hidden data.
        -> stego.png: The steganographic file containing hidden data.
        -> sectret_file.txt: The output file where the extracted message will be saved and mentioning the file 
           name is user choice and it is not nessery.
    -> Output:
   ------------------------------------------------------------
                ------------------------------------------------------------
                                Steganography : Decoding
                ------------------------------------------------------------
                INFO: Decoding operation requested....
                INFO: Varifying file names....
                INFO: Valid source file stego.bmp found
                INFO: Creating output file as default
                INFO: Created stego_secret.txt
                INFO: Validation of file names completed
                -------------------  Varifying the data  -------------------
                INFO: Accessing the files....
                INFO: Opening all the necessary files....
                INFO: Opened stego.bmp
                INFO: Opened stego_secret.txt
                INFO: All files successfully opened
                ---------------  Decoding Procedure Started  ---------------
                INFO: Decoding Magic string signature....
                INFO: Magic string signature successfully decoded
                INFO: Decoding secret file extention size.....
                INFO: Successfully decoded extention size i.e. 4 Bytes
                INFO: Decoding secret file extention....
                INFO: Successfully encoded .txt extention
                INFO: Decoding secret file size.....
                INFO: Successfully decoded secret file size i.e. 58 Bytes
                INFO: Decoding secret file data....
                INFO: Successfully decoded the secrect file data
                INFO: Decoded data is copied to file stego_secret.txt
                --------------  Decoding Procedure Completed  --------------

3. If commands are wrong:
   ------------------------------------------------------------
                ------------------------------------------------------------
                                      Steganography
                ------------------------------------------------------------
                ERROR: Invalid operation requested....
                ERROR: Encoding - minimum 4 arguments required
                ERROR: Decoding - minimum 3 arguments required
                
------------------------------------------------------------------------------------------------------------
Files enclosed :-

1. encode.h
2. decode.h
3. types.h
4. common.h
5. decode.c
6. encode.c
7. main.c
8. beautiful.bmp
9. secret.txt
10. description.txt

------------------------------------------------------------------------------------------------------------
                                                THE END
------------------------------------------------------------------------------------------------------------
