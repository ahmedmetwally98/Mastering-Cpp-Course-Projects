/*
 * proj1.cpp
 *
 *  Created on: Oct 12, 2021
 *      Author: ahmed
 */

#include <iostream>

using namespace std;

const int MAX_SPECIALIZATION = 20, MAX_QUEUE = 5;
int arr2d[MAX_SPECIALIZATION][MAX_QUEUE];
string names[MAX_SPECIALIZATION][MAX_QUEUE];
int count_patient[MAX_SPECIALIZATION] = {0};

void print_menu()
{
	cout << "Enter your choice\n";
	cout << "1) Add new patient\n";
	cout << "2) Print all patient\n";
	cout << "3) Get next patient\n";
	cout << "4) Exit\n";
}

void shifting_right(int spec, int status, string name)
{
	if (count_patient[spec-1] == 0)
	{
		names[spec - 1][0] = name;
		arr2d[spec - 1][0] = status;
	}
	else
	{
		for (int i = MAX_QUEUE - 2; i >= 0; i--)							// searching for the last patient 
		{
			if (arr2d[spec - 1][i] == 0 || arr2d[spec - 1][i] == 1)		
			{
				for (int j = i; j >= 0; j--)						// shifting all patients to add the urgent patient at the begin
				{
					names[spec - 1][j + 1] = names[spec - 1][j];
					arr2d[spec - 1][j + 1] = arr2d[spec - 1][j];
				}
				break;
			}
		}
	}
}

void addNewPatient()
{
	int spec, status;
	string name;
	cout << "Enter specialization, name, status: " << endl;
	cin >> spec >> name >> status;
	
	// Inputs validation.
	if (spec > 20 || spec < 1)
	{
		cout << "invalid specialization.\n\n";
		return;
	}
	if (!(status == 0 || status == 1))
	{
		cout << "invalid status.\n\n";
		return;
	}

	if (count_patient[spec-1] == 5)
	{
		cout << "Sorry we can't add more patients for this specialization.\n\n";
		return;
	}

	if (status == 1)		// adding urgent patient at the very begin of the query.
	{
		shifting_right(spec, status, name);
		names[spec - 1][0] = name;
		arr2d[spec - 1][0] = status;
	}
	else if (status == 0)					// adding regular patient at the end of the query.
	{
		names[spec - 1][count_patient[spec-1]] = name;
		arr2d[spec - 1][count_patient[spec-1]] = status;
	}
	cout << "Patient is added successfully\n\n";
	count_patient[spec-1] = count_patient[spec-1] + 1;
	return;
}

int printSpecPatients(int spec)
{
	if (count_patient[spec-1] == 0)
	{
		return 0;
	}
	cout << "There are " << count_patient[spec-1]
			<< " patients in specialization " << spec << endl;
	for (int i = 0; i < count_patient[spec-1]; ++i)
	{ 
		if (!arr2d[spec-1][i])			    // 0 fpr regular case
		{
			cout << "name: " << names[spec-1][i] << "\t status: regular\n";
		}
		else			    		        // 1 for urgent case
		{
			cout << "name: " << names[spec-1][i] << "\t status: urgent\n";
		}
	}
	cout << "\n";
	return 1;
}

void printAllPatients()
{
	cout << "*************************************\n";
	int counter = 0;
	for (int spec = 1; spec <= MAX_SPECIALIZATION; spec++)
	{
		counter = counter + printSpecPatients(spec);
	}
	if (!counter)
	{
		cout << "There is no patients on the system at the moment.\n";
	}
	cout << "\n";
}

void shifting_left(int spec)
{
	for (int i = 0; i < count_patient[spec-1]; i++)		
	{
		arr2d[spec-1][i] = arr2d[spec-1][i+1];
		names[spec-1][i] = names[spec-1][i+1];
	}
		
	arr2d[spec-1][count_patient[spec-1]] = -1;
	names[spec-1][count_patient[spec-1]] = "";
	count_patient[spec-1] = count_patient[spec-1] - 1;
}

void getNextPatient()
{
	int spec = -1;
	cout << "Enter specialization: ";
	cin >> spec;
	if (spec > 20 || spec < 1)
	{
		cout << "Invalid specialization\n\n";
		return;
	}
	if (count_patient[spec - 1] > 0)
	{
		cout << names[spec - 1][0] << " please go with the Dr.\n";
		// shifting all patient to left
		shifting_left(spec);
	}
	else
	{
		cout << "No patient at the moment. Have a rest, Dr.\n";
	}
	cout << endl;
}

int main()
{
	// initializing all specialization with -1
	for (int i=0; i<MAX_SPECIALIZATION; i++)
		for (int j=0; j<MAX_QUEUE; j++)
			arr2d[i][j] = -1;
	// initializing all names with emtpy string
	for (int i=0; i<MAX_SPECIALIZATION; i++)
		for (int j=0; j<MAX_QUEUE; j++)
			names[i][j] = "";
	int choice = 0;
	while(true)
	{
		print_menu();
		cin >> choice;
		if (choice > 4 || choice < 1)
		{
			cout << "invalid choice. try again\n";
		}
		else
		{
			switch (choice)
			{
			case 1: addNewPatient();
					break;
			case 2: printAllPatients();
					break;
			case 3: getNextPatient();
					break;
					
			default: break;
			}

			if (choice == 4)		// exit case.
			{
				break;
			}
		}
	}


	return 0;
}


