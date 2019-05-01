#include<stdio.h>
#include<stdint.h>
#define INT_MAX 256
#define K1      11
#define K2      5
#define K1_INV  163

void main(){
    char a;
    printf("Enter the character: ");
    scanf("%c", &a);

    // Encrypting character
    int t1 = ((unsigned int)a * K1) % INT_MAX;
    int t2 = (t1 + K2) % INT_MAX;
    char encr = (char)t2;
    printf("Encrypted character: %c\n", encr);

    // Decrypted character
    t2 = ((unsigned int)encr - K2) % INT_MAX;
    t1 = (t2 * K1_INV) % INT_MAX;
    char decr = (char)t1;
    printf("Decrypted character: %c\n", decr);
}
