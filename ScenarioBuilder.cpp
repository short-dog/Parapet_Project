//
// Created by BSHORT2 on 11/8/2023.
//

#include "ScenarioBuilder.h"
#include <iostream>

void ScenarioBuilder::scenarioBuilder() {
    //gathering scenario data
    std::cout << "Enter Current Spending or Type '0' to skip: " << std::endl;
    std::cin >> spendingLevel;

    if (spendingLevel == 0) {
        std::cout << "Enter Portfolio Value" << std::endl;
        std::cin >> portfolioValue;
    } else {
        std::cout << "Enter Portfolio Value or Type '0' to skip: " << std::endl;
        std::cin >> portfolioValue;
    }

    std::cout << "Enter the file path to your portfolio: " << std::endl;
    std::cin >> filePath;
}