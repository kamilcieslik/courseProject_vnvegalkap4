//
// Created by mrfarinq on 09.12.17.
//

#ifndef COURSEPROJECT_PEA2_INTERMEDIATESOLUTIONOFTHETABUSEARCHALGORITHM_H
#define COURSEPROJECT_PEA2_INTERMEDIATESOLUTIONOFTHETABUSEARCHALGORITHM_H


class IntermediateSolution {
    double timeFromTheBeginningOfTheAlgorithm;
    long long int currentCost;

public:
    IntermediateSolution(double timeFromTheBeginningOfTheAlgorithm, long long int actualResult);

    double getTimeFromTheBeginningOfTheAlgorithm() const;

    long long int getActualResult() const;
};


#endif //COURSEPROJECT_PEA2_INTERMEDIATESOLUTIONOFTHETABUSEARCHALGORITHM_H
