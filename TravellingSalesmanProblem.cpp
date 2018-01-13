//
// Created by mrfarinq on 16.06.17.
//

#include "TravellingSalesmanProblem.h"
#include "BranchAndBoundAlgorithm.h"
#include "BruteForceAlgorithm.h"
#include "TabuSearchAlgorithm.h"
#include <stack>
#include <random>

TravellingSalesmanProblem::TravellingSalesmanProblem() : amountOfCities(0), matrixOfCities(nullptr) {
}

TravellingSalesmanProblem::~TravellingSalesmanProblem() {
    DeleteTravellingSalesman();
}

void TravellingSalesmanProblem::DeleteTravellingSalesman() {
    for (auto i = 0; i < amountOfCities; i++)
        delete[] matrixOfCities[i];
    delete[] matrixOfCities;
    matrixOfCities = nullptr;
}

void TravellingSalesmanProblem::LoadArrayOfMatrixOfCities(int **_cities, int _amountOfCities,
                                                          std::string _fileName, std::string _graphType) {
    randomGeneratorData = false;
    if (matrixOfCities != nullptr) {
        for (int i = 0; i < amountOfCities; i++)
            delete[] matrixOfCities[i];
        delete[] matrixOfCities;
    }

    amountOfCities = _amountOfCities;

    matrixOfCities = new int *[amountOfCities];
    for (int i = 0; i < amountOfCities; i++)
        matrixOfCities[i] = new int[amountOfCities];

    for (int i = 0; i < amountOfCities; i++)
        for (int j = 0; j < amountOfCities; j++)
            matrixOfCities[i][j] = (int) _cities[i][j];

    fileName = std::move(_fileName);
    graphType = std::move(_graphType);

    std::cout << "Wczytywanie zakończone pomyślnie." << std::endl;
}

void TravellingSalesmanProblem::ReadCitiesFromNormalFile(std::string path) {
    if (matrixOfCities != nullptr)
        DeleteTravellingSalesman();

    std::fstream file(path, std::ios::in);
    if (file.is_open()) {
        file >> amountOfCities;

        matrixOfCities = new int *[amountOfCities];
        for (auto i = 0; i < amountOfCities; i++)
            matrixOfCities[i] = new int[amountOfCities];
        auto *securityMatrixForReadingPerLine = new int[amountOfCities];

        for (auto i = 0; i < amountOfCities; i++) {
            for (auto j = 0; j < amountOfCities; j++) {
                if (file.fail()) throw std::logic_error("Błąd odczytu danych w pliku.");
                file >> securityMatrixForReadingPerLine[j];
            }

            for (auto j = 0; j < amountOfCities; j++)
                matrixOfCities[i][j] = securityMatrixForReadingPerLine[j];
        }
        delete[] securityMatrixForReadingPerLine;
        file.close();
        std::cout << "Wczytywanie zakończone pomyślnie." << std::endl;
    } else
        std::cout << "Błąd otwarcia pliku." << std::endl;
}

void TravellingSalesmanProblem::GenerateRandomCities(int amountOfCities, int maxDistanceBetweenCity) {
    randomGeneratorData = true;
    if (matrixOfCities != nullptr)
        DeleteTravellingSalesman();

    if (amountOfCities == 0) {
        std::cout << "Podaj ilość miast: ";
        std::cin >> this->amountOfCities;
        if (this->amountOfCities < 1)
            throw std::invalid_argument("Liczba miast nie może być mniejsza od 1.");

        matrixOfCities = new int *[this->amountOfCities];
        for (auto i = 0; i < this->amountOfCities; i++)
            matrixOfCities[i] = new int[this->amountOfCities];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_distancesBetweenCities(1, maxDistanceBetweenCity);

        for (auto i = 0; i < this->amountOfCities; i++) {
            for (auto j = 0; j < this->amountOfCities; j++)
                matrixOfCities[i][j] = dist_distancesBetweenCities(gen);
        }
    } else {
        this->amountOfCities = amountOfCities;

        matrixOfCities = new int *[this->amountOfCities];
        for (auto i = 0; i < this->amountOfCities; i++)
            matrixOfCities[i] = new int[this->amountOfCities];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_distancesBetweenCities(1, maxDistanceBetweenCity);

        for (auto i = 0; i < this->amountOfCities; i++) {
            for (auto j = 0; j < this->amountOfCities; j++)
                matrixOfCities[i][j] = dist_distancesBetweenCities(gen);
        }
    }
}

void TravellingSalesmanProblem::PrintCitiesForTheTravellingSalesman(bool printInputMatrix) {
    if (matrixOfCities == nullptr)
        throw std::logic_error("Brak miast do wyświetlenia.");

    std::cout << "\e[1mProblem\e[0m" << std::endl;
    std::cout << "-------------------" << std::endl;
    if (printInputMatrix) {
        std::cout << std::endl;
        std::cout << "\t";
        for (auto i = 0; i < amountOfCities; i++) {
            std::cout << i << ".\t";
        }
        std::cout << "\v" << std::endl;
        for (auto i = 0; i < amountOfCities; i++) {
            for (auto j = 0; j < amountOfCities; j++) {
                if (j == 0) {
                    if (matrixOfCities[i][j] < 0) {
                        if (i == j)
                            std::cout << i << ".\t\b" << "\e[1mINF\e[0m";
                        else
                            std::cout << i << ".\t\b" << matrixOfCities[i][j];
                    } else {
                        if (i == j)
                            std::cout << i << ".\t" << "\e[1mINF\e[0m";
                        else
                            std::cout << i << ".\t" << matrixOfCities[i][j];
                    }
                } else {
                    if (matrixOfCities[i][j] < 0) {
                        if (i == j)
                            std::cout << "\t\b" << "\e[1mINF\e[0m";
                        else
                            std::cout << "\t\b" << matrixOfCities[i][j];
                    } else {
                        if (i == j)
                            std::cout << "\t" << "\e[1mINF\e[0m";
                        else
                            std::cout << "\t" << matrixOfCities[i][j];
                    }
                }
            }
            std::cout << "\v" << std::endl;
        }
    }
    if (!randomGeneratorData) {
        std::cout << "Name of TSPLIB file:\t" << fileName << std::endl;
        std::cout << "Graph type:\t\t" << graphType << std::endl;

    } else {
        std::cout << "Dane wejściowe zostały wygenerowane losowo." << std::endl;
        std::cout << "Graph type:\t\tATSP" << std::endl;
    }
    std::cout << "Number of cities:\t" << amountOfCities << std::endl;
}

void TravellingSalesmanProblem::PerformBruteForceAlgorithm() {
    if (matrixOfCities == nullptr)
        throw std::logic_error("Brak miast do przeprowadzenia algorytmu problemu komiwojażera.");

    whichTypeOfAlgorithm = "brute_force";
    optimalWay.clear();

    BruteForceAlgorithm algorithm(matrixOfCities, amountOfCities);
    algorithm.DoCalculations();
    optimalWay = algorithm.GetResults().first;
    optimalLength = algorithm.GetResults().second;
}

void TravellingSalesmanProblem::PerformBranchAndBoundAlgorithm() {
    if (matrixOfCities == nullptr)
        throw std::logic_error("Brak miast do przeprowadzenia algorytmu problemu komiwojażera.");

    whichTypeOfAlgorithm = "branch_and_bound";
    optimalWay.clear();

    BranchAndBoundAlgorithm algorithm(matrixOfCities, amountOfCities);
    algorithm.DoCalculations();
    optimalWay = algorithm.GetResults().first;
    optimalLength = algorithm.GetResults().second;
}

void TravellingSalesmanProblem::PerformTabuSearchAlgorithm(std::string neighborhoodType,
                                                           bool showIntermediateSolutionsInRuntime, int tabuSize) {
    if (matrixOfCities == nullptr)
        throw std::logic_error("Brak miast do przeprowadzenia algorytmu problemu komiwojażera.");

    whichTypeOfAlgorithm = "tabu_search";
    optimalWay.clear();

    TabuSearchAlgorithm algorithm(matrixOfCities, amountOfCities);

    if (neighborhoodType == "auto") {
        if (amountOfCities <= 30) {
            algorithm.DoCalculations(13, 500, 30, 10, "insert");
        } else if (amountOfCities > 30 && amountOfCities <= 150) {
            algorithm.DoCalculations(13, 1000, 400, 10, "insert");
        } else if (amountOfCities > 150 && amountOfCities <= 5000) {
            algorithm.DoCalculations(13, 50, 30, 20, "insert");
        } else
            algorithm.DoCalculations(13, 50, 100, 10, "insert");
    } else {
        if (amountOfCities <= 30) {
            algorithm.DoCalculations(tabuSize, 1000, 50, 10, neighborhoodType, showIntermediateSolutionsInRuntime);
        } else if (amountOfCities > 30 && amountOfCities <= 150) {
            algorithm.DoCalculations(tabuSize, 1000, 50, 10, neighborhoodType, showIntermediateSolutionsInRuntime);
        } else if (amountOfCities > 150 && amountOfCities <= 5000) {
            algorithm.DoCalculations(tabuSize, 1000, 50, 10, neighborhoodType, showIntermediateSolutionsInRuntime);
        } else
            algorithm.DoCalculations(tabuSize, 2000, 70, 10, neighborhoodType, showIntermediateSolutionsInRuntime);
    }

    optimalWay = algorithm.GetResults().first;
    optimalLength = algorithm.GetResults().second;
    intermediateSolutions = algorithm.getIntermediateSolutions();
}

void TravellingSalesmanProblem::PrintSolution() {
    std::cout << "\e[1mSolution\e[0m" << std::endl;
    if (whichTypeOfAlgorithm == "brute_force")
        std::cout << "\e[1mFull Search Algorithm\e[0m" << std::endl;
    else if (whichTypeOfAlgorithm == "branch_and_bound")
        std::cout << "\e[1mBranch and Bound Algorithm\e[0m" << std::endl;
    else if (whichTypeOfAlgorithm == "tabu_search")
        std::cout << "\e[1mTabu Search Algorithm\e[0m" << std::endl;

    std::cout << "-------------------" << std::endl;
    std::cout << "Length\t= " << optimalLength << std::endl;
    std::cout << "Path\t= ";

    if (whichTypeOfAlgorithm == "brute_force") {
        for (auto i = 0; i < amountOfCities; i++)
            std::cout << optimalWay[i] << " - ";
        std::cout << optimalWay[0] << std::endl;
    } else if (whichTypeOfAlgorithm == "branch_and_bound") {
        for (auto i = 0; i < amountOfCities; i++)
            std::cout << optimalWay[i] - 1 << " - ";
        std::cout << optimalWay[0] - 1 << std::endl;
    } else if (whichTypeOfAlgorithm == "tabu_search") {
        for (auto i = 0; i < amountOfCities; i++)
            std::cout << optimalWay[i] << " - ";

        std::cout << optimalWay[0] << std::endl;
        std::cout << "-------------------" << std::endl;
        std::cout << "\e[1mIntermediate Solutions\e[0m" << std::endl;
        std::cout << "\ttime[s]\t\t\tlength" << std::endl;
        for (auto i = 0; i < intermediateSolutions.size(); i++) {
            std::cout << i + 1 << ".\t" << std::fixed << std::setprecision(6)
                      << intermediateSolutions[i].getTimeFromTheBeginningOfTheAlgorithm() << "\t|\t"
                      << intermediateSolutions[i].getActualResult();
            if (i == 0)
                std::cout << "\t<-- Greedy Solution";
            std::cout << std::endl;
        }
    }
}

long long int TravellingSalesmanProblem::GetTourLength() {
    return optimalLength;
}

const std::vector<IntermediateSolutionOfTheTabuSearchAlgorithm> &
TravellingSalesmanProblem::getIntermediateSolutions() const {
    return intermediateSolutions;
}

