// includes 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// defines
#define MAX 9
#define MAXCARDS 52
#define MAXRANKS 13
#define MAXSUITS 4

// functions
void init(int *deck);
int shuffle(int *deck);

// structs 
typedef struct{
    int *rank;
    char suit[MAX];
} cards; 


char *faces[MAXRANKS]= {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
char suits[MAXSUITS][MAX] = {"Clubs", "Diamonds", "Hearts", "Spades"};

void init(int *deck){
    for(int i = 0; i < MAXCARDS; i++){
        deck[i] = i; 
    }
}

int shuffle(int *deck){
    int card, tmp; 

    for(int i = 0; i < MAXCARDS; i++){
        card = rand() % (i + 1);
        tmp = deck[i];
        deck[i] = deck[card];
        deck[card] = tmp;
    }
    return 0; 
}

int main(){
    char ch; 
    int deck[MAXCARDS];
    int card = 0;

    init(deck); 


    // DEBUG: testing shuffling 
    do{
        if(card <= MAXCARDS){
            card = shuffle(deck);
        }

        printf("%s of %s\t", faces[deck[card] % 13], suits[deck[card] / 13]);
        card++;

        printf("Enter to deal, Q to quit\n");
        ch = getchar();

    } while (toupper(ch) != 'Q'); 

    return 0; 
}