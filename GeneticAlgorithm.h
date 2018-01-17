//
// Created by mrfarinq on 10.01.18.
//

#ifndef COURSEPROJECT_PEA3_GENETICALGORITHM_H
#define COURSEPROJECT_PEA3_GENETICALGORITHM_H


#include <vector>
#include "IntermediateSolution.h"

class GeneticAlgorithm {
    class RouteComparison {
    public:
        bool operator()(const std::pair<std::vector<int>, int> &firstRoute,
                        const std::pair<std::vector<int>, int> &secondRoute) const {
            return (firstRoute.second < secondRoute.second);
        }
    };

    long long int length;
    int amountOfCities;
    int **matrixOfCities;
    std::vector<int> optimalWay;
    std::vector<std::pair<std::vector<int>, int>> population;
    RouteComparison routeComparison;

    void initRandomChromosomePopulation(int populationSize);

    void assessmentOfChromosomalAdaptation(std::vector<std::pair<std::vector<int>, int>> &chromosomes);

    void creationOfNewPopulation(std::vector<std::pair<std::vector<int>, int>> &newGeneration,
                                 unsigned long populationSize);

    std::vector<int> generateRandomSolution();

    std::vector<std::pair<std::vector<int>, int> > rankingListSelection(int parentsPopulationSize);

    std::vector<std::pair<std::vector<int>, int> >
    pmxCrossover(std::vector<std::pair<std::vector<int>, int> > parentPopulation, float crossoverCoefficient);

    void addCityWithoutConflict(std::pair<std::vector<int>, int> &child, std::pair<std::vector<int>, int> const &parent,
                                int mappingPosition, int matchingSectionFirstPosition,
                                int matchingSectionSecondPosition);

    void
    inversionMutation(float mutationCoefficient, std::vector<std::pair<std::vector<int>, int> > &mutatedGeneration);

    void invertOperation(std::vector<int> &chromosome, long city_1Position, long city_2Position);

public:
    GeneticAlgorithm(int **matrixOfCities, int amountOfCities);

    std::pair<std::vector<int>, int> GetResults();

    void PerformGeneticAlgorithm(unsigned long populationSize, int generationMaxAmount, int matingPoolSize,
                                 float crossoverCoefficient, float mutationCoefficient);
};


#endif //COURSEPROJECT_PEA3_GENETICALGORITHM_H
