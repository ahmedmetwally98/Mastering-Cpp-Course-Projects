#ifndef UsersManager_H_
#define UsersManager_H_

#include "HelperFunc.hpp"
#include "User.hpp"


struct UsersManager
{
    int Users_IDs;
    std::unordered_map<std::string, User> users;    // username, user_object
    User current_user;

    UsersManager();         // used to initialize the Users_ID value.
    bool LoadUsersDataBase(const std::string path);
    bool UpdateUsersDataBase(const std::string& path, bool append);
    void printSystemUsers(User& );
    bool login();
    bool singUp();          // here we generate the User ID

};

#endif