cmake_minimum_required(VERSION 3.9.1)
project(courseProject_pea3)

SET(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        AlgorithmTest.cpp
        AlgorithmTest.h
        main.cpp
        TimeMeasurement.cpp
        TimeMeasurement.h
        TravellingSalesmanProblem.cpp
        TravellingSalesmanProblem.h
        TSPLIB_Parser.cpp
		TSPLIB_Parser.h
		BranchAndBoundAlgorithm.cpp
		BranchAndBoundAlgorithm.h
		BruteForceAlgorithm.cpp
		BruteForceAlgorithm.h
		TabuSearchAlgorithm.cpp
		TabuSearchAlgorithm.h
		IntermediateSolution.cpp
		IntermediateSolution.h
		GeneticAlgorithm.cpp
		GeneticAlgorithm.h)

add_executable(courseProject_pea3 ${SOURCE_FILES})
