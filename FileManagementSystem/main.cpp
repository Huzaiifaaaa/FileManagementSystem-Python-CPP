#include <direct.h>
#include <unistd.h>
#include<iostream>
#include<stdio.h>
#include <fstream>
#include <chrono>
#include <ctime>   
#include<stdlib.h>
#include<string.h>
#include <algorithm>
#include <malloc.h>
#include<process.h>
#include<wchar.h>
#include <sys/types.h>
#include<vector>
#include <ctime>

using namespace std;

class Node
{
    public:
        string name;
        bool isFile;
        Node* parent;
        string data;
        vector<Node*> children;
        string createdAt;
        string modifiedAt;
        
        Node(string name, bool isFile, Node* parent)
        {
            time_t ttime = time(0);
            this->name = name;
            this->isFile = isFile;
            this->parent = parent;
            this->createdAt = this->modifiedAt = strtok(ctime(&ttime),"\n");

            if(!isFile)
            {
                this->data = "";    
            }
        }
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
    cout<<"touch [filename] - Create a file with the name [filename]." << endl;
    cout << "delete [filename] - Delete the file specified in [filename]." << endl;
    cout << "read [filename] - Reads the file specified in [filename]." << endl;
    cout << "write [filename] - Writes to the file specified in [filename]." << endl;
    cout << "help - Display the user manual." << endl;
    cout << "exit - Exit the shell.\n" << endl;
}

void printMessage(string message)
{
    cout << message << endl;
}

string reverseString(string directory)
{
    string rev = "";
    
    for(int i=directory.length()-1; i>=0; i--){
        rev = rev + directory[i];  
    }

    return rev;
}

int main()
{
    string path;
    struct Node *root = new Node("root", false, NULL);
    struct Node *current=root;
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
                if(root->children[i]->name==path && root->children[i]->isFile==false)
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
                struct Node *mkdir = new Node(path, false, current);
                current->children.push_back(mkdir);
                time_t ttime = time(0);
                current->modifiedAt = strtok(ctime(&ttime),"\n");
            }
        }
        else if(command == "ls")
        {
            if(current->children.size()!=0)
            {
                cout<<"No.\tName\tisFile\tSize\t\tCreated At\t\t\tModified At"<<endl;
                for(int i=0;i<current->children.size();i++)
                {
                    if(current->children[i]->isFile==1)
                    {
                        cout<<i+1<<"\t"<<current->children[i]->name<<"\t  "<<"True\t"<< current->children[i]->data.length() <<"\t"<<current->children[i]->createdAt<<"\t"<<current->children[i]->modifiedAt<<endl;
                    }
                    else
                    {
                        cout<<i+1<<"\t"<<current->children[i]->name<<"\t  "<<"False\t"<<"---\t"<<current->children[i]->createdAt<<"\t"<<current->children[i]->modifiedAt<<endl;
                    }
                    
                }
                cout<<endl;
            }
        }
        else if(command == "cd")
        {
            if(path==".." && current->parent!=NULL)
            {
                current=current->parent;  
                string temp=reverseString(directory);
                temp=temp.substr(temp.find("\\")+1,temp.length()-1);
                directory=reverseString(temp);
            }
            else
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
            }
        }
        else if(command=="touch")
        {
            bool exists=false;
            for(int i=0;i<current->children.size();i++)
            {
                if(current->children[i]->name==path && current->children[i]->isFile)
                {  
                    exists=true;
                    break;
                }
            }

            if(exists)
            {
                printMessage("File already exists");
            }
            else
            {
                struct Node *file = new Node(path, true, current);
                current->children.push_back(file);
                time_t ttime = time(0);
                current->modifiedAt = strtok(ctime(&ttime),"\n");
            }
        }
        else if(command=="delete")
        {
            int i=0;
            bool exists=false;
            for(i=0;i<current->children.size();i++)
            {
                if(current->children[i]->name==path && current->children[i]->isFile)
                {  
                    exists=true;
                    break;
                }
            }

            if(!exists)
            {
                printMessage("File doesnot exists");
            }
            else
            {
                current->children.erase(current->children.begin()+i);
                time_t ttime = time(0);
                current->modifiedAt = strtok(ctime(&ttime),"\n");
            }
        }
        else if(command=="write")
        {
            bool exists=false;
            for(int i=0;i<current->children.size();i++)
            {
                if(current->children[i]->name==path && current->children[i]->isFile)
                {  
                    exists=true;
                    break;
                }
            }

            if(!exists)
            {
                printMessage("File doesnot exists");
            }
            else
            {
                string data;
                string mode;

                while(1)
                {
                    cout<<"Enter mode (a/w): ";
                    getline(cin,mode);

                    if(mode=="a" || mode=="w")
                    {
                        break;
                    }
                }

                cout<<"Enter data: ";
                getline(cin,data);
                for(int i=0;i<current->children.size();i++)
                {
                    if(current->children[i]->name==path && current->children[i]->isFile)
                    {  
                        if(mode=="a")
                        {
                            current->children[i]->data+=data;
                        }
                        else
                        {
                            current->children[i]->data=data;
                        }
                        time_t ttime = time(0);
                        current->children[i]->modifiedAt = strtok(ctime(&ttime),"\n");
                        current->modifiedAt = current->children[i]->modifiedAt;
                        break;
                    }
                }
            }
        }
        else if(command=="read")
        {
            int i=0;
            bool exists=false;
            for(i=0;i<current->children.size();i++)
            {
                if(current->children[i]->name==path && current->children[i]->isFile)
                {  
                    exists=true;
                    break;
                }
            }

            if(!exists)
            {
                printMessage("File doesnot exists");
            }
            else if(current->children[i]->data.size()!=0)
            {
                string mode;
                string start="0";
                while(1)
                {
                    
                    cout<<"Enter mode (s/f): ";
                    getline(cin,mode);

                    if(mode=="f")
                    {
                        cout<<"Enter index: ";
                        getline(cin,start);
                    }
                    
                    if(mode=="s" || mode=="f")
                    {
                        break;
                    }
                }

                if(mode=="s")
                {
                    cout<<current->children[i]->data<<endl;

                }
                else
                {
                    for(int j=stoi(start);j<current->children[i]->data.size();j++)
                    {
                        cout<<current->children[i]->data[j];
                    }
                    cout<<endl;
                }
            }
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
