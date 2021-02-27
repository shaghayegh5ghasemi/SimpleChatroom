#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


int compare_strings(char a[], char b[])
{
    int c = 0;
    while (a[c] == b[c]) 
    {
        if (a[c] == '\0' || b[c] == '\0')
        break;
        c++;
    }
    if (a[c] == '\0' && b[c] == '\0')
    	return 0;
    else
    	return -1;
}

int main() {
    
    int welcomeSocket, Client1, Client2;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    char buffer[1024];

   //set port and address family
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // sets all memory cells to zero
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    //binds the socket to the address and port number
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    //server is waiting for clients ( listen for socket connections and limit the queue of incoming connections)
    if (listen(welcomeSocket,3) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    //connect clients and the server
    addr_size = sizeof serverStorage;
    Client1 = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    Client2 = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    int cmdEXIT = 0;
    //connection will be finished when the client enter exit
    while (cmdEXIT == 0)
    {
        //recieve messages from first client
        recv(Client1, buffer, 1024, 0);
        //send to second client
        printf ("%s\nSending to client2\n", buffer);
        send(Client2,buffer,1024,0);
        //condition de exit the chat
        if (compare_strings(buffer, "exit") == 0)
        {   
            cmdEXIT = 1;
        }
        else 
        {
            //empty the buffer
            memset(&buffer[0], 0, sizeof(buffer));
            //recieve messages from second client    
            recv(Client2, buffer, 1024, 0);
            //send to first client
            printf ("%s\nSending to client1\n", buffer);
            send(Client1,buffer,1024,0);
            //condition de exit the chat
            if (compare_strings(buffer, "exit")==0)
            {
                cmdEXIT = 1;
            }
        }
    }

    return 0;
}
