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
#define STARTAMT 1000

// functions
void init(int *deck);
int shuffle(int *deck);
int drawPlayerCards(int *deck, int *cardIndex, int numCards);
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

int drawPlayerCards(int *deck, int *cardIndex, int numCards){
    int playerTotal = 0;

    /// draw first two player cards
    for(int i = 0; i < numCards; i++){

        if(*cardIndex >= MAXCARDS){
            printf("No more cards to draw.\n");
            return 0;
        }

        int card = deck[*cardIndex];
        printf("%s of %s\n", faces[card % 13], suits[card / 13]);

        char *face = faces[card % 13];

        // calculate int value from char face
        if(strcmp(face, "A") == 0){
            playerTotal += 1;
        } else if(strcmp(face, "J") == 0 || strcmp(face, "Q") == 0 || strcmp(face, "K") == 0){
            playerTotal += 10;
        } else{
            playerTotal += atoi(face);
        }
        (*cardIndex)++;
    }
    return playerTotal;
}

void play(int *deck, int bankAmt){
    int betAmt;
    int card, cardIndex;
    int playerTotal; 

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

    // Draw cards logic
    printf("Player's hands:\n");
    playerTotal = drawPlayerCards(deck, &cardIndex, 2);
    printf("Player Total: %d\n", playerTotal);

    // Draw dealer card

    // hit/stand/double, win or bust

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

    // DEBUG: testing shuffling 
    // do{
    //     if(card <= MAXCARDS){
    //         card = shuffle(deck);
    //     }

    //     printf("%s of %s\t", faces[deck[card] % 13], suits[deck[card] / 13]);
    //     card++;

    //     printf("Enter to deal, Q to quit\n");
    //     ch = getchar();

    // } while (toupper(ch) != 'Q'); 

    return 0; 
}