#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>
#include <limits.h>
#include <vector>
#include <bits/stdc++.h> 
#include <iostream>
#include <unordered_map>
#include <cstring>
namespace tools {

void CreateARandomNumber(long int &n)
{   
    srand (time(NULL));
    //long int TheBase = 0;
    long int TheNumber = 0;
    std::random_device rd;


    /* Random number generator */
    std::default_random_engine generator(rd());

    /* Distribution on which to apply the generator */
    std::uniform_int_distribution<long int> distribution(LONG_MAX / 2, LONG_MAX);

    TheNumber = distribution(generator);

   // TheBase = rand() % 9 + 2;
  //  b = TheBase;
    n = TheNumber;
    //n = rand() % 1000000 + 2;
    if (n> 10000000)
    {
        TheNumber = distribution(generator);
        n= TheNumber;
    }
}
void CreateABaseNumber(int &b)
{
    srand (time(NULL)); 
    long int TheBase = 0;
    TheBase = rand() % 9 + 2;
    b = TheBase;

}
std::string conv(long int decimal, long int base)
{
    if(decimal == 0) return "0";
    char NUMS[] = "0123456789ABCDEF"; // Characters that may be used
    std::string result = ""; // Create empty string ready for data to be appended
    do{
        result.push_back(NUMS[decimal%base]);
        // Append the appropriate character in NUMS based on the equation decimal%base
        
        decimal /= base; // Calculate new value of decimal
    }while(decimal != 0); // Do while used for slight optimisation
    
    return std::string(result.rbegin(), result.rend());
}


std::vector<int>* CreateAPrimeNumber(int amountOfPrimeNumber)
{
    std::vector<int>* Prime = new std::vector<int>();
    Prime->push_back(2);
    Prime->push_back(3);
    for (int i=2; i<amountOfPrimeNumber; i++) 
        for (int j=2; j*j<=i; j++)
        {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i)) {
                Prime->push_back(i);

            }

        }   

    return Prime;
}
std::vector<std::string> split(std::string input, const std::string &delimiter)
{
    using namespace std;
    size_t loca = 0;
    vector<string> numberfromtext;
    while ((loca = input.find(delimiter)) != string::npos)
    {
        string TokenNumber = input.substr(0, loca);
        input.erase(0, loca+ delimiter.size());
        numberfromtext.push_back(TokenNumber);


    }
    numberfromtext.push_back(input);

    //string token = input.substr(0,)
    return numberfromtext;
}
int val(char c) 
{ 
    if (c >= '0' && c <= '9') 
        return (long int)c - '0'; 
    else
        return (long int)c - 'A' + 10; 
} 
long int toDeci(const char *str, int base) 
{ 
    long int len = strlen(str); 
    long int power = 1; // Initialize power of base 
    long long int num = 0;  // Initialize result 
    long int i; 
  
    // Decimal equivalent is str[len-1]*1 + 
    // str[len-1]*base + str[len-1]*(base^2) + ... 
    for (i = len - 1; i >= 0; i--) 
    { 
        // A digit in input number must be 
        // less than number's base 
        if (val(str[i]) >= base) 
        {
           return -1; 
        } 
  
        num += val(str[i]) * power; 
        power = power * base; 
    } 
  
    return num; 
} 
int PrimeFactor(int n, int prime)
{
    int counter = 0;
    while (n % prime == 0)
    {
        counter += 1;
        n = n/prime;
    }
    return counter;
}
bool isInGivenBase(long int a, int base) 
{ 
    while(a>0)
    {
        if (a%10 >=base)
            return false;
        a /=10;
    }
    return true;
} 
     
} 
































#endif