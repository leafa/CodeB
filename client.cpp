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
using namespace galik;
using namespace galik::net;

map<string, Stock> stocks;
map<string, Stock>::iterator it;
string owned_stocks[NUM_OWNED];
double my_cash = 0;

////////////////////////
// Action wrappers

bool sell(string ticker, double price, int shares)
{
    Stock &stk = stocks[ticker];
    stringstream oss;
    oss << "BID " << ticker << " " << price << " " << shares;
    string res;
    get_data(oss.str(), res);

    update_owned();
    update_orders(ticker);
    return (res == "BID_OUT DONE");
}

bool buy(string ticker, double price, int shares)
{
    Stock &stk = stocks[ticker];
    stringstream oss;
    oss << "ASK " << ticker << " " << price << " " << shares;
    string res;
    get_data(oss.str(), res);

    update_owned();
    update_orders(ticker);
    return (res == "ASK_OUT DONE");
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

////////////////////////
// Bleh

int main(int argc, char** argv)
{
    cout << setprecision(16);

    while(true) {
        update_cash();
        update_stocks();
        update_owned();
        vector<string> tickers = sort_value();

        // decide what to sell
        for (int i = 0; i < NUM_OWNED; i++) {
            string ticker = owned_stocks[i];
            if (ticker == "") continue;
            
            Stock &stk = stocks[ticker];
            Order maxbid = max_bid(ticker);
            if (stk.owned_val < maxbid.value)
                sell(ticker, maxbid.value, maxbid.shares);
        }

        // decide what to buy
        tickers = sort_value();
        for (int i = 0; i < NUM_OWNED; i++) {
            string ticker = owned_stocks[i];
            if (ticker != "") continue;

            int idx = (tickers.size() - NUM_OWNED)/2;
            ticker = tickers[idx + i];
            Order minask = min_ask(ticker);
            int shares = 10;
            buy(ticker, minask.value, minask.shares);
            owned_stocks[i] = ticker;
        }

        sleep(1);
    }
    return 0;
}
