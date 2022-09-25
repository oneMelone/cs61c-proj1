/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

#define WORD_LEN 61

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

int checkChar(char c) {
  if (c >= 'a' && c <= 'z') return 1;
  if (c >= 'A' && c <= 'Z') return 1;
  return 0;
}

/**
 * Check word is or is not in dictionary, print in different ways.
*/
void checkAndPrint(char *word) {
  int len = strnlen(word, WORD_LEN);
  char wordlower[len + 1];
  char wordlowernofirst[len + 1];
  for (int i = 0; i < len; i++) {
	if (word[i] >='A' && word[i] <= 'Z') {
	  wordlower[i] = word[i] + 32;
	} else {
	  wordlower[i] = word[i];
	}
  }
  wordlower[len] = 0;
  wordlowernofirst[0] = word[0];
  for (int i = 1; i < len; i++) {
	if (word[i] >='A' && word[i] <= 'Z') {
	  wordlowernofirst[i] = word[i] + 32;
	} else {
	  wordlowernofirst[i] = word[i];
	}

  }
  wordlowernofirst[len] = 0;

  void *wordexist = findData(dictionary, word);
  void *wordlowerexist = findData(dictionary, wordlower);
  void *wordlowernofirstexist = findData(dictionary, wordlowernofirst);
  if (wordexist != NULL || wordlowerexist != NULL || wordlowernofirstexist != NULL) {
	// exist in dictionary
	printf("%s", word);
  } else {
	// doesn't exist in dictionary
	printf("%s [sic]", word);
  }
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s) {
  char *string = (char *)s;
  unsigned int hashValue = 0;
  while (*string != 0) {
    hashValue = hashValue * 13 + *string;
	string = string + 1;
  }
  return hashValue;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  return !strncmp(string1, string2, 2250); 
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName) {
  FILE* fp = fopen(dictName, "r");
  if (fp == NULL) {
	fprintf(stderr, "The dictionary isn't exist\n");
	exit(1);
  }
  char *input = (char *)malloc(sizeof(char) * WORD_LEN);
  while (fscanf(fp, "%s", input) != EOF) {
	insertData(dictionary, input, input);
	input = (char *)malloc(sizeof(char) * WORD_LEN);
  }
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  char word[WORD_LEN];
  int ptr = 0;
  while (1) {
	char c = getchar();
	if (c == EOF) {
	  if (ptr == 0) return;
	  else {
		word[ptr] = 0;
		checkAndPrint(word);
		return;
	  }
	}

	if (!checkChar(c)) {
	  if (ptr != 0) {
		word[ptr] = 0;
		checkAndPrint(word);
		ptr = 0;
	  }
	  putchar(c);
	  continue;
    }

	word[ptr++] = c;
  }
}


