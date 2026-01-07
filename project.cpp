#include<iostream>
#include<string>
#include<ctime>
#include<fstream>
#include<iomanip>
#include<vector>

using namespace std;


void ShowTransactionsMenu();
void ShowMainMenuScreen();
void GoBackToTransactionsMenu();
void ShowManageUsersMenuScreen();
void LogIn();
void GoBackToManageUsersMenu();
void GoBackToMainMenu();




enum enLoginChoice { eShowClientsList = 1, eAddNewClient = 2, eFindClient = 3, eUpdateClient = 4, eDeleteClient = 5, eTransationsMenu = 6, eManageUsers = 7, eLogOut = 8 };
enum enTransactionsChoice { eDeposit = 1, eWithdrawal = 2, eTotalBalance = 3, eMainMenu = 4 };
enum enManageUsers{eListUsers = 1, eAddNewUser = 2, eUpdateUser = 3, eDeleteUser = 4, eFindUser = 5, eMainmenu = 6};
enum enPermissions
{
	pShowClients = 1,
	pAddClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUsers = 64,
	pAll = -1
};

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string PhoneNumber;
	int Permitions = 0;
	double AccountBalance = 0;
	bool MarkedAsDeleted = false;

};
struct stUser
{
	string UserName;
	string Password;
	int Permitions = 0;

};
stUser CurrentUser;


const string ClientsFileName = "Clients Data.txt";
const string UsersFileName = "Users.txt";



char ReadChar(string Message = "Enter Your Char: ")
{
	cout << Message;
	char c;
	cin >> c;

	while (cin.fail())
		cin >> c;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	return c;
}
double ReadDouble(const string& message)
{
	double n;

	while (true)
	{
		cout << message;

		if (cin >> n && n > 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return n;
		}

		cout << "Invalid input. Please enter a positive number.\n";

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}
string ReadString(string message = "Enter a string: ")
{
	string s = "";

	cout << message;
	getline(cin >> ws, s);

	return s;

}
vector <string> SplitString(string String, string delim = " ")
{
	string word = "";
	short pos = 0;
	vector<string> s;

	while ((pos = String.find(delim)) != std::string::npos)
	{
		word = String.substr(0, pos);

		if (word != " ")
			s.push_back(word);

		String.erase(0, pos + delim.length());
	}
	if (String != " ")
		s.push_back(String);

	return s;
}








stUser ConvertLineToUserRecord(string UserRecord, string delim = "#//#")
{
	stUser User;
	vector<string> vUser = SplitString(UserRecord, delim);
	if (vUser.size() < 3) return User;


	User.UserName = vUser[0];
	User.Password = vUser[1];
	User.Permitions = stoi(vUser[2]);


	return User;
}
string ConvertUserRecordToLine(stUser& User, string delim = "#//#")
{
	string Record = "";

	Record += User.UserName + delim;
	Record += User.Password + delim;
	Record += to_string(User.Permitions);

	return Record;
}
vector<stUser> LoadUsersFromFile(string fname)
{
	vector<stUser> vUsers;
	fstream fUsers;
	fUsers.open(fname, ios::in);

	if (fUsers.is_open())
	{
		string line;

		while (getline(fUsers, line))
			if(!line.empty())
				vUsers.push_back(ConvertLineToUserRecord(line));

		fUsers.close();
	}

	return vUsers;
}
void LoadUsersToFile(string fname, vector<stUser>& vUsers)
{
	fstream fUsers;
	fUsers.open(fname, ios::out);

	if (fUsers.is_open())
	{
		for (stUser& User : vUsers)
		{
			fUsers << ConvertUserRecordToLine(User) << endl;
		}


		fUsers.close();
	}

}
int ReadPermitionsToSet()
{
	int Permitions = 0;

	if (tolower(ReadChar("Do You Want To Give Full Access To This User? (y,n): ")) == 'y')
		return Permitions |= enPermissions::pAll;

	cout << "\nDo You Want To Give Access To : \n";

	if (ReadChar("Show Client List? (y,n): ") == 'y')
		Permitions |= enPermissions::pShowClients;

	if (tolower(ReadChar("Add New Client? (y,n): ")) == 'y')
		Permitions |= enPermissions::pAddClient;

	if (tolower(ReadChar("Find Client? (y,n): ")) == 'y')
		Permitions |= enPermissions::pFindClient;

	if (tolower(ReadChar("Update Client? (y,n): ")) == 'y')
		Permitions |= enPermissions::pUpdateClient;

	if (tolower(ReadChar("Delete Client? (y,n): ")) == 'y')
		Permitions |= enPermissions::pDeleteClient;
	
	if (tolower(ReadChar("Transactions? (y,n): ")) == 'y')
		Permitions |= enPermissions::pTransactions;

	if (tolower(ReadChar("Manage Users? (y,n): ")) == 'y')
		Permitions |= enPermissions::pManageUsers;


	return Permitions;
}
bool IsUserExist(const string& UserName)
{
	vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

	for (stUser U : vUsers)
	{
		if (U.UserName == UserName)
			return true;
	}
	return false;
}
bool FindUserByUserName(string UserName, stUser& User, vector<stUser>& vUsers)
{

	for (stUser U : vUsers)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}	
	}

	return false;
}




stClient ConvertLineToRecord(string Record, string delim = "#//#")
{
	stClient Client;
	vector<string> vClient = SplitString(Record, delim);
	if (vClient.size() < 5) return Client;


	Client.AccountNumber = vClient[0];
	Client.PinCode = vClient[1];
	Client.Name = vClient[2];
	Client.PhoneNumber = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}
string ConvertRecordToLine(stClient Record, string delim = "#//#")
{
	string Rec = "";

	Rec += Record.AccountNumber + delim;
	Rec += Record.PinCode + delim;
	Rec += Record.Name + delim;
	Rec += Record.PhoneNumber + delim;
	Rec += to_string(Record.AccountBalance);

	return Rec;
}
vector<stClient> LoadClientsDataToVector(string fname)
{
	fstream fClients;
	vector<stClient> vClients;
	fClients.open(fname, ios::in);

	if (fClients.is_open())
	{
		string line;

		while (getline(fClients, line))
		{
			if(!line.empty())
				vClients.push_back(ConvertLineToRecord(line));
		}

		fClients.close();
	}

	return vClients;
}
void LoadDataFromVectorToFile(string fname, vector<stClient>& vClients)
{
	fstream fClients;
	fClients.open(fname, ios::out);

	if (fClients.is_open())
	{
		for (stClient& Client : vClients)
			fClients << ConvertRecordToLine(Client) << endl;

		fClients.close();
	}
}
bool IsAccNumberExist(const string& AccNumber)
{
	vector<stClient> vClients = LoadClientsDataToVector(ClientsFileName);

	for (stClient& Client : vClients)
	{
		if (Client.AccountNumber == AccNumber)
			return true;
	}
	return false;
}
bool IsValidAmountOfMoney(double AmountOfMoney)
{
	return AmountOfMoney > 0 ? true : false;
}
stClient UpdateData()
{
	stClient Client;

	cout << "\n============================================\n";

	Client.PinCode = ReadString("Please Enter Account Pin Code : ");
	Client.Name = ReadString("Please Enter Name             : ");
	Client.PhoneNumber = ReadString("Please Enter Phone Number     : ");
	Client.AccountBalance = stod(ReadString("Please Enter Balance          : "));

	cout << "\n============================================\n";

	return Client;
}
stClient AddNewClient()
{
	stClient Client;


	cout << "\n============================================\n";

	Client.AccountNumber =       ReadString("Please Enter Account Number   : ");

	while (IsAccNumberExist(Client.AccountNumber))
	{
		cout << "\nAccount Number [" << Client.AccountNumber << "] Is Already Taken, Please Enter Another One.\n";
		Client.AccountNumber = ReadString("Please Enter Account Number   : ");
	}

	Client.PinCode =             ReadString("Please Enter Account Pin Code : ");
	Client.Name =                ReadString("Please Enter Name             : ");
	Client.PhoneNumber =         ReadString("Please Enter Phone Number     : ");
	Client.AccountBalance = stod(ReadString("Please Enter Balance          : "));

	cout << "\n============================================\n";


	return Client;
}




void PrintClientCardInfo(stClient& ClientCard)
{
	cout << "\n\t\t\tCleint Card Data\t\t\t\n\n";

	cout << "================================\n";
	cout << "Account Number   : " << ClientCard.AccountNumber << endl;
	cout << "Account Pin Code : " << ClientCard.PinCode << endl;
	cout << "Name             : " << ClientCard.Name << endl;
	cout << "Phone Number     : " << ClientCard.PhoneNumber << endl;
	cout << "Balance          : " << ClientCard.AccountBalance << endl;

	cout << "================================\n";

}
bool FindClient(const string& AccNumber,stClient&Client, vector<stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (AccNumber == C.AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
void MarkAccountToDelete(const string& AccNumber, vector<stClient>& vClients)
{
	for (stClient& Client : vClients)
	{
		if (AccNumber == Client.AccountNumber)
		{
			Client.MarkedAsDeleted = true;
			break;
		}
	}
}
void SaveDeleteStatus(string fname, vector<stClient>& vClients)
{
	fstream fClients;
	fClients.open(fname, ios::out);

	if (fClients.is_open())
	{
		for (stClient& Client : vClients)
		{
			if (!Client.MarkedAsDeleted)
				fClients << ConvertRecordToLine(Client) << endl;
		}

		fClients.close();
	}
}
void DeleteClient(const string& AccNumber, vector<stClient>& vClients)
{
	stClient Client;

	if (!FindClient(AccNumber, Client, vClients))
	{
		cout << "\nAccount Number [" << AccNumber << "] Is Not Found\n";
		return;
	}
	else
	{
		PrintClientCardInfo(Client);
		char Answer = ReadChar("\nDo You Really Want To Delete This Account? (y,n) : ");

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkAccountToDelete(AccNumber, vClients);
			SaveDeleteStatus(ClientsFileName, vClients);

			cout << "\nAccount Successfully Deleteted.\n";
		}
		else
			cout << "\nDelete Is Canceled.\n";
	}

}
void UpdateClientInfo(const string& AccNumber, vector<stClient>& vClients)
{
	stClient UpdatedClient;

	if (!FindClient(AccNumber, UpdatedClient, vClients))
	{
		cout << "\nAccount Number [" << AccNumber << "] Is Not Found\n";
		return;
	}
	else
	{
		PrintClientCardInfo(UpdatedClient);

		UpdatedClient = UpdateData();
		UpdatedClient.AccountNumber = AccNumber;

		
		char Answer = ReadChar("\n\nDo You Want To Apply These Changes?(y,n) : ");

		if (tolower(Answer) == 'y')
		{

			for (stClient& C : vClients)
				if (C.AccountNumber == UpdatedClient.AccountNumber)
				{
					C = UpdatedClient;
					break;
				}


			LoadDataFromVectorToFile(ClientsFileName, vClients);
			cout << "\nAccount Updated Successfully.\n";

		}

	}

}
void AddNewClients(vector<stClient> vClients)
{
	char AddMore = 'y';

	while (AddMore == 'y')
	{
		vClients.push_back(AddNewClient());

		AddMore = ReadChar("\nClient Added Successfully, Do You Want To Add More Clients? (y,n) : ");
	}

	LoadDataFromVectorToFile(ClientsFileName, vClients);
}
void PrintOneLineRecord(stClient& Record)
{
	cout << left << "| " << setw(15) << Record.AccountNumber;
	cout << left << "| " << setw(9) << Record.PinCode;
	cout << left << "| " << setw(35) << Record.Name;
	cout << left << "| " << setw(15) << Record.PhoneNumber;
	cout << left << "| " << setw(10) << Record.AccountBalance;
	cout << "\n";
}
void PrintClientBalance(stClient& Record)
{
	cout << left << "| " << setw(30) << Record.AccountNumber;
	cout << left << "| " << setw(35) << Record.Name;
	cout << left << "| " << setw(10) << Record.AccountBalance;
	cout << "\n";
}
void ShowClientsList(vector<stClient>& vClients)
{

	cout << "\n\t\t\t\tClients List (" << vClients.size() << ")\t\t\t\n";

	cout << "__________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(9) << "Pin Code" << 
		"| " << left << setw(35) << "Client Name" << "| " << left << setw(15) << "Phone Number"
		<< "| " << left << setw(10) << "Balance" << endl;

	cout << "__________________________________________________________________________________________________\n";


	for (stClient& Client : vClients)
	{
		PrintOneLineRecord(Client);
	}

	cout << "__________________________________________________________________________________________________\n";


}
void DepositMoney(vector<stClient>& vClients)
{
	stClient Client;
	string AccNumber = ReadString("Please Enter Account Number : ");

	if (!FindClient(AccNumber, Client, vClients))
	{
		cout << "\nAccount Number [" << AccNumber << "] Is Not Found\n";
		return;
	}
	else
	{
		PrintClientCardInfo(Client);
		double AmountToDeposit = ReadDouble("\nPlease enter amount of deposit : ");

		if (!IsValidAmountOfMoney(AmountToDeposit))
		{
			cout << "\nInvalid Amount Of Money.";
			return;
		}

		char Confirm = ReadChar("\nAre You Sure You Want To Withdraw [ " + to_string(AmountToDeposit) + " ] From This Account ? (y/n): ");

		if (tolower(Confirm) == 'y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccNumber)
				{
					C.AccountBalance += AmountToDeposit;
					break;
				}
			}
			cout << "\nAmount deposited successfully.\n";
			LoadDataFromVectorToFile(ClientsFileName, vClients);

		}
		else
			cout << "\nDeposit Is Canceled\n";
	}
}
void WithdrawMoney(vector<stClient>& vClients)
{
	stClient Client;
	string AccNumber = ReadString("Please Enter Account Number : ");

	if (!FindClient(AccNumber, Client, vClients))
	{
		cout << "\nAccount Number [" << AccNumber << "] Is Not Found\n";
		return;
	}

	PrintClientCardInfo(Client);

	double AmountToWithdraw = ReadDouble("\nPlease enter amount to withdraw : ");

	if (!IsValidAmountOfMoney(AmountToWithdraw))
	{
		cout << "\nInvalid Amount Of Money.\n";
		return;
	}

	// Edge Case: insufficient balance
	if (AmountToWithdraw > Client.AccountBalance)
	{
		cout << "\nInsufficient balance.\n";
		cout << "Available Balance: " << Client.AccountBalance << endl;
		return;
	}

	char Confirm = ReadChar("\nAre You Sure You Want To Withdraw [ " + to_string(AmountToWithdraw) + " ] From This Account ? (y/n): ");

	if (tolower(Confirm) == 'y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccNumber)
			{
				C.AccountBalance -= AmountToWithdraw;
				break;
			}
		}

		LoadDataFromVectorToFile(ClientsFileName, vClients);
		cout << "\nWithdrawal completed successfully.\n";
	}
	else
	{
		cout << "\nWithdrawal is canceled.\n";
	}
}
void GetClientsTotalBalance(vector<stClient>& vClients)
{
	cout << "\n\t\t\t\tClients Balances List\t\t\t\n";

	cout << "__________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(30) << "Account Number" <<"| " << left << setw(35)
		<< "Client Name" << "| " << left << setw(10) << "Balance" << endl;

	cout << "__________________________________________________________________________________________________\n";

	double TotalBalance = 0;

	for (stClient Client : vClients)
	{
		PrintClientBalance(Client);
		TotalBalance += Client.AccountBalance;
	}

	cout << "\n__________________________________________________________________________________________________\n";
	cout << "\n\t\t\t\tTotal Balance Is : " << TotalBalance << endl;


	GoBackToTransactionsMenu();
}


enLoginChoice ReadLoginChoice()
{
	short choice;

	cout << "Please enter your choice: ";
	cin >> choice;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Please enter a valid choice: ";
		cin >> choice;
	}

	return enLoginChoice(choice);
}
enTransactionsChoice ReadTransactionChoice()
{
	short choice;

	cout << "Please enter your choice: ";
	cin >> choice;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Please enter a valid choice: ";
		cin >> choice;
	}

	return enTransactionsChoice(choice);
}


void ShowAddClientScreen(vector<stClient>& vClients)
{
	cout << "\t\t----------------------\n";
	cout << " \t\t     Add Client     \n";
	cout << "\t\t----------------------\n\n";

	AddNewClients(vClients);

}
void ShowUpdateClientScreen(vector<stClient>& vClients)
{
	cout << "\t\t----------------------\n";
	cout << " \t\t   Update Client    \n";
	cout << "\t\t----------------------\n";

	cout << "\n============================================\n";

	UpdateClientInfo(ReadString("\nPlease Enter Account Number: "), vClients);

	cout << "\n============================================\n";


}
void ShowFindClientScreen(vector<stClient>& vClients)
{
	cout << "\t\t----------------------\n";
	cout << " \t\t   Find Client    \n";
	cout << "\t\t----------------------\n";

	cout << "\n============================================\n";


	stClient Client;
	Client.AccountNumber = ReadString("\nPlease Enter Account Number: ");

	if (!FindClient(Client.AccountNumber, Client, vClients))
	{
		cout << "\nAccount Number [" << Client.AccountNumber << "] Is Not Found\n";
	}
	else
		PrintClientCardInfo(Client);


}
void ShowDeleteClientScreen(vector<stClient>& vClients)
{
	cout << "\t\t----------------------\n";
	cout << "\t\t     Delete Client    \n";
	cout << "\t\t----------------------\n";

	cout << "\n============================================\n";


	DeleteClient(ReadString("\nPlease Enter Account Number : "), vClients);

}



void ShowDepositScreen(vector<stClient>& vClients)
{
	cout << "\t\t----------------------\n";
	cout << " \t\t   Deosit Screen    \n";
	cout << "\t\t----------------------\n";

	cout << "\n============================================\n";
	DepositMoney(vClients);
	cout << "\n============================================\n";

	
	GoBackToTransactionsMenu();
}
void ShowWithdrawalScreen(vector<stClient>& vClients)
{
	cout << "\t\t----------------------\n";
	cout << "\t\t  Withdrawal Screen   \n";
	cout << "\t\t----------------------\n";

	cout << "\n============================================\n";
	WithdrawMoney(vClients);
	cout << "\n============================================\n";

	GoBackToTransactionsMenu();
}
void ShowTotalBalanceScreen(vector<stClient>& vClients)
{
	GetClientsTotalBalance(vClients);
}
void GoBackToTransactionsMenu()
{
	cout << "\nPress Any Key To Go Back To Transactions Menu....";
	system("pause>0");
	system("cls");
}





void PrintUserCardInfo(stUser& User)
{
	cout << "\n\t\tUser's Card Info\n\n";

	cout << "--------------------------------\n";
	cout << "UserName   : " << User.UserName << endl;
	cout << "Password   : " << User.Password << endl;
	cout << "Permitions : " << User.Permitions << endl;

	cout << "--------------------------------\n\n";

}
void PrintOneLineUser(stUser& User)
{
	cout << left << "| " << setw(30) << User.UserName;
	cout << left << "| " << setw(20) << User.Password;
	cout << left << "| " << setw(35) << User.Permitions;
	cout << "\n";
}
void UpdateUserData(const string& UserName, vector<stUser>& vUsers)
{
	if (UserName == "Admin")
	{
		cout << "\nAccess Denied!\n";
		return;
	}

	for (stUser& User : vUsers)
	{
		if (User.UserName == UserName)
		{
			PrintUserCardInfo(User);

			User.Permitions = ReadPermitionsToSet();  

			LoadUsersToFile(UsersFileName, vUsers);  
			cout << "\nUser updated successfully.\n";
			return;
		}
	}

	cout << "\nUserName Not Found\n";
}
void AddNewUser(vector<stUser>& vUsers)
{
	stUser User;

	User.UserName = ReadString("Please Enter User Name: ");
	while (IsUserExist(User.UserName))
	{
		cout << "\nUser Name Is Already Exist!\n";
		User.UserName = ReadString("Please Enter User Name: ");
	}

	User.Password = ReadString("Please Enter Password : ");
	User.Permitions = ReadPermitionsToSet();

	if (tolower(ReadChar("\nAre you sure you want to confirm these permissions? (y/n): ")) == 'y')
	{
		vUsers.push_back(User);               
		LoadUsersToFile(UsersFileName, vUsers); 
		cout << "\nUser added successfully.\n";
	}
	else
	{
		cout << "\nAdding canceled.\n";
	}
}
void ShowListUsers(vector<stUser>& vUsers)
{
	cout << "\n\t\t\t\tClients List (" << vUsers.size() << ")\t\t\t\n";

	cout << "__________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(30) << "Account Number" << "| " << left << setw(20) << "Password" <<
		"| " << left << setw(35) << "Permitions" << endl;

	cout << "__________________________________________________________________________________________________\n";

	for (stUser& User : vUsers)
	{
		PrintOneLineUser(User);
	}

	cout << "__________________________________________________________________________________________________\n";

}
void ShowAddNewUserScreeen(vector<stUser>& vUsers)
{
	cout << "--------------------------\n";
	cout << "   Add New User Screen    \n";
	cout << "--------------------------\n\n";

	AddNewUser(vUsers);
}
void ShowUpdateUserScreeen(vector<stUser> vUsers)
{

	cout << "--------------------------\n";
	cout << "   Update  User Screen    \n";
	cout << "--------------------------\n\n";

	UpdateUserData(ReadString("Please Enter UserName : "), vUsers);
}
bool DeleteUserByUserName(const string& UserName, vector<stUser>& vUsers)
{

	for (auto it = vUsers.begin(); it != vUsers.end(); ++it)
	{
		if (it->UserName == UserName)
		{
			vUsers.erase(it);
			return true;
		}
	}
	return false;
}
void ShowDeleteUserScreen(vector<stUser>& vUsers)
{
	cout << "--------------------------\n";
	cout << "   Delete User Screen     \n";
	cout << "--------------------------\n\n";

	string UserName = ReadString("Please Enter UserName: ");

	if (UserName == "Admin")
	{
		cout << "\nYou cannot delete Admin user.\n";
		return;
	}

	stUser User;
	if (!FindUserByUserName(UserName, User, vUsers))
	{
		cout << "\nUser not found.\n";
		return;
	}

	PrintUserCardInfo(User);

	if (tolower(ReadChar("\nAre you sure you want to delete this user? (y/n): ")) == 'y')
	{
		DeleteUserByUserName(UserName, vUsers);
		LoadUsersToFile(UsersFileName, vUsers);
		cout << "\nUser deleted successfully.\n";
	}
	else
	{
		cout << "\nDelete canceled.\n";
	}
}



bool HasPermission(int UserPermissions, enPermissions Permission)
{
	if (UserPermissions == enPermissions::pAll)
		return true;

	return (UserPermissions & Permission) == Permission;
}
void ShowFindUserScreen(vector<stUser>& vUsers)
{
	cout << "--------------------------\n";
	cout << "   Find User Screen       \n";
	cout << "--------------------------\n\n";

	string UserName = ReadString("Please Enter UserName: ");

	stUser User;
	if (!FindUserByUserName(UserName, User, vUsers))
	{
		cout << "\nUser not found.\n";
		return;
	}

	PrintUserCardInfo(User);
}




enManageUsers ReadManageUsersMenuChoice()
{
	short choice = 0;

	cout << "\nPlease Enter Your Choice : ";
	cin >> choice;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	while (choice <= 0 || choice > 6)
	{
		cout << "Invalid Choice!\n";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	}

	return (enManageUsers)choice;
}
void PerformManageUsersMenuChoice(enManageUsers Choice, vector<stUser>& vUsers)
{
	system("cls");

	switch (Choice)
	{
	case::eListUsers:
		ShowListUsers(vUsers);
		GoBackToManageUsersMenu();
		break;

	case::eAddNewUser:
		ShowAddNewUserScreeen(vUsers);
		GoBackToManageUsersMenu();
		break;

	case::eUpdateUser:
		ShowUpdateUserScreeen(vUsers);
		GoBackToManageUsersMenu();
		break;

	case::eDeleteUser:
		ShowDeleteUserScreen(vUsers);
		GoBackToManageUsersMenu();
		break;

	case::eFindUser:
		ShowFindUserScreen(vUsers);
		GoBackToManageUsersMenu();
		break;

	case::eMainmenu:
		ShowMainMenuScreen();

	}
}
void ShowManageUsersMenuScreen()
{
	vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

	system("cls");

	cout << "===========================================\n";
	cout << "\t\tManage Users Minu Screen\n";
	cout << "===========================================\n";

	cout << "\t[1]: List Users.\n";
	cout << "\t[2]: Add New User.\n";
	cout << "\t[3]: Update User.\n";
	cout << "\t[4]: Delete User.\n";
	cout << "\t[5]: Find User.\n";
	cout << "\t[6]: Main Minu.\n";
	cout << "===========================================\n";

	PerformManageUsersMenuChoice( ReadManageUsersMenuChoice(), vUsers);

}
void GoBackToManageUsersMenu()
{
	cout << "\n\nPress Any Key To Go Back To Manage Users Menu....\n";
	system("pause>0");
	system("cls");
	ShowManageUsersMenuScreen();
}
void GoBackToMainMenu()
{
	cout << "\nPress Any Key To Go Back To Main Menu....";
	system("pause>0");
	system("cls");
	ShowMainMenuScreen();
}
void PerformTransactionsChoice(enTransactionsChoice Choice, vector<stClient>& vClients)
{
	system("cls");

	switch (Choice)
	{
	case::eDeposit:
		ShowDepositScreen(vClients);
		ShowTransactionsMenu();
		break;

	case::eWithdrawal:
		ShowWithdrawalScreen(vClients);
		ShowTransactionsMenu();
		break;

	case::eTotalBalance:
		ShowTotalBalanceScreen(vClients);
		ShowTransactionsMenu();
		break;

	case::eMainMenu:
		ShowMainMenuScreen();
		break;
	}
}
void PerformMainMenuChoice(enLoginChoice Choice)
{
	vector<stClient> vClients = LoadClientsDataToVector(ClientsFileName);

	system("cls");

	switch (Choice)
	{
	case::eShowClientsList:
		if (!HasPermission(CurrentUser.Permitions, pShowClients))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowClientsList(vClients);
		GoBackToMainMenu();
		break;


	case eAddNewClient:
		if (!HasPermission(CurrentUser.Permitions, pAddClient))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowAddClientScreen(vClients);
		GoBackToMainMenu();
		break;



	case::eFindClient:
		if (!HasPermission(CurrentUser.Permitions, pFindClient))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowFindClientScreen(vClients);
		GoBackToMainMenu();
		break;


	case::eUpdateClient:
		if (!HasPermission(CurrentUser.Permitions, pUpdateClient))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowUpdateClientScreen(vClients);
		GoBackToMainMenu();
		break;



	case::eDeleteClient:
		if (!HasPermission(CurrentUser.Permitions, pDeleteClient))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowDeleteClientScreen(vClients);
		GoBackToMainMenu();
		break;



	case eTransationsMenu:
		if (!HasPermission(CurrentUser.Permitions, pTransactions))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowTransactionsMenu();
		break;



	case::eManageUsers:
		if (!HasPermission(CurrentUser.Permitions, pManageUsers))
		{
			cout << "\nAccess Denied.\n";
			GoBackToMainMenu();
			return;
		}
		ShowManageUsersMenuScreen();
		break;



	case::eLogOut:
		system("cls");
		LogIn();

	}
}
void ShowTransactionsMenu()
{
	vector<stClient> vClients = LoadClientsDataToVector(ClientsFileName);
	system("cls");

	cout << "=====================================\n";
	cout << "      Transactions Menu\t\t\n";
	cout << "=====================================\n";

	cout << "\t[1]: Deposit\n";
	cout << "\t[2]: Withdrawal\n";
	cout << "\t[3]: Total Balance.\n";
	cout << "\t[4]: Main Menu.\n";

	cout << "=====================================\n";
	PerformTransactionsChoice(ReadTransactionChoice(), vClients);
}
void ShowMainMenuScreen()
{
	system("cls");

	cout << "===============================================\n";
	cout << "                 Main Menu                     \n";
	cout << "===============================================\n";

	cout << "    [1]: Show Clients List.\n";
	cout << "    [2]: Add New Client.\n";
	cout << "    [3]: Find Client.\n";
	cout << "    [4]: Update Client Info.\n";
	cout << "    [5]: Delete Client.\n";
	cout << "    [6]: Transactions.\n";
	cout << "    [7]: Manage Users.\n";
	cout << "    [8]: Log Out.\n";

	cout << "===============================================\n";
	PerformMainMenuChoice(ReadLoginChoice());

}
bool FindUserByCredentials(string UserName, string Password, stUser& User)
{
	vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

	for (stUser U : vUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}
void ShowLoginScreeen()
{
	vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

	string UserName = ReadString("Please Enter UserName : ");
	string Password = ReadString("Please Enter Password : ");

	while (!FindUserByCredentials(UserName, Password, CurrentUser))
	{
		system("cls");
		cout << "Invalid UserName/Password\n\n";

		UserName = ReadString("Please Enter UserName : ");
		Password = ReadString("Please Enter Password : ");
	}


	ShowMainMenuScreen();

}





void LogIn()
{
	cout << "--------------------------\n";
	cout << "    Login Screen          \n";
	cout << "--------------------------\n\n";

	ShowLoginScreeen();
}




int main()
{	
	
	LogIn();

	system("pause>0");
}