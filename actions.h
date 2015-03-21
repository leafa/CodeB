#ifndef __ACTIONS_H
#define __ACTIONS_H

#include <string>

void get_data(std::string command, std::string& res);
void update_orders(std::string ticker);
void update_stocks();
void update_cash();
void update_owned();
void print_everything();

#endif
