/*
An Incomplete Simple Web Server.

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

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>

#include "header.h"

/* Global variable */
int server_sockfd;
int running = TRUE;

/*
 * Signal handler
 */
void close_socket() {
  close(server_sockfd);
  running = FALSE;
}

/*
 * HTTP Response Status
 * 
 * HTTP/1.1 200 OK
 * Date: Wed, 06 Dec 2017 18:22:06 GMT
 * Server: Apache/2.4.10 (Debian)
 * Last-Modified: Mon, 28 Nov 2016 09:21:54 GMT
 * ETag: "2b-5425900a6ef18"
 * Accept-Ranges: bytes
 * Content-Length: 43
 * Content-Type: text/html
 * 
 * @param int payload_length size of payload
 * @param int * n store the size of header in return
 * @return pointer to new header
 */
char * get_header(int payload_length, int * n) {
  char * tmp = NULL;
  char * str = NULL;
  char * date = NULL;
  char * etag = NULL;
  int length = 0;

  str = (char *) malloc(MAX_BUF);

  tmp = str;
  strncpy(tmp, "HTTP/1.1 200 OK\n", 16);
  tmp = tmp + 16;
  date = get_date();
  sprintf(tmp, "Date: %s\n", date);
  tmp = tmp + 36;
  strncpy(tmp, "Server: Citadel (Namur)\n", 24);
  tmp = tmp + 24;
  sprintf(tmp, "Last-Modified: %s\n", date);
  tmp = tmp + 45;
  etag = random_str(16);
  sprintf(tmp, "ETag: \"%s\"\n", etag);
  tmp = tmp + 25;
  strncpy(tmp, "Accept-Ranges: bytes\n", 21);
  tmp = tmp + 21;
  length = sprintf(tmp, "Content-Length: %d\n", payload_length);
  tmp = tmp + length;
  strncpy(tmp, "Content-Type: text/html; charset=UTF-8\n", 39);
  tmp = tmp + 39;
  strncpy(tmp, "Connection: close\n", 18);
  tmp = tmp + 18;
  *tmp = 10;  // line feed \r
  tmp++;

  *tmp = '\0';
  *n = strlen(str);

  free(date);
  free(etag);
  return str;
}

/*
 * Payload - string
 */
char * get_payload(int *n, int page) {
  char * tmp;
  char * str;
  int length = 0;

  str = (char *) malloc(MAX_BUF);
  tmp = str;
  length = sprintf(tmp, "<html><body><h1>Mon dictionnaire</h1><p>C'est la page\
                   %d</p><a href=\"%d\">suivant</a></body></html>", \
                   page, page + 1);
  tmp = tmp + length;
  *tmp = 0; // null terminated string
  *n = strlen(str);
  return str;
}

/*
 * get the value until it meets a space
 * @param char * buf
 * @return pointer to new string value
 */
char * get_value(char * buf) {
  char * tmp = NULL;
  char * value = NULL;
  int length = 0;

  tmp = buf;
  while (*tmp != ' ') {
    tmp++;
    length++;
  }
  
  value = (char *) malloc(length + 1);
  strncpy(value, buf, length);
  value[length] = 0;  // null terminated string

  return value;
}

/*
 * This function will parse the string sent from client (web browser).
 * We just only check the GET request, we ignore other request.
 * If client send GET /number we will consider that number as page of
 * dictionary, otherwise we return 0 (error)
 * 
 * @param char * buf input buffer from client
 * @param int length size of buffer
 * @return an integer 0 in case of something else rather than number, otherwise
 * a number.
 */
int parse(char *buf, int length) {
  int step = 0;
  int index = 0;
  char * value = NULL;
  int number = 0;
  
  if (strncmp(buf, HTTP_GET, 3) == 0) {
    index = 3;
  } else {
    return 0;
  }

  index++; // espace entre GET Resource

  if (buf[index++] == '/') {
    value = get_value(&buf[index]);
    if (is_number(value)) {
      number = atoi(value);;
      free(value);
    }
  } else {
    number = 0;
  }

  return number;
}

/*
 * Prepare header for error respose
 * @param int payload_length size of payload to embed in header
 * @param int * n it will store the size of header in return
 * @return pointer to header
 */
char * error_header(int payload_length, int * n) {
  char * tmp = NULL;
  char * str = NULL;
  char * date = NULL;
  int length = 0;

  str = (char *) malloc(MAX_BUF);

  tmp = str;
  strncpy(tmp, "HTTP/1.1 404 Not Found\n", 23);
  tmp = tmp + 23;
  strncpy(tmp, "Server: Citadel (Namur)\n", 24);
  tmp = tmp + 24;
  strncpy(tmp, "Content-Type: text/html; charset=UTF-8\n", 39);
  tmp = tmp + 39;
  length = sprintf(tmp, "Content-Length: %d\n", payload_length);
  tmp = tmp + length;
  strncpy(tmp, "Connection: close\n", 18);
  tmp = tmp + 18;
  date = get_date();
  sprintf(tmp, "Date: %s\n", date);
  tmp = tmp + 36;

  *tmp = 10;  // new feed caracter \r
  tmp++;
  *tmp = '\0';
  // free
  free(date);
  
  // get length of header
  *n = strlen(str);
  
  return str;
}

/*
 * HTTP error payload
 * @param int * n contain size of payload
 * @return a pointer to new string
 */
char * error_payload(int *n) {
  char * tmp = NULL;
  char * str = NULL;
  int length = 0;

  str = (char *) malloc(MAX_BUF);
  tmp = str;
  length = sprintf(tmp, "<html><body>Not Found</body></html>");
  tmp = tmp + length;
  *tmp = '\0';
  *n = strlen(str);
  return str;
}

int main(int argc, char * argv[])
{
  int client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  int fd, nread, result, port;
  fd_set readfds, testfds;
  struct sigaction act;
  struct timeval timeout;
  char * header = NULL, * payload = NULL;
  int header_length, payload_length;
  int page = 0;
  char buf[MAX_BUF];

  // if port is passed as argument
  if (argc > 1) {
    port = atoi(argv[1]);
  } else {
    port = DEFAULT_PORT;
  }

  // signal handler for SIGINT
  act.sa_handler = close_socket;
  sigaction(SIGINT, &act, NULL);

  /* init */
  FD_ZERO(&readfds);

  /*  Create a socket */
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

  /*  Name the socket */
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

  /*  Create connection queue and wait for clients  */
  listen(server_sockfd, 5);

  /* Add server socket to the list to monitor */
  FD_SET(server_sockfd, &readfds);

  printf("server is listening on port %d\n", port);
  printf("go to your browser: http://localhost:%d\n", port);
  
  while (running) {
    printf("server waiting...\n");
    timeout.tv_sec = 2;
    timeout.tv_usec = 500000;
    testfds = readfds;
    result = select(FD_SETSIZE, &testfds, (fd_set*) NULL, (fd_set*) NULL, &timeout);

    if (result == -1) {
      perror("select");
      running = 0;
    }
    
    for (fd = 0; fd <= FD_SETSIZE; fd++) {
      if (FD_ISSET(fd, &testfds)) {
        if (fd == server_sockfd) {
          client_len = sizeof(client_address);
          client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
          FD_SET(client_sockfd, &readfds);
          printf("adding client on fd %d\n", client_sockfd);
        } else {
          ioctl(fd, FIONREAD, &nread);
          if (nread == 0) {
            close(fd);
            FD_CLR(fd, &readfds);
            printf("removing client on fd %d\n", fd);
          } else {
            while ((nread = read(fd, buf, MAX_BUF)) > 0) {
              printf("bytes received: %d\n", nread);

              parse(buf, nread);
              
              if (page = parse(buf, nread)) {
                payload = get_payload(&payload_length, page);
                header = get_header(payload_length, &header_length);
                printf("%s\n", header);
                write(fd, header, header_length);
                printf("%s\n", payload);
                write(fd, payload, payload_length);
                
              } else {
                payload = error_payload(&payload_length);
                header = error_header(payload_length, &header_length);
                printf("%s\n", header);
                write(fd, header, header_length);
                printf("%s\n", payload);
                write(fd, payload, payload_length);
              }
              free(header);
              free(payload);
            }
          }
        }
      }
    }
  }

  close(server_sockfd);
  exit(EXIT_SUCCESS);
}
