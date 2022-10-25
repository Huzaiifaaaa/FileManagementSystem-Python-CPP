#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>
#include<stdio.h>
#include <fstream>
#include<stdlib.h>
#include<string.h>
#include <malloc.h>
#include <dirent.h>
#include<process.h>
#include<wchar.h>
#include <sys/types.h>
#include<vector>

using namespace std;

class node
{
    public:
        string name;
        vector<node*> children;
};

void invalidCommand(string command)
{
    cout << "'"<<command <<"' "<<"is not recognized as an internal or external command, operable program or batch file.\n" << endl;
}

void getHelp()
{
    cout << "cd [directory] - Change the current directory to [directory]." << endl;
    cout <<"mkdir [directory] - Create the directory specified in [directory]." << endl;
    cout <<"touch [name] - Create the file in specified directory." << endl;
    cout <<"delete [name] - Delete the file in specified directory." << endl;
    cout << "ls [directory] - Display a list of files and subdirectories in a directory." << endl;
    cout << "help - Display the user manual." << endl;
    cout << "exit - Exit the shell.\n" << endl;
}
void printMessage(string message)
{
    cout << message << endl;
}


int main()
{
    struct node *root = new node;
    root->name = "root";
    //root->children.push_back(new node);

    struct node *current=root;
    string path;

    cout<<"OS File Management System [Version 10.0.0]. All rights reserved.\n\n"<<endl;
    while(1)
    {
        string command;

        if(current->name!=path)
        {
            path+=current->name;
        }

        
        cout << path << "\\> ";
        getline (cin, command);
        string path=command.substr(command.find(" ")+1,command.length()-2);
        command=command.substr(0,command.find(" "));



        if(command == "mkdir")
        {
            for(int i=0;i<current->children.size();i++)
            {
                if(root->children[i]->name==path)
                {
                    printMessage("Directory already exists");
                    continue;
                }
            }

            struct node *mkdir = new node;
            mkdir->name = path;
            current->children.push_back(mkdir);
        }
        else if(command == "ls")
        {
            for(int i=0;i<current->children.size();i++)
            {
                cout<<current->children[i]->name<<endl;
            }
        }
        else if(command == "cd")
        {
           for(int i=0;i<current->children.size();i++)
            {
                if(root->children[i]->name==path)
                {
                    current=current->children[i];
                    continue;
                }
            }
        }
        else if(command == "exit")
        {
            break;
        }
        else
        {
            invalidCommand(command);
        }
    }
}
