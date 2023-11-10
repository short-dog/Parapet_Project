//
// Created by BSHORT2 on 11/8/2023.
//

#include "ProcessPortfolio.h"

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

void ProcessPortfolio::processPortfolio() {

    ScenarioBuilder::scenarioBuilder();
    // Retrieving User Portfolio and Formatting it //
    // Retrieving the portfolio from a CSV file
    std::ifstream file(ScenarioBuilder::filePath);
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
    portfolioValue = ScenarioBuilder::portfolioValue;

}
