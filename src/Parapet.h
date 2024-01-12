//
// Created by BSHORT2 on 11/16/2023.
//
#ifndef PARAPET_H
#define PARAPET_H
#include <vector>
#include <iostream>
#include <map>

class Parapet {
public:
    static void runParapet();

    static void runMultiParapet(int runNumber);

    void calcAvgSuccess();

private:
    void scenarioBuilder();
    void processPortfolio();

    void printPortfolioStats();

    void runMonteCarlo();
    void calculateInitialSuccess();

    void printInitialSuccess();

    void calculateSuccess();
    void clearData();
    void findSpending();
    void findPortfolioNeeded();
    void getFilePath();

    void presetScenario();

    void findRetirementAge();
    void switchFinder();

    void findAverageEndValue();

    void printResults() const;

    void logData();
    void clearRunData();
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
    std::string socialEligibleStr;
    bool socialEligible;
    int socialAge;
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
    double totalMedianEndValue;
    double totalAvgEndValue;

    double successConfirm;
    double successProbability;
    int monteCarloRunCount = 0;

    int simLength = 1000;
    int lifeExpectancy = 95; //don't allow findAge to surpass
    double inflationRate = 0.03;
    double successLevel = 90;

    std::vector<double> monteCarloData;
    std::vector<double> planResults;
    std::vector<double> finalReturn;
    std::vector<Investment> investments;


    bool portfolioSwitch = false;
    bool spendingSwitch = false;
    bool retirementSwitch = false;
    bool printResultsBool = false;

    std::map<double, double> socialSecurityBenefit = {{62, 1125},{63, 1200},
        {64, 1298},{65, 1401},{66, 1500},{67, 1620},{68, 1740},{69, 1860}
    };
};

#endif //PARAPET_H
