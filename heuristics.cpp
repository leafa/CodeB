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
#include "heuristics.h"
using namespace std;

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

Order max_bid(string ticker)
{
    Stock &stk = stocks[ticker];
    vector<Order> &orders = stk.orders;
    double maxbid = 0; int imax = 0;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].isBid && orders[i].value > maxbid) {
            maxbid = orders[i].value;
            imax = i;
        }
    }
    return orders[imax];
}

Order min_ask(string ticker)
{
    Stock &stk = stocks[ticker];
    vector<Order> &orders = stk.orders;
    double minask = INF; int imin = 0;
    for (int i = 0; i < orders.size(); i++) {
        if (!orders[i].isBid && orders[i].value < minask) {
            minask = orders[i].value;
            imin = i;
        }
    }
    return orders[imin];
}

bool do_sell(int i)
{
    string ticker = owned_stocks[i];
    Stock &stk = stocks[ticker];
    if (stk.owned_val < max_bid(ticker).value)
        return true;
    return false;
}
