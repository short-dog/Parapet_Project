//
// Created by Ben Short on 12/6/23.
//
#ifndef PERIMETER_H
#define PERIMETER_H
#include <string>
#include <vector>

class Perimeter {
public:
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

 /* newItem.expenseName = "Fortnite Battle Pass";
    newItem.expenseMerchant = "Epic Games, Inc.";
    newItem.year = 2023;
    newItem.month = 12;
    newItem.day = 6;
    newItem.hour = 21;
    newItem.minute = 31;
    newItem.second = 26;
    newItem.expenseCost = 9.99;
    newItem.expenseCategory = "Gaming";
    newItem.isRecurring = false;
    newItem.expenseFundingLocation = "PayPal";
    newItem.isTaxRelevant = false; */
};



#endif //PERIMETER_H
