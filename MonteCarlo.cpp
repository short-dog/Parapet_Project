//
// Created by BSHORT2 on 11/1/2023.
//
//
// Created by bshort2 on 10/9/2023.
//

#include "MonteCarlo.h"
#include <random>
#include <iostream>
#include <vector>
#include <cmath>

// Running Monte Carlo Simulation on Portfolio //
void MonteCarlo::runMonteCarlo() {

            ProcessPortfolio::processPortfolio();
            //setting plan parameters
            int simLength = 1000;
            int years;
            double investmentAmount = ProcessPortfolio::portfolioValue;

            std::cout << std::endl << "Enter Length of Plan: " << std::endl;
            std::cin >> years;
            int runNum = years * simLength;

            //running simulation
            std::vector<double> monteCarloData{};
            for (int i = 0; i < runNum; ++i) {
                std::uniform_real_distribution<double> ParaMC(ProcessPortfolio::minMC, ProcessPortfolio::maxMC);
                std::random_device runSeed;
                std::mt19937 rng(runSeed());
                auto x = ParaMC(rng);
                monteCarloData.push_back(x);
            }
            //calculating portfolio effect of each return
            std::vector<double> planResults{};
            for (int sL = 0; sL < simLength; ++sL){
                for (int y = 0; y < years; ++y) {
                    int mCDN = y + (y * sL);
                    auto mCV = monteCarloData.at(mCDN);
                    auto yy = 1 + (0.01 * mCV);
                    planResults.push_back(yy);
                }
            }
            //calculating final portfolio results
            double endValue = ProcessPortfolio::portfolioValue;
            std::vector<double> finalReturn{};
            for (int i = 0; i < simLength; ++i){
                endValue = ProcessPortfolio::portfolioValue;
                for (int ii = 0 + i; ii < (years * simLength); ii += simLength) {
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
                if (n > successConfirm) {
                    ++successCount;
                }
            }
            std::cout << std::endl << "Success Probability: " << successCount * 0.1 << "%";
}