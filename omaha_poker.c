#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "omaha_poker.h"

#define BIT_SET(v, n) (v |= BIT(n))
#define BIT_CLEAR(v, n) (v &= ~(BIT(n)))
#define BIT_FLIP(v, n) (v ^= BIT(n))

/* TODO: TASK 1 (1 point) : Define 2 global instances of struct player--one for each player */
struct player player1, player2;

/* Parse card from a 2 char input */
struct card parse(const char *card)
{
	/* TODO: TASK 2 (10 points): Implement this function. 
	 * The function accepts a 2-character array called card (e.g., 'AH')
	 * Create an instance of struct card and populate it with the correct enum values based on the input chars value and suit.
	 * return the card */
	/* Hint: Use a switch statement to get the value from card[0] and another switch statement to get suit from card[1] */
	
	struct card result;	// create a struct card instance

	switch(card[0]){
        case '2':
            result.val = TWO;
            break;
        case '3':
            result.val = THREE;
            break;
        case '4':
            result.val = FOUR;
            break;
        case '5':
            result.val = FIVE;
            break;
        case '6':
            result.val = SIX;
            break;
        case '7':
            result.val = SEVEN;
            break;
        case '8':
            result.val = EIGHT;
            break;
        case '9':
            result.val = NINE;
            break;
		case 'T':
            result.val = TEN;
            break;
        case 'J':
            result.val = JACK;
            break;
        case 'Q':
            result.val = QUEEN;
            break;
        case 'K':
            result.val = KING;
            break;
        case 'A':
            result.val = ACE;
            break;
	}
	switch(card[1]){
		case 'C':
            result.suit = CLUB;
            break;
        case 'D':
            result.suit = DIAMOND;
            break;
        case 'H':
            result.suit = HEART;
            break;
        case 'S':
            result.suit = SPADE;
            break;
	}
	// return parse_ref(card); 
	return result;
}

/* Count the number of occurrences of each card 2 through Ace */
void count_cards(struct hand *h)
{
	/* TODO: TASK 3 (6 points): Implement this function
	 * The function accepts a pointer to a hand that has its cards already setup.
	 * This function must iterate through the cards in the hand and count the number of times
	 * a card appears, and update the counter for that card */
	
	// initializing card_coutn arr
	int i = 0;
	for(i = 0; i < 13; i++){
		h->card_count[i] = 0;
	}

	for(i = 0; i < 5; i++){
		int index = h->cards[i].val - 2; // 2's index is 0
		h->card_count[index]++;
	}
}

int is_flush(struct hand *h)
{

	/* TODO: TASK 4 (6 points): Implement this function.
	 * Return 1 if suits of all 5 cards in the hand are the same, else return 0 */
	for(int i = 1; i < 5; i++){
		if(h->cards[i].suit != h->cards[0].suit){
			return 0;
		}
	}
	return 1; 
}

int is_straight(struct hand *h)
{
	/* TODO: TASK 5 (10 points): Implement this function. 
	 * Return 1 if the 5 cards in the hand form a sequence, else return 0
	   NOTE: You must handle the exception for A2345, where A is not 1 but 13 */

	/* HINT: The card_count in the hand structure contains the number of times 
	 * each card value appears in the hand. See if 5 consecutive cards have a card_count value of 1 */
	
	count_cards(h);	// count the cards in hand
	int first_found = 0;
	if(h->card_count[11] == 0 && h->card_count[12] == 1){	// check for A2345
		first_found = 1;
	}
	for(int i = 0; i < 13; i++){
		if(first_found == 0){
			if(h->card_count[i] > 1){
				return 0;
			}
			else if (h->card_count[i] == 1){	
				first_found = 1;		// found the first card
				/* printf("Number: %d is the first card\n", i+2); */
			}
		}
		else if(first_found < 5){	// first card already found
			if(h->card_count[i] == 1){	// consecutive cards
				first_found++;
				/* printf("Number: %d is the %d card\n", i+2, first_found); */
			}
			else{	// no consecutive cards
				return 0;
			}
		}
	}
	if(first_found >= 5){
		/* printf("Firstfound = %d\n", first_found); */
		return 1;
	}
	return 0;
}

int is_straight_flush(struct hand *h)
{
	/* TODO: TASK 6 (1 point): Implement this function.
	 * Return 1 if the hand is a straight and a flush, else return 0. 
	 * HINT: Use the already implemented functions for flush and straight */
	
	if(is_flush(h)){
		if(is_straight(h)){
			return 1;
		}
	}
	return 0;

}
		

/* This function print's a hand. Useful in debugging */
void print_hand(struct hand *h)
{
	//added for print
	const char* valueStrings[] = {
        "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
        "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE"
    };

    const char* suitStrings[] = {
        "CLUB", "DIAMOND", "HEART", "SPADE"
    };
	
	int i;
	for(i = 0; i < 5; i++) {
		enum Value currentValue = h->cards[i].val; // Change this to the desired card value
    	enum Suit currentSuit = h->cards[i].suit; // Change this to the desired suit

   	 	printf("%s ", valueStrings[currentValue - 1]);
    	printf(" %s || ", suitStrings[currentSuit]);
	}
	printf("\n");
}


/* This is the main function that converts the player's hand into weighted unsigned long number. 
It is a 56 bit vector as shown below (2 is the LSB and StraightFlush is the MSB) */
/* 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A, 22, 33, 44, 55, 66, 77, 88, 99, TT, JJ, QQ, KK, AA,
222, 333, 444, 555, 666, 777, 888, 999, TTT, JJJ, QQQ, KKK, AAA, Straight, Flush, FullHouse, 2222, 3333, 
4444, 5555, 6666, 7777, 8888, 9999, TTTT, JJJJ, QQQQ, KKKK, AAAA, StraightFlush */
/* The number of occurrences of each number in the hand is first calculated in count_cards. 
Then, depending on the numeber of occurrences, the corresponding bit in the bit-vector is set. 
In order to find the winner, a simple comparison of the bit vectors (i.e., unsigned long integers) will suffice! */
/* TODO: TASK 7 (25 points): Implement this function.
	* Given a hand, iterate through the cards and use the BIT macros to set the appropriate bit in the hand vector */
// return eval_strength_ref(h); 
void eval_strength(struct hand *h)
{
	// set up the bit vector
	bool pair = false;
	bool threeOfaKind = false;
	count_cards(h);
	for(int i = 0; i < 13; i++){
		if(h->card_count[i] == 1){
			//set ith bit to 1
			BIT_SET(h->vector, i);
		}
		else if(h->card_count[i] == 2){
			//set i + 13th bit to 1
			BIT_SET(h->vector, i+13);
			pair = true;
			//printf("%d is a pair \n", i+2);
		}
		else if(h->card_count[i] == 3){
			//set i + 26th bit to 1
			BIT_SET(h->vector, i+26);
			threeOfaKind = true;
			// printf("%d is a trio \n", i+2);
		}
		else if(h->card_count[i] == 4){
			//set i + 42th bit to 1
			BIT_SET(h->vector, i+42);
		}
	}
	if(is_straight(h)){
		if(h->card_count[12] == 1 && h->card_count[0] == 1){
			// printf("A2345 Detected\n");
			BIT_CLEAR(h->vector, 12);
		}
		BIT_SET(h->vector, 39);
		// set 39
	}
	if(pair == true && threeOfaKind == true){
		// printf("A Full house\n");
		// set 41 Full house
		BIT_SET(h->vector, 41);
	}
	if(is_flush(h)){
		BIT_SET(h->vector, 40);
		// printf("Flush\n");
		// set 40
	}
	if(is_straight_flush(h)){
		BIT_SET(h->vector, 55);
		// printf("Straight Flush\n");
		// set 55
	}

}

void print_best_hand(struct hand *h){
	for (int i = 0; i <= 55; i++) {
        // Check the value of the bit at position 'i'
        if (h->vector & (1ULL << i)) {
            putchar('1');
        } else {
            putchar('0');
        }
	
        if (i % 8 == 0) {
            putchar(' ');  // Add a space every 8 bits for readability
        }
    }

    printf("\n");
}


void eval_players_best_hand(struct player *p)
{
	/* TODO: TASK 8 (10 points): Implement this function. 
	 * For each possible hand the player can make, evaluate the strength of the hand (by calling eval_strength).
	 * Then, set the best_hand vector for the player to point to the strongest hand.
	 */
	p->best_hand = &p->hands[0];
	for(int i = 1; i < MAX_COMBINATIONS; i++){
		struct hand *current_hand = &p->hands[i];
		eval_strength(current_hand);
		eval_strength(p->best_hand);
		if (current_hand->vector > p->best_hand->vector){
			p->best_hand = current_hand;
		}
	}
	/* print_hand(p->best_hand);
	print_best_hand(p->best_hand); */
	
	
}

void copy_card(struct card *dst, struct card *src)
{
	/* TODO: TASK 9 (3 points): Implement this function. 
	 * copy the value and suit from the src card to the dst card. 
	 */
	dst->val = src->val;
	dst->suit = src->suit;
}


void initialize_player_omaha(struct player *p, struct card *player_cards, struct card *community_cards)
{

	/* TODO: TASK 10 (25 points): Given the player cards and the community cards, initialize the array of hands in the player structure. 
	 * There are a total of MAX_COMBINATIONS number of possible hands that the player can make.
	 * Initialize each of the MAX_COMBINATIONS number of hands with different 5 card combinations. 
	 * Use the copy_card function.
	 */
	struct hand temp;
 	int a,b,i,j,k;
	int counter = 0;
	for(a = 0; a < 3; a++){
		for(b = a+1; b < 4; b++){
			for(i = 0; i < 3; i++){
				for(j = i+1; j < 4; j++){
					for(k = j+1; k < 5; k++){
						copy_card(&p->hands[counter].cards[0], &player_cards[a]);
						copy_card(&p->hands[counter].cards[1], &player_cards[b]);
						copy_card(&p->hands[counter].cards[2], &community_cards[i]);
						copy_card(&p->hands[counter].cards[3], &community_cards[j]);
						copy_card(&p->hands[counter].cards[4], &community_cards[k]);
						counter++;
					}
				}
			}
		}
	}
	/* HINT: Start by zeroing out the player instance. Then, copy each unique combination into a hand in the hands array. 
	 * For example: P1 P2 C1 C2 C3 is one possible combination (i.e., first 2 cards of the player and first 3 cards from community cards). 
	 * So, copy player_cards[0] into hands[0].cards[0], player_cards[1] into hands[0].cards[1], community_cards[0] into hands[0].cards[2], 
	 * community_cards[1] into hands[0].cards[3] and community_cards[2] into hands[0].cards[4]. Repeat this process for each possible combination
	 * such as P1 P2 C1 C2 C4, P3 P4 C1 C2 C3, etc. 
	 * Implement helper functions as necessary. */
	// return initialize_player_omaha_ref(p, player_cards, community_cards);
}

/* Parse each hand in the input file, evaluate the strengths of hands and identify a winner by comparing the weighted vectors */
void process_input_omaha(FILE *fp)
{
	char p1[4][3];
	char p2[4][3];
	char comm[5][3];
	struct card p1_cards[4], p2_cards[4], community_cards[5];
	int i;

	while(fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s", 
		&p1[0][0], &p1[1][0], &p1[2][0], &p1[3][0], &p2[0][0], &p2[1][0], &p2[2][0], &p2[3][0], 
		&comm[0][0], &comm[1][0], &comm[2][0], &comm[3][0], &comm[4][0]) == 13) {

		memset(p1_cards, 0, sizeof(struct card) * 4);
		memset(p2_cards, 0, sizeof(struct card) * 4); 
		memset(community_cards, 0, sizeof(struct card) * 5);

		for(i = 0; i < 4; i++) {
			p1_cards[i] = parse(&p1[i][0]);
			p2_cards[i] = parse(&p2[i][0]);
		}

		for(i = 0; i < 5; i++) 
			community_cards[i] = parse(&comm[i][0]);
		
		/* TODO: TASK 11 (1 point): Initialize each player with p1_cards and community_cards by calling initialize_player_omaha */
		struct player *player1 = (struct player*)malloc(sizeof(struct player));
		struct player *player2 = (struct player*)malloc(sizeof(struct player)); 

		initialize_player_omaha(player1, p1_cards, community_cards);
		initialize_player_omaha(player2, p2_cards, community_cards);
		/* TODO: TASK 12 (1 point): Evaluate the best hand for a player by calling eval_players_best_hand */
		eval_players_best_hand(player1);
		eval_players_best_hand(player2);
		/* TODO: TASK 13 (1 point): Depending on who has the stronger hand, print the winner. 
		 * If both have the same strong hand, print "No single winner" */
		eval_strength(player1->best_hand);
		eval_strength(player2->best_hand);
		if(player1->best_hand->vector > player2->best_hand->vector){
			printf("Player 1 wins\n");
		}
		else if(player1->best_hand->vector < player2->best_hand->vector ){
			printf("Player 2 wins\n");
		}
		else{
			printf("No single winner\n");
		}
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;

	if(argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
		printf("Unable to open input file\n");
		exit(-1);
	}
	
	process_input_omaha(fp);
	
	return 0;
}
	
