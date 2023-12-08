//
// Created by Ben Short on 12/6/23.
//

#ifndef PERIMETER_H
#define PERIMETER_H
#include <list>
#include <string>


class Perimeter {
    double targetBudget;
    double totalMonthlyExpenses;
    struct expenseItem {
        std::string expenseName;
        std::string expenseMerchant;
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        double expenseCost;
        std::string expenseCategory;
        bool isRecurring;
        std::string expenseFundingLocation;
        bool isTaxRelevant;
    };
    std::vector<expenseItem> expenseData;
    std::vector<expenseItem> recurringExpenses;

    void addNewExpense(expenseItem);

    void showTotalMonthlyExpenses();
};



#endif //PERIMETER_H
