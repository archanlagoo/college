#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
#include<iterator>
using namespace std;

/* UTILITY FUNCTIONS */
template<typename K, typename V>
void print_map(map<K,V> const &m){
    for (auto const& pair: m) {
        cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}
vector<string> get_list_of_words(string line){
    istringstream iss(line);
    vector<string> words(istream_iterator<string>{iss}, istream_iterator<string>());
    return words;
}

void prepare_MNT_ALA(map<string, int> & MNT, map<string, int> & ALA, string line, int *mdtc){
    vector<string> words = get_list_of_words(line);

    // Add entry to MNT and update MDTC
    MNT[words[0]] = *mdtc;

    // Add words to ALA
    for(int i = 1; i<words.size(); i++){
        ALA[words[i]] = i-1;
    }
}
string renamed_entry_1(string line, map<string, int> ALA){
    cout<<"Inside renamed_entry_1"<<endl;
    // Declarations
    map<string, int>::iterator it;
    string renamed_line = "";

    // Split line into words
    vector<string> words = get_list_of_words(line);

    // Output string
    for(int i = 0; i<words.size(); i++){
        it = ALA.find(words[i]);
        if(it==ALA.end()){
            renamed_line = renamed_line + words[i] + (i==(words.size()-1)?"":" ");
        }
        else{
            renamed_line = renamed_line + "#" + to_string(it->second) + (i==(words.size()-1)?"":" ");
        }
    }
    return renamed_line;
}
string renamed_entry_2(string line, map<string, string> ALA){
    // Declarations
    map<string, string>::iterator it;
    string renamed_line = "";

    // Split line into words
    vector<string> words = get_list_of_words(line);

    // Output string
    for(int i = 0; i<words.size(); i++){
        it = ALA.find(words[i]);
        if(it==ALA.end()){
            renamed_line = renamed_line + words[i] + (i==words.size()-1?"":" ");
        }
        else{
            renamed_line = renamed_line + it->second + (i==words.size()-1?"":" ");
        }
    }
    return renamed_line;
}
void write_line(map<string, int> & MNT, vector<string> & MDT, map<string, int> & ALA, ofstream & file, string line, int *mdtc){
    // Get the list of words
    vector<string> words = get_list_of_words(line);

    // Basic declarations
    int i;
    string mdt_entry, par_index;

    auto it = MNT.find(words[0]);
    if(it!=MNT.end()){
        // Generate ALA for pass 2
        map<string, string> ALA_2;
        for(i = 1; i<words.size(); i++){
            par_index = "#" + to_string(i-1);
            ALA_2[par_index] = words[i];
        }

        i = it->second;
        while(MDT[i]!="MEND"){
            file<<renamed_entry_2(MDT[i++], ALA_2)<<endl;
        }
    }
}

int main(){
    // File variables
    ifstream file("00_macroprocessor_input.txt");
    ofstream out("00_macroprocessor_output.txt");
    string line;
    string str;

    // Database variables
    map<string, int> MNT;
    map<string, int> ALA;
    vector<string> MDT;
    int mdtc=0;

    // Scan file
    while(getline(file, line)){
        if(line=="MACRO"){
            getline(file, line);
            prepare_MNT_ALA(MNT, ALA, line, &mdtc);

            while(line!="MEND"){
                str = renamed_entry_1(line, ALA);
                cout<<str<<endl;
                MDT.push_back(str);
                mdtc++;
                getline(file, line);
            }
            MDT[mdtc] = "MEND";
        }
        else{
            write_line(MNT, MDT, ALA, out, line, &mdtc);
        }
    }
}
