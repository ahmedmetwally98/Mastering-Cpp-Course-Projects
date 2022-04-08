
#ifndef HelperFunc_H_
#define HelperFunc_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <functional>

/***************************helper functions***************************/
std::vector<std::string> ReadFile(const std::string& path);

bool WriteFileLines (const std::string& path, const std::vector<std::string>& lines, bool append);

int CheckIntinRange(int low, int high);

int StrToInt(std::string str);

std::vector<std::string> SplitString(std::string str, char delimiter);

int ShowReadMenu (std::vector<std::string> choices);


#endif