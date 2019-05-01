#include<stdio.h>
#include<string.h>
#include<stdint.h>
#define BUF_LEN 200

int main(){
    char message[BUF_LEN];
    uint64_t m_len, p_len, i;

    // Read input
    printf("Enter the message:\n");
    scanf("%[^\n]s", message);

    // Calculate padding length
    m_len = (uint64_t)strlen(message) * 8;
    if(m_len % 512 < 448)
        p_len = 448 - m_len % 512;
    else
        p_len = 512 - m_len % 512 + 448;

    // Calculate the byte length of: message + padding + 64-bit length
    uint64_t byte_len = (m_len + p_len + 64)/8;
    uint8_t bytes[byte_len];

    // Write the message
    for(i = 0; i<m_len/8; i++){
        bytes[i] = (uint8_t)message[i];
    }

    // Write the padding
    bytes[i++] = 0x80;                  // Write 1000 0000 as the first byte
    for(; i<(m_len + p_len)/8; i++){
        bytes[i] = 0x00;                // Set all other bytes to 0000 0000
    }

    // Write the message length
    uint8_t *conv_bytes = (uint8_t *)&m_len;
    int c = 0;
    for(; i<byte_len; i++){
        bytes[i] = conv_bytes[c++];
    }

    printf("MESSAGE LENGTH: %lld\nPADDING LENGTH: %lld\nNUMBER OF BYTES: %lld\n", m_len, p_len, byte_len);

    return 0;
}
