#include <direct.h>
#include <unistd.h>
#include<iostream>
#include<stdio.h>
#include <fstream>
#include <chrono>
#include<cstring>
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
#include<pthread.h>

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

void invalidCommand(string command,ofstream outputfile)
{
    cout<< "'"<<command <<"' "<<"is not recognized as an internal or external command, operable program or batch file.\n" << endl;
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


int size = 512;//512KB
string path;
struct Node *root = new Node("root", false, NULL,0,0);

struct Node *current=root;
string directory=current->name;

int memory[512]={0};
int memcounter=0;

void *executeCommand(void *parameter)
{
    int thread=*((int*)(&parameter))+1;
    string inputdata[10];

    ifstream inputfile("input_thread"+to_string(thread)+".txt");
    string text;
    int i=0;
    while (getline (inputfile, text))
    {
        inputdata[i]=text;
        i++;

        if(i==10)
        {
            break;
        }
    }
    inputfile.close();

    ofstream outputfile("output_thread"+to_string(thread)+".txt");
    outputfile<<"\n\nOS File Management System [Version 10.0."<<thread<<"]. All rights reserved.\n\n"<<endl;

    int j=0;
    while(j<i)
    {
        int number=0;
        string start="";
        string mode="";
        string command="";
        string data="";
        command=inputdata[j];
        outputfile << directory << "\\> ";
        outputfile<<command<<endl;
        //getline (cin, command);


        for(int i=0; i<command.length(); i++)
        {
            command[i] = tolower(command[i]);
        }

        vector<string> tokens = tokenize(command, ' ');
        string arguments[5];

        int i=0;
        for(string s: tokens)
        {
            if(i<3)
            {
                arguments[i] = s;
                i++;
            }
            else
            {
                arguments[4] += s + " ";
            }
        }

        command=arguments[0];
        path=arguments[1];
        mode=arguments[2];
        start=arguments[3];
        data=arguments[4];

        for(int i=0;i<5;i++)
        {
            if(arguments[i]!="")
            {
                number++;
            }
        }

        if(command == "mkdir")
        {
            if(number>=2)
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
                    //printMessage("Directory already exists");
                    outputfile<<"Directory already exists"<<endl;
                }
                else
                {
                    struct Node *mkdir = new Node(path, false, current,0,0);
                    // size-=sizeof(mkdir);
                    current->children.push_back(mkdir);
                    time_t ttime = time(0);
                    current->modifiedAt = strtok(ctime(&ttime),"\n");
                }
            }
        }
        else if(command == "ls")
        {
            if(current->children.size()!=0)
            {
                outputfile<<"No.\tName\tisFile\tSize\t\tCreated At\t\t\tModified At"<<endl;
                for(int i=0;i<current->children.size();i++)
                {
                    if(current->children[i]->isFile==1)
                    {
                        outputfile<<i+1<<"\t"<<current->children[i]->name<<"\t  "<<"True\t"<< current->children[i]->data.length() <<"\t"<<current->children[i]->createdAt<<"\t"<<current->children[i]->modifiedAt<<endl;
                    }
                    else
                    {
                        outputfile<<i+1<<"\t"<<current->children[i]->name<<"\t  "<<"False\t"<<"---\t"<<current->children[i]->createdAt<<"\t"<<current->children[i]->modifiedAt<<endl;
                    }

                }
                outputfile<<endl;
            }
        }
        else if(command == "cd")
        {
            if(number>=2)
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
        }
        else if(command=="move")
        {
            int i=0;
            bool sourceexists=false;
            for(i=0;i<current->children.size();i++)
            {
                if(current->children[i]->name==path)
                {
                    sourceexists=true;
                    break;
                }
            }


            if(!sourceexists)
            {
                //printMessage("File does not exist");
                outputfile<<"File does not exist"<<endl;
            }
            else
            {
                Node *source=current->children[i];
                Node *destination=searchTree(root,mode);
                destination->children.push_back(source);
                current->children.erase(current->children.begin()+i);
                time_t ttime = time(0);
                current->modifiedAt = strtok(ctime(&ttime),"\n");
            }
        }
        else if(command=="create")
        {
            if(number>=2)
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


                if(size==0)
                {
                    outputfile<<"Not enough memory!"<<endl;
                    break;
                }

                if(exists)
                {
                    //printMessage("File already exists");
                    outputfile<<"File already exists"<<endl;
                }
                else
                {
                    struct Node *file = new Node(path, true, current,memcounter,memcounter+4);

                    for(int k=memcounter;k<memcounter+4;k++)
                    {
                        memory[k]=1;
                    }

                    current->children.push_back(file);
                    size-=sizeof(file);
                    time_t ttime = time(0);
                    current->modifiedAt = strtok(ctime(&ttime),"\n");
                    memcounter+=4;
                }
            }
        }
        else if(command=="delete")
        {
            if(number>=2)
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
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else
                {
                    size+=sizeof(current->children[i]);

                    for(int k=current->children[i]->startPos;k<current->children[i]->endPos;k++)
                    {
                        memory[k]=0;
                    }

                    current->children.erase(current->children.begin()+i);
                    time_t ttime = time(0);
                    current->modifiedAt = strtok(ctime(&ttime),"\n");
                }
            }
        }
        else if(command=="open")
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

            if(number>=2)
            {
                if(!exists)
                {
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else
                {
                    for(int i=0;i<current->children.size();i++)
                    {
                        if(current->children[i]->name==path && current->children[i]->isFile && current->children[i]->isOpen==false)
                        {
                            current->children[i]->isOpen=true;
                            current->children[i]->mode=mode;
                            //printMessage("File opened.");
                            outputfile<<"File opened."<<endl;
                            break;
                        }
                        else if(current->children[i]->isOpen==true)
                        {
                            //printMessage("File is already open.");
                            outputfile<<"File is already open."<<endl;
                        }
                    }
                }
            }
        }
        else if(command=="close")
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

            if(number>=2)
            {
                if(!exists)
                {
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else
                {
                    for(int i=0;i<current->children.size();i++)
                    {
                        if(current->children[i]->name==path && current->children[i]->isFile && current->children[i]->isOpen==true)
                        {
                            current->children[i]->isOpen=false;
                            current->children[i]->mode="";
                            //printMessage("File closed.");
                            outputfile<<"File closed."<<endl;
                            break;
                        }
                        else if(current->children[i]->isOpen==false)
                        {
                            //printMessage("File is already closed.");
                            outputfile<<"File is already closed."<<endl;
                        }
                    }
                }
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


            if(number>=3)
            {
                if(!exists)
                {
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else
                {
                    size-=sizeof(current->children[i]);
                    for(int i=0;i<current->children.size();i++)
                    {
                        if(current->children[i]->name==path && current->children[i]->isFile && current->children[i]->isOpen==true)
                        {
                            if(mode=="a")
                            {
                                current->children[i]->data+=data;
                            }
                            else if(mode=="w")
                            {
                                current->children[i]->data=data;
                            }
                            else
                            {
                                printMessage("Invalid mode");
                                break;
                            }

                            size+=sizeof(current->children[i]);
                            time_t ttime = time(0);
                            current->children[i]->modifiedAt = strtok(ctime(&ttime),"\n");
                            current->modifiedAt = current->children[i]->modifiedAt;
                            break;
                        }
                        else
                        {
                            //printMessage("File is not opened.");
                            outputfile<<"File is not opened."<<endl;
                        }
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

            if(number>=3)
            {
                if(!exists)
                {
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else if(current->children[i]->data.size()!=0 && current->children[i]->isOpen==true)
                {
                    if(mode=="s")
                    {
                        outputfile<<current->children[i]->data<<endl;
                    }
                    else if(mode=="f")
                    {
                        if(number>=4)
                        {
                            for(int j=stoi(start);j<current->children[i]->data.size();j++)
                            {
                                outputfile<<current->children[i]->data[j];
                            }
                            outputfile<<endl;
                        }
                    }
                    else
                    {
                        //printMessage("Invalid mode");
                        outputfile<<"Invalid mode"<<endl;
                    }
                }
                else if(current->children[i]->isOpen==false)
                {
                    //printMessage("File is not opened.");
                    outputfile<<"File is not opened."<<endl;
                }
            }
        }
        else if(command=="move")
        {
            if(number>=5)
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
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else
                {
                    size-=sizeof(current->children[i]);
                    string data=current->children[i]->data;
                    data=data.substr(stoi(mode),stoi(start));
                    current->children[i]->data.insert(stoi(arguments[4]),data);
                    size+=sizeof(current->children[i]);
                    time_t ttime = time(0);
                    current->children[i]->modifiedAt = strtok(ctime(&ttime),"\n");
                    current->modifiedAt = current->children[i]->modifiedAt;
                }
            }
        }
        else if(command=="truncate")
        {
            if(number>=3)
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
                    //printMessage("File doesnot exists");
                    outputfile<<"File doesnot exists"<<endl;
                }
                else
                {
                    size-=sizeof(current->children[i]);
                    current->children[i]->data=current->children[i]->data.substr(0,stoi(mode));
                    size+=sizeof(current->children[i]);
                    time_t ttime = time(0);
                    current->children[i]->modifiedAt = strtok(ctime(&ttime),"\n");
                    current->modifiedAt = current->children[i]->modifiedAt;
                }
            }
        }
        else if(command=="memory")
        {
            int temp=0;
            int used=512-size;
            outputfile<<"Total: 512 KB's"<<endl;
            outputfile<<"Used: "<<used<<" KB's"<<endl;
            outputfile<<"Free: "<<size<<" KB's"<<endl;
            outputfile<<"\n\n";
            //printTree(root,0);

            outputfile<<"\n\t";
            for(int i=1;i<=512;i++)
            {
                outputfile<<memory[i-1]<<" ";

                if(i%32==0)
                {
                    outputfile<<endl;
                    outputfile<<"\t";
                }
            }

        }
        else if(command=="clear")
        {
            system("cls");
            outputfile<<"OS File Management System [Version 10.0.0]. All rights reserved.\n\n"<<endl;
        }
        else if(command == "help")
        {
            //getHelp();
            outputfile << "ls [directory] - Display a list of files and subdirectories in a directory." << endl;
            outputfile<<"create [filename] - Create a file with the name [filename]." << endl;
            outputfile << "delete [filename] - Delete the file specified in [filename]." << endl;
            outputfile <<"mkdir [directory] - Create the directory specified in [directory]." << endl;
            outputfile << "cd [directory] - Change the current directory to [directory]." << endl;
            outputfile<<"move [source] [destinaiton] - Moves source to specified [directory]." << endl;
            outputfile << "open [filename] [m]- Open the file specified in [filename]." << endl;
            outputfile << "close [filename] - Close the file specified in [filename]." << endl;
            outputfile << "write [filename] [m] [p] [data]- Writes to the file specified in [filename]." << endl;
            outputfile << "read [filename] [m] [p] - Reads the file specified in [filename]." << endl;
            outputfile<< "truncate [filename] [p]- Truncates the file specified in [filename]." << endl;
            outputfile << "memory - Displays the memory tree." << endl;
            outputfile <<"clear - Clears the screen." << endl;
            outputfile << "help - Display the user manual." << endl;
            outputfile << "exit - Exit the shell.\n" << endl;
        }
        else if(command == "exit")
        {
            //exit(0);
            break;
        }
        else
        {
            //invalidCommand(command);
            outputfile << "'"<<command <<"' "<<"is not recognized as an internal or external command, operable program or batch file.\n" << endl;
        }
        j++;
    }
}

int main(int argc, char *argv[])
{
    /*if(argc!=2)
    {
        cout<<"Invalid number of arguments"<<endl;
        return 0;
    }*/

    int threadnum=2;//stoi(argv[1]);

    cout<<"Number of Threads: "<<threadnum<<endl;
    cout<<"\nDefault Input File   |   Default Output File"<<endl;
    for(int i=0;i<threadnum;i++)
    {
        cout<<"input_thread"<<i+1<<".txt    |   output_thread"<<i+1<<".txt"<<endl;
    }

    pthread_t thread[threadnum];

    for(int i=0;i<threadnum;i++)
    {
        pthread_create(&thread[i], NULL, &executeCommand, (void *)i);
    }

    for(int i=0;i<threadnum;i++)
    {
        pthread_join(thread[i], NULL);
    }

    cout<<"\n\nExiting..."<<endl;

}
