#include <stdio.h>
#include <ctype.h> /* toupper */
#include <string.h> /* strlen */

#include <conio.h> /* console */
#include <graph.h> /* color */

#include "msg.h"

void
uppercase_str(char *str)
{
  int pos;

  /* convert a string to uppercase */

  for (pos = 0; str[pos]; pos++) {
    str[pos] = toupper( str[pos] );
  }
}

char *
read_guess(char *guess, int len)
{
  int ltr;
  int ch;

  /* use conio to read a string of length "len" */

  _settextcolor(14); /* br yellow */
  _setbkcolor(4); /* red */

  _settextwindow(3,2, 6,22); /* define the window */
  _clearscreen(_GWINDOW); /* just the window */

  /* print the prompt */

  _settextposition(2,5);
  _outtext("Make a guess");

  _settextposition(3,10-(len/2));
  for (ltr = 0; ltr < len; ltr++) {
    putch('_');
  }

  /* read the guess */

  ltr = 0;
  _settextposition(3,10-(len/2)); /* reset cursor */

  do {
    ch = getch();

    if (ch == 0) {
      getch(); /* clear the ext key */
    }

    /* convert to uppercase */

    if ((ch >= 'a') && (ch <= 'z')) {
      ch = 'A' + (ch - 'a');
    }

    if ((ch >= 'A') && (ch <= 'Z')) {
      _settextposition(3,ltr + 10-(len/2)); /* position cursor */
      putch(ch);

      guess[ltr++] = ch;
    }

    /* if backspace */

    if (ch == 8) { /* BS */
      guess[ltr--] = '\0';

      if (ltr < 0) {
	ltr = 0; /* prevent underflow */
      }

      _settextposition(3,ltr + 10-(len/2)); /* erase prev letter */
      putch('_');
      _settextposition(3,ltr + 10-(len/2)); /* re-position cursor */
    }
  } while (ltr < len);

  return guess;
}

void
print_letter(char letter, int color, int lettertop, int letterleft)
{
  char *p;

  p = &letter; /* pointer assignment */

  /* use conio to display the letter in a color */

  _setbkcolor(color);
  _settextcolor(15); /* br white */

  /* this character box starts at top,left */
  /* this is row,col coordinates .. note that rows 1,2,3 are 3 rows high */

  _settextwindow(lettertop, letterleft, lettertop+2, letterleft+2);
  _clearscreen(_GWINDOW);

  _settextposition(2,2);
  _outmem(p, 1);

  /* slight pause so we get a kind of animation effect */

  delay(100);
}

int
compare_words(const char *word, const char *guess, int wordtop, int wordleft)
{
  int matches = 0;
  int ltr;
  int pos;
  int color;

#define BLACK 0
#define GREEN 2
#define YELLOW 6

  /* compare two strings and display the differences */

  for (ltr = 0 ; (word[ltr]) && (guess[ltr]) ; ltr++) {
    if (word[ltr] == guess[ltr]) {
      /* same letter */

      color = GREEN;
      print_letter( guess[ltr], color, wordtop, wordleft+(ltr*4) );

      matches++;
    }
    else {
      /* does this letter appear elsewhere? */

      color = BLACK;

      for (pos = 0; word[pos]; pos++) {
	if (word[pos] == guess[ltr]) {
	  color = YELLOW;
	}
      } /* for */

      print_letter( guess[ltr], color, wordtop, wordleft+(ltr*4) );
    }
  } /* for */

  return matches;
}

int
main()
{
  char guess[6]; /* 5 letters plus \0 */

  char word[] = "WORDY";

  int guessnum;
  int matches = 0;

  /* set up the screen */

  if (_setvideomode(_TEXTC80) == 0) {
    puts("Cannot set video mode: 80x25 color");
    return 1;
  }

  _setbkcolor(1); /* blue */
  _clearscreen(_GCLEARSCREEN); /* whole screen */

  print_title("WORDY - a word puzzle game");

  /* TODO: pick a random word from the file */

  /* let the user make 6 guesses */

  for (guessnum = 0; (guessnum < 6) && (matches < 5); guessnum++) {
    read_guess(guess, 5); /* read the guess: 5 letters */
    uppercase_str(guess); /* convert to upper */

    /* assume boxes are 3x3 plus 1 col (or plus 1 row) between them */
    matches = compare_words(word, guess, 3+(guessnum*4), 30);
  }

  if (matches == 5) {
    print_title("You guessed the secret word!");
  }
  else {
    print_title("Sorry, you didn't guess the word");
  }

  /* done */

  if (getch() == 0) {
    getch(); /* call getch again to clear the ext character */
  }

  _setvideomode(_DEFAULTMODE);

  return 0;
}
