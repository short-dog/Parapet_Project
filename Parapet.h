//
// Created by BSHORT2 on 11/16/2023.
//

#ifndef PARAPET_H
#define PARAPET_H
#include <vector>
#include <iostream>

class Parapet {
public:
    void scenarioBuilder();
    void processPortfolio();
    void runMonteCarlo();
    void calculateInitialSuccess();
    void calculateSuccess();
    void clearData();
    void findSpending();
    void findPortfolioNeeded();

    void findRetirementAge();

    void switchFinder();
    void printResults() const;

    std::string filePath;
    double monthlySpending;
    double initialPortfolioValue;
    double yearlyAddition;
    int planStartAge;
    int retirementAge;


private:
    int planLength;

    double portfolioReturn;
    double portfolioStdDev;
    double minMC;
    double maxMC;

    double investmentAmount;
    int runNum;
    int years;
    double yearlySpending;
    double yearlyGain;
    double returnNeeded;
    double portfolioValue;
    double successConfirm;
    double successProbability;
    int monteCarloRunCount = 0;

    int simLength = 1000;
    int lifeExpectancy = 95;
    double inflationRate = 0.03;
    double successLevel = 68;

    std::vector<double> monteCarloData{};
    std::vector<double> planResults{};
    std::vector<double> finalReturn{};

    struct Investment {
        std::string name;
        double rateOfReturn;
        double stdDeviation;
        double portionOfPortfolio;
    };
};

#endif //PARAPET_H
