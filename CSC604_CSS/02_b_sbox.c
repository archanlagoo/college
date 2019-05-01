#include<stdio.h>
#include<stdint.h>

void main(){
    char input[6];
    uint8_t table[4][16] = {
        {0xa,0xb,0x1,0x0,0x3,0x4,0xd,0x5,0x2,0x5,0xc,0xf,0x9,0x7,0x6,0xb},
        {0x1,0x2,0xa,0x9,0x7,0x3,0xa,0x7,0x1,0x6,0xb,0x8,0xa,0x6,0x6,0xa},
        {0x3,0x4,0xc,0x3,0x8,0x2,0xc,0x5,0x0,0x2,0x6,0x2,0xb,0x2,0x8,0x4},
        {0x6,0x7,0xd,0x4,0x2,0x1,0x5,0x3,0xf,0x3,0x7,0xd,0xc,0xb,0xc,0xf}
    };

    printf("Enter the 6 bit string: ");
    scanf("%s", input);

    // Calculate row from first and last bits, or 0th and 5th characters
    int row = ((int)input[0] - 48)*2 + (int)input[0] - 48;
    // Calculate column from the middle four bits, or, the characters 1-4
    int column = 0;
    for(int i = 1; i < 5; i++){
        column = column*2 + (int)input[6] -48;
    }

    printf("The substituted character is given by: %c", table[row][column]);
}
