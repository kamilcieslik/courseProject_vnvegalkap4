//
// Created by mrfarinq on 06.12.17.
//

#include <climits>
#include "BruteForceAlgorithm.h"

BruteForceAlgorithm::BruteForceAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(
        matrixOfCities), amountOfCities(
        amountOfCities), allPossiblePermutations(nullptr), optimalWay(nullptr) {}

BruteForceAlgorithm::~BruteForceAlgorithm() {
    DeleteContainers();
}

void BruteForceAlgorithm::DoCalculations() {
    delete[] optimalWay;

    optimalWay = new int[amountOfCities + 1];

    allPossiblePermutations = new int[amountOfCities];
    for (int i = 0; i < amountOfCities; i++) {
        allPossiblePermutations[i] = i;
    }

    length = INT_MAX;
    CalculateTheMostOptimalPermutation(0);

    delete[] allPossiblePermutations;
    allPossiblePermutations = nullptr;
}

void BruteForceAlgorithm::CalculateTheMostOptimalPermutation(int recursive_param) {
    if (recursive_param == amountOfCities - 1) {
        int lengthInThisPermutation = 0;
        for (auto i = 0; i < amountOfCities - 1; i++) {
            lengthInThisPermutation += matrixOfCities[allPossiblePermutations[i]][allPossiblePermutations[i +
                                                                                                          1]];
        }
        lengthInThisPermutation += matrixOfCities[allPossiblePermutations[amountOfCities -
                                                                          1]][allPossiblePermutations[0]];
        if (lengthInThisPermutation < length) {
            length = lengthInThisPermutation;
            for (auto i = 0; i < amountOfCities; i++) {
                optimalWay[i] = allPossiblePermutations[i];
            }
            optimalWay[amountOfCities] = allPossiblePermutations[0];
        }
    } else {
        for (auto i = recursive_param; i < amountOfCities; i++) {
            std::swap(allPossiblePermutations[recursive_param], allPossiblePermutations[i]);
            CalculateTheMostOptimalPermutation(recursive_param + 1);
            std::swap(allPossiblePermutations[recursive_param], allPossiblePermutations[i]);
        }
    }
}

std::pair<std::vector<int>, int> BruteForceAlgorithm::GetResults() {
    std::vector<int> optimalWayVector;
    for (auto i = 0; i < amountOfCities; i++)
        optimalWayVector.push_back(optimalWay[i]);
    return std::make_pair(optimalWayVector, length);
}

void BruteForceAlgorithm::DeleteContainers() {

    if (optimalWay != nullptr) {
        delete[] optimalWay;
        optimalWay = nullptr;
    }

    if (allPossiblePermutations != nullptr) {
        delete[] allPossiblePermutations;
        allPossiblePermutations = nullptr;
    }
}
