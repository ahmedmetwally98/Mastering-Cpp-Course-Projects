#ifndef Question_H_
#define Question_H_

#include "HelperFunc.hpp"
#include "User.hpp"

struct Question
{
    int question_ID;
    int question_status;        // -1 for new question Or queston ID for thread 
    std::string question;
    std::string answer;
    bool answer_status;         // 1 for answered question, 0 for Not answered yet
    bool is_anonymous;          // 1 for anonymous and 0 otherwise based on the user who the question is for
    int question_to_userID;
    int question_from_userID;
    std::string question_to_username;
    std::string question_from_username;   

};

#endif