#include <string>
#include "galik_socketstream.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <cassert>
#include "structs.h"
#include "actions.h"
#include "heuristics.h"
using namespace std;
using namespace galik;
using namespace galik::net;

socketstream ss;
map<string, Stock> stocks;
map<string, Stock>::iterator it;
string owned_stocks[NUM_OWNED];
double my_cash = 0;
int share_ct = 0;

////////////////////////
// Action wrappers

bool sell(string ticker, double price, int shares)
{
    cerr << "Selling " << shares << " of " << ticker << " at " << price << endl;
    price -= EPS;
    Stock &stk = stocks[ticker];
    stringstream oss;
    oss << "ASK " << ticker << " " << price << " " << shares;
    string res;
    get_data(oss.str(), res);

    update_owned();
    update_orders(ticker);
    return (res == "ASK_OUT DONE");
}

bool buy(string ticker, double price, int shares)
{
    if (shares <= 0) return false;
    cerr << "Buying " << shares << " of " << ticker << " at " << price << endl;
    price += EPS;
    Stock &stk = stocks[ticker];
    stringstream oss;
    oss << "BID " << ticker << " " << price << " " << shares;
    string res;
    get_data(oss.str(), res);

    stocks[ticker].owned_val = price;
    update_owned();
    update_orders(ticker);
    return (res == "BID_OUT DONE");
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

int get_share_num(double price)
{
    // spend 5% of our cash for each investment
    return (int) min(my_cash/(price*50), MAX_SHARES-share_ct);
}

bool worth_it(string ticker, double price, int shares)
{
    Stock &stk = stocks[ticker];
    int time_held = (int)time(NULL)-stk.owned_time; 
    double net_gain = (price-stk.owned_val)*shares+stk.div_rat*time_held*shares;
    // cerr << setprecision(16);
    // cerr << stk.owned_val << endl;
    cerr << "net_gain: " << net_gain << " time_held: " << time_held << endl;
    if (net_gain > 0.1*my_cash) stocks[ticker].chocolatey++;
    else if (net_gain < -0.1*my_cash) stocks[ticker].chocolatey--;
    return (net_gain > 0);
}

////////////////////////
// Bleh

int main(int argc, char** argv)
{
    cout << setprecision(16);
    ss.open("codebb.cloudapp.net", 17429);
    ss << "Z2 0312" << endl;

    while(true) {
//        cerr << "Updating info" << endl;
        update_cash();
        update_stocks();
        update_owned();
        cerr << setprecision(16);
//        print_everything();

        vector<string> tickers = sort_value();

        int repeat = 5;
        while (repeat > 0) {
            cerr << "Deciding what to sell" << endl;
            for (int i = 0; i < NUM_OWNED; i++) {
                string ticker = owned_stocks[i];
                if (ticker == "") continue;

                int idx = (tickers.size() - NUM_OWNED)/2;
                Stock &stk = stocks[ticker];
                if (stk.owned_num == 0) {
                    owned_stocks[i] = "";
                    continue;
                }
                Order maxbid = max_bid(ticker);
                double price = maxbid.value; int shares = min(stk.owned_num, maxbid.shares);
                if (worth_it(ticker, price, shares)) {
                    bool sell_res = sell(ticker, price, shares);
                    if (sell_res) share_ct -= shares;
                }
            }
            repeat--;
            sleep(1);
        }

        cerr << "Deciding what to buy" << endl;
        tickers = sort_value();
        for (int i = 0; i < NUM_OWNED; i++) {
            string ticker = owned_stocks[i];
            if (ticker != "" || share_ct > MAX_SHARES) continue;

            int idx = (tickers.size() - NUM_OWNED)/2;
            ticker = tickers[idx + i];
            Order minask = min_ask(ticker);
            int shares = get_share_num(minask.value)+stocks[ticker].chocolatey;
            bool buy_res = buy(ticker, minask.value, shares);
            if (buy_res) share_ct += shares;
            owned_stocks[i] = ticker;
            // cerr << "bought at price " << stocks[ticker].owned_val << endl;
        }

        cerr << "\nCurrent cash: " << my_cash << endl << endl;
        sleep(1);
    }
    string tmp;
    get_data("CLEAR_BID", tmp);
    get_data("CLEAR_ASK", tmp);
    return 0;
}
