//
// Created by mrfarinq on 06.12.17.
//

#include <stdexcept>
#include <climits>
#include <vector>
#include <stack>
#include "BranchAndBoundAlgorithm.h"

Subset::Subset() : isK1(true), parent(INT_MIN) {
}

void BranchAndBoundAlgorithm::DoCalculations() {
    upperBound = INT_MAX;
    treeOfSubsets.clear();
    optimalWay.clear();

    std::vector<std::vector<int>> inputMatrixOfCities(static_cast<unsigned long>(amountOfCities + 1),
                                                      std::vector<int>(static_cast<unsigned long>(amountOfCities + 1)));
    Subset subsetK2;
    subsetK2.isK1 = false;
    Subset subsetK1;

    PrepareMatrix(inputMatrixOfCities, matrixOfCities);

    std::pair<int, int> positionOfMatrixCell;
    std::pair<int, std::vector<std::vector<int>>> matrix;
    std::stack<std::pair<int, std::vector<std::vector<int>>>> stackOfMatrices;

    int id = 0;
    matrix.first = id;
    matrix.second = inputMatrixOfCities;
    stackOfMatrices.push(matrix);

    while (!stackOfMatrices.empty()) {
        id = stackOfMatrices.top().first;
        std::vector<std::vector<int>> actualMatrix(stackOfMatrices.top().second);
        stackOfMatrices.pop();

        subsetK1.lowerBound = StandarizationOfMatrix(actualMatrix);
        if (id == 0) {
            treeOfSubsets.push_back(subsetK1);
        }

        while (actualMatrix.size() > 3 and treeOfSubsets[id].lowerBound < upperBound) {
            subsetK2.lowerBound = treeOfSubsets[id].lowerBound +
                                  CalculateCostOfResignation(actualMatrix, subsetK1.route, positionOfMatrixCell);
            subsetK2.parent = id;
            treeOfSubsets.push_back(subsetK2);

            if (subsetK2.lowerBound < upperBound) {
                matrix.first = (int) (treeOfSubsets.size() - 1);
                matrix.second = actualMatrix;
                matrix.second[positionOfMatrixCell.first][positionOfMatrixCell.second] = INT_MAX;
                stackOfMatrices.push(matrix);
            }

            MatrixShortening(actualMatrix, positionOfMatrixCell.first, positionOfMatrixCell.second);

            EliminationOfSubtour(actualMatrix, (int) (treeOfSubsets.size() - 1), subsetK1.route);

            subsetK1.lowerBound = treeOfSubsets[id].lowerBound + StandarizationOfMatrix(actualMatrix);
            subsetK1.parent = id;
            treeOfSubsets.push_back(subsetK1);

            id = (int) (treeOfSubsets.size() - 1);
        }

        if (actualMatrix.size() == 3) {
            if (subsetK1.lowerBound < upperBound) {
                upperBound = subsetK1.lowerBound;
                SetOptimalWay(actualMatrix, (int) (treeOfSubsets.size() + 1));
            }
        }
    }
}

void BranchAndBoundAlgorithm::PrepareMatrix(std::vector<std::vector<int>> &matrix, int **arrayOfMatrixOfCities) {
    for (int i = 0; i < matrix.size(); i++) {
        matrix[i][0] = i;
        matrix[0][i] = i;
    }

    for (auto i = 0; i < matrix.size() - 1; i++) {
        for (auto j = 0; j < matrix.size() - 1; j++) {
            matrix[i + 1][j + 1] = arrayOfMatrixOfCities[i][j];
        }
        matrix[i + 1][i + 1] = INT_MAX;
    }
}

void BranchAndBoundAlgorithm::EliminationOfSubtour(std::vector<std::vector<int>> &activeMatrix, int index,
                                                   std::pair<int, int> &route) {
    std::vector<std::pair<int, int> > _routes;
    while (index != 0) {
        if (treeOfSubsets[index].isK1) {
            _routes.push_back(treeOfSubsets[index].route);
        }
        index = (int) treeOfSubsets[index].parent;
    }

    std::deque<int> subtour = {route.first, route.second};
    bool found = true;
    while (found) {
        found = false;
        for (const std::pair<int, int> &_route : _routes) {
            if (_route.second == subtour.front()) {
                subtour.push_front(_route.second);
                subtour.push_front(_route.first);
                found = true;
                break;
            } else if (_route.first == subtour.back()) {
                subtour.push_back(_route.first);
                subtour.push_back(_route.second);
                found = true;
                break;
            }
        }
    }

    std::pair<int, int> positionOfMatrixCell;
    int foundsPositionIndexes = 0;
    for (int i = 1; i < activeMatrix.size(); i++) {
        if (activeMatrix[i][0] == subtour.back()) {
            positionOfMatrixCell.first = i;
            foundsPositionIndexes++;
        }
        if (activeMatrix[0][i] == subtour.front()) {
            positionOfMatrixCell.second = i;
            foundsPositionIndexes++;
        }
    }

    if (foundsPositionIndexes == 2) {
        activeMatrix[positionOfMatrixCell.first][positionOfMatrixCell.second] = INT_MAX;
    }
}

int
BranchAndBoundAlgorithm::CalculateCostOfResignation(std::vector<std::vector<int>> &activeMatrix,
                                                    std::pair<int, int> &route,
                                                    std::pair<int, int> &positionOfMatrixCell) {
    int max = INT_MIN;
    for (int i = 1; i < activeMatrix.size(); i++) {
        for (int j = 1; j < activeMatrix.size(); j++) {
            if (activeMatrix[i][j] == 0) {
                int val = GetMinimumRow(activeMatrix, i, j) +
                          GetMinimumColumn(activeMatrix, j, i);
                if (max < val || max < 0) {
                    max = val;
                    positionOfMatrixCell.first = i;
                    positionOfMatrixCell.second = j;

                    route.first = activeMatrix[i][0];
                    route.second = activeMatrix[0][j];
                }
            }
        }
    }

    return max;
}

int BranchAndBoundAlgorithm::GetMinimumRow(std::vector<std::vector<int>> &activeMatrix, int row, int skipped) {
    int min = INT_MAX;
    for (int i = 1; i < activeMatrix.size(); i++) {
        int currentValue = activeMatrix[row][i];
        if (currentValue != INT_MAX && i != skipped) {
            min = (min < currentValue ? min : currentValue);
        }
    }

    return min;
}

int BranchAndBoundAlgorithm::GetMinimumColumn(std::vector<std::vector<int>> &activeMatrix, int column,
                                              int skipped) {
    int min = INT_MAX;
    for (int i = 1; i < activeMatrix.size(); i++) {
        int currentValue = activeMatrix[i][column];
        if (currentValue != INT_MAX && i != skipped) {
            min = (min < currentValue ? min : currentValue);
        }
    }

    return min;
}

int
BranchAndBoundAlgorithm::SubtractMinimalValuesFromTheRows(std::vector<std::vector<int>> &activeMatrix, int row) {
    int min = GetMinimumRow(activeMatrix, row);
    for (int i = 1; i < activeMatrix.size(); i++) {
        if (activeMatrix[row][i] != INT_MAX) {
            activeMatrix[row][i] = activeMatrix[row][i] - min;
        }
    }

    return min;
}

int
BranchAndBoundAlgorithm::SubtractMinimalValuesFromTheColumns(std::vector<std::vector<int>> &activeMatrix, int col) {
    int min = GetMinimumColumn(activeMatrix, col);
    for (int i = 1; i < activeMatrix.size(); i++) {
        if (activeMatrix[i][col] != INT_MAX) {
            activeMatrix[i][col] = activeMatrix[i][col] - min;
        }
    }

    return min;
}

int BranchAndBoundAlgorithm::StandarizationOfMatrix(std::vector<std::vector<int>> &activeMatrix) {
    int minRowTotal = 0;
    for (int i = 1; i < activeMatrix.size(); i++) {
        minRowTotal += SubtractMinimalValuesFromTheRows(activeMatrix, i);
    }

    int minColTotal = 0;
    for (int i = 1; i < activeMatrix.size(); i++) {
        minColTotal += SubtractMinimalValuesFromTheColumns(activeMatrix, i);
    }

    return minRowTotal + minColTotal;
}

void BranchAndBoundAlgorithm::SetOptimalWay(std::vector<std::vector<int>> &activeMatrix, int index) {
    Subset K1;

    for (int i = 1; i < 3; i++) {
        for (int j = 1; j < 3; j++) {
            if (activeMatrix[i][j] == 0) {
                K1.lowerBound = treeOfSubsets.back().lowerBound;
                K1.parent = treeOfSubsets.size() - 1;
                K1.route.first = activeMatrix[i][0];
                K1.route.second = activeMatrix[0][j];
                treeOfSubsets.push_back(K1);
            }
        }
    }

    std::vector<std::pair<int, int> > route;
    while (index != 0) {
        if (treeOfSubsets[index].isK1) {
            route.push_back(treeOfSubsets[index].route);
        }
        index = (int) treeOfSubsets[index].parent;
    }

    std::vector<int> optimalWay;
    auto pathSize = (int) route.size();
    for (int i = 0; i < route.size(); i++) {
        if (route[i].first == 1) {
            optimalWay.push_back(route[i].first);
            optimalWay.push_back(route[i].second);
            route.erase(route.begin() + i);
        }
    }

    while (optimalWay.size() != pathSize) {
        for (int i = 0; i < route.size(); i++) {
            if (optimalWay.back() == route[i].first) {
                optimalWay.push_back(route[i].second);
                route.erase(route.begin() + i);
            }
        }
    }

    this->optimalWay = optimalWay;
}

void BranchAndBoundAlgorithm::MatrixShortening(std::vector<std::vector<int>> &activeMatrix, int row, int col) {
    auto it_row = activeMatrix.begin() + row;
    activeMatrix.erase(it_row);

    for (auto &i : activeMatrix) {
        auto it_col = i.begin() + col;
        i.erase(it_col);
    }
}

std::pair<std::vector<int>, int> BranchAndBoundAlgorithm::GetResults() {
    return std::make_pair(optimalWay, upperBound);
}

BranchAndBoundAlgorithm::BranchAndBoundAlgorithm(int **matrixOfCities, int amountOfCities) : matrixOfCities(
        matrixOfCities), amountOfCities(amountOfCities) {}
