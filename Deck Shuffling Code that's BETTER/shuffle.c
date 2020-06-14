#include "shuffle.h"
#include <stdio.h>
#include <stdlib.h>

// You can add more functions to this file.  Helper functions must start with '_'.
void _print_cards(CardDeck printed_deck, CardDeck upper_deck, CardDeck lower_deck, int printed_deck_counter, int upper_deck_counter, int lower_deck_counter); //used for recursive calls

void _one_shuffle(CardDeck printed_deck, CardDeck upper_deck, CardDeck lower_deck, int printed_deck_counter, int upper_deck_counter, int lower_deck_counter, int k); //used for recursive calls

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
	if(upper_deck == NULL || lower_deck == NULL)
	{
		free(upper_deck);
		free(lower_deck);
	}
	else
	{
	divide(orig_deck, upper_deck, lower_deck);
	for(int i = 0; i < numpairs; i++)
	{
		interleave(upper_deck[i], lower_deck[i]);
	}

	free(upper_deck);
	free(lower_deck);
	}
}

void repeat_shuffle(CardDeck orig_deck, int k)
{
	if(k <= 0)
	{
		printDeck(orig_deck);
	}
	else
	{
		int numpairs = orig_deck.size - 1;
		CardDeck * upper_deck = malloc(sizeof(*upper_deck) * numpairs);
		CardDeck * lower_deck = malloc(sizeof(*lower_deck) * numpairs);
		if(upper_deck == NULL || lower_deck == NULL)
		{
			free(upper_deck);
			free(lower_deck);
		}
		else
		{

		divide(orig_deck, upper_deck, lower_deck);
		for(int i = 0; i < numpairs; i++)
		{
			CardDeck printed_deck = {.size = upper_deck[i].size + lower_deck[i].size};
			int printed_deck_counter = 0;
			int upper_deck_counter = 0;
			int lower_deck_counter = 0;

			_one_shuffle(printed_deck, upper_deck[i], lower_deck[i], printed_deck_counter, upper_deck_counter, lower_deck_counter, k);
		}

		free(upper_deck);
		free(lower_deck);
		}
	}
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
		printDeck(printed_deck);
	}
}

void _one_shuffle(CardDeck printed_deck, CardDeck upper_deck, CardDeck lower_deck, int printed_deck_counter, int upper_deck_counter, int lower_deck_counter, int k)
{
	if(printed_deck_counter < printed_deck.size)
	{
		if(upper_deck_counter < upper_deck.size) //puts a char value of upper deck into printed_deck
		{
			printed_deck.cards[printed_deck_counter] = upper_deck.cards[upper_deck_counter];
			_one_shuffle(printed_deck, upper_deck, lower_deck, printed_deck_counter + 1, upper_deck_counter + 1, lower_deck_counter, k);
		}

		if(lower_deck_counter < lower_deck.size) //puts a char value of lower deck into printed_deck
		{
			printed_deck.cards[printed_deck_counter] = lower_deck.cards[lower_deck_counter];
			_one_shuffle(printed_deck, upper_deck, lower_deck, printed_deck_counter + 1, upper_deck_counter, lower_deck_counter + 1, k);
		}
	}
	else
	{
		if(k - 1 > 0)
		{
		repeat_shuffle(printed_deck, k - 1);
		}
		else
		{
			printDeck(printed_deck);
		}
	}
}
