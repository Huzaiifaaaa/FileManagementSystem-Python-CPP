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

using namespace std;

std::string getCurrentDirectory() {
   char buff[FILENAME_MAX]; 
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}

void printMessage(string message)
{
    cout << message << endl;
}

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

void printCurrentDirectory()
{
    DIR *directory;//pointer to directory
    struct dirent *direct;//pointer to directory
    directory = opendir(".");//opening directory
    if (directory)
    {
        while ((direct = readdir(directory)) != NULL )//looping till every file is iterated
        {
            printf("%s\n", direct->d_name);//printing file name;
        }
        closedir(directory);//closing directory
        printf("\n");//new line
    }
}

void createFile(string path)
{
    fstream file;
    file.open(path, ios::out);
    if(!file)
    {
       printMessage("Error in creating file");
       return;
    }

    file.close();
}

void deleteFile(string path)
{
    if(remove( path.c_str() ) != 0 )
    {
        printMessage("Error in deleting file");
    }
}

int main()
{
    cout<<"OS File Management System [Version 10.0.0]. All rights reserved.\n\n"<<endl;
    while(1)
    {
        string command;
        cout << getCurrentDirectory() << "\\> ";
        getline (cin, command);
        string path=command.substr(command.find(" ")+1,command.length()-2);
        command=command.substr(0,command.find(" "));

        if(command == "mkdir")
        {
            int status=mkdir(path.c_str());

            if (status)
            {
                printMessage("Directory already exists.");
            }
        }
        else if(command == "ls")
        {
            printCurrentDirectory();
        }
        else if(command == "cd")
        {
            int status=chdir(path.c_str());
            if(status<0)
            {
                printMessage("The system cannot find the path specified.");
            }
        }
        else if(command=="touch")
        {
            createFile(path);
        }
        else if(command=="delete")
        {
            deleteFile(path);
        }
        else if(command=="help")
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
    return 0;
}
