#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void play();
int checkWinningMove(char** words, int size,char* curr);
int hash(const char* str);
void addToHashTable(const char* str);
char* easy_bot(char** spells , char* previous_spell);
char* medium_bot(char** spells , char* previous_spell);
char* castSpell_farine(char** spells, char* prev);
int* Start_Finish(char** spells, char last_letter);
int counter(char** spells, char letter);
char* bot_starts(char** spells);
char* bot_replies(char** spells , char last_letter);
void getName(char *name);
int toss();
char** displaySpells(int indices[], int* numWords);
int user(char *spell, char** words , int size , char *previous);
int spellUsed(const char* spell);
int checkWordInArray(char* word, char** wordArray , int size);
void supportUser(int indices[], char** words, char last_ch);
int check_conditions(int indices[], int countingarray[], char spell[]);
int minimax(char** words, int size, char* prev, char* curr, int depth, int isBot);
#endif