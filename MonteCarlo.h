//
// Created by BSHORT2 on 11/1/2023.
//

#ifndef PARAPET_PROJECT_MONTECARLO_H
#define PARAPET_PROJECT_MONTECARLO_H
#include <string>


class MonteCarlo {
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
};
#endif //PARAPET_PROJECT_MONTECARLO_H
