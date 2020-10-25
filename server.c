
/*
 * Name: Donner Hanson
 *Date: October 24, 2020
 *Project: Simple Server in C
 *Program Description:
 This program instantiates a server at a user defined IP address
 and port number that gives a client access to the page
 "index.html", OR some other developer-user defined page,
 when the client issues a GET request in the terminal or FireFox Browser
  "GET / HTTP/1.1"
  "GET / HTTP/1.0"

 The server can connect to multiple client sessions using threading.

 When a user sends a request the server notifies the user
 if there is an invalid request and the type of error otherwise will send
 the user the intended html document

 The server closes the connection upon sending either
 the document or the error information

 * compilation
 MacOS:
 gcc server.c -o server -pthread
LINUX:
gcc -Wall server.c -o server -pthread -lrt
 * setup
 server executable MUST be in the same local directory as
  index.html and 404.html

 * Running in first terminal
 ./server <IP> <port>

 ./server 127.0.0.1 6789

 * Accessing in second terminal
 telnet 127.0.0.1 6789

 *accessing in firefox browser - FireFox tested
http://localhost:6789



 NOTE: if there is any plain text (.txt) file or html (.html) file hosted
 in the same directory as server.c
http://localhost:6789/filename.extension
will return that file and the contents of the file

 IF the file doesn't exist:
 http://localhost:6789/any-non-existent-file.extension
Will redirect to 404.html


*OS Compatability:
This program is functional on MacOS and Linux


// REFERENCES
TRIM WHITE SPACE
https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

404 HTML BOILERPLATE CODE
https://github.com/h5bp/html5-boilerplate/blob/master/src/404.html

fread SPLITTING text into chunks
https://stackoverflow.com/questions/46669468/c-fread-split-textfile-into-chunks

 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUM 6789
#define MAX_LEN 2000
#define RESPONSE_LEN 28
#define PROTOCOL_PARSE_LEN 3
#define WRONG_PROT -3
#define BAD_HTTP_REQ -4


#define GET 0
#define FNAME 1
#define REQ_PROT 2

#define FNF -10
#define NO_ACCESS -11
#define HAS_ACCESS 0

char *trim(char *str);
// function searches for first_slash, next_space, second_slash, end
// otherwise an error number is placed into the returned int array
int *getProtocolIndexVals(char *str, int len, int* int_arr, int i_arr_sz);

int contains(int *int_arr, int i_arr_sz);

int checkPermissions(char* f_name);

void *client_handler(void *arg)
{
    void * ptr_rtn = NULL;
    char buf[MAX_LEN];
    int index_vals[PROTOCOL_PARSE_LEN] = {0};
    int *index_vals_ptr;
    index_vals_ptr = index_vals;
    int sockfd;
    char file_name[MAX_LEN] = {0};

    char* http_1 = "HTTP/1.0";
    char* http_0 = "HTTP/1.1";
    char* get_prot = "GET /";

    sockfd = *(int *)arg;
    //string_to_match = "GET / HTTP/1.1"; // searches for index.html
    //string_to_match = "GET /filename.html HTTP/1.1"; // searches for index.html

    /* read from client */
    int length = MAX_LEN;
    /////////////////////////
    char* text;
    int cont = 0;
    text = (char*) malloc (length); // dynamically allocate memory

    if (read (sockfd, text, length) == 0){
        _exit(EXIT_FAILURE); // nothing read and something bad happened
      }

        // trim whitespace off of buffer
        text = trim(text);
        int len_text = (int)strlen(text);
        //printf("\nTEXT COMMAND: %s, Length %d \n", text, len_text);
        // getProtocolIndexVals command

        /*
        TESTING STATEMENT
        for (int i = 0; i < PROTOCOL_PARSE_LEN; i++){
        printf("values before: %d\n", index_vals[i]);
      }*/
        index_vals_ptr = getProtocolIndexVals(text, len_text, index_vals_ptr, PROTOCOL_PARSE_LEN);
        /*
        TESTING STATEMENT
        for (int i = 0; i < PROTOCOL_PARSE_LEN; i++){
        printf("values after: %d\n", index_vals[i]);
      }
      */

    if (index_vals[GET] == WRONG_PROT) // if NOT "GET /"
    {
      snprintf(buf, sizeof(buf),
       "Was expecting %s but recieved command %s\r\n", get_prot, text);
       write(sockfd, buf, strlen(buf));
       cont = 1;
    }
    else if (index_vals[REQ_PROT] == BAD_HTTP_REQ) // IF NOT "HTTP/1.0"or"HTTP/1.1"
    {

      snprintf(buf, sizeof(buf),
       "Was expecting %s or %s but recieved %s\r\n", http_0, http_1,
       text + index_vals[FNAME]);
       write(sockfd, buf, strlen(buf));
       cont = 1;
    }


// GET FILE NAME AND STREAM CONTENTS
    if (cont == 0) {
      memset(&file_name[0],'\0',MAX_LEN);
      // NO FILENAME PASSED
      if (index_vals[GET] == index_vals[FNAME])
      {
        strcpy(file_name, "index.html");
      }
      else {
        // get the string that exist between GET and fName
        for (int i = 0, j = index_vals[GET] + 1; j <= index_vals[FNAME]; i++, j++)
        {
          file_name[i] = text[j];
        }

        /*
        TESTING STATEMENT
        printf("FILENAME = %s\n", file_name);
        */
      }

      // SET MSG RCV and MSG ERR
      char* found;
      char* not_found;
      if ('1' == text[index_vals[REQ_PROT]]){
        not_found = "HTTP/1.1 404 NOT FOUND\n\n";
        found = "HTTP/1.1 200 OK\n\n";
      }
      else if ('0' == text[index_vals[REQ_PROT]]) {
        not_found = "HTTP/1.0 404 NOT FOUND\n\n";
        found = "HTTP/1.0 200 OK\n\n";
      }


      // read file to intermediate buffer
      // max buffer length minus the response include null terminator

      int buff_allocation_len = MAX_LEN - RESPONSE_LEN;
      char source[MAX_LEN - RESPONSE_LEN + 1];
      FILE *fp = fopen(file_name, "r");
      // if null find out why
      if (fp != NULL) {


        // USE FREAD TO READ ALL INTO BUFFER AT ONCE
        //Seek to the end of the file to determine the file size
        //fseek(fp, 0L, SEEK_END);
        //size_t fileSize = ftell(fp);
        //fseek(fp, 0L, SEEK_SET);

        size_t size;
        snprintf(buf, sizeof(buf), "%s\r\n", found); // send OK response
        write(sockfd, buf, strlen(buf));
        size_t fsize;
        // READ FROM file and out to Socket until end of input file
        fseek(fp, 0L, SEEK_END); // get file size
        fsize = ftell(fp);
        // seek back to beginning
        fseek(fp, 0L, SEEK_SET);

        //printf("\nFSIZE: %zu\n",fsize );
        while ((size = (fread (&source, 1, (buff_allocation_len), fp))) > 1)
        {
          //printf("\nSIZE: %zu\n",size);
          //printf("FSIZE: %zu\n",fsize);
          //printf("BUFF ALL LEN: %d\n", buff_allocation_len);
          if (fsize > buff_allocation_len)
            fsize = fsize - buff_allocation_len;
          else
          {
            // this is the last block of the file to output
            snprintf(buf, sizeof(buf), "%s", source);
            write(sockfd, buf, fsize);
            memset(buf, '\0', strlen(buf));
            break;
          }

          snprintf(buf, sizeof(buf), "%s", source);
          write(sockfd, buf, strlen(buf));
          memset(&buf, '\0', strlen(buf));
          memset(&source, '\0', strlen(source));



        }
        fclose(fp);
        if ( ferror( fp ) != 0 ) {
          fputs("Error reading file", stderr);
        }






      }
      else {
        // FILE WASN'T FOUND or no permissions - send ERROR SPLASH PAGE
        int permission = checkPermissions(file_name);

        if ('1' == text[index_vals[REQ_PROT]]){
          if (permission == FNF){
            not_found = "HTTP/1.1 404 NOT FOUND\n\n";
            strcpy(file_name, "404.html");
          }
          else if (permission == NO_ACCESS){
            not_found = "HTTP/1.1 403 NOT FOUND\n\n";
            strcpy(file_name, "403.html");
          }
          else {
            // an unexpected error - send not found for now
            not_found = "HTTP/1.1 404 NOT FOUND\n\n";
            strcpy(file_name, "404.html");
          }
        }
        else if ('0' == text[index_vals[REQ_PROT]]) {
          if (permission == FNF){
            not_found = "HTTP/1.0 404 NOT FOUND\n\n";
            strcpy(file_name, "404.html");
          }
          else if (permission == NO_ACCESS){
            not_found = "HTTP/1.0 403 NOT FOUND\n\n";
            strcpy(file_name, "403.html");
          }
          else{
            // an unexpected error - send not found for now
            not_found = "HTTP/1.0 404 NOT FOUND\n\n";
            strcpy(file_name, "404.html");
          }


        }

        fp = fopen(file_name, "r");
        if (fp != NULL) {
          size_t fLength = fread(&source, sizeof(char),
                                    MAX_LEN - RESPONSE_LEN + 1, fp);
          if ( ferror( fp ) != 0 ) {
            fputs("Error reading file", stderr);
          }
          else {
            source[++fLength] = '\0'; /* Just to be safe. */
          }
        }
        fclose(fp);
        snprintf(buf, sizeof(buf), "%s\r\n%s\n", not_found, source);
        write(sockfd, buf, strlen(buf));
      }
    }
    /* send result to client */
    free (text); // get rid of dynamically allocated memory
    //write(sockfd, buf, strlen(buf));
    memset(&buf[0], '\0', MAX_LEN); // nullify buffer
    close(sockfd); // close socket
    ptr_rtn = (void *)&sockfd; // address of closed socket
    return ptr_rtn; // return value of address of closed socket

}

int main(int argc, char *argv[])
{

   const int backlog = 5;

   struct  sockaddr_in  server_addr;
   struct  sockaddr_in  client_addr;
   pthread_t tid;

   int sockfd, client_sockfd;
   unsigned int *clen;
   unsigned int clientlen;
   clen = &clientlen;


  // SET DEFAULT PORT NUM IF NOT SPECIFIED
   if (argc == 2) {
       server_addr.sin_port = htons(PORT_NUM);
   }
   if (!(argc == 3 || argc == 2)) {
       printf("Usage: %s <ip-address> <port>  or <ip-address> \n", argv[0]);
       return -1;
   }

   /* Create the socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd == -1) {
       perror("Could not create socket");
       return -1;
   }

   /* Name the socket */
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = inet_addr(argv[1]);
   if (argc == 3){
    server_addr.sin_port = htons(atoi(argv[2]));
  }

   /* bind to server socket */
   if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
       perror("Could not bind to socket");
       close(sockfd);
       return -1;
   }

   /* wait for client to connect */
   listen(sockfd, backlog);

   while (1) {

       /* Accept a connection */
       clientlen = sizeof(client_addr);
       client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clientlen);
       if (client_sockfd == -1) {
           perror("Unable to accept client connection request");
           continue;
       }

       if (pthread_create(&tid, NULL, client_handler, (void *)&client_sockfd) < 0) {
           perror("Unable to create client thread");
           break;
       }

   }

   close(sockfd);

   return 0;

}




/* https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way */
char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
            *str = '\0';
    else if( str + len - 1 != endp )
            *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}

int* getProtocolIndexVals(char* str, int len, int* int_arr, int i_arr_sz)
{
  char tempBuff[MAX_LEN] = {'\0'};
  int index_first_slash = 0;
  int fname_end_index = 0;
  int protocolNum_index = 0;
  int currIndex = 0;

  char* http_0 = "HTTP/1.0";
  char* http_1 = "HTTP/1.1";

  char* get_prot = "GET /";
  // IF NOT A GOOD ENTRY
  // set int arr indexes all to 0
  if ( str == NULL ) {
    int_arr[GET] = WRONG_PROT;
    return int_arr;
  }
  if ( str[0] == '\0' ) {

    int_arr[GET] = WRONG_PROT;
    return int_arr;
  }

// OTHERWISE PARSE

// GET PROTOCOL
 for (int i = currIndex; i < len; i++) {

   index_first_slash = i;
   tempBuff[i] = str[i];
   int_arr[GET] = i;
   if (str[i] == '/') {
     break;
   }
 }
 // check protocol
 printf("\nGET comm: %s, currIndex : %d\n", tempBuff, currIndex);
 if (strcmp(tempBuff, get_prot) != 0) {
   int_arr[GET] = WRONG_PROT;
   return int_arr;
 }

// print check


// empty buffer
memset(&tempBuff[0], '\0', MAX_LEN);


currIndex = index_first_slash + 1; // start on first letter next word

// GET FNAME index
 for (int i = currIndex, j = 0; i < len; i++, j++){
   fname_end_index = i;
   if (str[i] == ' ') {
     break;
   }
   tempBuff[j] = str[i];

 }
printf("FNAME: %s, fname_Index: %d\n", tempBuff, fname_end_index); // check fname

currIndex = (fname_end_index + 1); // after "fName " on first letter next word

int_arr[FNAME] = fname_end_index - 1; // remove space from fname index

// empty buffer
memset(&tempBuff[0], '\0', MAX_LEN);

// HTTP/NUM

/*
if running from FIREFOX - NEED TO REMOVE unneeded
browser request info from intended input
These exist after the GET request in the passed string: I.E.
GET /fname HTTP/1.1
Host:...
User-Agent:...

Host,User-Agent,Accept, Accept-Language,Accept-Encoding,DNT, Connection, Referer

*/

for (int i = currIndex, j = 0; i < len; i++, j++) {
  protocolNum_index = i;
  // printf ("STR %c : [%d]\n", str[i], i);
  tempBuff[j] = str[i];

  if (j < strlen(http_0) - 1) {
    ;
  }
  else{
    break;
  }
  if (str[i] == ' ' || str[i] == '\0') {
    printf("\n");
    break;
  }
}
printf("PROT/NUM: %s, end_index: %d \n", tempBuff, protocolNum_index);
if ((strcmp(tempBuff, http_0) != 0) && (strcmp(tempBuff, http_1) != 0))
{
  int_arr[REQ_PROT] = BAD_HTTP_REQ;
  return int_arr;
}
int_arr[REQ_PROT] = protocolNum_index;

  return int_arr;
}

int containsWrongProt(int* int_arr, int i_arr_sz)
{
  for (int i = 0; i < i_arr_sz; i++)
  {
    if (int_arr[i] == WRONG_PROT)
    {
      return -1;
    }
  }
  return 0;
}

int checkPermissions(char* str)
{
  // Check read access
  int status = 0;
  status = access (str, R_OK);
  printf ("ENOENT: %d\nEACCESS: %d", ENOENT, EACCES);
    if (errno == ENOENT)
      return FNF;
    else if (errno == EACCES)
      return NO_ACCESS;

return FNF;
}
