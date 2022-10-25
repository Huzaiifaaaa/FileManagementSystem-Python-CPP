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
    string path;
    struct node *root = new node;
    root->name = "root";
    struct node *current=root;
    string directory=current->name;

    cout<<"OS File Management System [Version 10.0.0]. All rights reserved.\n\n"<<endl;

    while(1)
    {
        string command;
        cout << directory << "\\> ";
        getline (cin, command);

        string path=command.substr(command.find(" ")+1,command.length()-2);
        command=command.substr(0,command.find(" "));

        if(command == "mkdir")
        {
            bool exists=false;
            for(int i=0;i<current->children.size();i++)
            {
                if(root->children[i]->name==path)
                {
                    
                    exists=true;
                    break;
                }
            }

            if(exists)
            {
                printMessage("Directory already exists");
            }
            else
            {
                struct node *mkdir = new node;
                mkdir->name = path;
                current->children.push_back(mkdir);
            }
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
                if(current->children[i]->name==path)
                {
                    directory+="\\"+current->children[i]->name;
                    current=current->children[i];
                    break;
                }
            }

            //printMessage("Directory does not exist");
        }
        else if(command == "help")
        {
            getHelp();
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
