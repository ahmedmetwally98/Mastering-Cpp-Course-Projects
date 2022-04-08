#include "UsersManager.hpp"

UsersManager::UsersManager()
{
    Users_IDs = 0;
}

bool UsersManager::singUp()
{
    std::string first_name{""};
    std::string last_name{""};
    std::string email{""};
    std::string username{""};
    std::string password{""};
    int anonymous{-1};
    
    std::cout << "Enter first name: ";
    std::cin >> first_name;
    std::cout << "Enter last name: ";
    std::cin >> last_name;
    std::cout << "Enter email: ";
    std::cin >> email;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    std::cout << "Allow anonymous questions (1 or 0)?: ";
    std::cin >> anonymous;

    if (users.count(username)) // the entered username is already exist in the database.
    {
        std::cout << "Invalid username! please try again.\n\n";
        return false;
    }
    
    User user;
    Users_IDs++;
    user.userID = Users_IDs;
    user.first_name = first_name;
    user.last_name = last_name;
    user.email = email;
    user.username = username;
    user.password = password;
    user.anonymous_status = anonymous;

    users.insert({username, user});
    current_user = user;
    Users_IDs++;
    return true;
}

bool UsersManager::login()
{
    std::string username;
    std::string password;
    std::cout << "Enter user name: ";
    std::cin >> username;
    if (!users.count(username))
    {
        std::cout << "Error! this username does NOT exist! please try again.\n\n";
        return false;
    }
    std::cout << "Enter password: ";
    std::cin >> password;

    while (password != users[username].password)
    {
        std::cout << "Invalid password! Try again or press (q) to exit: ";
        std::cin >> password;
        if (password == "q")
        {
            return false;
        }
    }
    current_user = users[username];
    return true;
}

bool UsersManager::LoadUsersDataBase(std::string path)
{
    /**
     * ------------ line in users database consists of: ------------
     * 1- Int value: holds user ID
     * 2- Int value: holds user Anonymous status
     * 2- string value: holds first name
     * 3- string value: holds last name
     * 4- string value: holds email
     * 5- string value: holds username
     * 6- string value: holds password
     */
    users.erase(users.begin(), users.end());
    std::vector<std::string> lines = ReadFile(path);
    if (lines.size() == 0)
    { // file can't be opened or empty file
        return false;
    }
    std::vector<std::string> subStrs;
    User user;
    Users_IDs = lines.size();
    for (auto &line : lines)
    {
        subStrs = SplitString(line, ';');

        user.userID = StrToInt(subStrs[0]);
        user.anonymous_status = StrToInt(subStrs[1]);
        user.first_name = subStrs[2];
        user.last_name = subStrs[3];
        user.email = subStrs[4];
        user.username = subStrs[5];
        user.password = subStrs[6];
        if (!users.count(user.username))
        {
            users.insert({user.username, user});
        }
        else 
        {   // can't load two accounts with the same username
            std::cout << "Error! Two users with the same username!\n\n";
            return false;
        }
    }

    return true;
}

bool UsersManager::UpdateUsersDataBase(const std::string &path, bool app = true)
{
    std::vector<std::string> user_line;
    std::string user_str = "";
    if (app)
    {
        user_str += std::to_string(current_user.userID);
        user_str += ';';
        user_str += std::to_string(current_user.anonymous_status);
        user_str += ";";
        user_str += current_user.first_name;
        user_str += ';';
        user_str += current_user.last_name;
        user_str += ';';
        user_str += current_user.email;
        user_str += ';';
        user_str += current_user.username;
        user_str += ';';
        user_str += current_user.password;
        user_line.push_back(user_str);

        return WriteFileLines(path, user_line, app);
    }
    else
    {
        for (auto &user_item : users)
        {
            user_str += std::to_string(user_item.second.userID);
            user_str += ',';
            user_str += std::to_string(current_user.anonymous_status);
            user_str += ";";
            user_str += user_item.second.first_name;
            user_str += ',';
            user_str += user_item.second.last_name;
            user_str += ',';
            user_str += user_item.second.email;
            user_str += ',';
            user_str += user_item.second.username;
            user_str += ',';
            user_str += user_item.second.password;
            user_line.push_back(user_str);
        }
        return WriteFileLines(path, user_line, app);
    }
}

void UsersManager::printSystemUsers(User &cur_user)
{
    std::cout << "Users:";
    for (auto &user : users)
    {
        std::cout << "\nID: " << user.second.userID
                  << "\t\tName: " << user.second.username;
        if (user.second.username == cur_user.username)
        {
            std::cout << "\t (current User)";
        }
    }
    std::cout << "\n\n";
}
