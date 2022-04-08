#ifndef User_H_
#define User_H_

#include "HelperFunc.hpp"
#include <list>

struct User
{
    int userID;
    std::string email;
    std::string password;
    std::string first_name;
    std::string last_name;
    std::string username;
    bool anonymous_status;      // 1 for allowing anonymous questions and 0 otherwise
    std::list<int> to_q_IDs;
};

#endif