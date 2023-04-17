###############################################################
//  Name: Kristen Hawkins
//  Date: 4/15/2023
###############################################################


#include <Python.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

bool quit = false;


struct PurchaseData {
	string itemName; 
	int count; 
};


void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}


int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}


int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}



void PrintPurchaseHistogram() {
	vector<PurchaseData> data = GetFrequencyDataFromFile("frequency.dat");

	for (unsigned int i = 0; i < data.size(); ++i) {
		cout << data.at(i).itemName << " ";
		for (unsigned int j = 0; j < data.at(i).count; ++j) {
			cout << "*";
		}
		cout << endl;
	}
}



void ProcessMenuSelection(string selection) {
	cout << endl;

	if (selection == "4") {
		quit = true;	
		cout << "Exiting application..." << endl;	
		return;
	}

	
	if (selection == "1") {
		cout << "Printing report of today's purchased items..." << endl;

		
		CallProcedure("PrintItemsPurchasedFrequency");
		cout << endl;
		return;
	}

	if (selection == "2") {
		string userInput;

		cout << "Which item would you like to view purchase frequency for?:" << endl;
		cin.ignore();
		cin >> userInput;	

		
		cout << userInput << ": " << callIntFunc("PrintItemPurchaseFrequency", userInput) << endl;

		cout << endl;

		return;
	}

	
	if (selection == "3") {
		cout << "Generating Histogram Report..." << endl;

		CallProcedure("GeneratePurchaseFrequencyReport");
		PrintPurchaseHistogram();

		cout << endl;
		return;
	}

	cout << "Please enter a valid selection..." << endl << endl;
}


void PrintMenu() {
	cout << "             Corner Grocer Sales History Utility" << endl;
	cout << "----------------------------------------------------------" << endl;
	cout << "1. Display today's items purchased report" << endl;
	cout << "2. Display today's purchase report for a specific item" << endl;
	cout << "3. Display histogram of today's purchased items" << endl;
	cout << "4. Exit Application" << endl;
	cout << "Please enter your selection as a number 1-4:" << endl;
}

int main()
{
	string userSelection;

	while (!quit) {
		PrintMenu();
		cin >> userSelection;
		ProcessMenuSelection(userSelection);	
	}

	return 0;
}
