#include "pch.h"
#include <iostream>
using namespace std;

const int OPERATOR_BUFFER = 0;
const int OPERATOR_NOT = 1;
const int OPERATOR_AND = 2;
const int OPERATOR_OR = 3;
const int OPERATOR_XOR = 4;
const int OPERATOR_IMPLY = 5;
const int OPERATOR_SAVE = 16;

const int OPERATION_VAR_A = 0;
const int OPERATION_VAR_B = 1;
const int OPERATION_OPERATOR = 2;
const int VARIABLE_SHORTCUT = 26;

/*
This class acts as an extendable array for the inputs used in boolean expression
*/
class inputList
{
	private:
		char* letter;		//Character of variables
		bool* special;		//Indication of intermediate variables
		bool* value;		//Values held by variable
		int size = 0;		//Size of array
		int realSize = 0;	//The size of array - intermediate values

	public:
		//Default Constructor
		inputList()
		{
			letter = new char[size];
			special = new bool[size];
			value = new bool[size];
		}

		//Constructor
		inputList(const int &sz)
		{
			size = sz;
			realSize = size;
			letter = new char[sz];
			special = new bool[sz];
			value = new bool[sz];
		}

		//Adds a new variable to the list
		void addVariable(const char &l, const bool &sp, const bool &val)
		{
			char* tempC = new char[size + 1];
			bool* tempB = new bool[size + 1];
			bool* tempV = new bool[size + 1];

			std::copy(letter, letter + size, tempC);
			std::copy(special, special + size, tempB);
			std::copy(value, value + size, tempV);

			delete[] letter;
			delete[] special;
			delete[] value;
			letter = tempC;
			special = tempB;
			value = tempV;

			letter[size] = l;
			special[size] = sp;
			value[size] = val;
			size++;

			if(!sp)
				realSize++;
		}

		//Returns location of letter in list
		int searchForCharacter(char c)
		{
			for (int i = 0; i < size; i++)
			{
				if (letter[i] == c)
					return i;
			}
			return -1;
		}

		//Prints contents in console
		void print()
		{
			for (int i = 0; i < size; i++)
			{
				cout << i << ": " << letter[i] << " " << value[i] << " " << special[i] << endl;
			}
			cout << "SIZE: " << size << endl;
			cout << "ALT SIZE: " << realSize << endl;
		}

		//Inverts the value held by a variable
		void invertInput(const int &index){value[index] = !value[index];}
		//Sets held value of a variable
		void setValue(const int &index, const bool &val){value[index] = val;}
		//Returns character that represents variable
		char getLetter(const int &index){return letter[index];}
		//REturns held value of variable
		bool getValue(const int &index){return value[index];}
		//Returns whether variable is intermediate or not
		bool isSpecial(const int &index){return special[index];}
		//ALL variables within list
		int getNumVariables() { return size; }
		//Only proper input variables within list
		int getRealNumVariables() { return realSize; }

		//Destructor
		~inputList()
		{
			delete[] letter;
			delete[] special;
			delete[] value;
		}
};

/*
This class acts as an extendable array for the operations performed in the function.
*/
class operationList
{
	private:
		int size = 0;		//Length of the list
		int levelCount = 1;	//The number of levels
		int* varA;			//List of variable A in equation
		int* varB;			//List of variable B in equation		
		int* operation;		//List of operations in equation

	public:
		//Constructor
		operationList()
		{
			operation = new int[size];
			varA = new int[size];
			varB = new int[size];
		}

		//Adds an operation to the list
		void addOperation(const int &op, const int &A, const int &B)
		{
			int* tempOp = new int[size + 1];
			int* tempA = new int[size + 1];
			int* tempB = new int[size + 1];

			std::copy(operation, operation + size, tempOp);
			std::copy(varA, varA + size, tempA);
			std::copy(varB, varB + size, tempB);

			delete[] operation;
			delete[] varB;
			delete[] varA;
			varA = tempA;
			varB = tempB;
			operation = tempOp;

			varA[size] = A;
			varB[size] = B;
			operation[size] = op;
			size++;
		}

		//Prints contents in console
		void print()
		{
			for (int i = 0; i < size; i++)
			{
				cout << i << ": " << operation[i] << " " << varA[i] << " " << varB[i] << endl;
			}
		}

		//Increments the level of operations (the bracket level)
		void incrementLevel() { levelCount++;}
		//Returns variable A in the operation
		int getVarA(const int &index){return varA[index];}
		//Returns variable B in the operation
		int getVarB(const int &index) { return varB[index]; }
		//Returns function of the operation
		int getOperator(const int &index) { return operation[index]; }
		//Returns the length of the list
		int getNumOperations(){return size;}
		//REturns the level count of the operations
		int getLevelCount() { return levelCount; }

		//DEstructor
		~operationList()
		{
			delete[] varA;
			delete[] varB;
			delete[] operation;
		}
};

bool boolFunction(inputList &varList, operationList &operations);
void printTruthTable(bool** table, inputList &varList, const int &height);
void generateTruthTable(bool** table, inputList &variables, operationList &operations, const int &height);
void searchVariables(char input[], inputList &varList, const int& inputSz);
void searchOperatorBracket(char input[], operationList &operations, inputList &variables, int& inputSz, const int& start);
void compileSection(char input[], operationList &operations, inputList &variables, const int& start, int end, int& inputSz);
int searchForVariableGreater(const char input[], inputList &variables, const int location, int &right);
int searchForVariableLess(const char input[], inputList &variables, const int location, int &left);
void replaceSection(char input[], int &inputSize, const int &start, const int &end, const char &replace);
bool op(bool a, bool b, const int func);

//main method
int main()
{
	inputList variables;		//Variables used in equation
	operationList operationSet; //Operatoin list
	char equation[] = "A";		//The "string" used to hold the equation
	int equationSize = 0;		//The size of the "string"
	bool** truthTable;			//The truth table array
	int tableHeight = 1;		//The height of the truth table

	cin.getline(equation, 255);	//Max size = 255
	//Get size of equation
	for (; equation[equationSize] != 0; equationSize++);
	equationSize++;

	cout << "READING INPUT..." << endl;
	searchVariables(equation, variables, equationSize);
	searchOperatorBracket(equation, operationSet, variables, equationSize, 0);
	tableHeight = (int) pow(2, variables.getRealNumVariables());

	//Initialize array
	truthTable = new bool*[variables.getRealNumVariables() + 1];
	for (int x = 0; x <= variables.getRealNumVariables(); x++)
	{
		truthTable[x] = new bool[tableHeight];

		for (int y = 0; y < tableHeight; y++)
		{
			truthTable[x][y] = false;
		}
	}

	cout << "GENERATING TABLE.." << endl;
	generateTruthTable(truthTable, variables, operationSet, tableHeight);

	cout << "PRINTING TABLE:" << endl;
	printTruthTable(truthTable, variables, tableHeight);

	for (int x = 0; x <= variables.getRealNumVariables(); x++)
		delete[] truthTable[x];
	delete[] truthTable;

	exit(0);
}

//Performs the provided equation for the row of the table
bool boolFunction(inputList &varList, operationList &operations)
{
	bool result = false;

	for (int i = 0; i < operations.getNumOperations(); i++)
	{
		if (operations.getOperator(i) == OPERATOR_SAVE)
			varList.setValue(operations.getVarA(i), result);
		else
			result = op(varList.getValue(operations.getVarA(i)), varList.getValue(operations.getVarB(i)), operations.getOperator(i));
	}

	return result;
}

//Prints the generated truth table in the console
void printTruthTable(bool** table, inputList &varList, const int &height)
{
	//Print headers
	for (int i = 0; i < varList.getRealNumVariables(); i++)
	{
		cout << varList.getLetter(i) << (char)9;
	}
	cout << "FINAL" << endl;

	//Print table
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x <= varList.getRealNumVariables(); x++)
		{
			cout << *(*(table + x) + y) << (char)9;
		}
		cout << endl;
	}
}

//Populates the array based on the operation provided
void generateTruthTable(bool** table, inputList &variables, operationList &operations, const int &height)
{
	bool carry = true;
	int index = 0;

	//Calulate row
	for (int y = 0; y < height; y++)
	{
		//Determine table value
		for (int x = 0; x <= variables.getRealNumVariables(); x++)
		{
			if (x < variables.getRealNumVariables())
				table[x][y] = variables.getValue(x);
			else
				table[x][y] = boolFunction(variables, operations);
		}

		//Carry bit
		for (index = 0, carry = true; carry && index < variables.getRealNumVariables(); index++)
		{
			carry = variables.getValue(variables.getRealNumVariables() - index - 1);
			variables.invertInput(variables.getRealNumVariables() - index - 1);
		}
	}
}

//Searches for existing variables within the equation in order
void searchVariables(char input[], inputList &varList, const int& inputSz)
{
	int order = 0;
	for (char c = 'A'; c <= 'Z'; c++)
	{
		for (int i = 0; i < inputSz; i++)
		{
			if (input[i] == c)
			{
				varList.addVariable(c, false, false);
				break;
			}
		}
	}
}

//Searches for next instance of level in equation OR the end of the current instance
void searchOperatorBracket(char input[], operationList &operations, inputList &variables, int& inputSz, const int& start)
{
	int end = 0;
	for (int i = start; i < inputSz; i++)
	{
		if (input[i] == '(')
		{
			searchOperatorBracket(input, operations, variables, inputSz, i + 1);
		}
		if (input[i] == ')')
		{
			cout << "COMPLETED BRACKET SET FROM " << start << " TO " << i << endl;
			cout << input << " -> ";
			input[start - 1] = ' ';
			input[i] = ' ';
			cout << input << endl;
			compileSection(input, operations, variables, start, i - 1, inputSz);
		}
	}
}

//Generates instructions within a block of the code
void compileSection(char input[], operationList &operations, inputList &variables, const int& start, int end, int& inputSz)
{
	bool constructOperation;
	int op = 0;
	int leftVariable = 0;
	int rightVariable = 0;

	for (int i = start; i <= end; i++)
	{
		constructOperation = true;

		//cout << "INDEX:" << i << endl;
		switch (input[i])
		{
			case '!':
				cout << "COMPILING NOT OPERATOR..." << endl;
				operations.addOperation(OPERATOR_NOT, searchForVariableGreater(input, variables, i, leftVariable), searchForVariableGreater(input, variables, i, rightVariable));
				leftVariable--; //Accounts for left and right variable being the same
				break;
			case '*':
				cout << "COMPILING AND OPERATOR..." << endl;
				operations.addOperation(OPERATOR_AND, searchForVariableLess(input, variables, i, leftVariable), searchForVariableGreater(input, variables, i, rightVariable));
				break;
			case '+':
				cout << "COMPILING OR OPERATOR..." << endl;
				operations.addOperation(OPERATOR_OR, searchForVariableLess(input, variables, i, leftVariable), searchForVariableGreater(input, variables, i, rightVariable));
				break;
			case '^':
				cout << "COMPILING XOR OPERATOR..." << endl;
				operations.addOperation(OPERATOR_XOR, searchForVariableLess(input, variables, i, leftVariable), searchForVariableGreater(input, variables, i, rightVariable));
				break;
			case '>':
				cout << "COMPILING IMPLY OPERATOR..." << endl;
				operations.addOperation(OPERATOR_IMPLY, searchForVariableLess(input, variables, i, leftVariable), searchForVariableGreater(input, variables, i, rightVariable));
				break;
			default:
				constructOperation = false;
		}

		if (constructOperation)
		{
			//Adds operation to save inter-calculation
			replaceSection(input, inputSz, leftVariable, rightVariable, operations.getLevelCount());
			variables.addVariable(operations.getLevelCount(), true, false);
			operations.addOperation(OPERATOR_SAVE, variables.getNumVariables() - 1, variables.getNumVariables() - 1);
			operations.incrementLevel();
			i = leftVariable;
			end -= rightVariable - leftVariable;
		}
	}
}

//returns variable that preceeds operator at location (returns -1 if unfound)
int searchForVariableGreater(const char input[], inputList &variables, const int location, int& right)
{
	//Search for variables
	for (int i = location; input[i] != 0; i++)
	{
		//Input Variable is found
		if ((input[i] >= 'A' && input[i] <= 'Z') || input[i] < 32)
		{
			cout << "FOUND VARIABLE " << input[i] << endl;
			right = i;
			return variables.searchForCharacter(input[i]);
		}
	}
	return -1;
}

//returns variable that comes before operator at location (returns -1 if unfound)
int searchForVariableLess(const char input[], inputList &variables, const int location, int& left)
{
	//Search for variables
	for (int i = location - 1; i >= 0; i--)
	{
		//Input Variable is found
		if ((input[i] >= 'A' && input[i] <= 'Z') || input[i] < 32)
		{
			cout << "FOUND VARIABLE " << input[i] << endl;
			left = i;
			return variables.searchForCharacter(input[i]);
		}
	}
	return -1;
}

//Removes bracket level from char[] and replaces it with a unique variable
void replaceSection(char input[], int &inputSize, const int &start, const int &end, const char &replace)
{
	cout << "REMOVE (" << start << ", " << end << ")" << endl;
	cout << input << " -> ";

	input[start] = replace;
	for (int i = 1; i < inputSize - (end - start); i++)
	{
		input[start + i] = input[end + i];
	}
	inputSize -= (end - start);

	cout << input << endl;
}

//Performs boolean operation
bool op(bool a, bool b, const int func)
{
	//cout << "OPERATION: " << func << " (" << a << ", " << b << ") = " << (a || b) << endl;

	switch (func)
	{
	case OPERATOR_NOT:
		return !a;
	case OPERATOR_AND:
		return a && b;
	case OPERATOR_OR:
		return a || b;
	case OPERATOR_XOR:
		return a ^ b;
	case OPERATOR_IMPLY:
		return (!a) || b;
	default:
		return false;
	}
}


