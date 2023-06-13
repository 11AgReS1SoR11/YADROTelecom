#include "YADROTelecom.h"

// Overloading hash functions for pairs
template <typename T, typename U>
std::size_t PairHash::operator()(const std::pair<T, U>& p) const 
{
    std::size_t h1 = std::hash<T>{}(p.first);
    std::size_t h2 = std::hash<U>{}(p.second);
    return h1 ^ h2;
}

// Overloading the parenthesis operator to compare pairs
template <typename T, typename U>
bool PairEqual::operator()(const std::pair<T, U>& lhs, const std::pair<T, U>& rhs) const 
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

// The function converts int to std::string
std::string Int_to_string(int num)
{
    if (num == 0)
        return "0";

    
    // Processing the case of a negative number
    bool isNegative = false;
    if (num < 0) 
    {
        isNegative = true;
        num = -num;
    }

    // Convert each digit into a string
    std::string result;
    while (num > 0)
    {
        char digit = '0' + (num % 10);
        result = digit + result;
        num /= 10;
    }

    if (isNegative)
        result = "-" + result;

    return result;
}

// The function finds the index of the operation
int FindOperation(std::string& expression, std::string& OP, const int start)
{
    for (int i = start; i < expression.size(); ++i)
    {
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '/' || expression[i] == '*')
        {
            if (expression[i] == '+') OP = Int_to_string(PLUS);
            else if (expression[i] == '-') OP = Int_to_string(MINUS);
            else if (expression[i] == '/') OP = Int_to_string(DIV);
            else if (expression[i] == '*') OP = Int_to_string(MULT);
            return i;
        }
    }
    return -1;
}

bool IsDigit(const std::string& str)
{
    int start = 0;
    if (str[0] == '-') start = 1;
    for (int i = start; i < str.size(); ++i)
    {
        if (!(std::isdigit(str[i])))
        {
            return false;
        }
    }
    return true;
}

// Finds the first occurrence of a digit and returns the index
int FindDigit(const std::string& expression, const int start)
{
    for (int i = start; i < expression.size(); ++i)
    {
        // If it is a negative number, then we check for a minus sign and that the next character is a digit
        if (std::isdigit(expression[i]) || (i < expression.size() - 1 && expression[i] == '-' && std::isdigit(expression[i+1]))) 
            return i;
    }
    return -1;
}

// converts std::string to an integer type
int String_to_int(std::string& str, int start, int end)
{
    if (!IsDigit(str))
    {
        throw std::runtime_error("It's no number: " + str);
    }

    int result = 0;
    
    // We check the presence of a minus sign
    int sign = 1;
    if (str[start] == '-')
    {
        sign = -1;
        ++start; // We start with the index of the next character
    }

    // Convert each digit into a number
    for (int i = start; i < end; ++i)
    {
        result = result * 10 + (str[i] - '0');
    }

    return result * sign;
}

// The idea is to replace all letters in expressions with column indexes. 
// This will help to access the cell in constant time
void ChangeExpression(std::string& expression, std::map<std::string, std::string>& Colom_Indexes, std::map<std::string, std::string>& Row_Indexes)
{
    int row_idx = FindDigit(expression, 1);
    if (row_idx == -1)
    {
        // No digits
        throw std::runtime_error("invalid cell format: " + expression);
    }

    std::string OP;
    int OP_idx = FindOperation(expression, OP, row_idx + 1);
    if (OP_idx == -1)
    {
        // No operation symbol
        throw std::runtime_error("invalid cell format: " + expression);
    }

    std::string col1 = expression.substr(1, row_idx - 1);  // row_idx = 1 means that there is no letter
    std::string row1 = expression.substr(row_idx, OP_idx - row_idx);
    if (!IsDigit(row1))
    {
        // The row must be set as a number
        throw std::runtime_error("invalid cell format: " + expression);
    }
    // if there are letters, then a column and a row will be specified, but the row must be from the table
    if (col1.size() != 0 && Row_Indexes.find(row1) == Row_Indexes.end())
    {
        throw std::runtime_error("No such row: " + row1);
    }
    if (Colom_Indexes.find(col1) == Colom_Indexes.end())
    {
        throw std::runtime_error("No such coloum: " + col1);
    }

    // Similar checks for the second number / address of the cell
    row_idx = FindDigit(expression, OP_idx+1);
    if (row_idx == -1)
    {
        throw std::runtime_error("invalid cell format: " + expression);
    }
    std::string col2 = expression.substr(OP_idx + 1, row_idx - OP_idx - 1); // row_id = OP_idx + 1 means that there is no letter
    std::string row2 = expression.substr(row_idx);
    if (!IsDigit(row2))
    {
        throw std::runtime_error("invalid cell format: " + expression);
    }
    if (col2.size() != 0 && Row_Indexes.find(row2) == Row_Indexes.end())
    {
        throw std::runtime_error("No such row: " + row2);
    }
    if (Colom_Indexes.find(col2) == Colom_Indexes.end())
    {
        throw std::runtime_error("No such coloum: " + col2);
    }

    // Changing the expression (now instead of letters and numbers separated by a space, the index of the column and row is written 
    // (or just the values of the number, if the column is not specified)
    expression = "= " + Colom_Indexes[col1] + " ";
    if (col1.size() == 0) expression += row1; // if there is no letter => then it's just a number
    else expression += Row_Indexes[row1];
    expression += " " + OP + " " + Colom_Indexes[col2] + " ";
    if (col2.size() == 0) expression += row2; // if there is no letter => then it's just a number
    else expression += Row_Indexes[row2];
}

// indexes for columns
std::map<std::string, std::string> Indexing(std::string coloums)
{
    std::map<std::string, std::string> alphabet;
    std::stringstream ss(coloums);
    std::string cell;
    int count = 0;
    while (std::getline(ss, cell, ','))
    {
        for (const auto& a : cell)
            if (std::isdigit(a) || a == '-')
                throw std::runtime_error("Wrong table-coloums format : " + cell);
        alphabet.insert({ cell, Int_to_string(count) }); // We match each column value with its ordinal number
        ++count;
    }

    // If only a number is specified in the expression, then there will be an empty string instead of a letter, 
    // which we will replace with -1, so that it will be easier to handle this case further
    alphabet.insert(std::pair<std::string, std::string>("", "-1"));
    return alphabet;
}

// indexes for row
std::map<std::string, std::string> Indexing(std::vector<std::vector<std::string>>& data)
{
    std::map<std::string, std::string> alphabet;
    for (int i = 1; i < data.size(); ++i)
    {
        if (!IsDigit(data[i][0]) || data[i][0][0] == '-')
        {
            throw std::runtime_error("Wrong table-row format : " + data[i][0]);
        }
        alphabet.insert({ data[i][0], Int_to_string(i) });
    }
    alphabet.insert(std::pair<std::string, std::string>("", "-1"));
    return alphabet;
}

// We go through all the cells and change the expressions into a more convenient format through the indexes
void CheckForCorrect(std::vector<std::vector<std::string>>& data, std::map<std::string, std::string>& Colom_Indexes, std::map<std::string, std::string>& Row_Indexes)
{
    for (auto& row : data)
    {
        for (auto& expression : row)
        {
            if (expression[0] == '=')
            {
                ChangeExpression(expression, Colom_Indexes, Row_Indexes);
            }
        }
    }
}


// Reading from a csv file and writing the read to a two-dimensional array (bad for memory, but fast access)
std::vector<std::vector<std::string>> Read(const std::string filename)
{
    
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    std::map<std::string, std::string> Colom_Indexes = Indexing(line); // The first row is the column names, you need to number them

    int size = Colom_Indexes.size() - 1; // table width
    std::vector<std::vector<std::string>> data(1, std::vector<std::string>(size));
    data[0] = { line };

    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line); // Convert a string into a stream
        std::string cell;

        int count = 0; 
        while (std::getline(ss, cell, ','))
        {
            ++count;
            if (!IsDigit(cell) && cell[0] != '=')
            {
                throw std::runtime_error("invalid cell format: " + cell);
            }
            row.push_back(cell);
        }

        // checking for the correct number of elements in the line
        if (count != size)
        {
            throw std::runtime_error("Wrong table format: row = " + Int_to_string(count) + " but should be " + Int_to_string(size));
        }
        data.push_back(row);
    }

    std::map<std::string, std::string> Row_Indexes = Indexing(data);
    CheckForCorrect(data, Colom_Indexes, Row_Indexes);

    return data;
}

// Table output
void PRINT(std::vector<std::vector<std::string>>& csv_data)
{
    for (const auto& row : csv_data)
    {
        for (int i = 0; i < row.size()-1; ++i)
        {
            std::cout << row[i] << ",";
        }
        std::cout << row[row.size()-1] << std::endl;
    }
}

// Storing in the form of indexes separated by 
// a space is convenient because you can split them and refer to them
std::vector<int> StringToIntArray(const std::string& input) {
    std::vector<int> result;
    std::istringstream iss(input.substr(2)); // We exclude the equal symbol
    int number;

    while (iss >> number) 
    {
        result.push_back(number);
    }

    return result;
}

// To solve all the expressions in the table, we will solve one until we solve it completely. 
// We will do this recurrently. For very large tables, recursion (as well as a two-dimensional array)
//  may not be suitable, but recursion can always be replaced with a loop, and a two-dimensional array with a two-dimensional list
int RecSolve(std::vector<std::vector<std::string>>& csv_data, int col, int row, std::unordered_set<std::pair<int, int>, PairHash, PairEqual>& Visited)
{
    if (col == -1)
        return row; // if there is no letter, it means it's just a number, not a cell address.

    // There is a cycle in the table - expressions are constructed in such a way  
    // that the expression in the cell leads to the same
    std::pair<int, int> CurPos = { col, row };
    if (Visited.count(CurPos) > 0) 
    {
        throw std::runtime_error("Cycle found ");
    }
    Visited.insert(CurPos); // Adding the visited cell

    // We reveal the following expressions
    if (csv_data[row][col][0] == '=') 
    {
        std::vector<int> A = StringToIntArray(csv_data[row][col]);
        int a = RecSolve(csv_data, A[0], A[1], Visited);
        Visited.erase({ A[0], A[1] }); // Deleting visited vertexes
        int b = RecSolve(csv_data, A[3], A[4], Visited);
        Visited.erase({ A[3], A[4] }); // Deleting visited vertexes
        if (A[2] == PLUS)
            return a + b;
        if (A[2] == MINUS)
            return a - b;
        if (A[2] == MULT)
            return a * b;
        if (A[2] == DIV)
        {
            if (b == 0)
            {
                throw std::runtime_error("division by 0: (" + Int_to_string(row) + ";" + Int_to_string(col) + ")");
            }
            else
            {
                return a / b;
            }
        }
    }

    Visited.erase(CurPos); // Deleting the visited position when exiting recursion

    return String_to_int(csv_data[row][col], 0, csv_data[row][col].size()); // Returning the value in the cell
}


// Wrapper for recursion - we go through all the expressions in the table
void DeepSolve(std::vector<std::vector<std::string>>& csv_data)
{
    for (int i = 0; i < csv_data.size(); ++i)
    {
        for (int j = 0; j < csv_data[i].size(); ++j)
        {
            if (csv_data[i][j][0] == '=')
            {
                std::unordered_set<std::pair<int, int>, PairHash, PairEqual> Visited;
                csv_data[i][j] = Int_to_string(RecSolve(csv_data, j, i, Visited));
            }
        }
    }
}