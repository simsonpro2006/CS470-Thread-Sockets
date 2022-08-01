#ifndef INI_H_INCLUDED
#define INI_H_INCLUDED

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include<bits/stdc++.h> 


class inireader
{
    public:
        inireader(std::string filename);
        std::string getinfo(std::string section,std::string key);

    private:
        std::unordered_map<std::string, std::string> myinfo;
        std::vector<std::string> split(std::string input, const std::string &delimiter);
        void ltrim(std::string &s);
        void rtrim(std::string &s);
        void trim(std::string &s);
};
std::string inireader::getinfo(std::string section,std::string key)
{
    std::string myinformation= section + key;
    transform(myinformation.begin(), myinformation.end(), myinformation.begin(), ::tolower);

    // Make sure myinformation exists in table
    if (myinfo.count(myinformation))
    {
        std::string thevalue = myinfo.find(myinformation)->second;
        return thevalue;
    }

    else
    {
        std::cout<< "The key doesnt exist: "<< section << ", " <<key<< std::endl;
    }
    return std::string("");
    
}

inireader::inireader(std::string filename)
{
    using namespace std;
    string line;
    ifstream myinifile(filename); 
    string currentsection = "";
    if (myinifile.good())
    {
    while ( getline (myinifile,line) )
    {   
        trim(line);
        if (line.empty())
        {
            continue;
        }

        if (line[0]=='[' && line[line.length()-1]==']')
        {
            line = line.erase(0,1);
            line = line.erase(line.length()-1,1);
            trim(line);
            currentsection = line;
            continue;
        }
        vector<string> mystuff = split(line,"=");
        if(mystuff.size() != 2)
        {
            cout << "error processing line " << line << endl;
            continue;
        }
        trim(mystuff[0]);
        trim(mystuff[1]);

        string key = currentsection + mystuff[0];
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        myinfo[key]= mystuff[1];
    }

    myinifile.close();
    }

  else  
        cout << "Unable to open file"; 
}
//#####################################
std::vector<std::string> inireader::split(std::string input, const std::string &delimiter)
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

void inireader::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void inireader::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void inireader::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

#endif