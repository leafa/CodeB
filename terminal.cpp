#include <string>
#include "galik_socketstream.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cassert>
using namespace std;
using namespace galik;
using namespace galik::net;

void get_data(string command, string& res) {
    res.clear();
    socketstream ss;
    ss.open("codebb.cloudapp.net", 17429);
    ss << "Z2 0312\n" << command << "\nCLOSE_CONNECTION\n" << endl;
    if (ss.good()) {
        string tmp;
        getline(ss, tmp);
        res += tmp;
    }
    else cerr << "get_data error: cannot get answer" << endl;
}

int main()
{
    string cmd;
    while(true) {
        string res;
        getline(cin, cmd);
        get_data(cmd, res);
        cout << res << endl;
    }
    return 0;
}
