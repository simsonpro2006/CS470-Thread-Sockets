#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
#include "tools.h"
#include <cstdlib>   
#include<unordered_map>
#include <map>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include "ini.h"
#define TRUE   1  
#define FALSE  0  
  
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER; 

struct attribute
{
    int sd;
    std::vector<int> *Primelist;
    int *counter;
    std::map<long int, int> *myHash;
    std::string theMessage;
    bool *checker;

};




void *pthreadfunction(void *s )
{
    
    std::cout<< "create something"<<std::endl;
    //recasting your null pointer to struct pointer
    attribute *p = reinterpret_cast<attribute*>(s);
    char* buffer = new char[1025];
    //
    while(true)
    {
    pthread_mutex_lock(&lock);
    if (*(p->counter) >= p->Primelist->size())
    {
        *(p)->checker = false;
        pthread_mutex_unlock(&lock);
         
        break;   
    }
    int valread;
    
    int nextPrime = (*p->Primelist)[(*p->counter)];
    
    *(p->counter) += 1;
    std::string sendmsg = p->theMessage + ","+ std::to_string(nextPrime) ;

    pthread_mutex_unlock(&lock);
    int error = 0;
    socklen_t len = sizeof(error);
    //error checking
    int retval = getsockopt(p->sd,SOL_SOCKET, SO_ERROR, &error, &len);
    if(error !=0 && retval != 0)
    {
        std::cout<<"something is wrong"<<std::endl;
        return nullptr;
    }
    //checking size of send message
    auto valsend = send(p->sd , sendmsg.c_str() , sendmsg.size() , 0 ); 
    if (valsend <= 0)
    {
        std::cout<< "Server failed to send message to client"<<std::endl;
        return nullptr;
    }


    valread = read( p->sd , buffer, 1024);
    if(valread <= 0)
    {
        std::cout<< "the server didnt receive any messages"<<std::endl;
        return nullptr;
    }

    buffer[valread] = '\0';
    std::cout<< buffer<< std::endl;
    // put message in to a string
    std::string message = buffer;
    std::vector<std::string>mymessage;
    mymessage =tools::split(message,",");
    long int thePrimeNumber = std::stol(mymessage[0]);
    int theExponent = std::stoi(mymessage[1]);
    pthread_mutex_lock(&lock1);   
    if (theExponent > 0)
    {
        (*(p->myHash))[thePrimeNumber] = theExponent;
    }
    pthread_mutex_unlock(&lock1);
    }
    delete p;
    delete [] buffer;
    return nullptr;    
}

int main(int argc , char *argv[])   
{

    using namespace std;
    
    int counter =0;
    inireader information(argv[1]);
    int PORT = std::stoi(information.getinfo("Connection","Port"));
    int NumberofPrime = std::stoi(information.getinfo("Other","n"));
    
    cout<<NumberofPrime<<endl;
    cout<<PORT<<endl;

    int Primecounter = 0;
    std::map<long int, int> myHash;
    char buffer[1025];
    bool checker = true;
    bool checkingtheNumber = false;
    long int theNumber;
    
    
    int theBase =0;
    tools::CreateABaseNumber(theBase);
    // while loop. 
    while(!checkingtheNumber)
    {
        tools::CreateARandomNumber(theNumber);
        std::cout << "Num: " << theNumber << ", Base: " << theBase << std::endl;

        checkingtheNumber = tools::isInGivenBase(theNumber,theBase);
        std::cout<<checkingtheNumber<<std::endl;
    }
    std::cout << "Num: " << theNumber << ", Base: " << theBase << std::endl;
    
    std::string stringtheNumber = to_string(theNumber);
    cout<<stringtheNumber<<endl;
    theNumber= tools::toDeci(stringtheNumber.c_str(),theBase);
    cout<<theNumber<<endl;
    if (theNumber<= 0)
    {
        cout<<"This number cant be converted"<<endl;
        return 1;
    }
    std::string theMessage = tools::conv(theNumber,theBase) + "," + std::to_string(theBase);
    std::vector<int>* Primelist = tools::CreateAPrimeNumber(NumberofPrime);
    int opt = TRUE;   
    int master_socket , addrlen , new_socket , client_socket[30] ,  
          max_clients = 30 , activity, i , valread , sd;   
    int max_sd;   
    struct sockaddr_in address;   
        
    //set of socket descriptors  
    fd_set readfds;   
         
    //a message  
    const char *message = "ECHO Daemon v1.0 \r\n";   
     
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }
    int flag = fcntl(master_socket,F_GETFL);   
    fcntl(master_socket,F_SETFL,flag | O_NONBLOCK);  
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 10) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
      
    //accept the incoming connection  
    addrlen = sizeof(address);   
    std::cout<<"Waiting for connections ..." << std::endl << std::flush; 
    std::cout<<"Hello"<<std::endl;   
    //########################################################################################################



   



    std::vector<pthread_t> pthreadid;
    while(checker)   
    {   
         
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            { 
                if (errno == EWOULDBLOCK) {
                printf("No pending connections; sleeping for one second.\n");
                sleep(1);
                continue;
            } else 
            {
                perror("error when accepting connection");
                exit(1);
            }
            }   
             
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
                  (address.sin_port));   
           
            // //send new connection greeting message  
            // if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            // {   
            //     perror("send");   
            // }   
                 
            // puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)   
            {   
                
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    // asign value 
                    pthread_t threadid;
                    attribute *s = new attribute();
                    s->sd = new_socket;
                    s->Primelist = Primelist;
                    s->counter = &Primecounter;
                    s->myHash= &myHash;
                    s->theMessage = theMessage;
                    s->checker = &checker;
                    void *vp = reinterpret_cast<void *> (s); 
                    //create thread.
                    pthread_create(&threadid,NULL,pthreadfunction,vp);
                    pthreadid.push_back(threadid);
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
   

    }
    
    
    for(int i  = 0; i< pthreadid.size();i++)
    {
        pthread_join(pthreadid[i],NULL);
    }

    for(auto const& pair: myHash)
    {
        std::cout<< pair.first<<"^"<< pair.second << "* ";
    }

    for(int i = 0; i <max_clients; i++)
    {
        if (client_socket[i] !=0)
        {
            close(client_socket[i]);
            client_socket[i] =0;  
        }
    }
    close(master_socket);

    return 0;   
}   