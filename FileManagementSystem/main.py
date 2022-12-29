import time
import os
import sys
from datetime import datetime

class Node:
    name = ""
    isFile= False
    isOpen= False
    parent= None
    children = []
    data = ""
    mode = ""
    createdAt = ""
    modifiedAt = ""
    startPos = 0
    endPos = 0

    def __init__(self, name, isFile, parent, start, end):
        self.name = name
        self.isFile = isFile
        self.isOpen = False
        self.parent = parent
        self.createdAt = self.modifiedAt = str(datetime.now())
        self.startPos = start
        self.endPos = end


def printTree(node, level):
    message=""
    if node.isFile:
        message+=("\t" * level)+node.name
    else:
        message+=("\t" * level)+node.name
        for child in node.children:
            message+=printTree(child, level + 1) 
    
    return message

def searchTree(node, name):
    if node.name == name:
        return node
    else:
        for child in node.children:
            result = searchTree(child, name)
            if result != None:
                return result
    return None

def getHelp():
    print("ls [directory] - Display a list of files and subdirectories in a directory.")
    print("create [filename] - Create a file with the name [filename].")
    print("delete [filename] - Delete the file specified in [filename].")
    print("mkdir [directory] - Create the directory specified in [directory].")
    print("cd [directory] - Change the current directory to [directory].")
    print("move [source] [destinaiton] - Moves source to specified [directory].")
    print("open [filename] [m]- Open the file specified in [filename].")
    print("close [filename] - Close the file specified in [filename].")
    print("write [filename] [m] [p] [data]- Writes to the file specified in [filename].")
    print("read [filename] [m] [p] - Reads the file specified in [filename].")
    print("truncate [filename] [p]- Truncates the file specified in [filename].")
    print("memory - Displays the memory tree.")
    print("clear - Clears the screen.")
    print("help - Display the user manual.")
    print("exit - Exit the shell.\n")


def tokenize(command):
    tokens = command.split(" ")
    return tokens

def main():
    size=512
    memory = [None] * size
    memcounter=0
    root=Node("root", False, None, 0, 0)
    path=""
    current=root
    directory=current.name

    print("\nOS File Management System [Version 10.1.1]. All rights reserved.\n\n")

    while True:
        number=0
        start=""
        mode=""
        command=""
        data=""

        command=input(directory + "\\> ")

        tokens=["", "", "", "", ""]
        tokens=tokenize(command)

        for i in range(0,len(tokens)):
            if tokens[i]!="":
                number+=1

        for i in range(0,number):
            if i==0 and tokens[i]!="":
                command=tokens[i]
            elif i==1 and tokens[i]!="":
                path=tokens[i]
            elif i==2 and tokens[i]!="":
                mode=tokens[i]
            elif i==3 and tokens[i]!="":
                start=tokens[i]
            elif i==4 and tokens[i]!="":
                data=tokens[i]

        if command=="mkdir":
            if(number>=2):
                exists=False
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile==False:
                        exists=True
                        break

                if exists:
                    print("Directory already exists.")
                else:
                    node=Node(path, False, current,0, 0)
                    current.children.append(node)
                    current.modifiedAt=str(datetime.now())

        elif command=="ls":
            if(len(current.children)!=0):
                print("No.\tName\tisFile\tSize\t\tCreated At\t\t\tModified At");
                for i in range(len(current.children)):
                    print(str(i+1)+"\t"+current.children[i].name+"\t"+str(current.children[i].isFile)+"\t"+str(sys.getsizeof(current.children[i])) +"\t"+current.children[i].createdAt+"\t"+current.children[i].modifiedAt)
                print("\n")        

        elif command=="cd":
            if(number>=2):
                if(path==".." and current.parent!=None):
                    current=current.parent
                    directory=directory[:directory.rfind("\\")]
                else:
                    for i in range(len(current.children)):
                        if current.children[i].name==path:
                            directory+="\\"+current.children[i].name
                            current=current.children[i]
                            break

        elif command=="move":
            if (number>=3):
                exists=False
                index=0
                for i in range(len(current.children)):
                    if current.children[i].name==path:
                        exists=True
                        index=i
                        break

                if exists:
                    source=current.children[index]
                    destination=searchTree(root, mode)
                    
                    if destination!=None and destination.isFile==False and source.name!=destination.name:
                        destination.children.append(source)
                        source.parent=destination
                        current.children.pop(index)
                        current.modifiedAt=str(datetime.now())

                else:
                    print("File does not exist.")

        elif command=="create":
            if(number>=2):
                exists=False
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile:
                        exists=True
                        break

                if(size==0):
                    print("Not enough memory!\n")
                    break

                if exists:
                    print("File already exists.")
                else:
                    node=Node(path, True, current, 0, 0)
                    current.children.append(node)
                    current.modifiedAt=str(datetime.now())

        elif command=="delete":
            if(number>=2):
                exists=False
                index=0
                for i in range(len(current.children)):
                    if current.children[i].name==path:
                        exists=True
                        index=i
                        break

                if exists:
                    current.children.remove(current.children[index])
                    current.modifiedAt=str(datetime.now())
                else:
                    print("File does not exist.")

        elif command=="open":
            if(number>=2):
                exists=False
                index=0
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile:
                        exists=True
                        index=i
                        break

                if exists:
                    current.children[index].isOpen=True
                    current.children[index].mode=mode
                    current.modifiedAt=str(datetime.now())
                else:
                    print("File does not exist.")

        elif command=="close":
            if (number>=2):
                exists=False
                index=0;
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile:
                        exists=True
                        index=i
                        break

                if exists:
                    current.children[index].isOpen=False
                    current.modifiedAt=str(datetime.now())
                else:
                    print("File does not exist.")

        elif command=="write":
            if(number>=4):
                exists=False
                index=0
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile:
                        exists=True
                        index=i
                        break

                if exists:
                    if current.children[index].isOpen==False:
                        print("File is not open.")
                    else:
                        data=""
                        for j in range(3,number):
                            data+=tokens[j]+" "

                        if mode=="w":
                            current.children[index].data=data
                            current.children[index].size=len(data)
                            current.modifiedAt=str(datetime.now())
                        elif mode=="a":
                            current.children[index].data+=data
                            current.children[index].size=len(data)
                            current.modifiedAt=str(datetime.now())
                else:
                    print("File does not exist.")

        elif command=="read":
            if(number>=3):
                exists=False
                index=0
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile:
                        exists=True
                        index=i
                        break

                if exists:
                    if current.children[index].isOpen==False:
                        print("File is not open.")
                    else:
                        if len(current.children[index].data)!=0:
                            if mode=="s":
                                print(current.children[index].data)
                            else:
                                if(number>=4):
                                    for j in range(int(start),len(current.children[index].data)):
                                        data+=current.children[index].data[j]
                                    print(data)
                else:
                    print("File does not exist.")

        elif command=="truncate":
            if(number>=3):
                exists=False
                index=0
                for i in range(len(current.children)):
                    if current.children[i].name==path and current.children[i].isFile:
                        exists=True
                        index=i
                        break

                if exists:
                    if current.children[index].isOpen==False:
                        print("File is not open.")
                    else:
                        current.children[index].data=current.children[index].data[:int(mode)]
                        current.modifiedAt=str(datetime.now())
                else:
                    print("File does not exist.")

        elif command=="memory":
            message=printTree(root, 0)
            print(message)
        elif command=="help":
            getHelp()
        elif command=="clear":
            os.system('cls')
            print("\nOS File Management System [Version 10.1.1]. All rights reserved.\n\n")
        elif command=="exit":
            exit()
        else:
            print("'"+command+"' is not recognized as an internal or external command, operable program or batch file.\n")

if __name__=="__main__":
    main()