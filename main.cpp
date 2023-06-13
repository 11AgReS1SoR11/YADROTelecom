#include "YADROTelecom.h"


int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];

    std::vector<std::vector<std::string>> csv_data;
    try
    {
        csv_data = Read(filename);
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Error!\n" << error.what() << std::endl;
    }

    try
    {
        DeepSolve(csv_data);
        PRINT(csv_data);
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Error!\n" << error.what() << std::endl;
    }
    return 0;
}
