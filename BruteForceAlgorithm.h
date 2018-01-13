//
// Created by mrfarinq on 06.12.17.
//

#ifndef COURSEPROJECT_PEA2_BRUTEFORCEALGORITHM_H
#define COURSEPROJECT_PEA2_BRUTEFORCEALGORITHM_H


#include <vector>

class BruteForceAlgorithm {
    int length;
    int amountOfCities;
    int **matrixOfCities;
    int *allPossiblePermutations;
    int *optimalWay;

public:
    BruteForceAlgorithm(int **matrixOfCities, int amountOfCities);

    ~BruteForceAlgorithm();

    void DoCalculations();

    std::pair<std::vector<int>, int> GetResults();

private:
    void CalculateTheMostOptimalPermutation(int recursive_param);

    void DeleteContainers();
};


#endif //COURSEPROJECT_PEA2_BRUTEFORCEALGORITHM_H
