#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <vector>

using namespace std;

vector<string> tokenize(string str, char delim){
    vector<string> tokens;
    string temp = "";
    for(int i = 0; i < str.length(); i++){
        if(str[i] == delim){
            tokens.push_back(temp);
            temp = "";
        }
        else
            temp += str[i];
    }
    tokens.push_back(temp);
    return tokens;
}

int main(int argc, char *argv[])
{

    if(argc < 2)
    {
        cout << "Please enter the server's hostname" << endl;
        return -1;
    }

    char *serverIp = argv[1]; 
    int port = atoi(argv[2]); 
    char msg[1500]; 

    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
 
    int status = connect(clientSd,(sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl;
        return -1;
    }

    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    memset(&msg, 0, sizeof(msg));//clear the buffer
    bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
    cout << msg << endl;

    while(1)
    {
        string data="";
        string input="";
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);

        cout << msg;
        //getline(cin, data);

        //memset(&msg, 0, sizeof(msg));
        //strcpy(msg, data.c_str());
        //send(clientSd, (char*)&msg, strlen(msg), 0);
    }
    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "Connection closed" << endl;
    return 0;    
}