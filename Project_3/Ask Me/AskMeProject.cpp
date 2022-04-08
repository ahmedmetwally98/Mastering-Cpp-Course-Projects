#include <bits/stdc++.h>
#include "Question.hpp"
#include "User.hpp"
#include "QuestionsManager.hpp"
#include "UsersManager.hpp"
#include "HelperFunc.hpp"

using namespace std;

struct AskMeProject
{
private:
    UsersManager users_manager;
    QuestionsManager question_manager;
    std::string folderPath;
    bool loading, writing;
    bool log_flag;

public:
    AskMeProject(const std::string path = "")
    {
        folderPath = path;
        writing = false;
        loading = false;
        log_flag = false;
    }

    bool loadDataBase(std::string path, char condition)
    {
        if (condition == 'u')
        {
            path += "users.txt";
            loading = users_manager.LoadUsersDataBase(path);
        }
        else
        {
            path += "questions.txt";
            loading = question_manager.loadQuestionsDatabase(path);
        }
        return loading;
    }

    bool updateDatabase(std::string path, char condition, bool append = true)
    {
        if (condition == 'u')
        {
            path += "users.txt";
            writing = users_manager.UpdateUsersDataBase(path, append);
        }
        else
        {
            path += "questions.txt";
            writing = question_manager.writeQuestionsDatabase(path);
        }
        return writing;
    }

    void run()
    {
        if (folderPath.size() == 0) // if path was not passed to the constructor
        {
            std::cout << "Please enter folder path to keep updates.";
            std::cin >> folderPath;
        }
        std::vector<std::string> menu1{"Login", "Sign Up", "Exit Program"};
        std::vector<std::string> menu2{"Print Questions To me.",
                                       "Print Questons From me.",
                                       "Ask Question.",
                                       "Answer Question.",
                                       "Delete Question.",
                                       "List System Users.",
                                       "Feed.",
                                       "Logout."};

        int choice{-1};
        while (true)
        {
            std::cout << std::endl;
            choice = ShowReadMenu(menu1);
            if (choice == 1)
            {
                loading = loadDataBase(folderPath, 'u');
                if (!loading)
                {
                    std::cout << "Users database is empty!\nPlease sign up first.\n\n";
                    continue;
                }
                loading = loadDataBase(folderPath, 'q');
                if (loading)    // to avoid unnecessary calling
                {   // using structure beinding in a range base for loop
                    for (auto& [username, user] : users_manager.users)
                    {
                        question_manager.fillUserQuestions(user);
                    }
                }
                log_flag = users_manager.login();
                if (!log_flag)
                {
                    continue;
                }
                std::cout << "login completed successfully!\n\n";
                break;
            }
            else if (choice == 2)
            {
                loading = loadDataBase(folderPath, 'u');
                loading = loadDataBase(folderPath, 'q');
                
                if (loading)    // to avoid unnecessary calling
                {   // using structure beinding in a range base for loop
                    for (auto& [username, user] : users_manager.users)
                    {
                        question_manager.fillUserQuestions(user);
                    }
                }
                log_flag = users_manager.singUp();
                if (log_flag)
                {
                    std::cout << "Sign up successfully!\n\n";
                    updateDatabase(folderPath, 'u');
                    break;
                }
                else
                {
                    continue;
                }
            }
            else if (choice == 3)
            {
                std::cout << "Program exits successfully.\n\n";
                return;
            }
        }

        while (true)
        {
            choice = ShowReadMenu(menu2);
            if (choice == 8)
            {
                break;
            }
            loadDataBase(folderPath, 'q');
            switch (choice)
            {
            case 1:
                question_manager.printQuestionsToMe(users_manager.current_user);
                break;
            case 2:
                question_manager.printQuestionsFromMe(users_manager.current_user);
                break;
            case 3:
                question_manager.askQuestion(folderPath, users_manager.current_user, users_manager.users);
                break;
            case 4:
                question_manager.answerQuestion(folderPath, users_manager.current_user);
                break;
            case 5:
                question_manager.deleteQuestion(folderPath, users_manager.current_user);
                break;
            case 6:
                users_manager.printSystemUsers(users_manager.current_user);
                break;
            case 7:
                question_manager.printFeed();
                break;
            }
        }
        // logout
        log_flag = false;
        run();
    }
};

int main()
{
    AskMeProject Service("Database_folder\\");
    Service.run();

    return 0;
}