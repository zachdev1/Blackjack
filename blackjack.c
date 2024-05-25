// includes 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include<windows.h>

// defines
#define MAX 9
#define MAXCARDS 52
#define MAXRANKS 13
#define MAXSUITS 4
#define STARTAMT 1000

// functions
void init(int *deck);
int shuffle(int *deck);
int calculateCardValue(char *face, int currentTotal);
int drawPlayerCards(int *deck, int *cardIndex, int numCards, int playerTotal);
int drawDealerCards(int *deck, int *cardIndex, int numCards);
int hitOrStand(int *deck, int *cardIndex, int numOfCards, int currentTotal);
void play(int *deck, int bankAmt);
int addMoney(int bankAmt); 

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

int calculateCardValue(char *face, int currentTotal){
    if(strcmp(face, "A") == 0){
        if(currentTotal + 11 > 21){
            return 1;
        } else{
            return 11;
        }
    } else if(strcmp(face, "J") == 0 || strcmp(face, "Q") == 0 || strcmp(face, "K") == 0){
        return 10;
    } else{
        return atoi(face);
    }
}

int drawPlayerCards(int *deck, int *cardIndex, int numCards, int playerTotal){
    char *face;
    int card = 0;

    /// draw first two player cards
    for(int i = 0; i < numCards; i++){

        if(*cardIndex >= MAXCARDS){
            printf("No more cards to draw.\n");
            return 0;
        }

        card = deck[*cardIndex];
        // Sleep(2000);
        printf("%s of %s\n", faces[card % 13], suits[card / 13]);

        face = faces[card % 13];

        // calculate int value from char face
        playerTotal += calculateCardValue(face, playerTotal);
        (*cardIndex)++;
    }
    return playerTotal;
}

int drawDealerCards(int *deck, int *cardIndex, int numCards){
    int dealerTotal = 0;
    char *face;
    int card, secondCard = 0;

    // draw first two dealer cards
    if(*cardIndex >= MAXCARDS){
        printf("No more cards to draw.\n");
        return 0;
    }

    card = deck[*cardIndex];
    printf("%s of %s\n", faces[card % 13], suits[card / 13]);

    face = faces[card % 13];

    // calculate int value from char face
    dealerTotal += calculateCardValue(face, dealerTotal);
    (*cardIndex)++;

    /*Implement second card logic*/
    
    return dealerTotal;
}


int hitOrStand(int *deck, int *cardIndex, int numOfCards, int currentTotal){
    char input;

    while(currentTotal < 21 ){
        printf("Hit or Stand (H/S):");
        scanf(" %c", &input);
        printf("\n");
        
        if(input == 'S'){
            break;
        } else if(input == 'H'){
            currentTotal = drawPlayerCards(deck, cardIndex, numOfCards, currentTotal);
            printf("current total: %d\n", currentTotal);

            if(currentTotal > 21){
                printf("Bust!\n");
                break;
            }
        } else{
            printf("Invalid command (H/S)\n");
        }
    }
    printf("\n");
    return currentTotal;
}

void play(int *deck, int bankAmt){
    int betAmt;
    int card, cardIndex;
    int playerTotal = 0, dealerTotal = 0; 

    printf("Current Bank Total: $%d\n", bankAmt);
    printf("Place Your Bet: ");

    // Bet logic
    while(1){
        scanf("%d", &betAmt);

        if(betAmt <= 0 || betAmt > bankAmt){
            printf("Invalid Bet of amount %d. Try again", betAmt);
        } else{
            break;
        }
    }
    bankAmt -= betAmt;
    printf("Your bet of $%d has been accepted.\n", betAmt);

    /* ACTUAL GAME LOGIC*/
    shuffle(deck);

    // Draw initial card logic
    printf("Player's hands:\n");
    playerTotal = drawPlayerCards(deck, &cardIndex, 2, playerTotal);
    printf("Player Total: %d\n", playerTotal);
    printf("\n");
    dealerTotal = drawDealerCards(deck, &cardIndex, 2);
    printf("Dealer Total %d\n", dealerTotal);
    printf("\n");

    // hit/stand/double, win or bust
    playerTotal = hitOrStand(deck, &cardIndex, 1, playerTotal);
    
    /*DEBUG*/
    printf("playerTotal after hit/stand: %d\n", playerTotal);
    

    printf("Current Bank Total: $%d\n", bankAmt);
    
}

int addMoney(int bankAmt){
    char input;
    int moneyToAdd; 

    while(input != 'N'){
        printf("Current bank amount: %d\n", bankAmt);

        printf("Do you want to add money (Y/N): ");
        scanf(" %c", &input); 

        if(input == 'Y'){
            printf("How much would you like to add: ");
            scanf(" %d", &moneyToAdd);
            bankAmt += moneyToAdd; 
        } else if(input == 'N'){
            
        } else{
            printf("Invalid command (Y/N)\n");
        }
    }
    return bankAmt; 
}

int main(){
    char ch;
    char input;
    int deck[MAXCARDS];
    int card = 0;
    int bankAmt = STARTAMT; 

    init(deck); 
     
    while(input != 'Q'){
        printf("Enter your choice (P: Play, $: Add Money, Q: Quit): ");
        scanf(" %c", &input);
        switch(input){
            case 'P':
                play(deck, STARTAMT);
                break;
            case '$':
                bankAmt = addMoney(bankAmt);
                break;
            case 'Q':
                exit(0); 
                break;
            default:
                printf("Not a valid command\n");
                break; 
        }
    }
    return 0; 
}