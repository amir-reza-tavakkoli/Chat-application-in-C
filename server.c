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

	//buffer size
int SIZE = 1024;
int flag = 2;
int flag2 = 2;
int clientCount = 0;
int backupinit = 1;
//int configme;
int lineofcode = 10;

// function prototypes
void *doRecieving(void *sockID);
void writenum(char *po);
int readnum();
int mainn(void);

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *doNetworking(void *ClientDetail);
void send_file(FILE *fp, int sockfd);

struct client
{
	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;
	char clientname[100];

};
struct client Client[10];

pthread_t thread[1024];

void delay(int number_of_seconds)
{

	int milli_seconds = 1000 * number_of_seconds;

	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)
	;
}

void helperfunction(void)
{

	printf("this is helper\n");

}

void writenum(char *po)
{
	// writes the number of lines of file in lineof.txt
	FILE * fp;
	int count = 0;
	char c;
	fp = fopen(po, "r");
	for (c = getc(fp); c != EOF; c = getc(fp))
		if (c == '\n')	// Increment count if this character is newline
			count = count + 1;
	printf("%d\n", count);
	fclose(fp);
	FILE * fpt;
	char filenamee[10] = "lineof.txt";
	fpt = fopen(filenamee, "w");
	char tt[3];
	sprintf(tt, "%d", count);
	fputs(tt, fpt);
	fclose(fpt);

}

int readnum()
{
	// reads number of lines from lineof.txt
	FILE * fpt;
	char filenamee[10] = "lineof.txt";
	fpt = fopen(filenamee, "r");
	char tt[3];
	fgets(tt, 3, fpt);
	int count;
	count = atoi(tt);
	fclose(fpt);
	return count;
}

void write_file(int sockfd, int sendid)
{
	// getting file from server
	int n;
	FILE * fp;
	char filename[20] = "recv2.txt";
	int SIZE = 1024;
	char buffer[SIZE];
	fp = fopen(filename, "w");
	int i = 0;
	while (i < 2)
	{
		i++;
		n = recv(sockfd, buffer, SIZE, 0);
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
		if (n <= 0)
		{
			break;
		}
	}

	fclose(fp);
	return;
}

void send_file(FILE *fp, int sockfd)
{
	// sending file to server
	int n;
	char data[SIZE];

	while (fgets(data, SIZE, fp) != NULL)
	{
		if (send(sockfd, data, sizeof(data), 0) == -1)
		{
			perror("[-]Error in sending file.");
			exit(1);
		}

		bzero(data, SIZE);
	}
}

void write_files(int sockfd, int sendid)
{
	int n;
	FILE * fp;
	char filename[40] = "recv";

	if (flag == 1)
	{
		strcat(filename, "m");
		flag2 = 1;
	}

	if (flag2 == 1)
	{
		strcat(filename, "m");
	}

	strcat(filename, ".txt");
	flag = 1;
	int SIZE = 1024;
	char buffer[SIZE];
	fp = fopen(filename, "w");
	int i = 0;
	int lineofcode;
	lineofcode = readnum();
	while (i < lineofcode)
	{
		i++;
		n = recv(sockfd, buffer, SIZE, 0);
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
		if (n <= 0)
		{
			break;
		}
	}

	fclose(fp);

	return;
}

void *doNetworking(void *ClientDetail)
{
	//here we wait for clients
	struct client *clientDetail = (struct client *) ClientDetail;
	int index = clientDetail->index;
	int clientSocket = clientDetail->sockID;
	printf("Client %d connected.\n", index + 1);

	while (1)
	{
		char data[1024];
		int read = recv(clientSocket, data, 1024, 0);
		data[read] = '\0';

		char output[1024];

		if (strcmp(data, "L") == 0)
		{
			int l = 0;

			for (int i = 0; i < clientCount; i++)
			{
				if (i != index)
					l += snprintf(output + l, 1024, "Client %d is at socket %d name %s.\n", i + 1, Client[i].sockID, Client[i].clientname);

			}

			send(clientSocket, output, 1024, 0);
			continue;

		}

		if (strcmp(data, "NAME") == 0)
		{
			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';
			strcpy(Client[index].clientname, data);
			send(clientSocket, "Your name changed", 1024, 0);
		}

		if (strcmp(data, "HI") == 0)
		{
			int mc = 0;
			for (; mc < 3; mc++)
			{
				send(Client[mc].sockID, "Hi everyone", 1024, 0);
			}
		}

		if (strcmp(data, "ME") == 0)
		{
			char s[200];
			sprintf(s, "You are number %d name %s\n", index + 1, Client[index].clientname);
			send(clientSocket, s, 1024, 0);
		}

		if (strcmp(data, "S") == 0)
		{
			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			send(Client[id].sockID, data, 1024, 0);

		}

		if (strcmp(data, "M") == 0)
		{
			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int mc = 0;
			for (; mc < 3; mc++)
			{
				send(Client[mc].sockID, data, 1024, 0);
			}
		}

		if (strcmp(data, "F") == 0)
		{
			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;
			write_files(clientSocket, id);
			FILE * fpp;
			fpp = fopen("recv.txt", "r");
			send(Client[id].sockID, "io", 1024, 0);
			send_file(fpp, Client[id].sockID);
			fclose(fpp);
		}
	}

	return NULL;

}

int mainn()
{
	// giving default name to each client
	int qu;
	for (qu = 0; qu < 10; qu++)
	{
		Client[qu].clientname[99] = '\0';
		strcpy(Client[qu].clientname, "Default name");

	}

	// creating actual socket
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	// giving port, address, etc
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8081);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	// binding sockets
	if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) return 0;
	// tcp connection
	if (listen(serverSocket, 1024) == -1) return 0;

	printf("Server's live on 8081 ...........\n");

	while (1)
	{
		// assigning stuff to each client
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr *) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;

		//assigning each thread to given function
		pthread_create(&thread[clientCount], NULL, doNetworking, (void*) &Client[clientCount]);
		clientCount++;

	}

	//joining(grouping) each thread
	for (int i = 0; i < clientCount; i++)
		pthread_join(thread[i], NULL);

}

void *doRecieving(void *sockID)
{
	// getting data

	int clientSocket = *((int*) sockID);
	int i;
	while (1)
	{
		char data[1024];
		int read = recv(clientSocket, data, 1024, 0);
		if (read <= 0)
		{
			close(clientSocket);
			mainn();
		}

		data[read] = '\0';

	}
}

int main()
{
	printf("I'm backup server\n");
	printf("H for help\n");
	FILE * fp;
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int cone;
	if ((cone = connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) == -1) return 1;

	printf("Connected\n");

	printf("in c\n");
	send(clientSocket, "hey", 1024, 0);

	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void*) &clientSocket);
	printf("You can't send anything btw\n");
	while (1)
	{
		1 + 1;

	}
}