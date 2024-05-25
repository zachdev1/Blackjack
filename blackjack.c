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
void shuffle(int *deck);
int calculateCardValue(char *face, int currentTotal);
int drawPlayerCards(int *deck, int *cardIndex, int numCards, int playerTotal);
int drawDealerCards(int *deck, int *cardIndex, int numCards, int dealerTotal);
int hitOrStand(int *deck, int *cardIndex, int numOfCards, int currentTotal);
int dealerAdditionalCards(int *deck, int *cardIndex, int numOfCards, int currentTotal);
int determineOutcome(int playerTotal, int dealerTotal, int bankAmt, int betAmt);
int play(int *deck, int bankAmt);
int addMoney(int bankAmt); 

// faces and suits arrays
char *faces[MAXRANKS]= {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
char suits[MAXSUITS][MAX] = {"Clubs", "Diamonds", "Hearts", "Spades"};

// initialize deck of cards
void init(int *deck){
    for(int i = 0; i < MAXCARDS; i++){
        deck[i] = i; 
    }
}

// Fisher-Yates shuffling algorithm
void shuffle(int *deck) {
    for(int i = MAXCARDS - 1; i > 0; i--){
        int j = rand() % (i + 1);
        int tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

// calculate card value
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

// draw first 2 player cards
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
        printf("%s of %s\n", faces[card % 13], suits[card / 13]);

        face = faces[card % 13];

        // calculate int value from char face
        playerTotal += calculateCardValue(face, playerTotal);
        (*cardIndex)++;
    }
    if(playerTotal == 21){
        printf("BlackJack!\n");
    }
    return playerTotal;
}

// draw initial dealer card
int drawDealerCards(int *deck, int *cardIndex, int numCards, int dealerTotal){
    char *face;
    int card;

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
   
    return dealerTotal;
}

// player decision to hit or stand 
int hitOrStand(int *deck, int *cardIndex, int numOfCards, int playerTotal){
    char input;

    while(playerTotal < 21){
        printf("Hit or Stand (H/S):");
        scanf(" %c", &input);
        printf("\n");
        
        if(input == 'S'){
            break;
        } else if(input == 'H'){
            playerTotal = drawPlayerCards(deck, cardIndex, numOfCards, playerTotal);
            printf("Current total %d\n", playerTotal);
            if(playerTotal > 21){
                printf("Bust!\n");
                break;
            }
        } else{
            printf("Invalid command (H/S)\n");
        }
    }
    return playerTotal;
}

// deal additional dealer cards if total less than 17
int dealerAdditionalCards(int *deck, int *cardIndex, int numOfCards, int currentTotal){
    int card;
    char *face;

    if(*cardIndex >= MAXCARDS){
        printf("No more cards to draw.\n");
        return 0;
    }

    while(currentTotal < 17 ){

    card = deck[*cardIndex];
    printf("%s of %s\n", faces[card % 13], suits[card / 13]);

    face = faces[card % 13];

    // calculate int value from char face
    currentTotal += calculateCardValue(face, currentTotal);
    (*cardIndex)++;
    } 

    return currentTotal;
}

// determine the outcome 
int determineOutcome(int playerTotal, int dealerTotal, int bankAmt, int betAmt){
    if(playerTotal > 21){
        printf("Player Busts! Dealer wins.\n");
    } else if(dealerTotal > 21){
        printf("Dealer Busts! Player wins.\n");
        bankAmt += (2 *betAmt);
    } else if(playerTotal > dealerTotal){
        printf("Player wins!\n");
        bankAmt += (2 *betAmt);
    } else if(dealerTotal > playerTotal){
        printf("Dealer wins!\n");
    } else{
        printf("It's a tie!\n");
        bankAmt += betAmt;
    }
    return bankAmt;
}

// 
int play(int *deck, int bankAmt){
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
    printf("\n");

    /* ACTUAL GAME LOGIC*/
    shuffle(deck);

    // draw initial card logic
    printf("Player's hands:\n");
    playerTotal = drawPlayerCards(deck, &cardIndex, 2, playerTotal);
    printf("Player Total: %d\n", playerTotal);
    printf("\n");

    // draw dealer card
    printf("Dealer's hands:\n");
    dealerTotal = drawDealerCards(deck, &cardIndex, 1, dealerTotal);
    printf("Dealer Total %d\n", dealerTotal);
    printf("\n");

    // player hit/stand
    playerTotal = hitOrStand(deck, &cardIndex, 1, playerTotal);
    printf("Player Total %d\n", playerTotal);
    printf("\n");

    // draw additional dealer cards
    dealerTotal = dealerAdditionalCards(deck, &cardIndex, 1, dealerTotal);
    printf("Dealer Total %d\n", dealerTotal);
    printf("\n");

    // determine the outcome, update bank
    bankAmt = determineOutcome(playerTotal, dealerTotal, bankAmt, betAmt);

    printf("Current Bank Total: $%d\n", bankAmt);
    return bankAmt;
}

// add money to bank
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

    srand(time(NULL));

    init(deck); 
     
    while(input != 'Q'){
        printf("Enter your choice (P: Play, $: Add Money, Q: Quit): ");
        scanf(" %c", &input);
        switch(input){
            case 'P':
                bankAmt = play(deck, bankAmt);
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