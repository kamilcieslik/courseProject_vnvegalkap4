//
// Created by mrfarinq on 10.01.18.
//

#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(matrixOfCities),
                                                                               amountOfCities(amountOfCities) {
}

std::pair<std::vector<int>, int> GeneticAlgorithm::GetResults() {
    return std::make_pair(optimalWay, length);
}

void
GeneticAlgorithm::PerformGeneticAlgorithm(unsigned long populationSize, int generationMaxAmount, int matingPoolSize,
                                          float crossoverCoefficient, float mutationCoefficient) {
    srand(static_cast<unsigned int>(time(nullptr)));

    int generationAmount = 0;
    std::vector<std::pair<std::vector<int>, int>> matingPool, newGeneration;

    // 1. Wybór populacji początkowej chromosomów (losowy).
    initRandomChromosomePopulation(populationSize);

    // 2. Ocena przystosowania chromosomów.
    assessmentOfChromosomalAdaptation(population);

    // 3. Sprawdzenie warunku zatrzymania (ilość pokoleń).
    while (generationAmount < generationMaxAmount) {
        // a. selekcja chromosomów metodą listy rankingowej - wybór populacji macierzystej
        matingPool = rankingListSelection(matingPoolSize);

        // b. krzyżowanie z częściowym odwzorowaniem (PMX) chromosomów z populacji rodzicielskiej.
        newGeneration = pmxCrossover(matingPool, crossoverCoefficient);

        // c. mutacja potomków metodą inversion.
        inversionMutation(mutationCoefficient, newGeneration);

        // d. ocena przystosowania chromosomów.
        assessmentOfChromosomalAdaptation(newGeneration);

        // e. utworzenie nowej populacji.
        creationOfNewPopulation(newGeneration, populationSize);

        generationAmount++;
    }

    // 4. Wyprowadzenie "najlepszego" rozwiązania.
    optimalWay = population[0].first;
    length = population[0].second;
}

void GeneticAlgorithm::initRandomChromosomePopulation(int populationSize) {
    std::pair<std::vector<int>, int> firstChromosome, newChromosome;
    unsigned long firstPosition, secondPosition;
    population.clear();

    firstChromosome.first = generateRandomSolution();
    population.push_back(firstChromosome);

    for (auto i = 1; i < populationSize; i++) {
        newChromosome.first = firstChromosome.first;
        firstPosition = rand() % newChromosome.first.size();
        do {
            secondPosition = (rand() % newChromosome.first.size());
        } while (firstPosition == secondPosition);
        invertOperation(newChromosome.first, firstPosition, secondPosition);
        population.push_back(newChromosome);
    }
}

void GeneticAlgorithm::assessmentOfChromosomalAdaptation(std::vector<std::pair<std::vector<int>, int>> &chromosomes) {
    int cost = 0;
    for (auto &chromosome :chromosomes) {
        cost = 0;
        for (auto i = 0; i < chromosome.first.size() - 1; i++)
            cost += matrixOfCities[chromosome.first[i]][chromosome.first[i + 1]];
        cost += matrixOfCities[chromosome.first[chromosome.first.size() - 1]][chromosome.first[0]];
        chromosome.second = cost;
    }
}

void GeneticAlgorithm::creationOfNewPopulation(std::vector<std::pair<std::vector<int>, int>> &newGeneration,
                                               unsigned long populationSize) {
    population.insert(population.end(), newGeneration.begin(), newGeneration.end());
    std::sort(population.begin(), population.end(), routeComparison);
    population.resize(populationSize);
}

std::vector<int> GeneticAlgorithm::generateRandomSolution() {
    std::vector<int> solution;
    for (auto i = 0; i < amountOfCities; i++)
        solution.push_back(i);
    std::shuffle(solution.begin(), solution.end(), std::mt19937(std::random_device()()));
    return solution;
}

std::vector<std::pair<std::vector<int>, int>> GeneticAlgorithm::rankingListSelection(int parentsPopulationSize) {
    std::vector<std::pair<std::vector<int>, int>> parentPopulation;
    parentPopulation = population;
    std::sort(parentPopulation.begin(), parentPopulation.end(), routeComparison);
    return std::vector<std::pair<std::vector<int>, int>>(parentPopulation.begin(),
                                                         parentPopulation.begin() + parentsPopulationSize);
}

std::vector<std::pair<std::vector<int>, int> >
GeneticAlgorithm::pmxCrossover(std::vector<std::pair<std::vector<int>, int> > parentPopulation,
                               float crossoverCoefficient) {
    std::vector<std::pair<std::vector<int>, int>> newGeneration;
    std::pair<std::vector<int>, int> firstChild, secondChild;
    int matchingSectionFirstPosition, matchingSectionSecondPosition, start = 0;

    for (auto i = 0; i < parentPopulation.size(); i++) {
        if ((float) rand() / RAND_MAX < crossoverCoefficient)
            for (auto j = start; j < parentPopulation.size(); j++) {
                if (i == j)
                    continue;

                if ((float) rand() / RAND_MAX < crossoverCoefficient) {

                    firstChild.first.resize(amountOfCities, -1);
                    secondChild.first.resize(amountOfCities, -1);

                    matchingSectionFirstPosition = rand() % amountOfCities;
                    do {
                        matchingSectionSecondPosition = rand() % amountOfCities;
                    } while (matchingSectionFirstPosition == matchingSectionSecondPosition);

                    if (matchingSectionFirstPosition > matchingSectionSecondPosition)
                        std::swap(matchingSectionFirstPosition, matchingSectionSecondPosition);

                    for (auto k = matchingSectionFirstPosition; k <= matchingSectionSecondPosition; k++) {
                        firstChild.first[k] = parentPopulation[j].first[k];
                        secondChild.first[k] = parentPopulation[i].first[k];
                    }

                    for (auto k = 0; k < amountOfCities; k++) {
                        if (k == matchingSectionFirstPosition) {
                            k = matchingSectionSecondPosition;
                            continue;
                        }
                        addCityWithoutConflict(firstChild, parentPopulation[i], k, matchingSectionFirstPosition,
                                               matchingSectionSecondPosition);
                        addCityWithoutConflict(secondChild, parentPopulation[j], k, matchingSectionFirstPosition,
                                               matchingSectionSecondPosition);
                    }

                    newGeneration.push_back(firstChild);
                    newGeneration.push_back(secondChild);
                    firstChild.first.clear();
                    secondChild.first.clear();
                }
            }
        start++;
    }

    return newGeneration;
}

void GeneticAlgorithm::addCityWithoutConflict(std::pair<std::vector<int>, int> &child,
                                              std::pair<std::vector<int>, int> const &parent, int mappingPosition,
                                              int matchingSectionFirstPosition,
                                              int matchingSectionSecondPosition) {
    int shift = 0, interval = matchingSectionSecondPosition - matchingSectionFirstPosition + 1;
    auto foundPosition =
            std::find(child.first.begin(), child.first.end(), parent.first[mappingPosition]) - child.first.begin();

    if (foundPosition == child.first.size())
        child.first[mappingPosition] = parent.first[mappingPosition];
    else {
        while (std::find(child.first.begin(), child.first.end(), parent.first[matchingSectionFirstPosition +
                                                                              (foundPosition -
                                                                               matchingSectionFirstPosition + shift) %
                                                                              interval]) != child.first.end())
            shift++;
        child.first[mappingPosition] = parent.first[matchingSectionFirstPosition +
                                                    (foundPosition - matchingSectionFirstPosition + shift) % interval];
    }
}

void GeneticAlgorithm::inversionMutation(float mutationCoefficient,
                                         std::vector<std::pair<std::vector<int>, int> > &mutatedGeneration) {
    unsigned long firstPosition, secondPosition;
    for (auto &i : mutatedGeneration) {
        if ((float) rand() / RAND_MAX < mutationCoefficient) {
            firstPosition = rand() % i.first.size();
            do {
                secondPosition = (rand() % i.first.size());
            } while (firstPosition == secondPosition);
            invertOperation(i.first, firstPosition, secondPosition);
        }
    }
}

void GeneticAlgorithm::invertOperation(std::vector<int> &chromosome, long city_1Position, long city_2Position) {
    if (city_1Position < city_2Position)
        std::reverse(std::begin(chromosome) + city_1Position,
                     std::end(chromosome) - (chromosome.size() - city_2Position - 1));
    else
        std::reverse(std::begin(chromosome) + city_2Position,
                     std::end(chromosome) - (chromosome.size() - city_1Position - 1));
}
