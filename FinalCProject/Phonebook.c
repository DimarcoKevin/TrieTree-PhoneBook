#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
typedef struct record {
	char* first_name;
	char* last_name;
	char* phone_number;
} Record;

typedef struct listNode{
	struct listNode* prev;
	struct listNode* next;
	struct record* record;
} ListNode;

typedef struct stackNode {
    struct record* rec;
    struct stackNode* next;
} StackNode;

typedef struct stack {
    struct stackNode* top;
} Stack;

typedef struct trieNode{
	struct trieNode* parent;
	struct listNode* records;
	struct trieNode* children[36];
} TrieNode;
int numOfRecords = 0;
ListNode* master = NULL;
/* Data Structure Utility Methods*/
Record* getRecord(){
	Record* r = malloc(sizeof(Record));
	r->first_name = malloc(sizeof(char) * 26);
	r->last_name = malloc(sizeof(char) * 26);
	r->phone_number = malloc(sizeof(char) * 11);
	return r;
}

TrieNode* getTrieNode(){
	TrieNode* node = malloc(sizeof(TrieNode));
	for(int i = 0; i < 36; i++){
		node->children[i] = NULL;
	}
	node->parent = NULL;
	node->records = NULL;
	return node;
}

ListNode* getListNode(Record* r){
	ListNode* node = malloc(sizeof(ListNode));
	node->next = NULL;
	node->prev = NULL;
	node->record = r;
	return node;
}

bool compareRecords(Record* r, Record* r1){
	if(strcmp(r->first_name, r1->first_name) == 0 &&
	   strcmp(r->last_name, r1->last_name) == 0 &&
	   strcmp(r->phone_number, r1->phone_number) == 0
	){
		return true;
	}
	return false;
}

Stack* initStack() {
    Stack* sp = malloc(sizeof(Stack));
    sp -> top = NULL;
    return sp;
}

int empty(Stack* S) {
    return (S -> top == NULL);
}

void push(Stack* S, Record* rec) {
    StackNode* np = malloc(sizeof(StackNode));
    np -> rec = rec;
    np -> next = S -> top;
    S -> top = np;
}

Record* pop(Stack* S) {
    if (empty(S)) {
        printf("\nAttempt to pop an empty stack\n");
        exit(1);
    }
    Record* hold = S -> top -> rec;
    StackNode* temp = S -> top;
    S -> top = S -> top -> next;
    free(temp);
    return hold;
}

//Inserts a node into the tree
void insert(TrieNode* root, char* word, Record* r){
	TrieNode* currentNode = root;
	while(*word != '\0'){
		if(isdigit(*word)){ //If the word has a number
			if(currentNode->children[*word - '0'] == NULL){
				//Insert number appearence into tree
				currentNode->children[*word - '0'] = (TrieNode*) getTrieNode();
				currentNode->children[*word - '0']->parent = currentNode;
			}
			currentNode = currentNode->children[*word - '0'];
		} else{ //Is a letter
			if(currentNode->children[tolower(*word) - 'a'] == NULL){
				//Insert character appearence into tree
				currentNode->children[tolower(*word) - 'a'] = (TrieNode*) getTrieNode();
				currentNode->children[tolower(*word) - 'a']->parent = currentNode;
			}
			currentNode = currentNode->children[tolower(*word) - 'a'];
		}
		*word++; //Go to the next character or "level" of the word
	}//End while
	
	//Insert record into tree
	ListNode* currentRecord = currentNode->records;
	ListNode* newRecord = getListNode(r);
	if(currentRecord == NULL){
		currentNode->records = newRecord;
	} else{
		if(currentRecord->next == NULL){
			newRecord->prev = currentRecord;
			currentRecord->next = newRecord;
		} else{
			while(currentRecord->next != NULL){
				currentRecord = currentRecord->next;
			}
			newRecord->prev = currentRecord;
			currentRecord->next = newRecord;
		}
	}
	if(master == NULL){
		ListNode* tmp = getListNode(r);
	}
}

//Returns a linked list of records that match the query
TrieNode* search(TrieNode* root, char* query){
	TrieNode* currentNode = root;
	while(*query != '\0'){
		if(isdigit(*query)){ //If the word has a number
			if(currentNode->children[*query - '0'] == NULL)
				return NULL; //Record not found
			currentNode = currentNode->children[*query - '0'];
		} else{ //Is a letter
			if(currentNode->children[tolower(*query) - 'a'] == NULL)
				return NULL; //Record not found
			currentNode = currentNode->children[tolower(*query) - 'a'];
		}
		*query++; //Go to the next character or "level" of the word
	}
	return currentNode; //Returns the list of records inside of the current node
}

ListNode* deleteNode(TrieNode* root, Record* r){
	TrieNode* currentNode;
	char* toDelete[3];
	toDelete[0] = r->first_name;
	toDelete[1] = r->last_name;
	toDelete[2] = r->phone_number;
	for(int i = 0; i < 3; i++){
		char* query = toDelete[i];
		currentNode = search(root, query);
		bool isLeaf = true;
		if(currentNode != NULL){ //exists
			printf("Node exists! \n");
			ListNode* results = currentNode->records;
			if(results != NULL){ //if this node has records
				printf("Node has results! \n");
				if(compareRecords(results->record, r)){ //Records match
					printf("Record matches");
					if(results->prev == NULL){ //If this is the head
						printf("This is the head!");
						if(results->next != NULL){
							printf("Node has a next node! \n");
							return results;
						} else{
							printf("This is the last node.");
							
							currentNode->records = NULL;
						}
					} else{
						printf("Records don't match'! \n");
						if(results->next != NULL){ //If there is no next node
							
							results->prev->next = results->next;
						} else{
							results->prev = results;
						}
					}
				}
				results = results->next;
			}
			for(int i = 0; i < 36; i++){
				if(currentNode->children[i] != NULL){
					printf("\n\nThis is a leaf\n");
					isLeaf = false;
					break;
				}
			}	
		
			while(currentNode->parent != NULL && isLeaf && currentNode->records == NULL){
				printf("This is a leaf\n");
				TrieNode* parent = currentNode->parent;
				for(int i = 0; i < 36; i++){
					if(parent->children[i] == currentNode){
						parent->children[i] = NULL;
						currentNode = parent;
					} else if(parent->children[i] != NULL){
						printf("Not a leaf");
						isLeaf = false;
						break;
					}
				}
			}
		}
		
	}
	return NULL; //No confirmation needed from user
}

ListNode* searchRecord(TrieNode* root, char* query, int attribute){
	TrieNode* treeNode = search(root, query);
	printf("was able to search\n");
	ListNode* results = (ListNode*) treeNode->records;
	printf("Initial Result: %s %s %s\n", results->record->first_name, results->record->last_name, results->record->phone_number);
	ListNode* toReturn = NULL;
	while(results != NULL){
		printf("Currently viewed record: %s %s %s\n", results->record->first_name, results->record->last_name, results->record->phone_number);
		if(attribute == 0){
			if(strcmp(results->record->first_name, query) == 0){
				if(toReturn = NULL){
					toReturn = getListNode(results->record);
				} else{
					ListNode* tmp = toReturn;
					while(tmp->next != NULL){
						tmp = tmp->next;
					}
					tmp->next = getListNode(results->record);
				}
			}
		} else if (attribute == 1){
			if(strcmp(results->record->last_name, query) == 0){
				if(toReturn = NULL){
					toReturn = getListNode(results->record);
				} else{
					ListNode* tmp = toReturn;
					while(tmp->next != NULL){
						tmp = tmp->next;
					}
					tmp->next = getListNode(results->record);
				}
			}
		} else if (attribute == 2){
			if(strcmp(results->record->phone_number, query) == 0){
				if(toReturn = NULL){
					toReturn = getListNode(results->record);
				} else{
					ListNode* tmp = toReturn;
					while(tmp->next != NULL){
						tmp = tmp->next;
					}
					tmp->next = getListNode(results->record);
				}
			}
		}
	}
	return toReturn;
}

/*Stack* sort(TrieNode* root, int attr){
	Stack* sortedRecords = initStack();
	Record records[numOfRecords];
	ListNode* tmp = master;
	while(tmp != NULL){
		push(root, tmp->record);
		tmp = tmp->next;
	}
	return sortedRecords;
}*/
