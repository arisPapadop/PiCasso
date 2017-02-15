#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 128

// Structure of entry to hash table
typedef struct Label_To_Address_Node {
	struct Label_To_Address_Node *next;
	char *label;
	uint32_t address;
} Label_To_Address_Node;

typedef Instruction(*Instr_Ptr)(StringArray);

typedef struct Mnemonic_To_Function_Node {
  struct Mnemonic_To_Function_Node *next;
  char *mnemonic;
  Instr_Ptr function;
} Mnemonic_To_Function_Node;

// Initialises hash table to 0
static Label_To_Address_Node *hashtable[HASH_SIZE];
static Mnemonic_To_Function_Node *functiontable[HASH_SIZE];

// Distributing hash code
uint32_t hash(char *string) {
	uint32_t hash_code = 0;
	int i = 0;
		while (string[i] != '\0') {
			hash_code = string[i]*i + 5*i;
			i++;
		}
	return hash_code % HASH_SIZE;
}

// If present, returns a pointer to matching node
Label_To_Address_Node *get_address(char *label) {

	Label_To_Address_Node *curr;

	for (curr = hashtable[hash(label)]; curr != NULL; curr = curr->next) {
		if (strcmp(label, curr->label) == 0) {
			return curr;
		}
	}
	return NULL;
}

// If not present, allocates memory for new entry
// Otherwise replaces value of old entry
Label_To_Address_Node *put_address(char *label, uint32_t address) {

	Label_To_Address_Node *curr;
	uint32_t hash_code;

	curr = get_address(label);
	if (curr == NULL) {
		curr = (Label_To_Address_Node *) calloc(1, sizeof(*curr));
		curr->label = strdup(label);
		if (curr == NULL || (curr->label == NULL)){
			return NULL;
		}
		hash_code = hash(label);
		curr->next = hashtable[hash_code];
		hashtable[hash_code] = curr;
	} else {
		free((void *)(&(curr->address)));
	}
	curr->address = address;
	if (curr->address == END_OF_STRING) {
		return NULL;
	}
	return curr;
}

//------------------------------------------------------------------------------
// If present, returns a pointer to matching node
Mnemonic_To_Function_Node *get(char *mnemonic) {

	Mnemonic_To_Function_Node *curr;

	for (curr = functiontable[hash(mnemonic)]; curr != NULL; curr = curr->next) {
		if (strcmp(mnemonic, curr->mnemonic) == 0) {
			return curr;
		}
	}
	return NULL;
}

// If not present, allocates memory for new entry
// Otherwise replaces value of old entry
Mnemonic_To_Function_Node *put_function(char *mnemonic, Instr_Ptr function) {

	Mnemonic_To_Function_Node *curr;
	uint32_t hash_code;

	if ((curr = get(mnemonic)) == NULL) {
		curr = (Mnemonic_To_Function_Node *) calloc(1, sizeof(*curr));
		if (curr == NULL || (curr->mnemonic = strdup(mnemonic)) == NULL){
			return NULL;
		}
		hash_code = hash(mnemonic);
		curr->next = functiontable[hash_code];
		functiontable[hash_code] = curr;
	} else {
		free((void *)(&(curr->function)));
	}
	if ((curr->function = function) == NULL) {
		return NULL;
	}
	return curr;
}

void destroy_function_nodes(Mnemonic_To_Function_Node *array[HASH_SIZE]) {
	for (int i = 0; i < HASH_SIZE; i++) {
		Mnemonic_To_Function_Node *curr = array[i];
		while (curr != NULL) {
			free(curr->mnemonic);
			Mnemonic_To_Function_Node *temp = curr;
			temp = temp->next;
			free(curr);
			curr = temp;
		}
	}
}

void destroy_address_nodes(Label_To_Address_Node *array[HASH_SIZE]) {
	for (int i = 0; i < HASH_SIZE; i++) {
		Label_To_Address_Node *curr = array[i];
		while (curr != NULL) {
			free(curr->label);
			Label_To_Address_Node *temp = curr;
			temp = temp->next;
			free(curr);
			curr = temp;
		}
	}
}  

void free_tables() {
   destroy_function_nodes(functiontable);
   destroy_address_nodes(hashtable);
}