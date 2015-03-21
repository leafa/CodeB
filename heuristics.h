#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include "structs.h"
#include <string>

double estimate_value(std::string ticker);
Order max_bid(std::string ticker);
Order min_ask(std::string ticker);
bool do_sell(int i);

#endif
