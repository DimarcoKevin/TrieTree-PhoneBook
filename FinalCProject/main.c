#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "Phonebook.h"

void removeEndline(char* word){
	char *pos;
	if ((pos=strchr(word, '\n')) != NULL)
    	*pos = '\0';
}
int main(void){
	TrieNode* tree = getTrieNode();
	TrieNode* node = NULL;
	ListNode* results = NULL;
	Record* r = NULL;
	//Adding a record
	r = getRecord();
	printf("New Record\nFirst Name\n>");
	fgets(r->first_name, 26, stdin);
	removeEndline(r->first_name);
	printf("Last Name\n>");
	fgets(r->last_name, 26, stdin);
	removeEndline(r->last_name);
	printf("Phone Number\n>");
	fgets(r->phone_number, 11, stdin);
	removeEndline(r->phone_number);
	insert(tree, r->first_name, r);
	insert(tree, r->last_name, r);
	insert(tree, r->phone_number, r);
	fflush(stdin);
	 
	//Searching for a record
	node = search(tree, r->first_name);
	if(node == NULL){
		printf("Sorry, no record matches the query provided\n");
	} else{
		results = node->records;
		for(int i = 1; results != NULL; results = results->next) //Print all records
			printf("Record %d: %s %s %s\n", i++, results->record->first_name, results->record->last_name, results->record->phone_number);
 	}
	
	//Deleting a record
	ListNode* duplicateRecords = deleteNode(tree, r);
	printf("deleted");
	node = search(tree, r->first_name);
	if(node == NULL){
		printf("Sorry, no record matches the query provided\n");
	} else{
		results = node->records;
		for(int i = 1; results != NULL; results = results->next) //Print all records
			printf("Record %d: %s %s %s\n", i++, results->record->first_name, results->record->last_name, results->record->phone_number);
 	}
}
