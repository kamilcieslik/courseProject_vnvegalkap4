//
// Created by mrfarinq on 06.12.17.
//

#ifndef COURSEPROJECT_PEA2_BRANCHANDBOUNDALGORITHM_H
#define COURSEPROJECT_PEA2_BRANCHANDBOUNDALGORITHM_H

struct Subset {
    std::pair<int, int> route;
    int lowerBound;
    long parent;
    bool isK1;

    Subset();
};

class BranchAndBoundAlgorithm {
    int amountOfCities;
    int upperBound;
    std::vector<Subset> treeOfSubsets;
    std::vector<int> optimalWay;
    int **matrixOfCities;

public:
    BranchAndBoundAlgorithm(int **matrixOfCities, int amountOfCities);

    std::pair<std::vector<int>, int> GetResults();

    void DoCalculations();

private:
    void PrepareMatrix(std::vector<std::vector<int>> &matrix, int **arrayOfMatrixOfCities);

    void EliminationOfSubtour(std::vector<std::vector<int>> &activeMatrix, int index,
                              std::pair<int, int> &route);

    int CalculateCostOfResignation(std::vector<std::vector<int>> &activeMatrix,
                                   std::pair<int, int> &route,
                                   std::pair<int, int> &positionOfMatrixCell);

    int GetMinimumRow(std::vector<std::vector<int>> &activeMatrix, int row, int skipped = -1);

    int GetMinimumColumn(std::vector<std::vector<int>> &activeMatrix, int column,
                         int skipped = -1);

    int SubtractMinimalValuesFromTheRows(std::vector<std::vector<int>> &activeMatrix, int row);

    int SubtractMinimalValuesFromTheColumns(std::vector<std::vector<int>> &activeMatrix, int col);

    int StandarizationOfMatrix(std::vector<std::vector<int>> &activeMatrix);

    void SetOptimalWay(std::vector<std::vector<int>> &activeMatrix, int index);

    void MatrixShortening(std::vector<std::vector<int>> &activeMatrix, int row, int col);
};


#endif //COURSEPROJECT_PEA2_BRANCHANDBOUNDALGORITHM_H
