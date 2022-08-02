//client side
/*!!!NOTE: IF YOU WANT TO RUN IT ON YOUR PC CHANGE DESKTOP PATH TO YOURS!!!*/
//linux only
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include <fcntl.h>

#include <pthread.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <sys/types.h>

#include <arpa/inet.h>

//max buffer size
int SIZE = 1024;
// checker variables dont mind them
int flag = 2;
int flag2 = 2;
int flag3 = 2;
int lineofcode = 10; //default value for number of line of files
// function prototypes
void * doRecieving(void * sockID);
void writenum(char * po);
int readnum();
int backupserver(void);
void helperfunction(void);
void delay(int number_of_seconds);

void delay(int number_of_seconds) {
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  clock_t start_time = clock();

  while (clock() < start_time + milli_seconds)
  ;
}

void helperfunction(void) {
  //interaction guide
  printf("----------------------------------------------------------------\n"
    "|                 Welcome to mini *chatbox*                    | \n"
    "|           Mainly designed for P.O.P course proj              | \n"
    "|                          9732593                             | \n"
    "|    I've provided an elaborated documentation for this app     | \n"
    "|       but for now,this is just a quick helper function.      | \n"
    "|                                                              | \n"
    "|Here are the commands(type into client and pess enter):       | \n"
    "|H ->> ask for help                                            | \n"
    "|L ->> prints out all active clients and assigned names        | \n"
    "|ME ->> prints out your assigned client number and name        | \n"
    "|NAME 0 <name> ->>you can assign a name for yourself           | \n"
    "|HI ->> quickly say hello to everyone                          | \n"
    "|S <clint numbr> <message> ->> send message to specific client | \n"
    "|M 0 <message> ->>send message to all active clients           | \n"
    "|F <client number> <file path> ->> send File contents to speci | \n"
    "|fic client.                                                   | \n"
    "|Q >> for quitting from client                                 | \n"
    "----------------------------------------------------------------\n");

}

void writenum(char * po) { // writes the number of lines of file in lineof.txt
  FILE * fp;
  int count = 0;
  char c;
  fp = fopen(po, "r");
  for (c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n') // Increment count if this character is newline
      count = count + 1;
  fclose(fp);
  FILE * fpt;
  char filenamee[100] = "/home/foli/Desktop/lineof.txt"; //DESKTOP PATH
  fpt = fopen(filenamee, "w");
  char tt[3];
  sprintf(tt, "%d", count);
  fputs(tt, fpt);
  fclose(fpt);

}

int readnum() {
  // reads number of lines from lineof.txt
  FILE * fpt;
  char filenamee[100] = "/home/foli/Desktop/lineof.txt"; //DESKTOP PATH
  fpt = fopen(filenamee, "r");
  char tt[3];
  fgets(tt, 3, fpt);
  int count;
  count = atoi(tt);
  fclose(fpt);
  return count;
}

void send_file(FILE * fp, int sockfd) {
  // sending file to server
  int n;
  char data[SIZE];
  // while there is sth
  while (fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

//for now ignore it
int backupserver() {
  printf("reconnecting\n");
  printf("H for help\n");
  FILE * fp;
  int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8081);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int cone;
  if ((cone = connect(clientSocket, (struct sockaddr * ) & serverAddr, sizeof(serverAddr))) == -1) return 1;

  printf("reconnected on 8081\n");
  pthread_t thread;
  pthread_create( & thread, NULL, doRecieving, (void * ) & clientSocket);

  while (1) {
    int cone2;
    char input[1024];
    scanf("%s", input);

    if (strcmp(input, "L") == 0) {

      cone2 = send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "H") == 0) {
      helperfunction();

    }

    if (strcmp(input, "ME") == 0) {
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "S") == 0) {

      send(clientSocket, input, 1024, 0);

      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%[^\n]s", input);
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "M") == 0) {

      send(clientSocket, input, 1024, 0);

      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%[^\n]s", input);
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "NAME") == 0) {

      send(clientSocket, input, 1024, 0);

      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%[^\n]s", input);
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "HI") == 0) {
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "F") == 0) {
      send(clientSocket, input, 1024, 0);
      char filename[100];
      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%s", filename);

      fp = fopen(filename, "r");
      writenum(filename);
      if (fp == NULL) {
        printf("no");
      }
      send_file(fp, clientSocket);
      fclose(fp);

    }
    if (strcmp(input, "Q") == 0) {
      printf("quiting");
      exit(0);
    }
  }
  printf("finished");
  exit(1);

}

// the function every thread needs to run
void * doRecieving(void * sockID) { // getting data
  //init socket id
  int clientSocket = * ((int * ) sockID);
  int i;
  while (1) {

    char data[1024];
    int read = recv(clientSocket, data, 1024, 0);
    // if server disconnects
    if (read <= 0) {
      close(clientSocket);
      delay(4);
      backupserver();
      exit(1);
    }

    data[read] = '\0';
    //means file is comming
    if (strcmp(data, "io") == 0) {
      int lineofcode;
      lineofcode = readnum(); //count

      FILE * fp;
      char filename[100] = "/home/foli/Desktop/recv-client";

      //file checker
      if (flag3 == 1) {
        strcat(filename, "-third");
      }
      if (flag2 == 1) {
        strcat(filename, "-second");
        flag3 = 1;
      }
      if (flag == 1) {
        strcat(filename, "-first");
        flag2 = 1;
      }

      strcat(filename, ".txt");
      flag = 1;

      int SIZE = 1024;
      char buffer[SIZE];
      fp = fopen(filename, "w");

      i = 0;
      printf("enter sth\n");
      while (i < lineofcode) {
        int n;
        n = recv(clientSocket, buffer, SIZE, 0);
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
        if (n <= 0) {
          break;
        }
        if (i >= lineofcode - 1) {
          i = lineofcode + 3;
          fclose(fp);
          break;
        }
        i++;

      }

      //break;
      continue;
      //fclose(fp);
    }

    printf("%s\n", data);

  }

}

int main() {
  printf("Welcome to multi-user chatbox!\n");
  printf("if you need any help using it, press H\n");
  FILE * fp;
  //ceating socket
  int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int cone; // connect it
  if ((cone = connect(clientSocket, (struct sockaddr * ) & serverAddr, sizeof(serverAddr))) == -1) return 1;

  printf("connected to sever on 8080\n");

  //crreating theaders and binding them to function
  pthread_t thread;
  pthread_create( & thread, NULL, doRecieving, (void * ) & clientSocket);

  while (1) { //infinite loop for clients
    int cone2;
    char input[1024];
    scanf("%s", input);
    //input checking
    if (strcmp(input, "L") == 0) { //list of users
      //sending input
      cone2 = send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "H") == 0) { //help
      helperfunction();

    }

    if (strcmp(input, "ME") == 0) { //your id
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "S") == 0) {

      send(clientSocket, input, 1024, 0);

      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%[^\n]s", input);
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "M") == 0) {

      send(clientSocket, input, 1024, 0);

      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%[^\n]s", input);
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "NAME") == 0) {

      send(clientSocket, input, 1024, 0);

      scanf("%s", input);
      send(clientSocket, input, 1024, 0);

      scanf("%[^\n]s", input);
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "HI") == 0) {
      //saying hi
      send(clientSocket, input, 1024, 0);

    }

    if (strcmp(input, "F") == 0) {
      send(clientSocket, input, 1024, 0);
      char filename[100];
      scanf("%s", input);
      send(clientSocket, input, 1024, 0);
      //getting client id
      scanf("%s", filename);
      //geting filename
      fp = fopen(filename, "r");
      //untill if that's vaild
      while (fp == NULL) {
        printf("no such file or directory, come again\n");
        scanf("%s", filename);
        fp = fopen(filename, "r");
      }
      writenum(filename);
      //count and write

      send_file(fp, clientSocket);
      fclose(fp);

    }
    if (strcmp(input, "Q") == 0) {
      //exiting
      send(clientSocket, "RU", 1024, 0);
      printf("quiting,\n bye bye!");
      exit(0);

    }
  }

}