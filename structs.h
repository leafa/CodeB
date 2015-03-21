#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <map>
#include <vector>
#include <string>
#include <ctime>
#include "galik_socketstream.h"
using namespace galik;
using namespace galik::net;

#define NUM_OWNED 6
#define MAX_SHARES 100
#define max(a,b) ((a>b)?(a):(b))
#define min(a,b) ((a<b)?(a):(b))
#define INF (1<<30)
#define EPS 1e-7

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
    int owned_time;
    int chocolatey;
Stock() : chocolatey(0), owned_val(-1), owned_num(0), owned_time(time(NULL)) {};
Stock(double n, double d, double v) : chocolatey(0), net_worth(n), div_rat(d), volat(v), owned_val(-1), owned_num(0), owned_time(time(NULL)) {};
};

extern socketstream ss;
extern std::map<std::string, Stock> stocks;
extern std::map<std::string, Stock>::iterator it;
extern std::string owned_stocks[NUM_OWNED];
extern double my_cash;
extern int share_ct;

#endif
