// You may modify this file for testing but this file
// will not be submitted.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "shuffle.h"

int main(int argc, char ** argv) {
	char deck[MAX_SIZE] = {'A', '2', '3', '4', '5', '6', '7', '8'};

	CardDeck orig_deck = {
		.size = 2,
		.cards = {0}  // initialize all to 0 since it will be populated using memcpy(..)
	};
	memcpy(orig_deck.cards, deck, MAX_SIZE * sizeof(orig_deck.cards[0]));

	repeat_shuffle(orig_deck, 2);
	return EXIT_SUCCESS;
}
