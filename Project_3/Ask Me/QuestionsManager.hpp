#ifndef QuestionsManager_H_
#define QuestionsManager_H_

#include "HelperFunc.hpp"
#include "Question.hpp"
#include "User.hpp"

struct QuestionsManager
{
    int all_questions;
    // STL container where key and value represent the question ID and the Question object respectively
    std::unordered_map<int, Question> questions;       
    // STL container where key and value represent the question ID and the Question -thread- object respectively   
    std::unordered_multimap<int, Question> threads;    

    QuestionsManager();                         // used to initialize all_questions
    
    /**
     * used to load all questions-threads database from "questions.txt" file.
     * return true if the loading done successfuly.
     * return false otherwise.
    */
    bool loadQuestionsDatabase(const std::string& path);

    // used to fill the to_q_IDs list in all users
    void fillUserQuestions(User& user);   

    // used in updating the "questions.txt" database file if a new question/thread is asked
    bool updateQuestionsDatabase(const std::string& path, const Question& q);   
    // used in overwrite 
    bool writeQuestionsDatabase(const std::string& path); 
    std::string combineDatainStr(const Question& _q); 
    bool askQuestion(std::string path, User& cur_user, std::unordered_map<std::string, User>& users); 
    bool answerQuestion(std::string path, User& cur_user);     
    void printQuestion(const Question& _q);    
    void printFeed();
    bool deleteQuestion(std::string path, User& cur_user);
    void printQuestionsToMe(User& cur_user);
    void printQuestionsFromMe(User& cur_user);
};

#endif