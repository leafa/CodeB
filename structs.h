#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <map>
#include <vector>
#include <string>

#define NUM_OWNED 3
#define max(a,b) ((a>b)?(a):(b))
#define min(a,b) ((a<b)?(a):(b))

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
    std::vector<Order> orders;
    double owned_val;
    int owned_num;
    double owned_div_rat;
    Stock() {};
    Stock(double n, double d, double v) :
        net_worth(n), div_rat(d), volat(v),
        owned_val(-1.0), owned_num(0), owned_div_rat(0) {};
};

extern std::map<std::string, Stock> stocks;
extern std::map<std::string, Stock>::iterator it;
extern std::string owned_stocks[NUM_OWNED];
extern double my_cash;

#endif
