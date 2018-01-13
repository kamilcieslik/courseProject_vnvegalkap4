//
// Created by mrfarinq on 06.12.17.
//

#include <algorithm>
#include <random>
#include <climits>
#include <iomanip>
#include "TabuSearchAlgorithm.h"
#include "TimeMeasurement.h"

TabuSearchAlgorithm::TabuSearchAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(matrixOfCities),
                                                                                     amountOfCities(amountOfCities) {
}

void TabuSearchAlgorithm::DoCalculations(int tabuListSize,
                                         int maximumIterationsWithoutBetterSolution,
                                         int maximumRestarts, int tenure,
                                         std::string neighborhoodType,
                                         bool showIntermediateSolutionsInRuntime) {
    TimeMeasurement timeMeasurement;
    timeMeasurement.TimeStart();

    srand(static_cast<unsigned int>(time(nullptr)));

    int amountOfNeighbours = amountOfCities * 2;
    int maximumAmountOfRestartsWithoutBetterSolution = static_cast<int>(trunc(maximumRestarts / 3));
    int actualAmountOfRestartsWithoutBetterSolution = 0;
    int actualAmountOfRestarts = 0;
    int actualAmountOfIterationsWithoutBetterSolution = 0;
    bool foundBetterSolutionAfterRestart = false;

    long long int currentCost;
    std::vector<int> currentSolution(GreedyAlgorithm());
    currentCost = countCost(currentSolution);

    optimalWay = currentSolution;
    length = currentCost;
    intermediateSolutions.push_back(
            IntermediateSolutionOfTheTabuSearchAlgorithm(timeMeasurement.GetTimeInSeconds(), length));

    std::pair<int, int> replacedCities;
    std::list<std::vector<int>> goodSolutions;

    while (actualAmountOfRestarts < maximumRestarts && timeMeasurement.GetTimeInSeconds() < 100) {
        std::list<std::pair<int, int>> listOfReplacedCities = findRandomCitiesToNeighbourhood(currentSolution,
                                                                                              amountOfNeighbours);

        currentSolution = GenerateNewCurrentSolutionFromNeighbourhood(currentSolution, listOfReplacedCities,
                                                                      replacedCities, neighborhoodType);
        currentCost = countCost(currentSolution);

        if (currentCost < length) {
            optimalWay = currentSolution;
            length = currentCost;

            goodSolutions.push_back(currentSolution);
            intermediateSolutions.push_back(
                    IntermediateSolutionOfTheTabuSearchAlgorithm(timeMeasurement.GetTimeInSeconds(), length));

            actualAmountOfIterationsWithoutBetterSolution = 0;
            foundBetterSolutionAfterRestart = true;

            if (showIntermediateSolutionsInRuntime) {
                std::cout << std::fixed << std::setprecision(6)
                          << intermediateSolutions.back().getTimeFromTheBeginningOfTheAlgorithm() << "\t|\t"
                          << intermediateSolutions.back().getActualResult() << std::endl;
            }
        } else
            actualAmountOfIterationsWithoutBetterSolution++;

        if (tabuList.size() > tabuListSize) {
            tabuList.pop_front();
        }

        std::pair<std::pair<int, int>, int> tabuMovement{replacedCities, tenure};
        tabuList.push_back(tabuMovement);
        decrementTenures(tabuList);

        if (actualAmountOfRestartsWithoutBetterSolution == maximumAmountOfRestartsWithoutBetterSolution) {
            Intensification(currentSolution, currentCost, goodSolutions);
            actualAmountOfRestartsWithoutBetterSolution = 0;
            foundBetterSolutionAfterRestart = true;
        }

        if (CriticalEvent(actualAmountOfIterationsWithoutBetterSolution, maximumIterationsWithoutBetterSolution)) {
            if (!foundBetterSolutionAfterRestart)
                actualAmountOfRestartsWithoutBetterSolution++;
            else
                actualAmountOfRestartsWithoutBetterSolution = 0;

            foundBetterSolutionAfterRestart = false;
            Diversification(currentSolution, currentCost);
            actualAmountOfIterationsWithoutBetterSolution = 0;
            actualAmountOfRestarts++;
        }
    }
}

std::vector<int> TabuSearchAlgorithm::GreedyAlgorithm() {
    std::vector<int> solution;
    for (auto i = 0; i < amountOfCities; i++) {
        solution.push_back(i);
    }

    bool *visitedCities = new bool[amountOfCities];
    for (auto i = 0; i < amountOfCities; i++) {
        visitedCities[i] = false;
    }

    length = 0;
    int currentMinLength;

    int nextCity = 0;
    int city = nextCity;
    visitedCities[city] = true;

    solution[0] = nextCity;

    for (auto j = 0; j < amountOfCities - 1; j++) {
        city = nextCity;
        currentMinLength = INT_MAX;
        for (auto i = 0; i < amountOfCities; i++) {
            if (matrixOfCities[city][i] < currentMinLength && !visitedCities[i]) {
                currentMinLength = matrixOfCities[city][i];
                nextCity = i;
            }
        }
        visitedCities[nextCity] = true;
        solution[j] = nextCity;
        length += matrixOfCities[city][nextCity];
    }
    solution[amountOfCities - 1] = 0;
    length += matrixOfCities[solution[amountOfCities - 2]][0];

    delete[] visitedCities;

    return solution;
}

std::vector<int> TabuSearchAlgorithm::generateRandomSolution() {
    std::vector<int> solution;
    for (auto i = 0; i < amountOfCities; i++) {
        solution.push_back(i);
    }
    std::shuffle(solution.begin(), solution.end(), std::mt19937(std::random_device()()));
    return solution;
}

int TabuSearchAlgorithm::countCost(std::vector<int> currentSolution) {
    long long int cost = 0;
    for (auto i = 0; i < currentSolution.size() - 1; i++) {
        cost += matrixOfCities[currentSolution[i]][currentSolution[i + 1]];
    }
    cost += matrixOfCities[currentSolution[currentSolution.size() - 1]][currentSolution[0]];

    return cost;
}

bool TabuSearchAlgorithm::citiesExistOnTabuList(std::pair<int, int> &neighborhoodCities,
                                                long long int neighborhoodCost) {
    auto iterator = tabuList.begin();
    while (iterator != tabuList.end()) {
        if (neighborhoodCities.first == iterator->first.second ||
            neighborhoodCities.second == iterator->first.first) {
            if (neighborhoodCost > length)
                return true;
        }
        iterator++;
    }
    return false;
}

std::pair<std::vector<int>, int> TabuSearchAlgorithm::GetResults() {
    return std::make_pair(optimalWay, length);
}

bool
TabuSearchAlgorithm::CriticalEvent(int &iterationsWithoutBetterSolution, int &maximumIterationsWithoutBetterSolution) {
    return iterationsWithoutBetterSolution > maximumIterationsWithoutBetterSolution;
}

void TabuSearchAlgorithm::Diversification(std::vector<int> &currentSolution, long long int &currentCost) {
    currentSolution = generateRandomSolution();
    currentCost = countCost(currentSolution);
    long long int newCost;
    std::vector<int> newRandomSolution;
    for (auto i = 0; i < 10; i++) {
        newRandomSolution = generateRandomSolution();
        newCost = countCost(newRandomSolution);
        if (newCost < currentCost) {
            currentSolution = newRandomSolution;
            currentCost = newCost;
        }
    }
}

void TabuSearchAlgorithm::Intensification(std::vector<int> &currentSolution, long long int &currentCost,
                                          std::list<std::vector<int>> &goodSolutions) {
    int positionOfRandomGoodSolution;
    if (goodSolutions.size() < 5) {
        currentSolution = optimalWay;
        currentCost = length;
    } else {
        positionOfRandomGoodSolution = (rand() % 5) + 1;
        currentSolution = *std::prev(goodSolutions.end(), positionOfRandomGoodSolution);
        currentCost = countCost(currentSolution);
    }
}

std::list<std::pair<int, int>>
TabuSearchAlgorithm::findRandomCitiesToNeighbourhood(std::vector<int> &currentSolution, int neighbourhoodSize) {
    std::list<std::pair<int, int>> randomCitiesToNeighbourhood;

    for (auto i = 0; i < neighbourhoodSize; i++) {
        auto city_1 = rand() % currentSolution.size();

        unsigned long city_2;
        do {
            city_2 = rand() % currentSolution.size();
        } while (city_1 == city_2);

        std::pair<int, int> neighborhoodCities;

        if (city_1 > city_2) {
            neighborhoodCities.first = currentSolution.at(city_2);
            neighborhoodCities.second = currentSolution.at(city_1);
        } else {
            neighborhoodCities.first = currentSolution.at(city_1);
            neighborhoodCities.second = currentSolution.at(city_2);
        }

        randomCitiesToNeighbourhood.push_back({currentSolution.at(city_1), currentSolution.at(city_2)});
    }

    return randomCitiesToNeighbourhood;
}

std::vector<int>
TabuSearchAlgorithm::GenerateNewCurrentSolutionFromNeighbourhood(std::vector<int> currentSolution,
                                                                 std::list<std::pair<int, int>> listOfReplacedCities,
                                                                 std::pair<int, int> replacedCitiesOfBestNeighbour,
                                                                 std::string neighborhoodType) {
    std::vector<int> bestNeighbour;

    int bestNeighbourCost = INT_MAX;
    int currentNeighbourCost;

    for (auto cities : listOfReplacedCities) {
        auto tempSolution = currentSolution;

        long city_1Position = distance(
                tempSolution.begin(),
                find(tempSolution.begin(), tempSolution.end(), cities.first)
        );

        long city_2Position = distance(
                tempSolution.begin(),
                find(tempSolution.begin(), tempSolution.end(), cities.second)
        );

        if (neighborhoodType == "swap")
            neighbourBySwap(tempSolution, city_1Position, city_2Position);
        else if (neighborhoodType == "insert")
            neighbourByInsert(tempSolution, city_1Position, city_2Position);
        else if (neighborhoodType == "invert")
            neighbourByInvert(tempSolution, city_1Position, city_2Position);

        currentNeighbourCost = countCost(tempSolution);

        if ((bestNeighbourCost == INT_MAX || currentNeighbourCost < bestNeighbourCost) &&
            !citiesExistOnTabuList(cities, currentNeighbourCost)) {
            bestNeighbourCost = currentNeighbourCost;
            bestNeighbour = tempSolution;
            replacedCitiesOfBestNeighbour = cities;
        }
    }

    return bestNeighbour;
}

const std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> &TabuSearchAlgorithm::getIntermediateSolutions() const {
    return intermediateSolutions;
}

void TabuSearchAlgorithm::decrementTenures(std::list<std::pair<std::pair<int, int>, int>> &tabuList) {
    for (auto iterator = tabuList.begin(); iterator != tabuList.end();) {
        if (0 == iterator->second)
            iterator = tabuList.erase(iterator);
        else {
            iterator->second--;
            ++iterator;
        }
    }
}

void TabuSearchAlgorithm::neighbourBySwap(std::vector<int> &tempSolution, long city_1Position, long city_2Position) {
    std::swap(tempSolution[city_1Position], tempSolution[city_2Position]);
}

void TabuSearchAlgorithm::neighbourByInsert(std::vector<int> &tempSolution, long city_1Position, long city_2Position) {
    if (city_1Position > city_2Position) {
        auto temp = tempSolution[city_1Position];
        for (auto i = city_1Position; i > city_2Position; i--)
            tempSolution[i] = tempSolution[i - 1];
        tempSolution[city_2Position] = temp;
    } else {
        auto temp = tempSolution[city_2Position];
        for (auto i = city_2Position; i > city_1Position; i--)
            tempSolution[i] = tempSolution[i - 1];
        tempSolution[city_1Position] = temp;
    }
}

void TabuSearchAlgorithm::neighbourByInvert(std::vector<int> &tempSolution, long city_1Position, long city_2Position) {
    if (city_1Position < city_2Position) {
        std::reverse(std::begin(tempSolution) + city_1Position,
                     std::end(tempSolution) - (tempSolution.size() - city_2Position - 1));
    } else {
        std::reverse(std::begin(tempSolution) + city_2Position,
                     std::end(tempSolution) - (tempSolution.size() - city_1Position - 1));
    }
}


