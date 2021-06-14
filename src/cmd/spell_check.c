#include "spell_check.h"

char *WORDS = "words.txt";
hash_table* htable;
// maximum length for a word
const int MAX_WORD_LEN = 45;

void hash_print(int i) {

    hash_node *curr;

    printf("------- Printing Hash Table ------ \n");
    //for ( i = 0; i < HASH_TABLE_SIZE; i++ ) {
        curr = htable->nodes[i];
        while ( curr != NULL ) {
            printf( "Key: %s\n ",  curr->word);
            curr = curr->next;
        }
    //}
    printf("------- End of table --------\n");
}

char** spell_check(char **text, int nb_word){
    // load dictionary;
    char *dict = WORDS;
    bool loaded = load(dict);
    // abort if dictionary not loaded
    if(!loaded)
        err(1, "Could not load dictionnary");

    // spell-check each word
    for(int i = 0; i < nb_word; i++){
        bool misspelled = !check(text[i]);
        if (misspelled)
            // TODO : correct the word
            printf("misspelled: %s\n", text[i]);
    }
    return text;
}

/* Loads dict into memory. */
bool load(char *dict){
    char word [MAX_WORD_LEN +1];
    bool insertWord;
    htable = hash_create();

    // Open dictionary file
    FILE *dictfp;
    dictfp = fopen(dict, "r");
    // Check to see if it exists
    if(dictfp == NULL)
        err(1, "Empty dictionnary");

    // Read in words from dictionary
    while(fscanf(dictfp, "%s", word) != EOF){
        insertWord = hash_insert(word);
        if(!insertWord)
            return false;
        htable->numwords++;
    }
    fclose(dictfp);
    return true;
}

/* Create Hash Table */
hash_table* hash_create(){
    // Allocate memory to the table
    hash_table *ptable = (hash_table*)malloc(sizeof(hash_table));
    if(!ptable)
        err(1,"Not enough memory!\n");

    // Allocate memory for the actual nodes of the table
    ptable->nodes = (hash_node**)malloc(sizeof(hash_node*) * HASH_TABLE_SIZE);
    if(!ptable->nodes)
        err(1, "Not enough memory!\n");

    // Initialize the nodes
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
        ptable->nodes[i] = NULL;

    return ptable;
}

/* Inserts a new data entry t the table */
bool hash_insert(char *word){
    // Perform hash funtion on a word and store integer in Index
    int index = hash_func(word);

    // Use index value to create a curr node
    hash_node *curr = htable->nodes[index];
    // If a node didnt exist then create it
    if(!curr){
        // Create new node if list is empty
        htable->nodes[index] = create_hash_node(word);
        htable->nodes[index]->next = NULL;
    }
    else{
        while(curr != NULL){
            // If key is already in list, simply update the data of this list
            if(strcmp(curr->word, word) == 0)
                return true;
            curr = curr->next;
        }
        // Create new node
        curr = create_hash_node(word);
        if(curr == NULL)
            return false;
        // Insert it into the beginning of the list
        curr->next = htable->nodes[index];
        htable->nodes[index] = curr;
    }
    return true;
}

/* Hash Function */
int hash_func(char* word){
    unsigned int hash = 0;
    for(int i=0, n=strlen(word); i<n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % HASH_TABLE_SIZE;
}

/* Create the Hash Node */
hash_node* create_hash_node( const char *word){
    hash_node* curr = (hash_node*)malloc(sizeof(hash_node));
    curr->word = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(curr->word, word);
    return curr;
}

/* Returns true if word is in dictionary else false. */
bool check(char *word){
    int hash_tmp;
    hash_node *curr;

    //  Create a key for the tmp by putting it in the hash function
    hash_tmp = hash_func(word);
    // set the new hashed tmp to a curr node
    curr = htable->nodes[hash_tmp];

    // While the node is not null see if the new node is equal to the tmp
    while (curr != NULL){
        if (strcmp (word, curr->word) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}


