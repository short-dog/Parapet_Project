//
// Created by Ben Short on 12/6/23.
//

#include "Perimeter.h"

#include <numeric>

void Perimeter::addNewExpense(expenseItem){
    expenseItem newItem;
    if(newItem.isRecurring == true) {
        recurringExpenses.push_back(newItem);
    }else expenseData.push_back(newItem);
}
void Perimeter::showTotalMonthlyExpenses() {
   // totalMonthlyExpenses += std::accumulate(expenseData.begin(), expenseData.end(), 0);
    //totalMonthlyExpenses += std::accumulate(expenseData.begin(), expenseData.end(),0);
}
