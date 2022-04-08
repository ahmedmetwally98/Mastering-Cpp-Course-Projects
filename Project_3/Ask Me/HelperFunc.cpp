#include "HelperFunc.hpp"

/***************helper functions***************/
std::vector<std::string> ReadFile(const std::string& path)
{
    std::vector<std::string> lines;
    std::ifstream ifile;
    ifile.open(path);
    if (ifile.fail())
    {
        //std::cout << "\nCan't open file to read data!\n";
        return lines;
    }

    std::string line;
    while (getline(ifile, line))
    {
        // ignore empty lines
        if (line.size() == 0)
        {
            continue;
        }
        lines.push_back(line);
    }
    ifile.close();
    return lines;
}

bool WriteFileLines (const std::string& path, const std::vector<std::string>& lines, bool append = true)
{
    std::ofstream ofile;
    if (append)
        ofile.open(path, std::ios::out | std::ios::app);
    else
        ofile.open(path, std::ios::out | std::ios::trunc);
    if (ofile.fail())
    {
        //std::cout << "\nCan't open file to write data!\n";
        return false;
    }

    for (auto& str : lines)
    {
        ofile << str << '\n';
    }
    ofile.close();
    return true;
}

int StrToInt(std::string str)
{
    int num {0};
    std::istringstream iss(str);
    iss >> num;
    return num;
}

int CheckIntinRange(int low, int high)
{
    int num{0};
    std::cout << "Enter number between [ " << low << " - " << high << " ]: ";
    std::cin >> num;

    if (num >= low && num <= high)
    {
        return num;
    }

    std::cout << "Invalid number!.. please try again\n";
    return CheckIntinRange(low, high);
}

std::vector<std::string> SplitString(std::string str, char delimiter = ';')
{
    std::vector<std::string> substrs;
    std::string s = "";
    for (auto& ch : str)
    {
        if (ch != delimiter)
        {
            s += ch;
        }
        else
        {
            substrs.push_back(s);
            s = "";
        }
    }
    substrs.push_back(s);
    return substrs;
}

int ShowReadMenu (std::vector<std::string> choices)
{
    std::cout << "Menu:\n";
    int i = 1;
    for(auto& c : choices)
    {
        std::cout << "\t\t" << i << "- " << c << '\n';
        ++i;
    }    
    return CheckIntinRange(1, (int) choices.size());
}
