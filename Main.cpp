//
// Created by BSHORT2 on 11/1/2023.
//
#include "Parapet.h"

int main(){
    Parapet run1;
    run1.scenarioBuilder();
    run1.processPortfolio();
    run1.runMonteCarlo();
    run1.calculateSuccess();

    return 1;
}