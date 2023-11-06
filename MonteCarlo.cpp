//
// Created by BSHORT2 on 11/1/2023.
//
//
// Created by bshort2 on 10/9/2023.
//

#include "MonteCarlo.h"
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Investment {
    std::string name;
    double rateOfReturn;
    double stdDeviation;
    double portionOfPortfolio;
};

void MonteCarlo::processPortfolio() {

    //##############################################
    // Retrieving User Portfolio and Formatting it #
    //##############################################

    //Retrieving the portfolio from a CSV file
    std::string filePath;
    std::cout << "Enter the file path to the CSV file: " << std::endl;
    std::cin >> filePath;
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
    for (const Investment& investment : investments) {
        portfolioReturn += investment.rateOfReturn * (investment.portionOfPortfolio / totalPortfolioValue);
        portfolioStdDev += pow(investment.stdDeviation, 2) * (investment.portionOfPortfolio / totalPortfolioValue);
    }

    portfolioStdDev = sqrt(portfolioStdDev);

    // Print individual investments and overall portfolio stats
    std::cout << std::endl << "Individual Investments:" << std::endl;
    for (const Investment& investment : investments) {
        std::cout << "Name: " << investment.name << ", Rate of Return: " << investment.rateOfReturn << "%"
                  << ", Standard Deviation: " << investment.stdDeviation << "%"
                  << ", Portion of Portfolio: " << investment.portionOfPortfolio << std::endl;
    }

    std::cout << "\nOverall Portfolio Metrics:" << std::endl;
    std::cout << "Overall Performance: " << std::setprecision(4) << portfolioReturn << "%" << std::endl;
    std::cout << "Overall Standard Deviation: " << std::setprecision(4) << portfolioStdDev << "%" << std::endl;

    //##############################################
    // Running Monte Carlo Simulation on Portfolio #
    //##############################################

    //establishing portfolio parameters
    const auto minMC = portfolioReturn - portfolioStdDev;
    const auto maxMC = portfolioReturn + portfolioStdDev;

    //setting plan parameters
    int simLength = 1000;
    int years;
    double investmentAmount;

    std::cout << std::endl << "Enter Current Investment Amount: " << std::endl;
    std::cin >> investmentAmount;

    std::cout << std::endl << "Enter Length of Plan: " << std::endl;
    std::cin >> years;
    int runNum = years * simLength;

    //running simulation
    std::vector<double> monteCarloData{};
    for (int i = 0; i < runNum; ++i) {
        std::uniform_real_distribution<double> ParaMC(minMC, maxMC);
        std::random_device runSeed;
        std::mt19937 rng(runSeed());
        auto x = ParaMC(rng);
        monteCarloData.push_back(x);
    }
    //calculating portfolio effect of each return
    std::vector<double> planResults{};
    for (int sL = 0; sL < simLength; ++sL){
        for (int y = 0; y < years; ++y){
            int mCDN = y + (y * sL);
            auto mCV = monteCarloData.at(mCDN);
            auto yy = 1 + (0.01 * mCV);
            planResults.push_back(yy);
        }
    }
    //calculating final portfolio results
    double endValue = investmentAmount;
    std::vector<double> finalReturn{};
    for (int i = 0; i < simLength; ++i){
        endValue = investmentAmount;
        for (int ii = 0 + i; ii < (years * simLength); ii += simLength){
            endValue *= planResults.at(ii);
        }
        finalReturn.push_back(endValue);
    }
    //calculating percentage of plans that succeed
    double successCriteria;
    double returnNeeded;

    std::cout << std::endl << "Enter return needed: " << std::endl;
    std::cin >> returnNeeded;

    successCriteria = pow((1 + returnNeeded), static_cast<double>(years));
    double successConfirm = investmentAmount * successCriteria;

    int successCount = 0;
    for (double n : finalReturn){
        if(n > successConfirm) {
            ++successCount;
        }
    }
    std::cout << std::endl << "Success Probability: " << successCount * 0.1 << "%";

}