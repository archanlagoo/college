#include<list>
#include<string>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<regex>
#include<bits/stdc++.h>
using namespace std;

// Table entry formats
struct MOT_entry { string name; int length;};
struct POT_entry { string name;};
struct ST_entry  { string name; int lc; int length; bool relocatable; };
struct LT_entry  { string name; int lc; int length; bool relocatable; };
struct BT_entry  { int reg; int value; };

template <typename T>
class with_name {
private:
    string _name;
public:
    with_name(string name):_name(name) {}
    bool operator() (const T & a) {
        if(a.name == _name) {
            return true;
        }
        return false;
    }
};

class with_register {
private:
    int _reg;
public:
    with_register(int reg):_reg(reg) {}
    bool operator() (BT_entry & a) {
        if(a.reg == _reg) {
            return true;
        }
        return false;
    }
};

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
bool isNumber(string word){
    char *p;
    long val = strtol(word.c_str(), &p, 0);
    return *p == 0;
}
string format_offset(int lc, list<BT_entry> *BT){
    int offset = INT_MAX;
    int base_reg = 0;
    string output = "";
    for(auto it = BT->begin(); it!=BT->end(); it++){
        if(offset > (lc - it->value)){
            offset = lc - it->value;
            base_reg = it->reg;
        }
    }
    output = output + to_string(offset) + "(0, " + to_string(base_reg) + ")";
    return output;
}

void init_db(list<MOT_entry> *MOT, list<POT_entry> *POT){
    MOT_entry mot_entry;
    POT_entry pot_entry;
    ST_entry  st_entry;
    LT_entry  lt_entry;

    // Populate MOT and POT
    mot_entry.name = "l"; mot_entry.length = 4; MOT->push_back(mot_entry);
    mot_entry.name = "a"; mot_entry.length = 4; MOT->push_back(mot_entry);
    mot_entry.name = "st"; mot_entry.length = 4; MOT->push_back(mot_entry);
    mot_entry.name = "br"; mot_entry.length = 4; MOT->push_back(mot_entry);
    mot_entry.name = "bne"; mot_entry.length = 4; MOT->push_back(mot_entry);

    pot_entry.name = "start"; POT->push_back(pot_entry);
    pot_entry.name = "using"; POT->push_back(pot_entry);
    pot_entry.name = "balr"; POT->push_back(pot_entry);
    pot_entry.name = "equ"; POT->push_back(pot_entry);
    pot_entry.name = "dc"; POT->push_back(pot_entry);
    pot_entry.name = "ds"; POT->push_back(pot_entry);
}
void Pass1(string fname, list<MOT_entry> *MOT, list<POT_entry> *POT, list<ST_entry> *ST, list<LT_entry> *LT){
    // Declaration
    ifstream file(fname); // Program file
    int lc=0;                             // Location counter
    string words[4];                    // List of words per line
    int length;                         // Stores length of data
    regex literal("=[fh]'[0-9]+'");         // Regex to match the literal syntax

    // Create database related variables
    MOT_entry mot_entry;
    POT_entry pot_entry;
    ST_entry  st_entry;
    LT_entry  lt_entry;
    list<MOT_entry>::iterator mot_iterator;
    list<POT_entry>::iterator pot_iterator;

    // Scan words
    while(file>>words[0]>>words[1]>>words[2]>>words[3]) {
        // Check if a label is present
        if(words[0]!="?") {
            // Check if the word is in the MOT
            mot_iterator = find_if(MOT->begin(), MOT->end(), with_name<MOT_entry>(words[1]));
            if(mot_iterator!=MOT->end()){
                /* If the symbol is in MOT, that means that the first word is a label.
                   Assign a name to the symbol and give it the location counter.
                   Load the label into the symbol table. */
                st_entry.name        = words[0];
                st_entry.lc          = lc;
                st_entry.length      = 1;
                st_entry.relocatable = true;
                ST->push_back(st_entry);
                lc = lc + mot_iterator->length;

                /* Check for literals and if they exist, add them to the
                   literal table. */
                if(regex_match(words[3], literal)){
                    lt_entry.name        = words[3];
                    lt_entry.lc          = -1;
                    lt_entry.length      = read_length_constant(words[3]);
                    lt_entry.relocatable = 1;
                    LT->push_back(lt_entry);
                }
            }
            // Check if the word is in the POT table
            pot_iterator = find_if(POT->begin(), POT->end(), with_name<POT_entry>(words[1]));
            if(pot_iterator!=POT->end()){
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
                    ST->push_back(st_entry);
                }
                else if(words[1] == "equ"){
                    st_entry.name        = words[0];
                    st_entry.lc          = stoi(words[2]);
                    st_entry.length      = 1;
                    st_entry.relocatable = false;
                    ST->push_back(st_entry);
                }
                else if(words[1] == "dc"){
                    length = read_length_constant(words[2]);
                    st_entry.name        = words[0];
                    st_entry.lc          = lc;
                    st_entry.length      = length;
                    st_entry.relocatable = true;
                    ST->push_back(st_entry);
                    lc = lc + length;
                }
                else if(words[1] == "ds"){
                    length = extract_number(words[2]);
                    st_entry.name        = words[0];
                    st_entry.lc          = lc;
                    st_entry.length      = length;
                    st_entry.relocatable = true;
                    ST->push_back(st_entry);
                    lc = lc + length;
                }
            }
        }
        else {
            mot_iterator = find_if(MOT->begin(), MOT->end(), with_name<MOT_entry>(words[1]));
            if(mot_iterator!=MOT->end()){
                if(regex_match(words[3], literal)){
                    lt_entry.name        = words[3];
                    lt_entry.lc          = -1;
                    lt_entry.length      = read_length_constant(words[3]);
                    lt_entry.relocatable = 1;
                    LT->push_back(lt_entry);
                }
                lc = lc + mot_iterator->length;
            }
        }
    }
}

void Pass2(string fname, list<MOT_entry> *MOT, list<POT_entry> *POT, list<ST_entry> *ST, list<LT_entry> *LT, list<BT_entry> *BT){
    // File variables
    ifstream inp_file(fname);
    ofstream out_file("out_"+fname);
    string   words[4];

    // Assembler variables
    BT_entry bt_entry;
    int lc = 0, reg, reg_val;
    list<MOT_entry>::iterator mot_iterator;
    list<POT_entry>::iterator pot_iterator;
    list<ST_entry>::iterator st_iterator;
    list<LT_entry>::iterator lt_iterator;
    list<BT_entry>::iterator bt_iterator;

    // Navigate the file
    while(inp_file>>words[0]>>words[1]>>words[2]>>words[3]){
            // Search in MOT and POT
            mot_iterator = find_if(MOT->begin(), MOT->end(), with_name<MOT_entry>(words[1]));
            pot_iterator = find_if(POT->begin(), POT->end(), with_name<POT_entry>(words[1]));

            // If the instruction is in MOT, replace operands if required.
            if(mot_iterator!=MOT->end()){
            out_file<<words[1]<<" ";
            // Check if the first operand is a number. Else, check in ST and LT.
            if(isNumber(words[2])){
                out_file<<words[2]<<" ";
            }
            else{
                // Check if the word is a symbol
                st_iterator = find_if(ST->begin(), ST->end(), with_name<ST_entry>(words[2]));
                if(st_iterator != ST->end()){
                    cout<<"Operand 1: "<<st_iterator->name<<"\t Relocatable: "<<st_iterator->relocatable<<endl;

                /*=========================================================================
                | If the symbol is relocatable, calculate the offset using the formula:   |
                |     offset = LC_value - nearest_base_value                              |
                | It will be represented in the following format:                         |
                |     offset(index, base_register)                                        |
                | The index is assumed to be zero as of now.                              |
                =========================================================================*/

                if(st_iterator->relocatable){
                    out_file<<format_offset(st_iterator->lc, BT)<<" ";
                }
                else{
                    out_file<<st_iterator->lc<<" ";
                }
            }
            }

            // Check if the second operand is a number. Else, check in ST and LT.
            if(isNumber(words[3])){
                out_file<<words[3];
            }
            else if(words[3]=="?"){}
            else{
                // Check if the word is a symbol
                st_iterator = find_if(ST->begin(), ST->end(), with_name<ST_entry>(words[3]));
                if(st_iterator != ST->end()){
                    cout<<"Operand 2: "<<st_iterator->name<<"\t Relocatable: "<<st_iterator->relocatable<<endl;

                if(st_iterator->relocatable){
                    out_file<<format_offset(st_iterator->lc, BT);
                }
                else{
                    out_file<<st_iterator->lc;
                }
                }
                }
                out_file<<endl;
                lc = lc + mot_iterator->length;
            }
            if(pot_iterator!=POT->end()){

            // Check for each individual instruction
            if(words[1]=="using"){
                    reg = stoi(words[3]);
                    if(words[2]=="*")
                        reg_val = lc;
                    else
                        reg_val = stoi(words[2]);
                    // Check if the register exists in the base table
                    bt_iterator = find_if(BT->begin(), BT->end(), with_register(reg));
                    if(bt_iterator==BT->end()){
                        bt_entry.reg = reg;
                        bt_entry.value = reg_val;
                        BT->push_back(bt_entry);
                    }
                    else{
                        bt_iterator->value = reg_val;
                    }
            }
            else if(words[1]=="dc"){
                lc = lc + read_length_constant(words[2]);
            }
            else if(words[1]=="ds"){
                lc = lc + extract_number(words[2]);
            }

            }
}
}

int main(){
    // Declaration and initialization of databases
    list<MOT_entry> MOT;
    list<POT_entry> POT;
    list<ST_entry>  ST;
    list<LT_entry>  LT;
    list<BT_entry>  BT;
    init_db(&MOT, &POT);

    // Other variables
    string file_input = "SPCC_2PA_file.txt";

    // Run Pass 1
    Pass1(file_input, &MOT, &POT, &ST, &LT);

    // Display both tables
    cout<<"=================SYMBOL TABLE================="<<endl;
    for(auto it = ST.begin(); it!=ST.end(); it++) {
        cout<<"name: "<<it->name<<"; lc: "<<it->lc<<"; length: "<<it->length<<"; relocatable: "<<it->relocatable<<endl;
    }

    // Display literal table
    cout<<"=================LITERAL TABLE================="<<endl;
    for(auto it = LT.begin(); it!=LT.end(); it++) {
        cout<<"name: "<<it->name<<"; lc: "<<it->lc<<"; length: "<<it->length<<"; relocatable: "<<it->relocatable<<endl;
    }

    // Run Pass 2
    Pass2(file_input, &MOT, &POT, &ST, &LT, &BT);

    return 0;
}
