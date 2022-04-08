#include <iostream>

using namespace std;

const int MAX_SPECIALIZATION = 20, MAX_QUEUE = 5;

struct hospital_queue {
    int status[MAX_QUEUE];
    int len;
    int spec;
    string names[MAX_QUEUE];
    
    hospital_queue()
    {
        len = 0;
        spec = -1;
    }

    hospital_queue(int _spec)
    {
        len = 0;
        spec = _spec;
    }

    bool add_end (string name, int st)          // add regular patient
    {
        // each queue should have only 5 patients as maximum.
        if (len == 5)
        {
            return false;
        }
        names[len] = name;
        status[len] = st;
        len++;
        return true;
    }

    bool add_front (string name, int st)        // add urgent patient
    {
        // each queue should have only 5 patients as maximum.
        if (len == 5)
        {
            return false;
        }

        for (int i = len-1; i >= 0; i--)
        {
            names[i+1] = names[i];
            status[i+1] = status[i]; 
        }

        names[0] = name;
        status[0] = st;
        len++;
        return true;
    }

    bool remove_front()
    {
        if (len == 0)
        {
            return false;
        }
        cout << names[0] << " please go with the Dr for specialization " << spec + 1 << "\n\n";
        for (int i = 0; i < len-1; i++)
        {
            names[i] = names[i+1];
            status[i] = status[i+1];
        }
        len--;
        return true;
    }

    int print()
    {
        if (len == 0)
        {
            return 0;
        }

        cout << "There are " << len << " patients in specialization " << spec + 1 << endl;

        for (int i = 0; i < len; i++)
        {
            cout << "name: " << names[i];
            if (status[i])
            {
                cout << " \tstatus: urgent" << endl;
            }
            else
            {
                cout << " \tstatus: regular" << endl;
            }
        }
        return 1;
    }
};

struct hospital_system 
{
    hospital_queue queues[MAX_SPECIALIZATION];

    hospital_system()
    {
        for (int i = 0; i < MAX_SPECIALIZATION; i++)
            queues[i] = hospital_queue {i};
    }

    
    int menu()
    {
        int choice = -1;
        while (choice == -1)
        {
            cout << "Enter your choice\n";
	        cout << "1) Add new patient\n";
	        cout << "2) Print all patient\n";
	        cout << "3) Get next patient\n";
	        cout << "4) Exit\n";

            cin >> choice;
            if (choice > 4 || choice < 1)
                choice = -1;
        }
        return choice;
    }

    void add_patient()
    {
        int spec, st;
        string name;
        bool flag = true;
        cout << "Enter specialization, name and status\n";

        cin >> spec >> name >> st;
        if (st == 0)
        {
            flag = queues[spec-1].add_end(name, st);
        }
        else 
        {
            flag = queues[spec-1].add_front(name, st);
        }
        if (flag)
        {
            cout << "Patient is add successfuly\n\n";
        }
        else
            cout << "Sorry, we can't add more patients for specialization " << spec << "\n\n";
    }

    void print_patients()
    {
        cout << "****************************************\n";
        int print_counter = 0;
        for (int i = 0; i < MAX_SPECIALIZATION; i++)
        {
            print_counter = print_counter + queues[i].print();
        }

        if (!print_counter)
        {
            cout << "No patient on the hospital system now!.\n";
        }
        cout << "\n";
    }

    void get_new_patient()
    {
        bool flag  = true;
        int spec;
        cout << "Enter specialization: ";
        cin >> spec;
        flag = queues[spec-1].remove_front();
        if (!flag)
        {
            cout << "No patients at the moment. Have a rest Dr.\n\n";
        }
    }

    void run ()
    {
        while (true)
        {
            int choice = menu();

            if (choice == 1)
            {
                add_patient();
            }
            else if (choice == 2)
            {
                print_patients();
            }
            else if (choice == 3)
            {
                get_new_patient();
            }
            else if (choice == 4)
            {
                break;
            }
        }
    }

};

int main()
{
    hospital_system hospital;
    hospital.run();

    return 0;
}