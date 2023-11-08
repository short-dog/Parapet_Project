//
// Created by BSHORT2 on 11/1/2023.
//

#include "ParapetRun.h"
#include <iostream>

void ParapetRun::scenarioBuilder() {
    //gathering scenario data
    std::cout << "Enter Current Spending or Type '0' to skip: " << std::endl;
    std::cin >> spendingLevel;

    if (spendingLevel == 0){
        std::cout << "Enter Portfolio Value" << std::endl;
        std::cin >> portfolioValue;
    }
    else{
        std::cout << "Enter Portfolio Value or Type '0' to skip: " << std::endl;
        std::cin >> portfolioValue;
    }

    std::cout << "Enter the file path to your portfolio: " << std::endl;
    std::cin >> filePath;
}
