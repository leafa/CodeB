#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include "structs.h"

double estimate_value(string ticker);
Order max_bid(string ticker);
Order min_ask(string ticker);
bool do_sell(int i);

#endif
