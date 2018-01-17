//
// Created by mrfarinq on 16.06.17.
//

#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <queue>
#include <algorithm>
#include <map>
#include "IntermediateSolution.h"

class TravellingSalesmanProblem {

private:
    int amountOfCities;
    int **matrixOfCities;
    std::vector<int> optimalWay;
    long long int optimalLength;
    std::string fileName;
    std::string graphType;
    bool randomGeneratorData;
    std::string whichTypeOfAlgorithm;
    std::vector<IntermediateSolution> intermediateSolutions;

public:
    TravellingSalesmanProblem();

    ~TravellingSalesmanProblem();

    void DeleteTravellingSalesman();

    void LoadArrayOfMatrixOfCities(int **_cities, int _amountOfCities,
                                   std::string _fileName, std::string _graphType);

    void ReadCitiesFromNormalFile(std::string path);

    void GenerateRandomCities(int amountOfCities = 0, int maxDistanceBetweenCity = 99);

    void PrintCitiesForTheTravellingSalesman(bool printInputMatrix);

    void PerformBruteForceAlgorithm();

    void PerformBranchAndBoundAlgorithm();

    void PerformGeneticAlgorithm();

    void
    PerformTabuSearchAlgorithm(std::string neighborhoodType = "auto", bool showIntermediateSolutionsInRuntime = false, int tabuSize = 13);

    void PrintSolution();

    long long int GetTourLength();

    const std::vector<IntermediateSolution> &getIntermediateSolutions() const;
};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
