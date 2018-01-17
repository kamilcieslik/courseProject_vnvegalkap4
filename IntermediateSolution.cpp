//
// Created by mrfarinq on 09.12.17.
//

#include "IntermediateSolution.h"

IntermediateSolution::IntermediateSolution(double timeFromTheBeginningOfTheAlgorithm, long long int actualResult)
        : timeFromTheBeginningOfTheAlgorithm(timeFromTheBeginningOfTheAlgorithm), currentCost(actualResult) {}

double IntermediateSolution::getTimeFromTheBeginningOfTheAlgorithm() const {
    return timeFromTheBeginningOfTheAlgorithm;
}

long long int IntermediateSolution::getActualResult() const {
    return currentCost;
}