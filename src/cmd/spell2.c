#include "spell_check.h"

char *WORDS = "words.txt";
// maximum length for a word
const int MAX_WORD_LEN = 45;
const int NB_LINES = 323579;


int levenshtein(const char *s, int ls, const char *t, int lt){
    int a, b, c;

    /* if either string is empty, difference is inserting all chars
     * from the other
     */
    if (!ls) return lt;
    if (!lt) return ls;

    /* if last letters are the same, the difference is whatever is
     * required to edit the rest of the strings
     */
    if (s[ls - 1] == t[lt - 1])
        return levenshtein(s, ls - 1, t, lt - 1);

    /* else try:
     *      changing last letter of s to that of t; or
     *      remove last letter of s; or
     *      remove last letter of t,
     * any of which is 1 edit plus editing the rest of the strings
     */
    a = levenshtein(s, ls - 1, t, lt - 1);
    b = levenshtein(s, ls, t, lt - 1);
    c = levenshtein(s, ls - 1, t, lt);

    if (a > b) a = b;
    if (a > c) a = c;

    return a + 1;
}

char *correct(char *word, char **word_list){
    int dist = levenshtein(word, strlen(word), word_list[0], strlen(word_list[0]));
    int new_dist = dist;
    char *res = word_list[0];
    for(int i = 1; i < NB_LINES; i++){
        new_dist = levenshtein(word, strlen(word), word_list[i], strlen(word_list[i]));
        if(new_dist == 0)
            return word;
        if(new_dist < dist){
            dist = new_dist;
            res = word_list[i];
        }
    }
    return res;
}

char** spell_check(char **text, int nb_word){
    // load dictionary;
    char *dict = WORDS;
    char **word_list = calloc(NB_LINES + 1, sizeof(char));
    //printf("non");
    load(dict, word_list);
    //printf("oui");
    // spell-check each word
    for(int i = 0; i < nb_word; i++){
        printf("%s\n", text[i]);
        text[i] = correct(text[i], word_list);
        printf("%s\n", text[i]);
    }
    return text;
}

/* Loads dict into memory. */
void load(char *dict, char **word_list){
    char word [MAX_WORD_LEN +1];
    long i = 46570;

    // Open dictionary file
    FILE *dictfp = fopen(dict, "r");
    // Check to see if it exists
    if(dictfp == NULL)
        err(1, "Empty dictionnary");

    // Read in words from dictionary
    while(fscanf(dictfp, "%s", word) != EOF){
        printf("%s\n", word);
        if(i < 46572)
            word_list[i] = word;
        i ++;
    }
    fclose(dictfp);
}
