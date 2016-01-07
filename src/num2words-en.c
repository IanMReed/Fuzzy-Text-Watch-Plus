#include "num2words-en.h"
#include "string.h"
//#include <stdio.h>

static const char* const ONES[] = {
  "zero",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

static const char* const TEENS[] ={
  "",
  "eleven",
  "twelve",
  "thirteen",
  "fourteen",
  "fifteen",
  "sixteen",
  "seventeen",
  "eighteen",
  "nineteen"
};

static const char* const TENS[] = {
  "",
  "ten",
  "twenty",
  "thirty",
  "fourty",
  "fifty",
  "sixty",
  "seventy",
  "eighty",
  "ninety"
};

#define OCLOCK "o'clock"
#define QUARTER "quarter"
#define HALF "half"

#define PAST "past"
#define TO "to"

#define MAX_LEN 7

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }

  if (len > MAX_LEN)
  {
    int fullSize = strlen(words);
    words[fullSize - (len - MAX_LEN)] = 0;
    len = MAX_LEN;
  }

  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}


void time_to_words(int hours, int minutes, char* words, size_t length) {

  size_t remaining = length;
  memset(words, 0, length);

  // Fuzzy time
  minutes = (minutes + 3) / 5 * 5;

  // Handle minute wrapping
  if (minutes > 55)
  {
    minutes -= 60;
    hours++;
  }

  switch (minutes)
  {
    case 0:
      break;
    case 5:
    case 10:
    case 20:
      remaining -= append_number(words, minutes);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, PAST);
      remaining -= append_string(words, remaining, " ");
      break;
    case 15:
      remaining -= append_string(words, remaining, QUARTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, PAST);
      remaining -= append_string(words, remaining, " ");
      break;
    case 25:
      remaining -= append_number(words, 20);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_number(words, 5);
      remaining -= append_string(words, remaining, " ");    
      remaining -= append_string(words, remaining, PAST);
      remaining -= append_string(words, remaining, " ");    
      break;
    case 30:
      remaining -= append_string(words, remaining, HALF);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, PAST);
      remaining -= append_string(words, remaining, " ");
      break;
    case 35:
      remaining -= append_number(words, 20);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_number(words, 5);
      remaining -= append_string(words, remaining, " ");    
      remaining -= append_string(words, remaining, TO);
      remaining -= append_string(words, remaining, " ");    
      hours++;
      break;
    case 40:
    case 50:
    case 55:
      remaining -= append_number(words, 60 - minutes);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, TO);
      remaining -= append_string(words, remaining, " ");
      hours++;
      break;
    case 45:
      remaining -= append_string(words, remaining, QUARTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, TO);
      remaining -= append_string(words, remaining, " ");
      hours++;
      break;
  }

  // Handle hour wrapping
  hours += 12; // If hours == 0
  while (hours > 12)
  {
    hours -= 12;
  }

  remaining -= append_string(words, remaining, "*"); // Make hours bold
  remaining -= append_number(words, hours);
  remaining -= append_string(words, remaining, " ");

  // Add o'clock to whole hours
  if (minutes == 0)
  {
    remaining -= append_string(words, remaining, OCLOCK);
    remaining -= append_string(words, remaining, " ");    
  }
}

void time_to_greeting(int hour, char* greeting)
{
  if (hour < 5 || hour >= 22) {
    strcpy(greeting, "Good night ");
  } else if (hour < 11) {
#ifdef PBL_PLATFORM_CHALK
    strcpy(greeting, "Good morning ");
#else
    strcpy(greeting, "Good mor- ning ");
#endif
  } else if (hour < 17) {
#ifdef PBL_PLATFORM_CHALK
    strcpy(greeting, "Good day ");
#else
    strcpy(greeting, "Good  day ");
#endif
  } else {
    strcpy(greeting, "Good even- ing ");
  }
}
