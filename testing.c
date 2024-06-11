#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include <ctype.h> 

#define MAX_WORD_LENGTH 50
#define HASH_SIZE 1000

typedef struct {
    char key[100]; // Assuming a maximum string length of 100 characters
    int value;
} KeyValuePair;

KeyValuePair hashTable[HASH_SIZE];


int main() {
    /* int hash(const char* str)
    requires a string
    returns the hash value of the string
    glassbox testing
    */
    printf("if string is hi, it returns 2 %d", hash("hi"));
    /* void addToHashTable(const char* str)
    requires a string
    calculates the hash value for the input string using the hash function,
    using it as an index to store the string and an int value of 1
    */
    KeyValuePair hashTable[HASH_SIZE];
    addToHashTable("hi");
    printf("Index 0: Key = %s, Value = %d\n",hashTable[0].key, hashTable[0].value);
    /* int spellUsed(const char* str)
    requires a string
    returns 1 if the string exists in the hashtable and 0 otherwise
    */
    printf("checking if the word hi is in the hashtable (hint: the number is going to be 1 if found): %d", spellUsed("hi"));
    /* int checkWordInArray(char* word, char** wordArray, int size)
    requires a string, a list to be searched inside for the given string and the size of the list
    returns 0 if the string is found in the list and 1 otherwise
    */
    char arr[3];
    strcpy(arr[0], "hi");
    strcpy(arr[1], "hello");
    strcpy(arr[2], "hola");
    
    printf("checking if the word hi is in the list (hint: the number is going to be 0 if found): %d", checkWordInArray("hi",arr,3));
    /* void getName(char *name)
    requires a string
    prompts the user to type his name and saves it to a local variable called name
    */
    char name[11];
    getName(name);
    /* int toss()
    returns 1 if its the player turn and 0 if its the bot/ second player's turn
    */
    printf("1 for heads and 0 for tails. I shall now toss the coin: %d", toss());
    /* char** displaySpells(int indices[], int* size)
    requires an array to store indices of the starting position of each letter (a to z) and a pointer to an integer to store the size
    of the dynamic array
    reads the spells.txt file and returns an array filled with the spells by alphabetical order
    */
    int indices[27];    //list of indices
    int size;
    char** spells = displaySpells(indices, &size); //list of spells
    printf("All Spells:\n");
    for (int i = 0; i < size; i++) {
        printf("%-20s", spells[i]);
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    /*int user(char *spell, char* words, int size , char *previous)
        requires the spell casted and the list of spells and the size of spell and the previously casted spell
        returns: 1 if the spell has already been casted 
                2 if the spell casted isnt from the list of spells
                3 if the spell casted doesnt start with the last letter of previous
                0 otherwise
                glassbox testing
   */
    printf("\n\n previous is accio and casted spell is avis: %d ", user("avis", spells, 5, "accio"));
    printf("\n\n previous is accio and casted spell is obliviate: %d ", user("obliviate", spells, 5, "accio"));
    hash("obliviate");
    printf("\n\n previous is accio and casted spell is obliviate : %d ", user("obliviate", spells, 5, "accio"));

    /*void supportUser(int indices[], char* words, char last_ch)
            requires the array containing the indices, the list of spells and a character 
            returns, prints the spells in the list that start with that letter
    */
   supportUser(indices, spells, 'a');
    /*int check_conditions(int indices[], int countingarray[], char spell[])
            requires the array of indices, countingarray and the casted spell
            returns: 4 if after casting a spell there will be no more words to satisfy the games condition
                    0 if there is spells left to choose from */
    int countingarray[27];
    printf("checking if the conditions are met (hint: 4 if there is no more words to satisfy the games condition after casting the current spell): %d", check_conditions(indices,countingarray,"accio"));
    /*int Start_Finish(char** spells, char last_letter)
        requires the list of spells and a character
        returns the an array where array[0]=index at which the words that start with the "last_letter" begin and at array[1] its where those word end 
        glassbox testing 
    */
    int * index = Start_Finish(spells, 'e');
    printf("\n\n the words that start with the letter e start on index: %d and end on index: %d\n", index[0], index[1]);
    /* char* bot_starts(char** spells)
    requires a list of spells
    returns the first element in spells*/
    printf("%d is the first element in the spells list", bot_starts(spells));
    /* char* easy_bot(char** spells, char* previous_spell)
    requires the list of spells and the previously casted spell 
    returns the first spell in the list if bot is starting otherwise it returns the first spell it encounters that starts with the last letter of previous spell and hasn't been used */
    printf("The easy bot will return the spell %s if the previous spell was accio.", easy_bot(spells, "accio"));
    /*int counter(char* spells, char letter)
            requires the list of spells and a character 
            returns the number of unused spells that start with that character 
            glassbox testing 
    
    */
    printf("\n\n there is %d unused spells that start with the letter a\n", counter(spells, 'a'));
    /*char bot_replies(char** spells , char last_letter)
        requires the list of spells and the previous spell
        returns the spell that will lead to the smallest number of options for the oponent to play
        glassbox testing 
    */
   printf("\n\n when the oponent casts the spell duro and no other spell has been casted bot returns %s", bot_replies(spells, 'o'));
   /*char medium_bot(char** spells , char* previous_spell)
            requires the list of spells and the previously casted spell
            returns a spell satisfying the conditions of the game 
   */
     printf("The medium bot will return the spell %s if the previous spell was accio.", medium_bot(spells, "accio"));
    /* int checkWinningMove(char** words, int size,char* curr)
    requires a list of words and the size of the list and a word
    checks if there is a word in the array that starts with the last letter of the current word that has not been used before. return 1 if there is no such word 
    indicating that the current player has won and 0 otherwise*/
    printf("Checking if you casting the spell accio wins you the game (hint: if its 1 you won): %d"),checkWinningMove(spells, 78, "accio");
    /* int minimax(char** words, int size, char* prev, char* curr, int depth, int isBot)
    requires an array of words, the size of the array, the previously casted spell, the current casted spell, an integer revealing the depth of the search and a flag indicating if it is the bot's turn
    the algorithm is recursive, it explores all the possible moves up to a certain depth and assigns a score to each move. The score is based on whether the move leads to a win or a loss and how many
    moves it takes to reach that outcome, then returns the score of the best move*/
    int score = minimax(spells, 78, "accio", "obliviate", 10, 0);
    printf("The score of a previously casted spell accio then casting the spell obliviate grants you a score of: %d", score);
    /* char* castSpell_farine(char** spells, char* prev)
    requires an array of spells and the previously casted spell
    it uses the minimax function to determine the best move for the bot. if there is a winning move available, it returns that winning move spell. Otherwise, it seaches for the best move using the minimax algorithm
    if there are multiple moves with the same score, it chooses the move that leads to the fewest number of winning moves for the opponent.*/
    printf("the castSpell_farine will return the spell %s if the previous spell was accio.", castSpell_farine(spells, "accio"));
    /*void play()
        requires nothing
        returns nothing 
        this is where the game is happening
     */
}