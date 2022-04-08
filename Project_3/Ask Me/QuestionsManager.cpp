#include "QuestionsManager.hpp"

QuestionsManager::QuestionsManager()
{
    all_questions = 0;
}

bool QuestionsManager::loadQuestionsDatabase(const std::string &path)
{
    /**
     * Question line in database consists of:
     * 1- Int holds: -1 for new question or Question ID for thread
     * 2- Int holds: question anonymous status
     * 3- Int holds: question ID
     * 4- Int holds: user ID who ask the question
     * 5- Int holds: user ID who the question is for
     * 6- Int holds: if the question is answered or not
     * 7- string holds: username who ask question
     * 8- string holds: username who question is for
     * 9- string holds: question
     * 10- string holds: answer if exist (based on the answer status)
     */
    questions.erase(questions.begin(), questions.end());
    threads.erase(threads.begin(), threads.end());
    std::vector<std::string> lines;
    std::vector<std::string> line;
    lines = ReadFile(path);
    all_questions = lines.size();
    if (lines.size() == 0)
    { // file can't be opened or empty file
        std::cout << "Question database is empty!\n";
        return false;
    }
    Question q_obj;

    for (auto &question_line : lines)
    {
        line = SplitString(question_line, ';');
        if (!(line.size() >= 9 && line.size() <= 10))
        {
            // ignore corrupted lines (line size should be 9 if question is answered or 8 if not)
            continue;
        }
        q_obj.question_status = StrToInt(line[0]);
        q_obj.is_anonymous = StrToInt(line[1]);
        q_obj.question_ID = StrToInt(line[2]);
        q_obj.question_from_userID = StrToInt(line[3]);
        q_obj.question_to_userID = StrToInt(line[4]);
        q_obj.answer_status = StrToInt(line[5]);
        q_obj.question_from_username = line[6];
        q_obj.question_to_username = line[7];
        q_obj.question = line[8];
        if (q_obj.answer_status)
            q_obj.answer = line[9];
        if (q_obj.question_status == -1)
        {
            questions[q_obj.question_ID] = q_obj;
        }
        else
        {
            threads.insert({q_obj.question_status, q_obj});
        }
    }
    return true;
}

void QuestionsManager::fillUserQuestions(User &user)
{
    for (auto &_question : questions)
    {
        if (_question.second.question_to_userID == user.userID)
        {
            user.to_q_IDs.push_back(_question.second.question_ID);
        }
    }
    for (auto &_thread : threads)
    {
        if (_thread.second.question_to_userID == user.userID)
        {
            user.to_q_IDs.push_back(_thread.second.question_ID);
        }
    }
}

std::string QuestionsManager::combineDatainStr(const Question& q_obj)
{
    std::string str{""};
    str += std::to_string(q_obj.question_status);
    str += ";";
    str += std::to_string(q_obj.is_anonymous);
    str += ";";
    str += std::to_string(q_obj.question_ID);
    str += ";";
    str += std::to_string(q_obj.question_from_userID);
    str += ";";
    str += std::to_string(q_obj.question_to_userID);
    str += ";";
    str += std::to_string(q_obj.answer_status);
    str += ";";
    str += q_obj.question_from_username;
    str += ";";
    str += q_obj.question_to_username;
    str += ";";
    str += q_obj.question;
    if (q_obj.answer_status)
    {
        str += ";";
        str += q_obj.answer;
    }

    return str;
}

bool QuestionsManager::updateQuestionsDatabase(const std::string &path, const Question& q_obj)
{
    // used for adding a new question whether it is a question or thread
    std::vector<std::string> question_line;
    std::string q_str = combineDatainStr(q_obj);

    question_line.push_back(q_str);
    return WriteFileLines(path, question_line, true);
}

bool QuestionsManager::writeQuestionsDatabase(const std::string &path)
{
    std::vector<std::string> all_lines;
    std::string packet_str = "";
    all_lines.reserve(50);                      //used for optimizing vector's operations
    for (auto &question : questions)
    {
        packet_str = "";
        packet_str += combineDatainStr(question.second);
        all_lines.push_back(packet_str);
    }
    for (auto &thread : threads)
    {
        packet_str = "";
        packet_str += combineDatainStr(thread.second);
        all_lines.push_back(packet_str);
    }

    return WriteFileLines(path, all_lines, false);
}

bool QuestionsManager::askQuestion(std::string path, User &cur_user, std::unordered_map<std::string, User> &users)
{
    path += "questions.txt";
    std::string user_name{""};
    std::cout << "Enter username or (q) to eixt: ";
    std::cin >> user_name;
    if (user_name == "q" || user_name == cur_user.username)
    { // user decides to not asking now
        return false;
    }
    // checking username validity
    if (!users.count(user_name))
    { // the user whose question is for, not exist on the system
        std::cout << "this username ( " << user_name << " ) is not exist on the system!\n\n";
        return false;
    }
    int q_status{0};
    std::cout << "For a new Question enter -1, "
              << "For a Thread enter question ID: ";
    std::cin >> q_status;
    if (q_status != -1)
    {
        // checking question validity
        if (!questions.count(q_status))
        {
            std::cout << "Error! no question exists with ID: " << q_status << "\n\n";
            return false;
        }
    }
    Question q_;
    all_questions++;
    std::getline(std::cin, q_.question);
    std::cout << "Enter question text: ";
    std::getline(std::cin, q_.question);
    q_.question_status = q_status;
    // we initialize the question ID based on the user ID
    q_.question_ID = cur_user.userID * 10 + all_questions;
    q_.question_from_userID = cur_user.userID;
    q_.question_from_username = cur_user.username;
    q_.question_to_userID = users[user_name].userID;
    q_.question_to_username = users[user_name].username;
    q_.answer_status = 0;
    q_.is_anonymous = users[user_name].anonymous_status;
    users[user_name].to_q_IDs.push_back(q_.question_ID);

    if (q_status != -1)
    {
        threads.insert({q_status, q_});
    }
    else
    {
        questions[q_.question_ID] = q_;
    }
    all_questions++;
    std::cout << std::endl;
    return updateQuestionsDatabase(path, q_);
}

void QuestionsManager::printQuestion(const Question &q_obj)
{
    if (q_obj.question_status == -1)
    {
        std::cout << "Question ID (" << q_obj.question_ID << "): ";

        if (q_obj.is_anonymous)
        {
            std::cout << "To -> user: (" << q_obj.question_to_userID << ") " << q_obj.question_to_username;
            std::cout << "\nQuestion: " << q_obj.question;
        }
        else
        {
            std::cout << "From user: (" << q_obj.question_from_userID << ") " << q_obj.question_from_username;
            std::cout << "\tTo -> user: (" << q_obj.question_to_userID << ") " << q_obj.question_to_username;
            std::cout << "\nQuestion: " << q_obj.question;
        }
        if (q_obj.answer_status)
        {
            std::cout << "\nAnswer: " << q_obj.answer << "\n\n";
        }
        else
        {
            std::cout << "\nNot answered yet!\n\n";
        }
    }
    else
    {
        std::cout << "\tQ ID (" << q_obj.question_status
                  << ") -> Thread ID (" << q_obj.question_ID << "): ";
        
        if (q_obj.is_anonymous)
        {
            std::cout << "To -> user: (" << q_obj.question_to_userID << ") " << q_obj.question_to_username;
            std::cout << "\n\tQuestion: " << q_obj.question;
        }
        else
        {
            std::cout << "From user: (" << q_obj.question_from_userID << ") " << q_obj.question_from_username;
            std::cout << "\tTo -> user: (" << q_obj.question_to_userID << ") " << q_obj.question_to_username;
            std::cout << "\n\tQuestion: " << q_obj.question;
        }
        if (q_obj.answer_status)
        {
            std::cout << "\n\tAnswer: " << q_obj.answer << "\n\n";
        }
        else
        {
            std::cout << "\n\tNot answered yet!\n\n";
        }
    }
}

bool QuestionsManager::answerQuestion(std::string path, User &cur_user)
{
    path += "questions.txt";
    int q_ID{-1};
    std::cout << "Enter question ID or -1 to exit: ";
    std::cin >> q_ID;
    if (q_ID == -1)
    {
        std::cout << std::endl;
        return false;
    }

    bool thread_flag = false;
    auto iter = questions.find(q_ID);
    // check if the question is exist or not in the database
    if (iter == questions.end())
    {
        for (auto &t : threads)
        {
            if (q_ID == t.second.question_ID)
            {
                thread_flag = true;
                auto t_iter = threads.equal_range(t.first); // return pair of iterators (to begin and end of range)
                for (auto it = t_iter.first; it != t_iter.second; it++)
                {
                    if (it->second.question_ID == q_ID)
                    {
                        iter = it;
                        break;
                    }
                }
                break;
            }
        }
        if (!thread_flag)
        {
            std::cout << "Question with ID ( " << q_ID << " ) is not exist!\n\n";
            return false;
        }
    }
    // check if user can answer this question or not
    if (std::find(cur_user.to_q_IDs.begin(), cur_user.to_q_IDs.end(), q_ID) == cur_user.to_q_IDs.end())
    {
        std::cout << "Error! you can't answer this question!\n\n";
        return false;
    }

    printQuestion(iter->second);
    if (iter->second.answer_status)
    {
        std::cout << "Warning: Answer will be updated\n";
    }
    std::getline(std::cin, iter->second.answer);
    std::cout << "Enter answer: ";
    std::getline(std::cin, iter->second.answer);

    iter->second.answer_status = 1;
    std::cout << std::endl;
    return writeQuestionsDatabase(path);
}

bool QuestionsManager::deleteQuestion(std::string path, User &cur_user)
{
    path += "questions.txt";
    int q_ID{-1};
    std::cout << "Enter question ID or -1 to cancel: ";
    std::cin >> q_ID;
    if (q_ID == -1)
    { // user decides to cancel
        std::cout << std::endl;
        return false;
    }

    auto iter = questions.find(q_ID);
    // search question in questions and threads database
    if (iter == questions.end())
    {
        for (auto &t : threads)
        {
            if (t.second.question_ID == q_ID)
            {
                auto t_iter = threads.equal_range(t.first);
                for (auto it = t_iter.first; it != t_iter.second; it++)
                {
                    if (it->second.question_ID == q_ID)
                    {
                        threads.erase(it);
                        return writeQuestionsDatabase(path);
                    }
                }
            }
        }
        std::cout << "Error! question with ID: (" << q_ID << ") does NOT exist";
        return false;
    }
    // check if user can answer this question or not
    if (std::find(cur_user.to_q_IDs.begin(), cur_user.to_q_IDs.end(), q_ID) == cur_user.to_q_IDs.end())
    {
        std::cout << "Error! you have no access to this question!\n\n";
        return false;
    }
    else
    {
        cur_user.to_q_IDs.remove(q_ID);
    }

    questions.erase(iter);                          // delete question
    auto t_iter = threads.equal_range(iter->first); // delete all possible threads
    if (t_iter.first != t_iter.second)
        threads.erase(t_iter.first, t_iter.second);

    std::cout << std::endl;
    return writeQuestionsDatabase(path);
}

void QuestionsManager::printFeed()
{
    std::cout << std::endl;
    for (auto &_q : questions)
    {
        printQuestion(_q.second);
        auto iter = threads.equal_range(_q.first);
        for (auto &it = iter.first; it != iter.second; it++)
        {
            printQuestion(it->second);
        }
    }
}

void QuestionsManager::printQuestionsFromMe(User &cur_user)
{
    std::cout << '\n';
    for (auto &q : questions)
    {
        if (q.second.question_from_userID == cur_user.userID)
        {
            printQuestion(q.second);
        }
    }
    for (auto &t : threads)
    {
        if (t.second.question_from_userID == cur_user.userID)
        {
            printQuestion(t.second);
        }
    }
}

void QuestionsManager::printQuestionsToMe(User &cur_user)
{
    std::cout << '\n';
    for (auto &q : questions)
    {
        if (q.second.question_to_userID == cur_user.userID)
        {
            printQuestion(q.second);
            auto t_iter = threads.equal_range(q.first);
            for (auto it = t_iter.first; it != t_iter.second; it++)
            {
                printQuestion(it->second);
            }
        }
    }
    for (auto &t : threads)
    {
        if ((t.second.question_to_userID == cur_user.userID) && 
        (std::find(cur_user.to_q_IDs.begin(), cur_user.to_q_IDs.end(), t.first) == cur_user.to_q_IDs.end()))
        {
            printQuestion(t.second);
        }
    }
}
