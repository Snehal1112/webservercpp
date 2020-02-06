#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

using namespace std;

int main()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return  -1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        cerr << "Can't bind to IP/port";
        return -2;
    }

    // Tell Winsock the socket is for listening
    if (listen(listening, SOMAXCONN) == -1) {
        cerr << "Can't listen!";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];


    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if (clientSocket == -1) {
        cerr << "Problem with client connecting!";
        return -4;
    }

    close(listening);


    memset(host,0, NI_MAXHOST);
    memset(svc,0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client),host,NI_MAXHOST,svc,NI_MAXSERV,0);

    if(result){
        cout<< host << "connected on" << svc << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << "connected on " << ntohs(client.sin_port) << endl;
    }

    int MAX_BUFF = 4096;
    char buf[MAX_BUFF];
    while (true) {
        // clear the buffer
        memset(buf, 0, MAX_BUFF);
        int bytesRecv = recv(clientSocket, buf, MAX_BUFF, 0);
        if (bytesRecv == -1) {
            cerr << "There was a connection issue" << endl;
            break;
        }

        if(bytesRecv == 0){
            cout << "The client disconnected" << endl;
        }
        // display message
        cout << "Received : " << string(buf, 0, bytesRecv) << endl;

        // send message
        send(clientSocket, buf, bytesRecv + 1, 0);

    }

    close(clientSocket);
}