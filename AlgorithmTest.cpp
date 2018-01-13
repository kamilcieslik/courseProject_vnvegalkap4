//
// Created by mrfarinq on 22.05.17.
//

#include "AlgorithmTest.h"
#include "TimeMeasurement.h"
#include "TravellingSalesmanProblem.h"
#include "TSPLIB_Parser.h"

AlgorithmTest::AlgorithmTest() {

}

AlgorithmTest::~AlgorithmTest() {

}

void AlgorithmTest::TravellingSalesmanProblem_Test_BruteForce(int numberOfRepetitions) {
    TravellingSalesmanProblem s;
    TimeMeasurement t;
    std::vector<double> results;
    int firstAmountOfCities = 3;
    int amountOfCities = firstAmountOfCities;
    double sum = 0;
    std::ofstream file;
    file.open("test_tsp_brute_force.txt", std::ios::out);
    file << "Test czasów wykonywania algorytmu zupełnego komiwojażera z dnia - " << t.currentDateTime()
         << ".\nDla każdego zestawu danych wyniki uśrednione z " << numberOfRepetitions
         << " losowych instancji." << std::endl << std::endl << std::endl;

    for (auto i = 0; i < 5; i++) {
        results.push_back((double &&) amountOfCities);

        //Algorytm zupełny.
        sum = 0;

        for (auto j = 0; j < numberOfRepetitions; j++) {
            s.GenerateRandomCities(amountOfCities, 99);
            t.TimeStart();
            s.PerformBruteForceAlgorithm();
            t.TimeStop();
            sum += t.GetTimeInSeconds();
        }
        sum = sum / numberOfRepetitions;
        results.push_back(sum);

        std::cout << "." << std::endl << std::endl;

        //Zwiększenie ilości miast

        if (amountOfCities == 3) {
            amountOfCities = 5;
        } else if (amountOfCities == 5) {
            amountOfCities = 8;
        } else if (amountOfCities == 8) {
            amountOfCities = 11;
        } else if (amountOfCities == 11) {
            amountOfCities = 13;
        }
    }

    file << "Il_miast\tZupełny\n";
    for (int i = 0; i < results.size(); i++) {
        file << results[i] << "\t";
        if (((i + 1) % 2) == 0) {
            file << "\n";
        }
    }

    file << std::endl << std::endl << "Czas zakończenia testów - " << t.currentDateTime() << "." << std::endl;
    file.close();
}

void AlgorithmTest::TravellingSalesmanProblem_Test_BranchAndBound(int numberOfRepetitions) {
    TravellingSalesmanProblem s;
    TimeMeasurement t;
    std::vector<double> results;
    int amountOfCities = 33;
    double sum = 0;
    std::string path = "test/atsp/ftv33.atsp";
    std::ofstream file;
    file.open("test_atsp_branch_and_bound.txt", std::ios::out);
    file << "Test czasów wykonywania algorytmu podziału i ograniczeń problemu komiwojażera z dnia - "
         << t.currentDateTime()
         << ".\nDla każdego zestawu danych wyniki uśrednione z " << numberOfRepetitions
         << " losowych instancji." << std::endl << std::endl << std::endl;

    for (auto i = 0; i < 4; i++) {
        results.push_back((double &&) amountOfCities);

        //Algorytm podziału i ograniczeń.
        sum = 0;

        for (auto j = 0; j < numberOfRepetitions; j++) {
            TSPLIB_Parser parser(path);
            s.LoadArrayOfMatrixOfCities(parser.GetArrayOfMatrixCities(), parser.GetDimension(),
                                        parser.GetFileName(), parser.GetGraphType());
            t.TimeStart();
            s.PerformBranchAndBoundAlgorithm();
            t.TimeStop();
            sum += t.GetTimeInSeconds();
        }
        sum = sum / numberOfRepetitions;
        results.push_back(sum);
        results.push_back(s.GetTourLength());

        std::cout << "." << std::endl << std::endl;

        //Zwiększenie ilości miast - wczytanie danych z kolejnego pliku.

        if (path == "test/atsp/ftv33.atsp") {
            path = "test/atsp/ftv35.atsp";
            amountOfCities = 35;
        } else if (path == "test/atsp/ftv35.atsp") {
            path = "test/atsp/ftv38.atsp";
            amountOfCities = 38;
        } else if (path == "test/atsp/ftv38.atsp") {
            path = "test/atsp/ftv44.atsp";
            amountOfCities = 44;
        }
    }

    file << "Il_miast\tTabuSearch\tDługość\n";
    for (int i = 0; i < results.size(); i++) {
        file << results[i] << "\t";
        if (((i + 1) % 3) == 0) {
            file << "\n";
        }
    }

    file << std::endl << std::endl << "Czas zakończenia testów - " << t.currentDateTime() << "." << std::endl;
    file.close();
    std::cout << "Test zakończony pomyślnie." << std::endl << std::endl;
}

void AlgorithmTest::TravellingSalesmanProblem_Test_TabuSearch(int numberOfRepetitions) {
    TravellingSalesmanProblem s;
    std::vector<double> results;
    double sumTime = 0;
    double sumPath = 0;
    int tabuSize = 5;
    std::string path = "tests/tsp/fl1400.atsp";
    std::ofstream file;
    file.open("results/tsp/test_fl1400tsp_tabu_invert_tabusize5_8_11_14_17_20.txt", std::ios::out);

    for (auto i = 0; i < 6; i++) {
        results.push_back((double &&) tabuSize);
        sumTime = 0;
        sumPath=0;

        for (auto j = 0; j < numberOfRepetitions; j++) {

            TSPLIB_Parser parser(path);
            s.LoadArrayOfMatrixOfCities(parser.GetArrayOfMatrixCities(), parser.GetDimension(),
                                        parser.GetFileName(), parser.GetGraphType());
            s.PerformTabuSearchAlgorithm("invert", false, tabuSize);
            sumTime+=s.getIntermediateSolutions().back().getTimeFromTheBeginningOfTheAlgorithm();
            sumPath+=s.getIntermediateSolutions().back().getActualResult();
        }
        sumTime = sumTime / numberOfRepetitions;
        sumPath = sumPath / numberOfRepetitions;
        results.push_back(sumTime);
        results.push_back(sumPath);

        std::cout << "." << std::endl << std::endl;

        if (tabuSize == 5) {
            tabuSize = 8;
        } else if (tabuSize == 8) {
            tabuSize = 11;
        } else if (tabuSize == 11) {
            tabuSize = 14;
        } else if (tabuSize == 14) {
            tabuSize = 17;
        } else if (tabuSize == 17) {
            tabuSize = 20;
        }
    }

    file << "Rozm_tabu\tCzas\tDługość\n";
    for (int i = 0; i < results.size(); i++) {
        file << results[i] << "\t";
        if (((i + 1) % 3) == 0) {
            file << "\n";
        }
    }
}

