#include<iostream>
using namespace std;

bool extract_bit(int loc, char inp){
    bool out = (inp >> (7-loc))&1;
    return out;
}
void place_bit(int loc, char *out, bool bit){
    uint8_t ot = bit;
    ot = ot << loc;
    *out = *out + ot;
}

int main(){
    // Declaration
    char input[8], output[8];
    bool inbit, out[8][8];
    int key[8][8] = {
        {58, 56, 42, 34, 26, 18, 10, 2},
        {60, 52, 44, 36, 28, 20, 12, 4},
        {62, 54, 46, 38, 30, 22, 14, 6},
        {64, 56, 48, 40, 32, 24, 16, 8},
        {57, 49, 41, 33, 25, 17,  9, 1},
        {59, 51, 43, 35, 27, 19, 11, 3},
        {61, 53, 45, 37, 29, 21, 13, 5},
        {63, 55, 47, 39, 31, 23, 15, 7}
    };
    int row, col;

    // Read input
    cout<<"Enter the string: ";
    cin>>input;

    // Encrypt
    for(int i=0; i<8; i++){
        output[i] = '\0';
        cout<<"Input: "<<(int)input[i]<<endl;
        // Access matrix elements
        for(int j=0; j<8; j++){
            key[i][j]--;
            inbit = extract_bit(j, input[i]);
            row = key[i][j]/8; col = key[i][j]%8;
            out[row][col] = inbit;
        }
        cout<<"\n";
    }

    // Display output
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            cout<<out[i][j]<<"\t";
        }
        cout<<endl;
    }
    // End
    return 0;
}
