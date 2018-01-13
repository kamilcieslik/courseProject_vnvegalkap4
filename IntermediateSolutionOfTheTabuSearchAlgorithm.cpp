//
// Created by mrfarinq on 09.12.17.
//

#include "IntermediateSolutionOfTheTabuSearchAlgorithm.h"

IntermediateSolutionOfTheTabuSearchAlgorithm::IntermediateSolutionOfTheTabuSearchAlgorithm(double timeFromTheBeginningOfTheAlgorithm, long long int actualResult)
        : timeFromTheBeginningOfTheAlgorithm(timeFromTheBeginningOfTheAlgorithm), currentCost(actualResult) {}

double IntermediateSolutionOfTheTabuSearchAlgorithm::getTimeFromTheBeginningOfTheAlgorithm() const {
    return timeFromTheBeginningOfTheAlgorithm;
}

long long int IntermediateSolutionOfTheTabuSearchAlgorithm::getActualResult() const {
    return currentCost;
}