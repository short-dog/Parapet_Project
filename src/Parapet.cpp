//
// Created by BSHORT2 on 11/16/2023.
//
#include "Parapet.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <ranges>
#include <algorithm>

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
    std::cout << "You Are Eligible for Social Security? Enter yes or no." << std::endl;
    std::cin >> socialEligibleStr;
    if (socialEligibleStr == "yes") {
        socialEligible = true;
    }else socialEligible = false;

    if (socialEligible == true) {
        std::cout << "Do You Plan to Defer Your Benefits? If not, type 1. If so, what age (between 62 & 70): " << std::endl;
        std::cin >> socialAge;
        if (socialAge == 1) {
            socialAge = retirementAge;
        }
        while (socialAge < retirementAge) {
            std::cout << "Input Error: Please enter an age above retirement age." << std::endl;
            std::cout << "Enter Deferment Age: " << std::endl;
            std::cin >> socialAge;
        }
    }
    planLength = retirementAge - planStartAge;
    portfolioValue = initialPortfolioValue;
    yearlySpending = monthlySpending * 12;
}
void Parapet::getFilePath() {
    std::cout << "Enter the file path to your portfolio: " << std::endl;
    std::cin >> filePath;
}
void Parapet::presetScenario() {
    monthlySpending = 5500;
    initialPortfolioValue = 10000;
    yearlyAddition = 12000;
    planStartAge = 23;
    retirementAge = 60;
    socialAge = 70;
    planLength = retirementAge - planStartAge;
    portfolioValue = initialPortfolioValue;
    yearlySpending = monthlySpending * 12;
    filePath = "/Users/short/CLionProjects/Parapet_Project/src/investments.csv";
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
        }else {
            std::cerr << "Error: Invalid CSV format." << std::endl;
        }
    }
    // Calculate total return and standard deviation of the portfolio
    for (const Investment &investment: investments) {
        portfolioReturn += investment.rateOfReturn * (investment.portionOfPortfolio / totalPortfolioValue);
        portfolioStdDev += pow(investment.stdDeviation, 2) * (investment.portionOfPortfolio / totalPortfolioValue);
    }
    //establishing portfolio parameters for Monte Carlo Simulation
    portfolioStdDev = sqrt(portfolioStdDev);
    minMC = portfolioReturn - portfolioStdDev;
    maxMC = portfolioReturn + portfolioStdDev;
}
void Parapet::printPortfolioStats(){ // Print individual investments and overall portfolio stats
        std::cout << std::endl << "Individual Investments:" << std::endl;
        for (const Investment &investment: investments) {
            std::cout << "Name: " << investment.name << ", Rate of Return: " << investment.rateOfReturn << "%"
                      << ", Standard Deviation: " << investment.stdDeviation << "%"
                      << ", Portion of Portfolio: " << investment.portionOfPortfolio << std::endl;
        }

        std::cout << "\nOverall Portfolio Metrics:" << std::endl;
        std::cout << "Overall Performance: " << std::setprecision(4) << portfolioReturn << "%" << std::endl;
        std::cout << "Overall Standard Deviation: " << std::setprecision(4) << portfolioStdDev << std::endl;
}

void Parapet::runMonteCarlo() {
    //setting plan parameters
    investmentAmount = portfolioValue;
    runNum = (lifeExpectancy - planStartAge) * simLength;
    years = lifeExpectancy - planStartAge;
    //running simulation
    monteCarloRunCount++;
    for (int i = 0; i < runNum; ++i) {
        std::uniform_real_distribution<> paraMC(minMC, maxMC);
        std::random_device runSeed;
        std::mt19937 rng(runSeed());
        auto x = paraMC(rng);
        monteCarloData.push_back(x);
    }
    //calculating portfolio effect of each return
    for (int i = 0; i < simLength; ++i){
        for (int ii = 0; ii < years; ++ii) {
            const auto mCResultData = ii + ii * i;
            const auto mCValue = monteCarloData.at(mCResultData);
            auto yy = 1 + 0.01 * mCValue;
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
                //adjust for inflation
                double adjustedSpending = yearlySpending * pow(1 + inflationRate,ii/1000);
                endValue -= adjustedSpending;
                //add social security income - also adjusted for inflation
                if (ii > 62 * 1000 - 1000) {
                    if (socialEligible == true) {
                        if (socialAge < 62) {
                            endValue += 1125 * pow(1 + inflationRate, ii/1000);
                        }
                        if (61 < socialAge < 70) {
                            auto socialAddOn = socialSecurityBenefit.find(socialAge) -> second;
                            endValue += socialAddOn * pow(1 + inflationRate, ii/1000);
                        }
                        if (socialAge >= 70) {
                            endValue += 1980 * pow(1 + inflationRate, ii/1000);
                        }
                    }
                }
            }
        }
        finalReturn.push_back(endValue);
        //totalAvgValue += endValue;
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
}
void Parapet::printInitialSuccess() {
        if (monthlySpending != 1 && initialPortfolioValue != 1 && retirementAge != 1) {
            std::cout << std::endl << "Success Probability of " << successProbability << "% with: \nSpending Level of $"
            << std::fixed << std::setprecision(0) << monthlySpending << "\nPortfolio Value of: $" << initialPortfolioValue
            << "\nand a Retirement Age of: " << retirementAge << std::endl;
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
            yearlySpending = yearlyAddition * retirementAge;
            findSpending();
            spendingSwitch = true;
    }
    switch(static_cast<int>(retirementAge)) {
        case 1:
            findRetirementAge();
            retirementSwitch = true;

    }
    if(initialPortfolioValue && monthlySpending && retirementAge > 1) {
        printResults();
    }
}
void Parapet::findAverageEndValue() {
    std::ranges::sort(finalReturn);
    double totalMedValue = 0;
    double totalAvgValue = 0;
    const int i = finalReturn.size();
    for (int ii = i*0.45; ii < i*0.55; ii++) {
        totalMedValue += finalReturn[ii];
    }
    for (int ii = 0; ii < i; ii++) {
        totalAvgValue += finalReturn[ii];
    }
    totalMedianEndValue += totalMedValue/(i*0.1);
    totalAvgEndValue += totalAvgValue/(i);
}
void Parapet::printResults() const {
    if (printResultsBool == true){
        if (portfolioSwitch == true || spendingSwitch == true || retirementSwitch == true) {
            std::cout << "Success Probability Set: " << std::fixed << std::setprecision(0) << successLevel << "%\n";
            std::cout << "Success Probability Achieved: " << std::fixed << std::setprecision(0) << successProbability
            << "%\n" << std::endl;
        }
        if (portfolioSwitch == true) {
            std::cout << "Suggested Portfolio Amount: $" << std::fixed << std::setprecision(0) << portfolioValue << "\n";
            std::cout << "Monthly Spending of: $" << std::fixed << std::setprecision(0) << yearlySpending/12 << "\n";
            std::cout << "Retirement Age of: " << std::fixed << std::setprecision(0) << retirementAge << std::endl;
        }
        if (spendingSwitch == true) {
            std::cout << "Suggested Monthly Spending: $" << std::fixed << std::setprecision(0) << yearlySpending/12
            << "\n";
            std::cout << "Portfolio Value of: $" << std::fixed << std::setprecision(0) << portfolioValue << "\n";
            std::cout << "Retirement Age of: " << std::fixed << std::setprecision(0) << retirementAge << std::endl;
        }
        if (retirementSwitch == true) {
            std::cout << "Suggested Retirement Age: " << std::fixed << std::setprecision(0) << retirementAge << "\n";
            std::cout << "Portfolio Value of: $" << std::fixed << std::setprecision(0) << portfolioValue << "\n";
            std::cout << "Monthly Spending of: $" << std::fixed << std::setprecision(0) << yearlySpending/12 << std::endl;
        }
        std::cout << "\nNumbers of Times Monte Carlo Run: " << std::fixed << monteCarloRunCount << ", Iterations per Run: "
        << simLength << "\n";
        std::cout << "Average Ending Portfolio Value: $" << std::fixed << totalAvgEndValue / monteCarloRunCount << std::endl;
        std::cout << "Median Ending Portfolio Value: $" << std::fixed << totalMedianEndValue / monteCarloRunCount << std::endl;
    }
}
void Parapet::logData() {
    std::ofstream logFile("/Users/short/CLionProjects/Parapet_Project/src/log.csv", std::ios::app);
    logFile << successProbability << "," << totalAvgEndValue << "," << successLevel << "," << monthlySpending << ","
    << initialPortfolioValue << "," << yearlyAddition << "," << retirementAge << "," << planStartAge << "," << socialAge << "," << simLength
    << "," << lifeExpectancy << "," << inflationRate << "," << monteCarloRunCount << "," << filePath << "\n";
    logFile.close();
}
void Parapet::clearRunData() {
    monteCarloData.clear();
    planResults.clear();
    finalReturn.clear();
    investments.clear();
    portfolioReturn = 0;
    portfolioStdDev = 0;
}
void Parapet::runParapet() {
    Parapet run1;
    run1.printResultsBool = true;
    //run1.scenarioBuilder();
    //run1.getFilePath();
    run1.presetScenario();
    run1.processPortfolio();
    run1.printPortfolioStats();
    run1.runMonteCarlo();
    run1.calculateInitialSuccess();
    run1.printInitialSuccess();
    run1.calculateSuccess();
    run1.findAverageEndValue();
    run1.switchFinder();
    run1.logData();
    run1.clearRunData();
}
void Parapet::runMultiParapet(int runNumber) {
    Parapet run1;
    double sumSuccess = 0;
    std::cout << "Running..." << std::endl;
    for (int i = 0; i < runNumber; i++) {
        run1.presetScenario();
        run1.processPortfolio();
        run1.runMonteCarlo();
        run1.calculateInitialSuccess();
        run1.calculateSuccess();
        run1.switchFinder();
        sumSuccess += run1.successProbability;
        run1.logData();
        run1.clearRunData();
    }
    double avgSuccess = sumSuccess / runNumber;
    std::cout << "Complete." << "\n";
    std::cout  << "Average Success Rate: " << std::setprecision(2) << avgSuccess << "%" << std::endl;
}