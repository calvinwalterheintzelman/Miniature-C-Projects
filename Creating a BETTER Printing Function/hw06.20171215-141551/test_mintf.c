#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "mintf.h"

int main (int argc, char** argv){
	//NOTE: the actual output should explain what each test was used for.
	
	//general
	mintf("I'm itchy!!!");
	mintf("");
	mintf("noskip");
	mintf("\n");
	mintf("This should be on a line by itself.\n");
	mintf("4206969");
	
	//use of %
	mintf("\nThere should be one %% sign; is there?\n");
	mintf("YES! THERE IS ONLY ONE!!!!");
	mintf("\n %z %a %D %~ %C % %. %");

	//ints
	mintf("\nThis should print one and an exclamation mark: %d!", 1);
	mintf("\nThis should print ten, nine, eleven, and an exclamation mark: %d %d %d!", 10, 9, 11);
	mintf("\nPrint negative one, nine, ten, eleven, zero, and a tilde: %d %d %d %d %d~", -1, -9, -10, -11, 0);
	printf("\n(This is printf) The highest int is: %d.\n", INT_MAX);
	printf("(This is printf) The lowest int is: %d.\n", INT_MIN);
	mintf("(back to mintf) highest int: %d.\nlowest int: %d.\n", INT_MAX, INT_MIN);

	//hex
	mintf("\nhex time!\nhighest int: %x.\nlowest int: %x.\nzero: %x.\none: %x.\nnegative one: %x.\n", INT_MAX, INT_MIN, 0, 1, -1);

	//binary
	mintf("\n_BINARY_\nhigh: %b.\nlow: %b.\nzero: %b.\none: %b.\nnegative one: %b.\n", INT_MAX, INT_MIN, 0, 1, -1);

	//doubles
	mintf("\n$doubles$\nhigh: %$.\nlow: %$.\nzero: %$.\none: %$.\nnegative one: %$.\n", INT_MAX + .9999, INT_MIN - .9999, 0.0, 1.0, -1.0);
	mintf("$265.44 == %$, -$345.54 == %$, $3.21 == %$.", 265.4456, -345.543, 3.219999);
	mintf("\n$40.00 == %$, -$765.99 == %$, $9.97 == %$.\n", 40.0, -765.993, 9.970);

	//strings
	mintf("\n~Strings~\n");
	mintf("Tissue Paper. == %s.\n", "Tissue Paper");
	mintf("%s", "There should be 2 empty lines following this line.");
	mintf("%s", "\n\n");
	mintf("\nthe%stly two l%s", "re was exac", "ines.");
	mintf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", "\n", "Each", " ", "string", " ", "is", " ", "a", " ", "word", " ", "or", " ", "space.", "\n");

	//characters
	mintf("\n/Characters\\\n");
	mintf("%c%c%c%c%c%c%c", 'C', 'a', 'l', 'v', 'i', 'n', '\n');
	mintf("The last letter of the alphabet is (capitalized): %c! Yay! :D\n", 'Z');

	//combinations
	mintf("%c%s have all %d percent signs (%%) options together. It cost me %$, so my computer bytes say %x or %b %cents.\n", '\n', "This should", 6, 2.4, 240, 240, 'c');

	//use of too many parameters
	mintf("\nThis has too many parameters.\n", 235, 'c', "blah blah", '%', "hahaIbetyou'retryingfairlyhardtoreadthisorsomething;)");

	//when the parameters don't match the type but should still work
	mintf("\noDdBaLlS\n");
	mintf("char a == %c, 97 == %d\n", 97, 'a');
	mintf("char c == %c, 0x63 == %x\n", 99, 99);
	mintf("char e == %c, 0b1100101 == %b\n", 101, 101);

	return EXIT_SUCCESS;
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
