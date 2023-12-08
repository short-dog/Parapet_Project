//
// Created by BSHORT2 on 11/16/2023.

#include "Parapet.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <set>

void Parapet::scenarioBuilder() {
    //gathering scenario data
    std::cout << "Enter Current Monthly Spending or Type '1' to find: " << std::endl;
    std::cin >> monthlySpending;

    if (monthlySpending == 1) {
        std::cout << "Enter Portfolio Value" << std::endl;
        std::cin >> initialPortfolioValue;
    } else {
        std::cout << "Enter Portfolio Value or Type '1' to find: " << std::endl;
        std::cin >> initialPortfolioValue;
    }
    std::cout << "How Much Are You Currently Contributing to This Account, Per Year?" << std::endl;
    std::cin >> yearlyAddition;

    std::cout << "Enter Current Age: " << std::endl;
    std::cin >> planStartAge;

    std::cout << "Enter Retirement Age or Type '1' to find: " << std::endl;
    std::cin >> retirementAge;

    while (retirementAge <= planStartAge) {
        while (retirementAge != 1){
            std::cout << "Input Error: Please enter an age above current age." << std::endl;
            std::cout << "Enter Retirement Age or Type '1' to find: " << std::endl;
            std::cin >> retirementAge;
        }
        break;
    }
    while (retirementAge >= lifeExpectancy) {
        std::cout << "Input Error: Please enter an age lower than life expectancy of: " << lifeExpectancy << std::endl;
        std::cout << "Enter Retirement Age or Type '1' to find: " << std::endl;
        std::cin >> retirementAge;
    }

    planLength = retirementAge - planStartAge;
    portfolioValue = initialPortfolioValue;
    yearlySpending = monthlySpending * 12;

}
void Parapet::getFilePath() {
    std::cout << "Enter the file path to your portfolio: " << std::endl;
    std::cin >> filePath;
}
struct Investment {
    std::string name;
    double rateOfReturn;
    double stdDeviation;
    double portionOfPortfolio;
};
void Parapet::processPortfolio() {
    // Retrieving User Portfolio and Formatting it //
    // Retrieving the portfolio from a CSV file
    std::ifstream file(filePath);
    while(!file.is_open()) {
        std::cerr << "Error: Could not open file. Please retry." << std::endl;
        getFilePath();
        std::ifstream file(filePath);
    }

    //Establishing structure for the CSV info
    std::vector<Investment> investments;
    double totalPortfolioValue = 0.0;
    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::istringstream stream(line);
        std::vector<std::string> columns;
        std::string column;

        while (getline(stream, column, ',')) {
            columns.push_back(column);
        }

        if (columns.size() == 4) {
            Investment investment;
            investment.name = columns[0];
            investment.rateOfReturn = std::stod(columns[1]);
            investment.stdDeviation = std::stod(columns[2]);
            investment.portionOfPortfolio = std::stod(columns[3]);
            totalPortfolioValue += investment.portionOfPortfolio;

            investments.push_back(investment);
        } else {
            std::cerr << "Error: Invalid CSV format." << std::endl;
        }
    }

    // Calculate total return and standard deviation of the portfolio
    for (const Investment &investment: investments) {
        portfolioReturn += investment.rateOfReturn * (investment.portionOfPortfolio / totalPortfolioValue);
        portfolioStdDev += pow(investment.stdDeviation, 2) * (investment.portionOfPortfolio / totalPortfolioValue);
    }

    portfolioStdDev = sqrt(portfolioStdDev);

    // Print individual investments and overall portfolio stats
    std::cout << std::endl << "Individual Investments:" << std::endl;
    for (const Investment &investment: investments) {
        std::cout << "Name: " << investment.name << ", Rate of Return: " << investment.rateOfReturn << "%"
                  << ", Standard Deviation: " << investment.stdDeviation << "%"
                  << ", Portion of Portfolio: " << investment.portionOfPortfolio << std::endl;
    }

    std::cout << "\nOverall Portfolio Metrics:" << std::endl;
    std::cout << "Overall Performance: " << std::setprecision(4) << portfolioReturn << "%" << std::endl;
    std::cout << "Overall Standard Deviation: " << std::setprecision(4) << portfolioStdDev << "%\n" << std::endl;

    //establishing portfolio parameters for Monte Carlo Simulation
    minMC = portfolioReturn - portfolioStdDev;
    maxMC = portfolioReturn + portfolioStdDev;
}

void Parapet::runMonteCarlo() {
    //setting plan parameters
    investmentAmount = portfolioValue;
    runNum = (lifeExpectancy - planStartAge) * simLength;
    years = lifeExpectancy - planStartAge;

    //running simulation
    monteCarloRunCount++;
    for (int i = 0; i < runNum; ++i) {
        std::uniform_real_distribution<> ParaMC(minMC, maxMC);
        std::random_device runSeed;
        std::mt19937 rng(runSeed());
        auto x = ParaMC(rng);
        monteCarloData.push_back(x);
    }
    //calculating portfolio effect of each return
    for (int sL = 0; sL < simLength; ++sL){
        for (int y = 0; y < years; ++y) {
            const int mCDN = y + y * sL;
            const auto mCV = monteCarloData.at(mCDN);
            auto yy = 1 + 0.01 * mCV;
            planResults.push_back(yy);
        }
    }
    yearlyGain = portfolioReturn * 0.01 * portfolioValue;
    returnNeeded = yearlySpending - yearlyGain;

    //calculating final portfolio results
    for (int i = 0; i < simLength; ++i){
        double endValue = portfolioValue;
        for (int ii = 0 + i; ii < years * simLength; ii += simLength) {
            endValue *= planResults[ii];
            if (ii < planLength * 1000 - 1000) {
                endValue += yearlyAddition;
            }
            if (ii > planLength * 1000 - 1000) {
                double adjustedSpending = yearlySpending * pow(1 + inflationRate,ii/1000);
                endValue -= adjustedSpending;
            }
        }
        finalReturn.push_back(endValue);
    }
}
void Parapet::calculateInitialSuccess() {
    //determine success probability of plan
    int successCount = 0;
    for (double n : finalReturn) {
        if (n > 0) {
            ++successCount;
        }
    }
    successProbability = 100 * (static_cast<double>(successCount) / static_cast<double>(simLength));
    if (monthlySpending != 1 && initialPortfolioValue != 1 && retirementAge != 1) {
        std::cout << std::endl << "Success Probability of " << successProbability << "% with: \n Spending Level of $" << std::fixed << std::setprecision(0) << monthlySpending << "\n Portfolio Value of: $" << initialPortfolioValue << "\n and a Retirement Age of: " << retirementAge << std::endl;
    }
}
void Parapet::calculateSuccess() {
    //determine success probability of plan
    int successCount = 0;
    for (double n : finalReturn) {
        if (n > 0) {
            ++successCount;
        }
    }
    successProbability = 100 * (static_cast<double>(successCount) / static_cast<double>(simLength));
}
void Parapet::clearData() {
    monteCarloData.clear();
    planResults.clear();
    finalReturn.clear();
}
void Parapet::findSpending() {
    //find optimal spending level
    while(successProbability > successLevel + 1 || successProbability < successLevel - 1) {
        if (successProbability < successLevel - 80){
            clearData();
            yearlySpending *= 0.5;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 65){
            clearData();
            yearlySpending *= 0.6;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 40){
            clearData();
            yearlySpending *= 0.7;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 15){
            clearData();
            yearlySpending *= 0.8;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 7.5){
            clearData();
            yearlySpending *= 0.9;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 1){
            clearData();
            yearlySpending *= 0.99;
            runMonteCarlo();
            calculateSuccess();
        }
        while(successProbability > successLevel + 1) {
            clearData();
            yearlySpending *= 10;
            runMonteCarlo();
            calculateSuccess();
        }
    }
}
void Parapet::findPortfolioNeeded() {
    //find needed amount to support spending
    while(successProbability < successLevel - 1) {
        if (successProbability < successLevel - 80){
            clearData();
            portfolioValue += 25000;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 65){
            clearData();
            portfolioValue += 15000;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 40){
            clearData();
            portfolioValue += 7500;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 15){
            clearData();
            portfolioValue += 1500;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 7.5){
            clearData();
            portfolioValue += 500;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 1){
            clearData();
            portfolioValue += 100;
            runMonteCarlo();
            calculateSuccess();
        }
        while(successProbability > successLevel + 1) {
            clearData();
            portfolioValue -= 100;
            runMonteCarlo();
            calculateSuccess();
            }
    }
}
void Parapet::findRetirementAge() {
    //find retirement age for current portfolio and spending level
    while(successProbability < successLevel - 1) {
        if (successProbability < successLevel - 80){
            clearData();
            retirementAge += 25;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 65){
            clearData();
            retirementAge += 20;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 40){
            clearData();
            retirementAge += 15;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 15){
            clearData();
            retirementAge += 10;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 7.5){
            clearData();
            retirementAge += 5;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (successProbability < successLevel - 1){
            clearData();
            retirementAge += 1;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
        }
        else if (retirementAge >= lifeExpectancy) {
            printResults();
        }
        while(successProbability > successLevel + 1) {
            clearData();
            retirementAge -= 1;
            planLength = retirementAge - planStartAge;
            runMonteCarlo();
            calculateSuccess();
            }
    }
}
void Parapet::switchFinder() {
    switch(static_cast<int>(initialPortfolioValue)) {
        case 1:
            findPortfolioNeeded();
            portfolioSwitch = true;
    }
    switch(static_cast<int>(monthlySpending)) {
        case 1:
            findSpending();
            spendingSwitch = true;
    }
    switch(retirementAge) {
        case 1:
            findRetirementAge();
            retirementSwitch = true;

    }
    if(1 > initialPortfolioValue && monthlySpending && retirementAge > 1) {
        printResults();
    }
}
void Parapet::printResults() const {
    if (portfolioSwitch == true || spendingSwitch == true || retirementSwitch == true) {
        std::cout << "Success Probability Set: " << std::fixed << std::setprecision(0) << successLevel << "%\n";
        std::cout << "Success Probability Achieved: " << std::fixed << std::setprecision(0) << successProbability << "%\n" << std::endl;
    }
    if (portfolioSwitch == true) {
        std::cout << "Suggested Portfolio Amount: $" << std::fixed << std::setprecision(0) << portfolioValue << "\n";
        std::cout << "Monthly Spending of: $" << std::fixed << std::setprecision(0) << yearlySpending/12 << "\n";
        std::cout << "Retirement Age of: " << std::fixed << std::setprecision(0) << retirementAge << std::endl;
    }
    if (spendingSwitch == true) {
        std::cout << "Suggested Monthly Spending: $" << std::fixed << std::setprecision(0) << yearlySpending/12 << std::endl;
        std::cout << "Portfolio Value of: $" << std::fixed << std::setprecision(0) << portfolioValue << std::endl;
        std::cout << "Retirement Age of: " << std::fixed << std::setprecision(0) << retirementAge << std::endl;
    }
    if (retirementSwitch == true) {
        std::cout << "Suggested Retirement Age: " << std::fixed << std::setprecision(0) << retirementAge << std::endl;
        std::cout << "Portfolio Value of: $" << std::fixed << std::setprecision(0) << portfolioValue << std::endl;
        std::cout << "Monthly Spending of: $" << std::fixed << std::setprecision(0) << yearlySpending/12 << "\n";
    }
    std::cout << "Numbers of Times Monte Carlo Run: " << std::fixed << monteCarloRunCount << std::endl;
}
void Parapet::runProgram() {
    Parapet run1;
    run1.scenarioBuilder();
    run1.getFilePath();
    run1.processPortfolio();
    run1.runMonteCarlo();
    run1.calculateInitialSuccess();
    run1.calculateSuccess();
    run1.switchFinder();
}