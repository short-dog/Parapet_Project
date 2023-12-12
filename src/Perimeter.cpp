//
// Created by Ben Short on 12/6/23.
//

#include "Perimeter.h"

#include <iomanip>
#include <iostream>
#include <numeric>

void Perimeter::addNewExpense(expenseItem newExpense){
    totalMonthlyExpenses += newExpense.expenseCost;
    if(newExpense.isRecurring == true) {
        recurringExpenses.push_back(newExpense);
    }else expenseData.push_back(newExpense);
    std::cout << "Total Monthly Expenses: " << std::fixed << std::setprecision(2) << totalMonthlyExpenses << std::endl;
}