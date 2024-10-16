// Online C compiler to run C program online
#include <stdio.h>

#define BITS 8

int main() {
    int arr[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    unsigned char ch;
    int i;
    printf("Enter a charecter to encod : ");
    scanf("%c", &ch );
    printf("Elemtens of array are : ");
    for ( i = 0; i < BITS; i++ ) {
        printf("%d ", arr[i] );
    }
    printf("\nRemove LSB from the numbers: ");
    for ( i = 0; i < BITS; i++ ) {
        arr[i] = arr[i] & ( (unsigned int ) ~1 ); //clear LSB of the elements in arr
        printf("%d ", arr[i] );
    }
    printf("\nEncoded as: ");
    for ( i = 0; i < BITS; i++ ) {
        arr[i] = arr[i] | ( 1 & ( ch >> ( BITS - i - 1) ) );
        printf("%d ", arr[i] );
    }
    printf("\n");
    return 0;
}