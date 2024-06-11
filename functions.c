#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // for the toss function
#include <ctype.h> // to lower function

#define MAX_WORD_LENGTH 50
#define HASH_SIZE 1000

typedef struct {
    char key[100]; // Assuming a maximum string length of 100 characters
    int value;
} KeyValuePair;

KeyValuePair hashTable[HASH_SIZE]; // Hash table to check if spell used or not


int hash(const char* str) {
    int hash = 0;
    while (*str) {
        hash += *str;
        str++;
    }
    return hash % HASH_SIZE;
}

void addToHashTable(const char* str) {
    int index = hash(str);
    strcpy(hashTable[index].key, str);
    hashTable[index].value = 1;
}

int spellUsed(const char* str) {
    int index = hash(str);
    if (strcmp(hashTable[index].key, str) == 0) {
        return 1; // String exists
    }
    return 0; // String does not exist
}

int checkWordInArray(char* word, char** wordArray, int size) {  // checks if word is in the list of spells
     for(int i = 1; i<size; i++){
        if(strcasecmp(word,wordArray[i]) == 0){
            return 0;
        }
     }
     return 1;
}

void getName(char *name){
    printf("Enter your name: ");
    scanf("%s", name);
}

int toss(){     //this function for randomized turn 
    time_t t;
    srand((unsigned) time(&t));
    int coin = (int)(rand() % 10);
    //printf("%d", coin);
    if(coin<= 5){
        return 1;
    }
    else{
        return 0;
    }
}

char** displaySpells(int indices[], int* size){     // this function displays all the spells at the start of the round while saving all the spells in a dynamic array and filling the
    FILE* file = fopen("spells.txt", "r");          // indices array(indices array saves where the first word of a certain letter starts assuming the file is sorted in alphabetical order like the spells.txt given)
    if (file == NULL) {
        perror("Error opening file");
        return NULL; 
    }

    int numWords = 1;
    int missing_character_checker = 0;
    char** words = NULL;

    char buffer[MAX_WORD_LENGTH];
    char first = 'a';
    while (fgets(buffer, MAX_WORD_LENGTH, file) != NULL) {
        // Remove newline character if present
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0'; // Remove the newline
        }

        char* word = (char*)malloc(len + 1);
        if (word == NULL) {
            perror("Memory allocation error");
            return NULL; 
        }
        strcpy(word, buffer);
        if(word[0] != first){
            if(word[0] - 97 != missing_character_checker){
                for(int k = missing_character_checker; k<word[0] - 97;k++){
                    indices[k] = -1;
                }
                missing_character_checker = word[0] - 97;
            }
            first = word[0];
            indices[word[0] - 97] = numWords - 1;
            missing_character_checker++;
        }
        printf("%-20s ", word);
        if(numWords % 5 == 0){
            printf("\n");
        }

        words = (char**)realloc(words, (numWords + 1) * sizeof(char*));
        if (words == NULL) {
            perror("Memory allocation error");
            return NULL; 
        }
        words[numWords] = word;
        numWords++;
    }
    words[numWords] = NULL;
    *size = numWords;
    for(int k = missing_character_checker; k<27; k++){
        indices[k] = -1;
    }
    indices[0] = 0;
    //printf("%d", numWords);

    fclose(file);


    return words;
}
// runs when it's user 1's turn
int user(char *spell, char** words, int size , char *previous){
    if(spellUsed(spell) == 1){
        return 1;
    }
    if(checkWordInArray(spell,words, size) == 1){ // checks if word in list of spells 
        return 2;
    }
    if(strlen(previous) != 0){ // if it's not the first turn
        if(tolower(previous[strlen(previous) - 1]) != tolower(spell[0])){  //if last character in the previous word is not equal to last character in current word
            return 3;
        }
    }
    return 0;
}

// this function is used if user wants to be supported
void supportUser(int indices[], char** words, char last_ch){
    //if the character is not in the list the number in indices array will be -1
    if(indices[last_ch - 97] == -1){
        printf("No words to choose from :( \n");
        return;         
    }
    int m = 1;
    while(indices[last_ch + m - 97] == -1){
            m++;
        }
    printf("Words to choose from: ");
    for(int i = indices[last_ch - 97]; i<indices[last_ch - 97 + m]; i++){
        printf("%s ",words[i + 1]);
    }
    printf("\n");
}

int check_conditions(int indices[], int countingarray[], char spell[]) {
    int m = 1;
    while (indices[spell[strlen(spell) - 1] + m - 97] == -1) {
        m++;
    }

    if (indices[spell[strlen(spell) - 1] + m - 97] - indices[spell[strlen(spell) - 1] - 97] == countingarray[spell[strlen(spell) - 1] - 97] || indices[spell[strlen(spell) - 1] - 97] == -1) {
        return 4;
    }
    return 0; // Condition not met
}
int* Start_Finish(char** spells, char last_letter){
    //helper function that finds the indices of all the words that start with the last letter of previous in the list spells
    int* indices = malloc(2 * sizeof(int));
    int start=1;
    
    while(spells[start][0]!=last_letter){
        start++;
    }
    indices[0]=start;
    int end=start;
    while(spells[end][0]==last_letter)
        end++;
    indices[1]=end;
    return indices;
} 
char* bot_starts(char** spells){
    return spells[1];
}
//easy bot and its helper functions go here

char* easy_bot(char** spells, char* previous_spell){
    char* return_value;
    if(strlen(previous_spell) == 0){
        return_value = bot_starts(spells);
    }
    else {
        char last_letter = previous_spell[strlen(previous_spell) - 1];
        int* index = Start_Finish(spells, last_letter);
        for(int i = index[0]; i<index[1]; i++){
            if(spellUsed(spells[i]) == 0){
                return_value = spells[i];
                break;
            }
        }
    }

    return return_value;
}


//medium bot and all of its helper functions

int counter(char** spells, char letter){
    int start=1;
    int count=0;
    int size=1;
    while(spells[size] != NULL){
        size++;
    }
    for(int i=1; i<size; i++){
        if(spells[i][0]== letter){
            start=i;
            break;
        }
    }
    int end=start;
    for(int i=start+1; i<size; i++){
        if(spells[i][0]!= letter){
            end=i;
            break;
        }
    }
    if(start==end){
        return 0;
    }
    for(int i=start; i<end; i++){
        if(spellUsed(spells[i])==0){
            count++;
        }
    }
    return count;
}


char* bot_replies(char** spells , char last_letter){
    char** adequate_spells;
    adequate_spells = (char**) malloc(sizeof(char*)*10);
    int count=0;
    int* indices = Start_Finish(spells , last_letter);
    for(int i=indices[0]; i<=indices[1]; i++){
        if(spellUsed(spells[i])==0){
            adequate_spells[count]=spells[i];
            count++;
        }
    }
    if(count ==1){
        return adequate_spells[0];
    }
    int* counting;
    counting = (int*) malloc(sizeof(int) * 4);
    for(int i=0; i<count; i++){
        counting[i]= counter(spells, adequate_spells[i][strlen(adequate_spells[i])-1]);
    }
    int min=counting[0];
    int location=0;
    for(int i=0; i<count; i++){
        if(counting[i]<min){
            min= counting[i];
            location=i;
        }
    }
    free(counting);
    return adequate_spells[location];

}
char* medium_bot(char** spells , char* previous_spell){
    char* return_value;
    if(strlen(previous_spell)== 0){
        return_value = bot_starts(spells);
    }
    else{
        char last_letter=previous_spell[strlen(previous_spell) - 1];
        return_value = bot_replies(spells, last_letter);
    }
    return return_value;
}

//hard bot and helper functions(take this bot and all its helper functions for the fineeq battle royale) (checkwinningmove, minimax, hardbot , spellUsed)

// when we dont find a valid spell that starts with the last character of the current word.
int checkWinningMove(char** words, int size,char* curr) {
    int i;
    int j;
    for (i = 1; i < size; i++) {
        if (words[i] != NULL && words[i][0] == curr[strlen(curr) - 1] && spellUsed(curr) == 0) {
             break;
        }
    }
   
    if (i == size) {
        return 1;
    }
    return 0;
}
int minimax(char** words, int size, char* prev, char* curr, int depth, int isBot) {
    if (depth == 0) {
        return isBot ? 1000000 : -1000000;
    }

    int score;
    if (isBot) {
        score = -1000;
        for (int i = 1; i < size; i++) {
            if (words[i] != NULL && words[i][0] == prev[strlen(prev) - 1]) {
                char* next = words[i];
                words[i] = NULL;
                int nextScore = minimax(words, size, next, curr, depth - 1, 0);
                words[i] = next;
                if (nextScore > score) {
                    score = nextScore;
                }
            }
        }
        if (checkWinningMove(words, size, curr)) {
            score = -10000000;
        }
    } else {
        score = 1000;
        for (int i = 1; i < size; i++) {
            if (words[i] != NULL && words[i][0] == curr[strlen(curr) - 1]) {
                char* next = words[i];
                words[i] = NULL;
                int nextScore = minimax(words, size, prev, next, depth - 1, 1);
                words[i] = next;
                if (nextScore < score) {
                    score = nextScore;
                }
            }
        }
        if (checkWinningMove(words, size, curr)) {
            score = 10000000;
        }
    }

    return score;
}

char* castSpell_farine(char** spells, char* prev) {
    int size=1;
    while(spells[size] != NULL){
        size++;
    }
    //printf(" youhooo %d  %d   ", (int)(sizeof(spells)/sizeof(spells[0])), size);
    if(strlen(prev) != 0){
        char* bestSpell = NULL;
        int bestScore = -1000000;

        for (int i = 1; i < size; i++) {
            if (spells[i] != NULL && spells[i][0] == prev[strlen(prev) - 1] && spellUsed(spells[i]) == 0) {
                char* curr = spells[i];
                spells[i] = NULL;
                int score = minimax(spells, size - 1, prev, curr, 10, 0);           //10 is the depth. the larger the depth the better the search but I do not think more than 10 is needed
                //printf("%s %d\n", curr, score); this prints the score of everyword if curious
                spells[i] = curr;
                if (score > bestScore) {
                    bestScore = score;
                    bestSpell = curr;
                }
            }
        }
        if(bestSpell == NULL){                  // this if statement is entered if all choices have same score
            int bestchoice = 0;
            int finalchoice = 999999;
            int i;
            int j;
            for(i = 1; i<size; i++){
                if(spells[i] != NULL && spells[i][0] == prev[strlen(prev) - 1] && spellUsed(spells[i]) == 0){
                    for(j = 1; j<size;j++){
                        if(spells[j] != NULL && spells[j][0] == spells[i][strlen(spells[i]) - 1] && spellUsed(spells[j]) == 0){
                            if(checkWinningMove(spells,size,spells[j])){
                                bestchoice = 999999;
                                break;
                            }
                            bestchoice++;
                        }
                    }
                    if(bestchoice <= finalchoice){
                        finalchoice = bestchoice;
                        bestSpell = spells[i];
                    }
                    bestchoice = 0;
                }
            }
        }
        return bestSpell;
    }
    else{
        // when previous is null (the bot throws a spell first)loop through all the spells and check for a spell that will end the game immediately, if not found just return the first spell in the list of spells
        for(int z = 1; z<size; z++){
            if(checkWinningMove(spells, size,spells[z])){
                return spells[z];
            }
        }

        return spells[1];
    }
}





void play(){
    char name[11];
    int helpUser;
    int bot_difficulty;
    printf("Would you like additional support during the game? If yes enter 1 | If no enter 0\nWe can provide assistance by presenting a list of spells for you to consider. For instance, if the spell is \"avis,\" we will show you all the spells starting with \"s.\" However, you are not limited to the displayed options and are free to choose any spell you desire.");
    scanf("%d",&helpUser);
    getName(name);
    printf("Hi %s \n", name);
    printf("Would you like to play against easy, medium, or hard bot? 0 for easy, 1 for medium, 2 for hard: ");
    scanf("%d", &bot_difficulty);
    printf("Now we will proceed in tossing a coin. %s is heads and bot is tails. \n\n", name);

    int indices[27];    //list of indices
    int countingarray[27];
    int size;
    char** spells = displaySpells(indices, &size); //list of spells
    
    char* input;
    input = (char*) malloc(4*sizeof(char));
    char* botReturn;
    botReturn= (char*) malloc(4*sizeof(char));
    char* spell;
    spell = (char*) malloc(4*sizeof(char));
    int turns = toss();
    int sum = 0;
    int y = 0;
    char* previous;
    previous= (char*) malloc(4*sizeof(char));
    if(turns == 1){
        printf("\n\n%s will start\n \nPlease enter a spell: ", name);
        scanf("%s", input);
        spell = input;
        y = user(spell,spells, size, previous);
    }
    else{
        if(bot_difficulty == 0){
            // easy bot goes here
        }
        else if (bot_difficulty == 1)
        {
            botReturn = medium_bot(spells, previous);
        }
        else{
            botReturn = castSpell_farine(spells,previous);
        }
        
        spell= botReturn;
        printf("\n\nBot will start\n\nBot entered the spell: %s\n", spell);
        
    }
    countingarray[spell[0] - 97]++;
    addToHashTable(spell);
    strcpy(previous,spell);
    sum = y + check_conditions(indices,countingarray,spell);
    while(sum == 0){
        if(turns == 1){
            if(helpUser == 1){
                supportUser(indices,spells,tolower(previous[strlen(previous) -1]));
            }
             if(bot_difficulty == 0){
                
                botReturn = easy_bot(spells, previous);
            }
            else if (bot_difficulty == 1)
            {
                botReturn = medium_bot(spells, previous);
            }
            else{
                botReturn = castSpell_farine(spells,previous);
            }
            spell= botReturn;
            printf("\n\n Bot enetered the spell: %s\n", spell);
            turns = 0;
            strcpy(previous,spell);
        }
        else{
            if(helpUser == 1){
                supportUser(indices,spells,tolower(previous[strlen(previous) -1]));
            }
            printf("%s please enter a spell: ", name);
            scanf("%s",input);
            spell = input;
            y = user(spell,spells, size , previous);
            turns = 1;
            strcpy(previous, spell);
        }
        countingarray[spell[0] - 97]++; //added here
        addToHashTable(spell);
        sum = y + check_conditions(indices,countingarray,spell);
    }
    if(sum == 4){
        if(turns == 0){
            printf("%s you have lost because no more spells in the list satisfy character matching condition. Bot won!!" , name);
        }
        else{
            printf("Bot lost because no more spells in the list satisfy character matching condition. Congratulations %s you have won!!" ,name);
        }
    }
    if(y == 1){
        printf("%s you have lost due to repeating the same word. Bot won!!", name);
    }
    if(y == 2){
        printf("%s you have lost due to picking a word not in the list. Bot won!!", name);
    }
     if(y == 3){
         printf("%s you have lost because you did not pick a word that starts with the last character of the previous word. Bot won!!", name);
    }

    return;
}

