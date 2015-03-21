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

#define NUM_OWNED 3

struct Order {
    bool isBid;
    double value;
    int shares;
    Order() : isBid(false), value(0), shares(0) {};
    Order(bool b, double p, int s) : isBid(b), value(p), shares(s) {};
};

struct Stock {
    double net_worth;
    double div_rat;
    double volat;
    vector<Order> orders;
    double owned_val;
    int owned_num;
    double 
    Stock() {};
    Stock(double n, double d, double v) :
        net_worth(n), div_rat(d), volat(v), owned_val(-1.0), owned_num(0) {};
};

map<string, Stock> stocks;
map<string, Stock>::iterator it;
string owned_stocks[NUM_OWNED];

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

vector<string> sort_value()
{
    vector<pair<double, string> > things;
    for (it = stocks.begin(); it != stocks.end(); it++) {
        double val = estimate_value((*it).first);
        things.push_back(make_pair(val, (*it).first));
    }
    sort(things.begin(), things.end());
    vector<string> res;
    for (int i = 0; i < things.size(); i++)
        res.push_back(things[i].second);
    return res;
}

double estimate_value(string ticker)
{
    Stock &stk = stocks[ticker];
    vector<Order> &orders = stk.orders;
    double avg = 0, total_shares = 0;
    for (int i = 0; i < orders.size(); i++) {
        avg += orders[i].value * orders[i].shares;
        total_shares = orders[i].shares;
    }
    return avg/total_shares;
}

double max_bid(string ticker)
{
    Stock &stk = stocks[ticker];
    vector<Order> &orders = stk.orders;
    double maxbid = 0;
    for (int i = 0; i < orders.size(); i++)
        if (orders[i].isBid && orders[i].value > maxbid)
            maxbid = orders[i].value;
    return maxbid;
}

double min_ask(string ticker)
{
    Stock &stk = stocks[ticker];
    vector<Order> &orders = stk.orders;
    double minask = 0;
    for (int i = 0; i < orders.size(); i++)
        if (!orders[i].isBid && orders[i].value < minask)
            minask = orders[i].value;
    return minask;
}

void update_orders(string ticker) {
    cerr << "Getting orders info for " << ticker << endl;
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

void update_info() {
    cerr << "Getting info" << endl;
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

bool sell(string ticker, double price, int shares)
{
    Stock &stk = stocks[ticker];
    stringstream oss;
    oss << "BID " << ticker << " " << price << " " << shares;
    string res;
    get_data(oss.str(), res);
    update_orders(ticker);
    return (res == "BID_OUT DONE");
}

int main(int argc, char** argv)
{
    cout << setprecision(16);

    while(true) {
        update_info();
        vector<string> tickers = sort_value();

        // decide what to sell
        for (int i = 0; i < NUM_OWNED; i++) {
            string ticker = owned_stocks[i];
            if (ticker == "") continue;
            while (stocks[ticker].owned_val < max_ask(ticker)) {
                
            }
        }

        // decide what to buy
        for (int i = 0; i < NUM_OWNED; i++) {
            string ticker = owned_stocks[i];
            if (ticker == "") continue;
        }        
            

        sleep(1);
    }
    return 0;
}
