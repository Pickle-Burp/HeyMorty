#include "spell_check.h"

char *WORDS = "words.txt";
hash_table *hashTable;
// maximum length for a word
const int MAX_WORD_LEN = 45;
// dictionary lines number
const int NB_LINES = 32380;
int app_name = -1;

/* Check each word and correct it if necessary */
char **spell_check(char **text, int nb_word){
    char *dict = WORDS;
    char **closest_word = calloc(NB_LINES, sizeof(char));

    // try to load dictionary and abort if not loaded
    if(load(dict) != 0)
        err(1, "Could not load dictionary");

    // spell-check each word
    for(int i = 0; i < nb_word; i++){
        if(strcmp(text[i], "trouve") == 0 && i != nb_word - 1){
            text[i + 1] = text[i + 1];
            nb_word--;
        } else if(strcmp(text[i], "recherche") == 0)
            app_name = 0;
        else if(strcmp(text[i], "ouvre") == 0)
            nb_word--;
        int misspelled = check(text[i], closest_word);
        if((strcmp(text[i], "sur") == 0 || strcmp(text[i], "dans") == 0) && app_name != -1)
            i++;
        if(misspelled != 0)
            text[i] = correct(text[i], closest_word, misspelled);
    }
    return text;
}

/* Loads dict into memory */
int load(char *dict){
    char word[MAX_WORD_LEN + 1];
    hashTable = hash_create();

    // Open dictionary file
    FILE *dictfp;
    dictfp = fopen(dict, "r");
    // Check if it exists and not empty
    if(dictfp == NULL)
        err(1, "Empty dictionnary");

    // Read from dictionary to add in hash table
    while(fscanf(dictfp, "%s", word) != EOF){
        if(hash_insert(word) != 0)
            return -1;
        hashTable->numwords++;
    }
    fclose(dictfp);
    return 0;
}

/* Create Hash Table */
hash_table *hash_create(){
    // Allocate memory to the table
    hash_table *pTable = (hash_table *) malloc(sizeof(hash_table));
    if(!pTable)
        err(1, "Not enough memory!\n");

    // Allocate memory for the actual nodes of the table
    pTable->nodes = (hash_node **) malloc(sizeof(hash_node *) * HASH_TABLE_SIZE);
    if(!pTable->nodes)
        err(1, "Not enough memory!\n");

    // Initialize the nodes
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
        pTable->nodes[i] = NULL;

    return pTable;
}

/* Inserts a new data entry to the table */
int hash_insert(char *word){
    // Perform hash funtion on a word and store integer in Index
    int index = hash_func(word);

    // Use index value to create a curr node
    hash_node *curr = hashTable->nodes[index];
    // If a node didn't exist then create it
    if(!curr){
        // Create new node if list is empty
        hashTable->nodes[index] = create_hash_node(word);
        hashTable->nodes[index]->next = NULL;
    } else{
        while(curr != NULL){
            // If key is already in list, simply update the data of this list
            if(strcmp(curr->word, word) == 0)
                return 0;
            curr = curr->next;
        }
        // Create new node
        curr = create_hash_node(word);
        if(curr == NULL)
            return -1;
        // Insert it into the beginning of the list
        curr->next = hashTable->nodes[index];
        hashTable->nodes[index] = curr;
    }
    return 0;
}

/* Hash Function */
int hash_func(char *word){
    unsigned int hash = 5381;
    for(unsigned int i = 0; i < strlen(word); word++, i++)
        hash = ((hash << 5) + hash) + (*word);
    return hash % HASH_TABLE_SIZE;
}

/* Create the Hash Node */
hash_node *create_hash_node(const char *word){
    hash_node *curr = (hash_node *) malloc(sizeof(hash_node));
    curr->word = (char *) malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(curr->word, word);
    return curr;
}

/* Check if a word is in dictionary */
int check(char *word, char **closest_word){
    int hash_tmp;
    hash_node *curr;
    int i = 0;
    //  Create a key for the tmp by putting it in the hash function
    hash_tmp = hash_func(word);
    // set the new hashed tmp to a curr node
    curr = hashTable->nodes[hash_tmp];
    // While the node is not null see if the new node is equal to the tmp
    while(curr != NULL){
        // add all closest word to a list
        closest_word[i] = curr->word;
        i++;
        if(strcmp(word, curr->word) == 0){
            return 0;
        }
        curr = curr->next;
    }
    return i;
}

/* Correct a word using levenshtein distance */
char *correct(char *word, char **closest_word, int nb_word){
    int dist = 4; // maximum distance to the word
    int new_dist;
    char *res = word;
    for(int i = 1; i < nb_word; i++){
        // perform for each word levenshtein distance algorithm
        new_dist = levenshtein(word, (int) strlen(word), closest_word[i], (int) strlen(closest_word[i]));
        if(new_dist == 0)
            return word;
        // keep the low distanced word
        if(new_dist < dist){
            dist = new_dist;
            res = closest_word[i];
        }
    }
    return res;
}

/* Calculate the levenshtein distance between two words */
int levenshtein(const char *s, int ls, const char *t, int lt){
    int a, b, c;

    // if either string is empty, difference is inserting all chars from the other
    if(!ls)
        return lt;
    if(!lt)
        return ls;

    // if last letters are the same, the difference is whatever is required to edit the rest of the strings
    if(s[ls - 1] == t[lt - 1])
        return levenshtein(s, ls - 1, t, lt - 1);

    // try changing last letter of s to that of t;
    // or remove last letter of s;
    // or remove last letter of t, any of which is 1 edit plus editing the rest of the strings
    a = levenshtein(s, ls - 1, t, lt - 1);
    b = levenshtein(s, ls, t, lt - 1);
    c = levenshtein(s, ls - 1, t, lt);

    if(a > b)
        a = b;
    if(a > c)
        a = c;

    return a + 1;
}