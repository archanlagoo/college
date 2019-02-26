#include<list>
#include<string>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<regex>
using namespace std;

template <typename T>
class with_mnemonic {
private:
    string _mnemonic;
public:
    with_mnemonic(string mnemonic):_mnemonic(mnemonic) {}
    bool operator() (const T & a) {
        if(a.mnemonic == _mnemonic) {
            return true;
        }
        return false;
    }
};

// Table entry formats
struct MOT_entry { string mnemonic; int length;};
struct POT_entry { string mnemonic;};
struct ST_entry  { string name; int lc; int length; bool relocatable; };
struct LT_entry  { string literal; int lc; int length; bool relocatable; };

int read_length_constant(string str){
    for(int i = 0; i<str.length(); i++){
        if(str.c_str()[i]=='f'){
            return 4;
        }
        if(str.c_str()[i]=='h'){
            return 2;
        }
    }
}
int extract_number(string str){
    int val = 0, s;
    switch(str.c_str()[0]){
        case 'f' : s = 4;
                   break;
        case 'h' : s = 2;
                   break;
    }
    regex rgx("[0-9]");
    smatch match;
    if(regex_search(str, match, rgx)){
        val = stoi(match[0]);
    }
    return val*s;
}

int main() {
    // Declaration
    ifstream file("00_assembler_input.txt"); // Program file
    int lc=0;                             // Location counter
    string words[4];                    // List of words per line
    int length;                         // Stores length of data
    regex literal("=[fh]'[0-9]+'");         // Regex to match the literal syntax

    // Create tables and related data
    list<MOT_entry> MOT;
    list<POT_entry> POT;
    list<ST_entry>  ST;
    list<LT_entry>  LT;
    MOT_entry mot_entry;
    POT_entry pot_entry;
    ST_entry  st_entry;
    LT_entry  lt_entry;
    list<MOT_entry>::iterator mot_iterator;
    list<POT_entry>::iterator pot_iterator;

    // Populate MOT and POT
    mot_entry.mnemonic = "l"; mot_entry.length = 4; MOT.push_back(mot_entry);
    mot_entry.mnemonic = "a"; mot_entry.length = 4; MOT.push_back(mot_entry);
    mot_entry.mnemonic = "st"; mot_entry.length = 4; MOT.push_back(mot_entry);
    mot_entry.mnemonic = "br"; mot_entry.length = 4; MOT.push_back(mot_entry);
    mot_entry.mnemonic = "bne"; mot_entry.length = 4; MOT.push_back(mot_entry);

    pot_entry.mnemonic = "start"; POT.push_back(pot_entry);
    pot_entry.mnemonic = "using"; POT.push_back(pot_entry);
    pot_entry.mnemonic = "balr"; POT.push_back(pot_entry);
    pot_entry.mnemonic = "equ"; POT.push_back(pot_entry);
    pot_entry.mnemonic = "dc"; POT.push_back(pot_entry);
    pot_entry.mnemonic = "ds"; POT.push_back(pot_entry);

    // Scan words
    while(file>>words[0]>>words[1]>>words[2]>>words[3]) {
        // Check if a label is present
        if(words[0]!="?") {
            // Check if the word is in the MOT
            mot_iterator = find_if(MOT.begin(), MOT.end(), with_mnemonic<MOT_entry>(words[1]));
            if(mot_iterator!=MOT.end()){
                /* If the symbol is in MOT, that means that the first word is a label.
                   Assign a name to the symbol and give it the location counter.
                   Load the label into the symbol table. */
                st_entry.name        = words[0];
                st_entry.lc          = lc;
                st_entry.length      = 1;
                st_entry.relocatable = true;
                ST.push_back(st_entry);
                lc = lc + mot_iterator->length;

                /* Check for literals and if they exist, add them to the
                   literal table. */
                if(regex_match(words[3], literal)){
                    lt_entry.literal     = words[3];
                    lt_entry.lc          = -1;
                    lt_entry.length      = read_length_constant(words[3]);
                    lt_entry.relocatable = 1;
                    LT.push_back(lt_entry);
                }
            }
            // Check if the word is in the POT table
            pot_iterator = find_if(POT.begin(), POT.end(), with_mnemonic<POT_entry>(words[1]));
            if(pot_iterator!=POT.end()){
                /*=============================================================
                | If the word is in POT, it is an assembler directive. There  |
                | are only four assembler directives that are processed in    |
                | pass 1:                                                     |
                | 1. START : Assign the value to the right to the label.      |
                | 2. EQU   : Do the same as above. The only difference is     |
                |            that the symbol is absolute.                     |
                | 3. DC    : Assign the name, the LC counter and  the length  |
                |            and set it as relocatable. Update the LC value   |
                |            as well.                                         |
                | 4. DS    : Do the same as above.                            |
                =============================================================*/
                if(words[1] == "start"){
                    st_entry.name        = words[0];
                    st_entry.lc          = stoi(words[2]);
                    st_entry.length      = 1;
                    st_entry.relocatable = true;
                    ST.push_back(st_entry);
                }
                else if(words[1] == "equ"){
                    st_entry.name        = words[0];
                    st_entry.lc          = stoi(words[2]);
                    st_entry.length      = 1;
                    st_entry.relocatable = false;
                    ST.push_back(st_entry);
                }
                else if(words[1] == "dc"){
                    length = read_length_constant(words[2]);
                    st_entry.name        = words[0];
                    st_entry.lc          = lc;
                    st_entry.length      = length;
                    st_entry.relocatable = true;
                    ST.push_back(st_entry);
                    lc = lc + length;
                }
                else if(words[1] == "ds"){
                    length = extract_number(words[2]);
                    st_entry.name        = words[0];
                    st_entry.lc          = lc;
                    st_entry.length      = length;
                    st_entry.relocatable = true;
                    ST.push_back(st_entry);
                    lc = lc + length;
                }
            }
        }
        else {
            mot_iterator = find_if(MOT.begin(), MOT.end(), with_mnemonic<MOT_entry>(words[1]));
            if(mot_iterator!=MOT.end()){
                if(regex_match(words[3], literal)){
                    lt_entry.literal     = words[3];
                    lt_entry.lc          = -1;
                    lt_entry.length      = read_length_constant(words[3]);
                    lt_entry.relocatable = 1;
                    LT.push_back(lt_entry);
                }
                lc = lc + mot_iterator->length;
            }
        }
    }

    // Display symbol table
    cout<<"=================SYMBOL TABLE================="<<endl;
    for(auto it = ST.begin(); it!=ST.end(); it++) {
        cout<<"name: "<<it->name<<"; lc: "<<it->lc<<"; length: "<<it->length<<"; relocatable: "<<it->relocatable<<endl;
    }

    // Display literal table
    cout<<"=================LITERAL TABLE================="<<endl;
    for(auto it = LT.begin(); it!=LT.end(); it++) {
        cout<<"name: "<<it->literal<<"; lc: "<<it->lc<<"; length: "<<it->length<<"; relocatable: "<<it->relocatable<<endl;
    }

    // End
    return 0;
}
