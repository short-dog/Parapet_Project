//
// Created by BSHORT2 on 11/8/2023.
//

#ifndef PARAPET_PROJECT_PROCESSPORTFOLIO_H
#define PARAPET_PROJECT_PROCESSPORTFOLIO_H
#include "ScenarioBuilder.h"

class ProcessPortfolio : ScenarioBuilder {
public:
    struct Investment {
        std::string name;
        double rateOfReturn;
        double stdDeviation;
        double portionOfPortfolio;
    };

    void processPortfolio();

    double portfolioReturn;
    double portfolioStdDev;
    double portfolioValue;

    double minMC;
    double maxMC;

};


#endif //PARAPET_PROJECT_PROCESSPORTFOLIO_H
