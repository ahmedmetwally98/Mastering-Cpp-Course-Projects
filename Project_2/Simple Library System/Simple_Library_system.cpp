#include <iostream>
#include <algorithm>

using namespace std;

const int MAX_USERS = 1000;
const int MAX_BOOKS_NUMBERS = 10000;
const int MAX_ALLOWED_NUMBER = 10;

struct User_system
{
    string user_name;
    long national_id;
    short borrowed_books;
    string book_name[MAX_ALLOWED_NUMBER];
    short book_ID[MAX_ALLOWED_NUMBER];

    User_system()
    {
        borrowed_books = 0;
        for (short i = 0; i<MAX_ALLOWED_NUMBER; i++)
        {
            book_name[i] = "";
            book_ID[i] = -1;
        }
    }

    User_system(string name, long n_ID)
    {
        borrowed_books = 0;
        user_name = name;
        national_id = n_ID;
        for (short i = 0; i<MAX_ALLOWED_NUMBER; i++)
        {
            book_name[i] = "";
            book_ID[i] = -1;
        }
    }

    void add_user(string n, long n_ID)
    {
        user_name = n;
        national_id = n_ID;
    }

    short user_borrowed_book(string book_n, long book_id)
    {
        if (borrowed_books >= MAX_ALLOWED_NUMBER)
        {
            // can't borrow book as user has the max allowed number of borrowed books
            return 0;
        }
        for (short i = 0; i < borrowed_books; i++)
        {
            if (book_n == book_name[i])
            {
                // can't borrow the same book twice for the same user
                return -1;
            }
        }

        book_name[borrowed_books] = book_n;
        book_ID[borrowed_books] = book_id;
        borrowed_books++;
        return 1;
    }

    bool user_return_book(string book_n)
    {
        if (borrowed_books > 0)
        {
            for (short i = 0; i<borrowed_books; i++)      // search for the book inside user books
            {
                if (book_name[i] == book_n)
                {   // let's shift the array to delet this book.
                    for (short j = i+1; j<borrowed_books; j++)
                    {
                        book_name[j-1] = book_name[j];
                        book_ID[j-1] = book_ID[j];
                    }
                    borrowed_books--;
                    return true;
                }
            }
        }
        return false;
    }

    void print_user()
    {
        cout << "User name: " << user_name
            << "\tNational ID: " << national_id
            << "\ttotal borrowed book = " << borrowed_books;
        if (borrowed_books > 0)
        {
            sort (book_ID, book_ID + borrowed_books);
            cout << " --> Books name and IDs: ";
            for (short i = 0; i < borrowed_books; i++)
            {
                cout << book_name[i] << "->" << book_ID[i] << " ";
            }
        }
        cout << endl;
    }

    bool  print_user_borrowed_book (string book_n)
    {
        if (borrowed_books > 0)
        {
            for (short i = 0; i < borrowed_books; i++)
            {
                if (book_n == book_name[i])
                {
                    cout << user_name << " " << national_id << endl;
                    return true;
                }
            }
        }
        return false;
    }
};
/**************************************************************************************************/
struct Book_system
{
    string book_name;
    string user_name;
    short book_id;
    short total_copies;
    short total_borrowed;

    Book_system()
    {
        total_borrowed = 0;
        total_copies = 0;
    }

    Book_system(string b_name, long b_id, short tot_c)
    {
        book_name = b_name;
        book_id = b_id;
        total_copies = tot_c;
        total_borrowed = 0;
    }

    void add_book(string b_name, long b_id, short tot_c)
    {
        book_name = b_name;
        book_id = b_id;
        total_copies += tot_c;
    }

    bool borrow_book()
    {
        if (total_copies > 0)
        {
            total_copies--;
            total_borrowed++;
            return true;
        }
        return false;           // this book is not on the system at the moment
    }

    bool return_book()
    {
        if (total_borrowed > 0)
        {
            total_copies++;
            total_borrowed--;
            return true;
        }
        return false;       // all copies of this book exist in the library
    }

    bool has_prefix(string prefix)
    {
        bool flag = true;
        for (short i = 0; i < (int) prefix.size(); i++)
        {
            if (book_name[i] != prefix[i])
            {
                flag = false;
                break;
            }
        }

        if (flag)
            return true;
        else
            return false;
    }
};

/*****************************************************************************************************/
bool sort_by_id(Book_system &book1, Book_system &book2)
{
    return book1.book_id < book2.book_id;
}

bool sort_by_name(Book_system &book1, Book_system &book2)
{
    return book1.book_name < book2.book_name;
}

bool sort_users_by_id (User_system &user1, User_system &user2)
{
    return user1.national_id < user2.national_id;
}
/*****************************************************************************************************/

struct Library_system
{
    User_system users[MAX_USERS];
    Book_system books[MAX_BOOKS_NUMBERS];

    long total_users;
    long total_books;

    Library_system()
    {
        total_users = 0;
        total_books = 0;
    }

    void add_book_to_system()
    {
        if (total_books >= MAX_BOOKS_NUMBERS)
        {
            cout << "Can't add new book. The system is complete!\n\n"; 
            return;
        }
        else
        {
            string b_name;
            long b_id;
            long total_c;

            cout << "Enter book info.\n";
            cout << "Book id: ";
            cin >> b_id;
            cout << "Book name: ";
            cin >> b_name;
            cout << "Total copies: ";
            cin >> total_c;

            
            for (long i = 0; i < total_books; i++)
            {
                if (books[i].book_name == b_name && books[i].book_id != b_id)
                {
                    cout << "Error! This name has been assigned to another book.\n\n";
                    return;
                }
                if (books[i].book_name != b_name && books[i].book_id == b_id)
                {
                    cout << "Error! This ID has been assigned for another book.\n\n";
                    return;
                }
                if (books[i].book_name == b_name && books[i].book_id == b_id)   // increaseing the copies of existing book
                {
                    books[i].add_book(b_name, b_id, total_c);
                    cout << endl;
                    return;
                }
            }
            books[total_books].add_book(b_name, b_id, total_c);
            total_books++;
            cout << endl;
        }
    }

    void search_book_prefix()
    {
        string book_prefix;
        cout << "Enter book name prefix: ";
        cin >> book_prefix;
        long counter = 0;

        sort (books, books + total_books, sort_by_name);
        for (int i = 0; i < total_books; i++)
        {
            if (books[i].has_prefix(book_prefix))
            {
                cout << books[i].book_name << "\n";
                counter++;
            }
        }    

        if (counter == 0)
        {
            cout << "Error! No books with such prefix.\n";
        }
        cout << endl;
    }

    void print_who_borrowed_book_by_name()
    {
        string b_name;
        cout << "Enter the book name: ";
        cin >> b_name;
        
        long b_id_idx = search_id_idx(b_name);

        if (b_id_idx == -1)
        {
            cout << "Error! This book does not exist in the library\n\n";
            return;
        }

        long counter = 0;

        for (long i = 0; i<total_users; i++)
        {
            counter = counter + users[i].print_user_borrowed_book(b_name);
        }
        if(counter == 0)
        {
            cout << "Error! This book is not borrowed to any user!\n\n";
            return;
        }
        cout << endl;
    }

    void print_library_by_ID()
    {
        if (total_books == 0)
        {
            cout << "Error! There are no books on the system at the moment!\n\n";
            return;
        }
        sort(books, books + total_books, sort_by_id);
        for (long i = 0; i < total_books; i++)
        {
            cout << "book ID = " << books[i].book_id 
                << " Book name: " << books[i].book_name
                << " Total quantities: " << books[i].total_copies
                << " Total borrowed: " << books[i].total_borrowed
                << endl;
        }
        cout << endl;
    }

    void print_library_by_name()
    {
        if (total_books == 0)
        {
            cout << "Error! There are no books on the system at the moment!\n\n";
            return;
        }
        sort(books, books + total_books, sort_by_name);
        for (long i = 0; i < total_books; i++)
        {
            cout << "book ID = " << books[i].book_id 
                << " Book name: " << books[i].book_name
                << " Total quantities: " << books[i].total_copies
                << " Total borrowed: " << books[i].total_borrowed
                << endl;
        }
        cout << endl;
    }

    void add_user_to_system()
    {
        if (total_users >= MAX_USERS)
        {
            cout << "Error! Can't add new user to the system. the system is complete.\n\n";
           return;
        }
        else
        {
            string user_n;
            long national_id;
            cout << "Enter user info.\n";
            cout << "User name: ";
            cin >> user_n;
            cout << "User national ID: ";
            cin >> national_id;

            for (long i = 0; i<total_users; i++)
            {
                if (users[i].national_id == national_id && users[i].user_name == user_n)
                {
                    cout << "Error! Can't add the user. The user already exist!\n\n";
                    return;
                }
                if (users[i].national_id == national_id && users[i].user_name != user_n)
                {
                    cout << "Error! Can't add two users with the same ID.\n\n";
                    return;
                }
            }
            users[total_users].add_user(user_n, national_id);
            total_users++;
        }   
        cout << endl;
    }

    int search_id_idx(string& name, long user_id = 0, char type = 'b')
    {
        // searching using binary search algorithm
        long high, low, mid;
        high = total_books - 1;
        low = 0;
        
        if (type == 'b')
        { // search for the book
            sort (books, books + total_books, sort_by_name);
            while(low <= high)
            {
                mid = (high + low) / 2;
                if (books[mid].book_name == name)
                {
                    return mid;
                }
                else if (books[mid].book_name > name)
                {
                    high = mid - 1;
                }
                else
                {
                    low = mid + 1;
                }
            }
            return -1;
        }
        else
        {   // search for the user
            sort (users, users + total_users, sort_users_by_id);
            while(low <= high)
            {
                mid = (high + low) / 2;
                if (users[mid].national_id == user_id)
                {
                    return mid;
                }
                else if (users[mid].national_id > user_id)
                {
                    high = mid - 1;
                }
                else
                {
                    low = mid + 1;
                }
            }
            return -1;
        }
    }

    void user_borrow_book()
    {
        string u_name, b_name;
        long n_ID;
        bool flag = false;

        cout << "Enter user name and natioanl ID: ";
        cin >> u_name >> n_ID;

        long u_id_idx = search_id_idx(u_name, n_ID, 'u');

        if (u_id_idx == -1)
        {
            cout << "Error! This user does not exist on the library system!\n\n";
            return; 
        }

        cout << "Enter Book name: ";
        cin >> b_name;

        long b_id_idx = search_id_idx(b_name);
        long b_id = books[b_id_idx].book_id;

        if (b_id_idx == -1)
        {
            cout << "Error! This book does not exist on the library system!\n\n";
            return;
        }

        {   // borrow_flag is a scope variable
            short borrow_flag = users[u_id_idx].user_borrowed_book(b_name, b_id);
            if (borrow_flag == 0)
            {
                cout << "Error! Can't borrow this book now. you exceed the maximum number!\n\n";
                return;
            }
            else if (borrow_flag == -1)
            {
                cout << "Error! Can't borrow this book. you already have this one!\n\n";
                return;
            }
            // end of borrow_flag life 
        }

        if (!books[b_id_idx].borrow_book())
        {   // ensure that there are copies of the book
            cout << "Error! Can't borrow this book now. There are no copies of this book\n\n";
            flag = users[u_id_idx].user_return_book(b_name);
            return;
        }  
        cout << endl;
        return;
    }

    void user_return_book()
    {
        string u_name, b_name;
        long national_id;
        bool flag_user = false;

        cout << "Enter user name and national ID: ";
        cin >> u_name >> national_id;

        long u_id_idx = search_id_idx(u_name, national_id, 'u');

        if (u_id_idx == -1)
        {
            cout << "Error! This user does not exist on the library system!\n\n";
            return; 
        }

        cout << "Enter book name: ";
        cin >> b_name;

        long b_id_idx = search_id_idx(b_name);
        if (b_id_idx == -1)
        {
            cout << "Error! This book is not exist on the library system!\n\n";
            return;
        }

        if (!books[b_id_idx].return_book())
        {   // ensure that this book is borrowed to any user
            cout << "Error! This book is not borrowed to any user yet!\n\n";
            return;
        }
        if(!users[u_id_idx].user_return_book(b_name))
        {
            cout << "Error! This book is not with this user!\n\n";
            flag_user = books[b_id_idx].borrow_book();     // increase the book borrowed number again
            return;
        }
        cout << endl;
    }

    void print_users()
    {
        if (total_users == 0)
        {
            cout << "There are no users at the moment on the system!\n\n";
            return;
        }
        for (long i = 0; i<total_users; i++)
        {
            users[i].print_user();
        }
        cout << endl;
    }

    short menu()
    {
        short choice = -1;
        while(choice == -1)
        {
            cout << "Library Menu:\n";
            cout << "1) Add book\n";
            cout << "2) Search books by prefix\n";
            cout << "3) Print how borrowed books by name\n";
            cout << "4) Print library by ID\n";
            cout << "5) Print library by name\n";
            cout << "6) Add user\n";
            cout << "7) User borrow book\n";
            cout << "8) User return book\n";
            cout << "9) Print users\n";
            cout << "10) Exit\n";

            cin >> choice;
            if (choice > 10 || choice < 1)
            {
                cout << "Invalid choice! Try again\n\n";
                choice == -1;       // loop again
            }
        }
        return choice;
    }

    void run()
    {
        while (true)
        {
            short choice = menu();
        
            if (choice == 10)
            {
                break;
            }

            switch(choice)
            {
                case 1: add_book_to_system();
                    break;
                case 2: search_book_prefix();
                    break;
                case 3: print_who_borrowed_book_by_name();
                    break;
                case 4: print_library_by_ID();
                    break;
                case 5: print_library_by_name();
                    break;
                case 6: add_user_to_system();
                    break;
                case 7: user_borrow_book();
                    break;
                case 8: user_return_book();
                    break;
                case 9: print_users();
                    break;
        }
        }
    }

};

int main()
{
    Library_system library;
    library.run();
    return 0;
}