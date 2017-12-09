/*
Header files.

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

#define TRUE 1
#define FALSE 0
#define MAX_BUF 1024

#define DEFAULT_PORT 8080

#define HTTP_GET "GET"

char * get_date();
char * random_str(int length);
int is_number(char * buf);