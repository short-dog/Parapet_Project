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

void Parapet::scenarioBuilder() {
    //gathering scenario data
    std::cout << "Enter Current Monthly Spending or Type '0' to find: " << std::endl;
    std::cin >> monthlySpending;

    if (monthlySpending == 0) {
        std::cout << "Enter Portfolio Value" << std::endl;
        std::cin >> initialPortfolioValue;
    } else {
        std::cout << "Enter Portfolio Value or Type '0' to find: " << std::endl;
        std::cin >> initialPortfolioValue;
    }
    std::cout << "How Much Are You Currently Contributing to This Account, Per Year?" << std::endl;
    std::cin >> yearlyAddition;

    std::cout << "Enter Current Age: " << std::endl;
    std::cin >> planStartAge;

    std::cout << "Enter Retirement Age or Type '0' to find: " << std::endl;
    std::cin >> retirementAge;
    planLength = retirementAge - planStartAge;

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
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
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
    std::cout << "Overall Standard Deviation: " << std::setprecision(4) << portfolioStdDev << "%" << std::endl;

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
    std::vector<double> monteCarloData{};
    for (int i = 0; i < runNum; ++i) {
        std::uniform_real_distribution<> ParaMC(minMC, maxMC);
        std::random_device runSeed;
        std::mt19937 rng(runSeed());
        auto x = ParaMC(rng);
        monteCarloData.push_back(x);
    }
    //calculating portfolio effect of each return
    std::vector<double> planResults{};
    for (int sL = 0; sL < simLength; ++sL){
        for (int y = 0; y < years; ++y) {
            const int mCDN = y + y * sL;
            const auto mCV = monteCarloData.at(mCDN);
            auto yy = 1 + 0.01 * mCV;
            planResults.push_back(yy);
        }
    }
    yearlySpending = 12 * monthlySpending;
    yearlyGain = portfolioReturn * 0.01 * portfolioValue;
    returnNeeded = yearlySpending - yearlyGain;

    //calculating final portfolio results
    portfolioValue = initialPortfolioValue;
    for (int i = 0; i < simLength; ++i){
        double endValue = portfolioValue;
        for (int ii = 0 + i; ii < years * simLength; ii += simLength) {
            endValue *= planResults.at(ii);
            if (ii < planLength * 1000 - 1000) {
                endValue += yearlyAddition;
            }
            if (ii > planLength * 1000 - 1000) {
                double adjustedSpending = yearlySpending * pow((1 + inflationRate),ii/1000);
                endValue -= adjustedSpending;
            }
        }
        finalReturn.push_back(endValue);
    }
}
void Parapet::calculateSuccess() {
    //determine success probability of plan
    int successCount = 0;
    for (double n : finalReturn){
        if (n > 0) {
            ++successCount;
        }
    }
    const double successProbability = 100 * (static_cast<double>(successCount) / static_cast<double>(simLength));
    std::cout << std::endl << "Success Probability: " << successProbability << "%";
}