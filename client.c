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

int flag = 2;
int flag2 = 2;
int flag3 = 2;
int clientCount = 0;
int SIZE = 1024;
int backupcount = 0;
int lineofcode = 10;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int readnum();
void *doNetworking(void *ClientDetail);
void send_file(FILE *fp, int sockfd);
void write_file(int sockfd, int sendid);

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

int readnum()
{
	FILE * fpt;
	char filenamee[100] = "/home/foli/Desktop/lineof.txt";
	fpt = fopen(filenamee, "r");
	char tt[3];
	fgets(tt, 3, fpt);
	int count;
	count = atoi(tt);
	fclose(fpt);
	return count;
}

void send_file(FILE *fp, int sockfd)
{
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

	//fclose(fp);
}

void write_file(int sockfd, int sendid)
{
	int n;
	FILE * fp;
	char filename[100] = "/home/foli/Desktop/server-recv";
	if (flag3 == 1)
	{
		strcat(filename, "-third");
	}

	if (flag2 == 1)
	{
		strcat(filename, "-second");
		flag3 = 1;
	}

	if (flag == 1)
	{
		strcat(filename, "-first");
		flag2 = 1;
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
	//every thread runs this function
	// assigning to each client
	struct client *clientDetail = (struct client *) ClientDetail;
	int index = clientDetail->index;
	int clientSocket = clientDetail->sockID;
	//creating clientsocket for each client
	printf("Client %d connected.\n", index + 1);

	while (1)
	{
		//init data
		char data[1024];
		int read = recv(clientSocket, data, 1024, 0);
		data[read] = '\0';
		char output[1024];
		//checking data
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
			//changing client name
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
			int mc = 0;	//sending hi to everyone
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
			//send message to specified client

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			send(Client[id].sockID, data, 1024, 0);

		}

		if (strcmp(data, "M") == 0)
		{
			//send message to everyone

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int mc = 0;
			for (; mc < 10; mc++)
			{
				send(Client[mc].sockID, data, 1024, 0);
			}
		}

		//file input
		if (strcmp(data, "F") == 0)
		{
			read = recv(clientSocket, data, 1024, 0);
			data[read] = '\0';

			int id = atoi(data) - 1;
			write_file(clientSocket, id);
			FILE * fpp;
			fpp = fopen("/home/foli/Desktop/server-recv.txt", "r");
			send(Client[id].sockID, "io", 1024, 0);
			//send_file(fpp,Client[id].sockID);
			send_file(fpp, Client[id].sockID);
			fclose(fpp);
		}

		//backup server settings
		if (strcmp(data, "jj") == 0)
		{
			if (backupcount > 1)
			{
				printf("errorr");
				exit(1);
			}

			strcpy(Client[index].clientname, "BACKUP SERVER");
			backupcount++;
			int mc = 0;
			for (; mc < 10; mc++)
			{
				send(Client[mc].sockID, "BACKUP SERVER ADDED", 1024, 0);
			}
		}

		//quiting a client
		if (strcmp(data, "RU") == 0)
		{
			char ydata[45];
			sprintf(ydata, "%d disconnected\n", index + 1);
			int mc = 0;
			for (; mc < 10; mc++)
			{
				send(Client[mc].sockID, ydata, 1024, 0);
			}
		}
	}

	return NULL;

}

int main()
{
	// giving default name to each client
	int qu;
	for (qu = 0; qu < 20; qu++)
	{
		Client[qu].clientname[99] = '\0';
		strcpy(Client[qu].clientname, "default name");

	}

	// creating actual socket
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	// giving port, address, etc
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	// binding sockets to port and address
	if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) return 0;
	// tcp connection
	if (listen(serverSocket, 1024) == -1) return 0;

	printf("Server'S live on 8080 ...........\n");

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