#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LINE_SIZE     40
#define NAME_SIZE     20
#define VAR_SIZE      20
#define MDT_SIZE_INIT 20
#define MNT_SIZE_INIT 10
#define ALA_SIZE_INIT 5

// Define structures
typedef struct MDT_element MDT_element;
typedef struct MNT_element MNT_element;
typedef struct ALA_element ALA_element;
struct MDT_element{
	char line[LINE_SIZE];
};
struct MNT_element{
	char name[NAME_SIZE];
	int mdtc;
};
struct ALA_element{
	char var_name[VAR_SIZE];
};

// Insert into MDT
void MDT_insert(MDT_element mdt_element, MDT_element *MDT, int *MDT_size){
	// Check if the size is enough
	int size = ((*MDT_size/MDT_SIZE_INIT)+1)*MDT_SIZE_INIT;
	if(*MDT_size<size){
		realloc(MDT, size+MDT_SIZE_INIT);
	}

	// Insert element at the highest position
	MDT[*MDT_size] = mdt_element;
	*MDT_size++;
}
// Insert into MNT
void MNT_insert(MNT_element mnt_element, MNT_element *MNT, int *MNT_size){
	// Check if the size is enough
	int size = ((*MNT_size/MNT_SIZE_INIT)+1)*MNT_SIZE_INIT;
	if(*MNT_size<size){
		realloc(MNT, size+MNT_SIZE_INIT);
	}

	// Insert element at the highest position
	MNT[*MNT_size] = mnt_element;
	*MNT_size++;
}
// Insert into ALA
void ALA_insert(ALA_element ala_element, ALA_element *ALA, int *ALA_size){
	// Check if the size is enough
	int size = ((*ALA_size/ALA_SIZE_INIT)+1)*ALA_SIZE_INIT;
	if(*ALA_size<size){
		realloc(ALA, size+ALA_SIZE_INIT);
	}

	// Insert element at the highest position
	ALA[*ALA_size] = ala_element;
	*ALA_size++;
}

void prepare_name_ala(MNT_element *MNT, MDT_element *MDT, ALA_element *ALA, int *MNT_size, int *MDT_size, int *ALA_size, int MDTC, char *line, ssize_t line_len){
	// Declaration
	int i, j=0;
	MNT_element mnt_element;
	ALA_element ala_element;

	// Read the name
	for(i=0; i<line_len; i++){
		if(line[i]!=' ')
			mnt_element.name[j++] = line[i];
		else
			break;
	}
	mnt_element.name[j] = '\0';
	mnt_element.mdtc    = *MDT_size;
	MNT_insert(mnt_element, MNT, MDT_size);

	// Read the arguments and add into ALA
	j = 0;
	for(; i<line_len; i++){
		if(line[i]!=' ')
			ala_element.var_name[j++] = line[i];
		else{
			
		}
	}
}

void main(){
	// General declarations
	int i;

	// Create database variables
	MDT_element MDT[MDT_SIZE_INIT];
	MNT_element MNT[MNT_SIZE_INIT];
	ALA_element ALA[ALA_SIZE_INIT];
	ALA_element ala_element;
	int MDT_size = 0, MNT_size=0, ALA_size=0;

	// Declare file reading variables
	FILE *file = fopen("prog.txt", "r");
	char *line = NULL;
	ssize_t line_len;
	size_t len = 0;

	// Read file line by line
	while((line_len = getline(&line, &len, file))!=-1){
		// Check if the keyword is MACRO
		if(strcmp(line, "MACRO")!=0){
		/*======================================================================
		| A macro has been declared. Read the next line and prepare the MNT    |
		| entry with the current MDTC value/MDTC_size. Insert all the          |
		| arguments into ALA for the macro. Enter the line into MDT.           |
		======================================================================*/

		line_len = getline(&line, &line, file);
		
		// Read the different words in the first line
		prepare_name_ala(MNT, MDT, ALA, &MNT_size, &MDT_size, &ALA_size, MDT_size, line, line_len);

		/*=====================================================================*/
		}
	}
}