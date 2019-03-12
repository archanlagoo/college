#include<iostream>
#include<fstream>
#include<map>
#include<regex>
using namespace std;

/* Basic structs */
struct token_format_entry{
	regex  format;
	string type;
}
struct token_entry{
	string token;
	string type;
}

/* Main functions */
void tokenize(string word, vector<token_format_entry> & TokenFormat, vector<token_entry> & tokens){
	// Basic declaration
	char c_word = word.c_str();
	int  i;
	int  len = word.size();

	// Token declarations
	string token = "", next_token="";

	// Traverse word character by character
	for(i = 0; i<len; i++){
		next_token.append(c_word[i]);
		if(is_valid(next_token, TokenFormat)){
			token = next_token;
		}
		else{
			tokens.push_back((token_entry){.token = token, .type = get_type(token, TokenFormat)});
			token = 
		}
	}
}

int main(){
	// File variables
	ifstream file("00_lex_input.txt");
	string word;

	// Databases
	vector<token_format_entry> TokenFormat;
	vector<token_entry> tokens;

	// Read file
	while(file >> word){
		tokenize(word, TokenFormat, tokens);
	}
	return 0;
}