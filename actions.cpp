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
#include "structs.h"
#include "actions.h"
using namespace std;
using namespace galik;
using namespace galik::net;

////////////////////////
// Updating functions

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

void update_orders(string ticker) {
    stocks[ticker].orders.clear();
    
    string rawres, str;
    get_data("ORDERS " + ticker, rawres);
    stringstream iss(rawres);
    iss >> str;
    if (str != "SECURITY_ORDERS_OUT") {
        cerr << "did not get orders info correctly for " << ticker << endl;
        return;
    }
    
    Order ord;
    while(iss >> str >> ord.value >> ord.shares)
        stocks[ticker].orders.push_back(ord);
}

void update_stocks() {
    string rawres, str;
    get_data("SECURITIES", rawres);
    stringstream iss(rawres);
    iss >> str;
    if (str != "SECURITIES_OUT") {
        cerr << "did not get securities info correctly" << endl;
        return;
    }
    
    Stock stk;
    while(iss >> str >> stk.net_worth >> stk.div_rat >> stk.volat) {
        stocks[str] = stk;
        update_orders(str);
    }
}

void update_cash() {
    string rawres, str;
    get_data("MY_CASH", rawres);
    stringstream iss(rawres);
    iss >> str >> my_cash;
}

void update_owned() {
    string rawres, str;
    get_data("MY_SECURITIES", rawres);
    stringstream iss(rawres);
    iss >> str;
    if (str != "MY_SECURITIES_OUT") {
        cerr << "did not get owned securities info correctly" << endl;
        return;
    }

    int shares; double div_rat;
    while(iss >> str >> shares >> div_rat) {
        stocks[str].owned_num = shares;
        stocks[str].owned_div_rat = div_rat;
    }
}
