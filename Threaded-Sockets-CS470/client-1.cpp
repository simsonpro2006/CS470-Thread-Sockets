#include <stdio.h> 

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "tools.h"
#include <stdlib.h> 
#include "ini.h"
//#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    using namespace std;
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    const char *hello = "Hello from client"; 
    inireader information(argv[1]);

    int PORT = std::stoi(information.getinfo("Connection","Port"));
    std::string IP = information.getinfo("Connection","IP");
    int timeout = std::stoi(information.getinfo("Connection","Timeout"));
    
    int n = 0;
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, IP.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    while (true)
    {
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0) 
        { 
            break;
        } 
        n++;
        if (n>timeout)
        {
            std::cout<<"Error time out reached "<<std::endl;
            return 1;
        }
        else

        {
            std::cout<<"connection failed. Retry in 1s "<<std::endl;
            sleep(1);
        }
    }
    while(1)
    {
    valread = read( sock , buffer, 1024); 
    if (valread<=0)
    {
        break;
    }
    
    buffer[valread] = '\0';
    std::string message = buffer;
    cout<<message.size()<<endl;
    cout<<message<<endl; 
    
    
    std::vector<std::string>mymessage;
    
   
    mymessage =tools::split(message,",");
    cout<<mymessage[1]<<endl;
    long int theBase=0;
    long int thePrimenumber =0;
    long int theNumber=0;
    int theCounter =0;
    
    cout<<mymessage[2]<<endl;
    theBase = std::stol(mymessage[1]);
    
    thePrimenumber = std::stol(mymessage[2]);
    theNumber = tools::toDeci(mymessage[0].c_str(),theBase);
    std::cout<<"the Num: "<< theNumber << "the  prime number is: "<< thePrimenumber <<std::endl;
    theCounter = tools::PrimeFactor(theNumber, thePrimenumber);
    std::string theMessage = std::to_string(thePrimenumber)+ "," +std::to_string(theCounter);
    send(sock,theMessage.c_str(),theMessage.size(),0);
    

    usleep(100);
    }
    
    return 0; 
} 

