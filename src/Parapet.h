//
// Created by BSHORT2 on 11/16/2023.
//

#ifndef PARAPET_H
#define PARAPET_H
#include <vector>
#include <iostream>

class Parapet {
public:
    static void runParapet();
private:
    void scenarioBuilder();
    void processPortfolio();
    void runMonteCarlo();
    void calculateInitialSuccess();
    void calculateSuccess();
    void clearData();
    void findSpending();
    void findPortfolioNeeded();
    void getFilePath();
    void findRetirementAge();
    void switchFinder();
    void printResults() const;
    struct Investment {
        std::string name;
        double rateOfReturn;
        double stdDeviation;
        double portionOfPortfolio;
    };

private:
    std::string filePath;
    std::ifstream file();

    double monthlySpending;
    double initialPortfolioValue;
    double yearlyAddition;
    double planStartAge;
    double retirementAge;
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
    int lifeExpectancy = 95; //don't allow findAge to surpass
    double inflationRate = 0.03;
    double successLevel = 90;

    std::vector<double> monteCarloData{};
    std::vector<double> planResults{};
    std::vector<double> finalReturn{};

    bool portfolioSwitch = false;
    bool spendingSwitch = false;
    bool retirementSwitch = false;
};

#endif //PARAPET_H
