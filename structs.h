#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <map>
using namespace std;

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
    double owned_div_rat;
    Stock() {};
    Stock(double n, double d, double v) :
        net_worth(n), div_rat(d), volat(v),
        owned_val(-1.0), owned_num(0), owned_div_rat(0) {};
};

map<string, Stock> stocks;
map<string, Stock>::iterator it;
string owned_stocks[NUM_OWNED];
double my_cash = 0;

#endif
