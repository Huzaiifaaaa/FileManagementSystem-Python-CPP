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
#include <chrono>
#include<cstring>
#include <ctime>
#include<stdlib.h>
#include<string.h>
#include <algorithm>
#include <malloc.h>
#include<wchar.h>
#include<vector>
#include <ctime>

using namespace std;

class Node
{
    public:
        string name;
        bool isFile;
        bool isOpen;
        Node* parent;
        string data;
        string mode;
        vector<Node*> children;
        string createdAt;
        string modifiedAt;
        int startPos;
        int endPos;

        Node(string name, bool isFile, Node* parent,int s,int e)
        {
            time_t ttime = time(0);
            this->name = name;
            this->isFile = isFile;
            this->isOpen=false;
            this->parent = parent;
            this->createdAt = this->modifiedAt = strtok(ctime(&ttime),"\n");
            startPos=s;
            endPos=e;

            if(!isFile)
            {
                this->data = "";
            }
        }
};

string invalidCommand(string command)
{
    return "'"+command+"'is not recognized as an internal or external command, operable program or batch file.\n\n";
}

void getHelp()
{
    cout << "ls [directory] - Display a list of files and subdirectories in a directory." << endl;
    cout<<"create [filename] - Create a file with the name [filename]." << endl;
    cout << "delete [filename] - Delete the file specified in [filename]." << endl;
    cout <<"mkdir [directory] - Create the directory specified in [directory]." << endl;
    cout << "cd [directory] - Change the current directory to [directory]." << endl;
    cout<<"move [source] [destinaiton] - Moves source to specified [directory]." << endl;
    cout << "open [filename] [m]- Open the file specified in [filename]." << endl;
    cout<< "close [filename] - Close the file specified in [filename]." << endl;
    cout << "write [filename] [m] [p] [data]- Writes to the file specified in [filename]." << endl;
    cout << "read [filename] [m] [p] - Reads the file specified in [filename]." << endl;
    cout<< "truncate [filename] [p]- Truncates the file specified in [filename]." << endl;
    cout << "memory - Displays the memory tree." << endl;
    cout <<"clear - Clears the screen." << endl;
    cout << "help - Display the user manual." << endl;
    cout << "exit - Exit the shell.\n" << endl;
}

string printMessage(string message)
{
    //cout << message << endl;
    return message+"\n";
}

string reverseString(string directory)
{
    string rev = "";

    for(int i=directory.length()-1; i>=0; i--){
        rev = rev + directory[i];
    }

    return rev;
}

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


Node* searchTree(Node *root,string name)
{
    if(root->name == name)
    {
        return root;
    }
    else
    {
        for(int i=0;i<root->children.size();i++)
        {
            Node* temp = searchTree(root->children[i],name);
            if(temp != NULL)
            {
                return temp;
            }
        }
    }
    return NULL;
}

void printTree(Node *root, int level)
{
    for(int i=0;i<level;i++)
    {
        cout << " |-- ";
    }
    cout << root->name << endl;
    for(int i=0;i<root->children.size();i++)
    {
        printTree(root->children[i],level+1);
    }
}


int main(int argc, char *argv[])
{
    int size = 512;//512KB  
    struct Node *root = new Node("root", false, NULL,0,0);
    int memory[512]={0};
    int memcounter=0;
    string path;
    struct Node *current=root;
    string directory=current->name;
    int port = atoi(argv[1]);
    char msg[1500];
     
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }

    listen(serverSd, 5);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);

    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }

    cout << "Connected with client!" << endl;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    int bytesRead, bytesWritten = 0;

    string output="OS File Management System [Version 10.0.4]. All rights reserved.\n\n";
    strcpy(msg, output.c_str()); 
    send(newSd, (char*)&msg, strlen(msg), 0);
    string command="";

    while(1)
    {
        string data="";
        string dir=directory+"\\> "+command;
        strcpy(msg, dir.c_str()); 
        send(newSd, (char*)&msg, strlen(msg), 0);

        getline(cin,command);

        memset(&msg, 0, sizeof(msg));
        recv(newSd, (char*)&msg, sizeof(msg), 0);
    }

    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "Connection closed..." << endl;
    return 0;   
}