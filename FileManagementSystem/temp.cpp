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
                    string output="Directory already exists\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
                }
                else
                {
                    struct Node *mkdir = new Node(path, false, current,0,0);
                    string output="";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);  
                    current->children.push_back(mkdir);
                    time_t ttime = time(0);
                    current->modifiedAt = strtok(ctime(&ttime),"\n");
                }
            }
        }
        /*else if(command == "ls")
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
        }*/
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
                            string output="";
                            strcpy(msg, output.c_str()); 
                            send(newSd, (char*)&msg, strlen(msg), 0);                            
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
                string output="File does not exist\n";
                strcpy(msg, output.c_str()); 
                send(newSd, (char*)&msg, strlen(msg), 0);
            }
            else
            {
                Node *source=current->children[i];
                Node *destination=searchTree(root,mode);
                destination->children.push_back(source);
                string output="";
                strcpy(msg, output.c_str()); 
                send(newSd, (char*)&msg, strlen(msg), 0);  
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
                    string output="Not enough memory!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
                }

                if(exists)
                {
                    string output="File already exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
                    
                }
                else
                {
                    size+=sizeof(current->children[i]);

                    for(int k=current->children[i]->startPos;k<current->children[i]->endPos;k++)
                    {
                        memory[k]=0;
                    }

                    string output="";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);  
                    current->children.erase(current->children.begin()+i);
                    time_t ttime = time(0);
                    current->modifiedAt = strtok(ctime(&ttime),"\n");
                }
            }
        }
        /*else if(command=="open")
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
                }
                else
                {
                    for(int i=0;i<current->children.size();i++)
                    {
                        if(current->children[i]->name==path && current->children[i]->isFile && current->children[i]->isOpen==false)
                        {
                            current->children[i]->isOpen=true;
                            current->children[i]->mode=mode;
                            string output="File opened!\n";
                            strcpy(msg, output.c_str()); 
                            send(newSd, (char*)&msg, strlen(msg), 0);
                            break;
                        }
                        else if(current->children[i]->isOpen==true)
                        {
                            string output="File is already opened!\n";
                            strcpy(msg, output.c_str()); 
                            send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
                }
                else
                {
                    for(int i=0;i<current->children.size();i++)
                    {
                        if(current->children[i]->name==path && current->children[i]->isFile && current->children[i]->isOpen==true)
                        {
                            current->children[i]->isOpen=false;
                            current->children[i]->mode="";
                            string output="File closed!\n";
                            strcpy(msg, output.c_str()); 
                            send(newSd, (char*)&msg, strlen(msg), 0);
                            break;
                        }
                        else if(current->children[i]->isOpen==false)
                        {
                            string output="File is already closed!\n";
                            strcpy(msg, output.c_str()); 
                            send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
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
                                string output="Invalid mode!\n";
                                strcpy(msg, output.c_str()); 
                                send(newSd, (char*)&msg, strlen(msg), 0);
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
                            string output="File is not opened!\n";
                            strcpy(msg, output.c_str()); 
                            send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
                }
                else if(current->children[i]->data.size()!=0 && current->children[i]->isOpen==true)
                {
                    if(mode=="s")
                    {
                        cout<<current->children[i]->data<<endl;
                    }
                    else if(mode=="f")
                    {
                        if(number>=4)
                        {
                            for(int j=stoi(start);j<current->children[i]->data.size();j++)
                            {
                                cout<<current->children[i]->data[j];
                            }
                            cout<<endl;
                        }
                    }
                    else
                    {
                        string output="Invalid mode!\n";
                        strcpy(msg, output.c_str()); 
                        send(newSd, (char*)&msg, strlen(msg), 0);
                    }
                }
                else if(current->children[i]->isOpen==false)
                {
                    string output="File not opened!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
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
                    string output="File doesnot exists!\n";
                    strcpy(msg, output.c_str()); 
                    send(newSd, (char*)&msg, strlen(msg), 0);
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
            cout<<"Total: 512 KB's"<<endl;
            cout<<"Used: "<<used<<" KB's"<<endl;
            cout<<"Free: "<<size<<" KB's"<<endl;
            cout<<"\n\n";
            printTree(root,0);

            cout<<"\n\t";
            for(int i=1;i<=512;i++)
            {
                cout<<memory[i-1]<<" ";

                if(i%32==0)
                {
                    cout<<endl;
                    cout<<"\t";
                }
            }

            cout<<endl;
        }
        else if(command=="clear")
        {
            system("cls");
            string output="OS File Management System [Version 10.0.0]. All rights reserved.\n\n";
            strcpy(msg, output.c_str()); 
            send(newSd, (char*)&msg, strlen(msg), 0);
        }
        else if(command == "help")
        {
            getHelp();
        }
        else if(command == "exit")
        {
            exit(0);
        }*/
        else
        {
            string output="'"+command+"'is not recognized as an internal or external command, operable program or batch file.\n\n";
            strcpy(msg, output.c_str()); 
            send(newSd, (char*)&msg, strlen(msg), 0);
        }