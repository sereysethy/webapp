/*
Utilities functions.

Copyright (C) 2017  Sereysethy Touch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

char * days[]   = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", \
                   "Sep", "Oct", "Nov", "Dec"};

/*
 * This function return current date time in GMT as following format
 * Wed, 09 Dec 2017 00:00:00 GMT
 * @return a pointer to a string containing date in GMT
 */
char * get_date() {
  time_t t = time(NULL);
  struct tm tm = *gmtime(&t);
  char * date;

  date = (char *) malloc(29);
  sprintf(date, "%s, %.2d %s %d %.2d:%.2d:%.2d GMT\0", days[tm.tm_wday], \
          tm.tm_mday, months[tm.tm_mon], tm.tm_year + 1900, tm.tm_hour,  \
          tm.tm_min, tm.tm_sec);
  return date;
}

/*
 * Generate a random string of length
 * @param int length of string to generate
 * @return a pointer to random string generated
 */
char * random_str(int length) {
  int i, rank;
  char alphanum[] = {'a', 'b', 'c', 'd', 'e', \
                     'f', 'g', 'h', 'i', 'j', \
                     'k', 'l', 'm', 'n', 'o', \
                     'p', 'q', 'r', 's', 't', \
                     'u', 'v', 'w', 'x', 'y', \
                     'z', '0', '1', '2', '3', \
                     '4', '5', '6', '7', '8', \
                     '9' };
  int size;
  char * buf;

  size = sizeof(alphanum);
  srand((unsigned int)time(NULL) * length);
  buf = (char *) malloc(length);

  for (i = 0; i < length; i++) {
    rank = rand() % size;
    buf[i] = alphanum[rank];
  }

  buf[i] = '\0';

  return buf;  
}

/*
 * Test if string is digit until a \0 is encountered
 * @return TRUE if buf is digit otherwise FALSE
 */
int is_number(char * buf) {
  int digit = TRUE;

  while ((*buf != 0) && digit) {
    if (!isdigit(*buf)) 
      digit = FALSE;
    else
      buf++;
  }

  return digit;
}

