#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

struct PairHash 
{
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U>& p) const;
};

struct PairEqual 
{
    template <typename T, typename U>
    bool operator()(const std::pair<T, U>& lhs, const std::pair<T, U>& rhs) const;
};

// Integer value for operation symbols
enum
{
    PLUS = 43,
    MINUS = 45,
    MULT = 42,
    DIV = 47
};


std::string Int_to_string(int num);

int FindOperation(std::string& expression, std::string& OP, const int start);

bool IsDigit(const std::string& str);

int FindDigit(const std::string& expression, const int start);

int String_to_int(std::string& str, int start, int end);

void ChangeExpression(std::string& expression, std::map<std::string, std::string>& Colom_Indexes, std::map<std::string, std::string>& Row_Indexes);

std::map<std::string, std::string> Indexing(std::string coloums);

std::map<std::string, std::string> Indexing(std::vector<std::vector<std::string>>& data);

void CheckForCorrect(std::vector<std::vector<std::string>>& data, std::map<std::string, std::string>& Colom_Indexes, std::map<std::string, std::string>& Row_Indexes);

std::vector<std::vector<std::string>> Read(const std::string filename);

void PRINT(std::vector<std::vector<std::string>>& csv_data);

std::vector<int> StringToIntArray(const std::string& input);

int RecSolve(std::vector<std::vector<std::string>>& csv_data, int col, int row, std::unordered_set<std::pair<int, int>, PairHash, PairEqual>& Visited);

void DeepSolve(std::vector<std::vector<std::string>>& csv_data);