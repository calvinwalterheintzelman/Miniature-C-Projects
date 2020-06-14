#include "shuffle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// You may add more functions in this file.

void _print_cards(CardDeck printed_deck, CardDeck upper_deck, CardDeck lower_deck, int printed_deck_counter, int upper_deck_counter, int lower_deck_counter); //used for recursive calls

void divide(CardDeck orig_deck, CardDeck* upper_deck, CardDeck* lower_deck)
{
	for(int i1 = 0; i1 < orig_deck.size - 1; i1++)
	{
		upper_deck[i1].size = 0;
		lower_deck[i1].size = 0;
		int i2 = 0;
		int i3 = 0;

		while(i2 <= i1)
		{
			upper_deck[i1].cards[i2] = orig_deck.cards[i2];
			i2++;
			upper_deck[i1].size += 1; 
		}

		while(i2 < orig_deck.size)
		{
			lower_deck[i1].cards[i3] = orig_deck.cards[i2];
			i2++;
			i3++;
			lower_deck[i1].size += 1;
		}
		
		//interleave(upper_deck[i1], lower_deck[i1]);
	}
}

void interleave(CardDeck upper_deck, CardDeck lower_deck)
{	
	CardDeck printed_deck = {.size = upper_deck.size + lower_deck.size};
	int printed_deck_counter = 0;
	int upper_deck_counter = 0;
	int lower_deck_counter = 0;

	_print_cards(printed_deck, upper_deck, lower_deck, printed_deck_counter, upper_deck_counter, lower_deck_counter);
}

void shuffle(CardDeck orig_deck)
{
	int numpairs = orig_deck.size - 1;
	
	CardDeck * upper_deck = malloc(sizeof(*upper_deck) * numpairs);
	CardDeck * lower_deck = malloc(sizeof(*lower_deck) * numpairs);

	divide(orig_deck, upper_deck, lower_deck);
	for(int i = 0; i < numpairs; i++)
	{
		interleave(upper_deck[i], lower_deck[i]);
	}

	free(upper_deck);
	free(lower_deck);
}

void _print_cards(CardDeck printed_deck, CardDeck upper_deck, CardDeck lower_deck, int printed_deck_counter, int upper_deck_counter, int lower_deck_counter)
{
	if(printed_deck_counter < printed_deck.size)
	{
		if(upper_deck_counter < upper_deck.size) //puts a char value of upper deck into printed_deck
		{
			printed_deck.cards[printed_deck_counter] = upper_deck.cards[upper_deck_counter];
			_print_cards(printed_deck, upper_deck, lower_deck, printed_deck_counter + 1, upper_deck_counter + 1, lower_deck_counter);
		}

		if(lower_deck_counter < lower_deck.size) //puts a char value of lower deck into printed_deck
		{
			printed_deck.cards[printed_deck_counter] = lower_deck.cards[lower_deck_counter];
			_print_cards(printed_deck, upper_deck, lower_deck, printed_deck_counter + 1, upper_deck_counter, lower_deck_counter + 1);
		}
	}
	else
	{
		print_deck(printed_deck);
		printf("\n");
	}
}
