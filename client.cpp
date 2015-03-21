/*
 * File:   main.cpp
 * Author: atamarkin2
 *
 * Created on June 26, 2014, 5:11 PM
 */

#include <string>
#include "galik_socketstream.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

using namespace galik;
using namespace galik::net;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    size_t start = 0, end = 0;
    while ((end = s.find(delim, start)) != string::npos) {
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    elems.push_back(s.substr(start));
    return elems;
}

/*
 *
 */
int main(int argc, char** argv) {
    string name("Z2");
    string password("0312");

    while(!cin.eof()) {
        cout << ">> ";
        string command;
        getline(cin, command);
        socketstream ss;
        ss.open("codebb.cloudapp.net", 17429);
        ss << name << " " << password << "\n" << command << "\nCLOSE_CONNECTION\n" << endl;
        while (ss.good() && !ss.eof()) {
            string line;
            getline(ss, line);
            cout << line << endl;
        }
    }
    return 0;
}
