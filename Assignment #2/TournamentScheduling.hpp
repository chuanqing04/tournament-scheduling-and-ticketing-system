/*Assumptions
Only consider 1v1 matches in three locations, Arena A, Arena B, Arena C.
Code developed by Yap Kay Xuan.*/
#pragma once
class Player;
#include "Player.hpp"
#include "MatchTracking.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;
bool validateInput(string input);
bool isValidTimestamp(string timestamp);
string calculateEndTimestamp(string starttimestamp);
Schedule* assignWinnerRecursive(Schedule* current, Player* player, int no);
string trim(const string& s);

struct Schedule {
	int no;
	string tournamentName;
	int tournamentlevel;
	string group;
	string player1;
	string player2;
	string winners;
	string location;
	string starttimestamp;
	string endtimestamp;
	string status;
	Schedule* left;
	Schedule* right;
};


class BST {
	Schedule* root = nullptr;
	string BSTName = "";
	int size = 0;
	MatchStack matchHistory; // Add MatchStack member (**not developed by me)

public:
	BST(string BSTName) {
		this->BSTName = BSTName;
	}

	Schedule* getRoot() {
		return root;
	}

	~BST() {
		cout << BSTName << " is removed.\n";
	}

	void insert(int no, string tournamentName, int tournamentlevel, string group, string player1, string player2, string winners, string location, string starttimestamp, string endtimestamp, string status) {
		Schedule* newnode = new Schedule;
		newnode->no = no;
		newnode->tournamentName = tournamentName;
		newnode->tournamentlevel = tournamentlevel;
		newnode->group = group;
		newnode->player1 = player1;
		newnode->player2 = player2;
		newnode->winners = winners;
		newnode->location = location;
		newnode->starttimestamp = starttimestamp;
		newnode->endtimestamp = endtimestamp;
		newnode->status = status;
		newnode->left = nullptr;
		newnode->right = nullptr;

		if (root == nullptr) {
			root = newnode;
		}
		else {
			Schedule* parent = nullptr;
			Schedule* current = root;
			while (current != nullptr) {
				if ((current->tournamentName == newnode->tournamentName) && (current->starttimestamp == newnode->starttimestamp)
					&& (current->location == newnode->location)) {
					cout << "Duplicate, skipped.\n";
					delete newnode;
					return;
				}
				if (current->location == location) {
					if (starttimestamp < current->endtimestamp && endtimestamp > current->starttimestamp) {
						cout << "Schedule clash detected at " << location << ". Please choose a different time.\n";
						delete newnode;
						return;
					}
				}
				parent = current;
				if ((newnode->starttimestamp > parent->starttimestamp) ||
					(newnode->starttimestamp == parent->starttimestamp) && (newnode->location > parent->location))
					current = current->right;
				else
					current = current->left;
			}
			if ((newnode->starttimestamp > parent->starttimestamp) ||
				(newnode->starttimestamp == parent->starttimestamp) && (newnode->location > parent->location))
				parent->right = newnode;
			else
				parent->left = newnode;
		}
		size++;
	}

	void removeNode(int tournamentID) {
		root = removeNode(root, tournamentID);
	}

	void inorder() {
		cout << " ------------------------------------------------------------------------------------------------------------------\n" <<
			"    ID, Tournament Name, Tournament Level, Player 1, Player 2, Winners, Location, Start Time, End Time, Status\n" <<
			" ------------------------------------------------------------------------------------------------------------------\n";
		inorder(root);
		cout << endl;
	};


	void inorder(string choice, string query) {
		inorder(root, choice, query);
		cout << endl;
	}

	Schedule* maxValue() {
		return maxValue(root);
	};

	Schedule* minValue() {
		return minValue(root);
	};

	int CountNodes() {
		return CountNodes(root);
	};

	bool searchNode(string tournamentname, string starttimestamp) {
		return searchNode(root, tournamentname, starttimestamp);
	};

	// Function to call recordMatchResult in MatchStack (** Hew Wen chang)
	void recordMatch(int matchNo, string tournamentName, int tournamentLevel, string group, string player1, string player2, string winners, int player1Set1Score, int player2Set1Score, int player1Set2Score, int player2Set2Score, int player1Set3Score, int player2Set3Score, string location, string startTimestamp, string endTimestamp, string status) {
		matchHistory.recordMatchResult(matchNo, tournamentName, tournamentLevel, group, player1, player2, winners, player1Set1Score, player2Set1Score, player1Set2Score, player2Set2Score, player1Set3Score, player2Set3Score, location, startTimestamp, endTimestamp, status);
	}


	void displayMatchHistory() {
		matchHistory.displayHistory();
	}

	Schedule* findScheduleByNo(Schedule* current, int matchNo) { // Member function
		if (current == nullptr) {
			return nullptr; // Not found
		}

		if (current->no == matchNo) {
			return current; // Found
		}
		else if (matchNo < current->no) {
			return findScheduleByNo(current->left, matchNo); // Search left subtree
		}
		else {
			return findScheduleByNo(current->right, matchNo); // Search right subtree
		}
	}
private:
	Schedule* removeNode(Schedule* current, int tournamentID) {
		if (current == nullptr) {
			cout << "Match not found.\n";
			return nullptr;
		}

		if (tournamentID < current->no) {
			current->left = removeNode(current->left, tournamentID);
		}
		else if (tournamentID > current->no) {
			current->right = removeNode(current->right, tournamentID);
		}
		else {
			string confirmation;
			cout << "Found match with ID " << current->no << ":\n"
				<< current->tournamentName << ", " << current->location << ", "
				<< current->starttimestamp << "\n"
				<< "Are you sure you want to delete this match? (yes/no): ";
			getline(cin, confirmation);
			if (confirmation != "yes") {
				cout << "Not confirmed. Deletion cancelled.\n";
				return current;
			}

			// Case 1: No children (leaf node)
			if (current->left == nullptr && current->right == nullptr) {
				delete current;
				cout << "Schedule is deleted.\n";
				return nullptr;
			}
			// Case 2: Only one child (right child exists)
			else if (current->left == nullptr) {
				Schedule* temp = current->right;
				delete current;
				cout << "Schedule is deleted.\n";
				return temp;
			}
			// Case 2: Only one child (left child exists)
			else if (current->right == nullptr) {
				Schedule* temp = current->left;
				delete current;
				cout << "Schedule is deleted.\n";
				return temp;
			}
			// Case 3: Two children: Find the in-order successor (minimum in right subtree)
			else {
				Schedule* successor = minValue(current->right);
				// Copy successor's data into current node
				current->no = successor->no;
				current->tournamentName = successor->tournamentName;
				current->tournamentlevel = successor->tournamentlevel;
				current->group = successor->group;
				current->player1 = successor->player1;
				current->player2 = successor->player2;
				current->winners = successor->winners;
				current->location = successor->location;
				current->starttimestamp = successor->starttimestamp;
				current->endtimestamp = successor->endtimestamp;
				current->status = successor->status;
				// Recursively remove the successor node
				current->right = removeNode(current->right, successor->no);
			}
		}
		return current;
	}

	bool searchNode(Schedule* current, string tournamentName, string starttimestamp) {
		if (current == nullptr) {
			cout << "Empty\n";
			return false;
		}
		else if ((current->tournamentName == tournamentName) && (current->starttimestamp == starttimestamp)) {
			return true;
		}
		else if (current->starttimestamp > starttimestamp) {
			searchNode(current->right, tournamentName, starttimestamp);
		}
		else if (current->starttimestamp < starttimestamp) {
			searchNode(current->left, tournamentName, starttimestamp);
		}
		else {
			return false;
		}
	};

	void inorder(Schedule* current) {
		if (current != nullptr) {
			inorder(current->left);
			cout << current->no << ": " <<
				current->tournamentName << ", " << current->tournamentlevel << ", " << current->player1 << ", " <<
				current->player2 << ", " << current->winners << ", " << current->location << ", " << current->starttimestamp << ", " << current->endtimestamp
				<< ", " << current->status << "\n";
			inorder(current->right);
		}
	};

	void inorder(Schedule* current, string option, string query) {
		if (current != nullptr) {
			transform(option.begin(), option.end(), option.begin(), ::tolower);
			option.erase(option.find_last_not_of(" \n\r\t") + 1);
			if (option == "tournamentname") {
				inorder(current->left, option, query);
				if (current->tournamentName == query) {
					cout << current->no << ": " <<
						current->tournamentName << ", " << current->tournamentlevel << ", " << current->player1 << ", " <<
						current->player2 << ", " << current->winners << ", " << current->location << ", " << current->starttimestamp << ", " << current->endtimestamp
						<< ", " << current->status << "\n";
				}
				inorder(current->right, option, query);
			}
			else if (option == "location") {
				inorder(current->left, option, query);
				if (current->location == query) {
					cout << current->no << ": " <<
						current->tournamentName << ", " << current->tournamentlevel << ", " << current->player1 << ", " <<
						current->player2 << ", " << current->winners << ", " << current->location << ", " << current->starttimestamp << ", " << current->endtimestamp
						<< ", " << current->status << "\n";
				}
				inorder(current->right, option, query);
			}
			else if (option == "starttimestamp") {
				inorder(current->left, option, query);
				if (current->starttimestamp == query) {
					cout << current->no << ": " <<
						current->tournamentName << ", " << current->tournamentlevel << ", " << current->player1 << ", " <<
						current->player2 << ", " << current->winners << ", " << current->location << ", " << current->starttimestamp << ", " << current->endtimestamp
						<< ", " << current->status << "\n";
				}
				inorder(current->right, option, query);
			}
			else
				cout << "Unidentified column.\n";
		}
	}

	Schedule* maxValue(Schedule* current) {
		if (current == nullptr) {
			exit(1);
		}
		while (current->right != nullptr) {
			current = current->right;
		}
		return current;
	}

	Schedule* minValue(Schedule* current) {
		if (current == nullptr) {
			exit(1);
		}
		while (current->left != nullptr) {
			current = current->left;
		}
		return current;
	};

	int CountNodes(Schedule* current) {
		if (current == nullptr) {
			return 0;
		}
		return 1 + CountNodes(current->left) + CountNodes(current->right);
	};
};

void addSchedule(BST& Schedule1) {
	string tournamentName, tournamentlevel, group, player1 = "N/A", player2 = "N/A", location, starttimestamp, endtimestamp, status = "Pending";
	int noint = Schedule1.CountNodes() + 1;
	string winners = "N/A";
	int tournamentlevelint;

	cout << "Enter tournament name: ";
	getline(cin, tournamentName);
	cin.clear();
	if (validateInput(tournamentName)) {
		cout << "Invalid input. Special character ',' found.\n";
		return;
	}

	cout << "Enter tournament level (Qualifiers = 1, Knockout = 2, Quarter = 3, Semi = 4, Finals = 5, Exhibition = 6): ";
	getline(cin, tournamentlevel);

	bool isNumeric = all_of(tournamentlevel.begin(), tournamentlevel.end(), ::isdigit);
	if (!isNumeric) {
		cout << "Invalid input. Please enter a number.\n";
		return;
	}

	tournamentlevelint = stoi(tournamentlevel);
	if (tournamentlevelint < 1 || tournamentlevelint > 6) {
		cout << "Invalid tournament level. Must be between 1 and 6.\n";
		return;
	}

	cout << "Enter group: ";
	getline(cin, group);
	cin.clear();
	if (validateInput(group)) {
		cout << "Invalid input. Special character ',' found.\n";
		return;
	}

	cout << "Enter Location (Arena A, Arena B, Arena C): ";
	getline(cin, location);
	cin.clear();
	if ((location != "Arena A") && (location != "Arena B") && (location != "Arena C")) {
		cout << "Invalid input, please enter 'Arena A/B/C'.\n";
		return;
	}

	cout << "Enter starttimestamp: ";
	getline(cin, starttimestamp);
	cin.clear();

	if (!isValidTimestamp(starttimestamp)) {
		cout << "Invalid input, must be 'yyyymmddhhmm'.\n";
		return;
	}
	endtimestamp = calculateEndTimestamp(starttimestamp);

	Schedule1.insert(noint, tournamentName, tournamentlevelint, group, player1, player2, winners, location, starttimestamp, endtimestamp, status);

};


// Find "," in input, ',' is used as delimiter and therefore banned
bool validateInput(string input) {
	return input.find(',') != string::npos;
};

// Makes sure timestamp is in right format
bool isValidTimestamp(string timestamp) {
	if (timestamp.length() != 12) return false; // Must be 12 characters

	for (char c : timestamp) {
		if (!isdigit(c)) return false; // Must be all digits
	}

	int year, month, day, hour, minute;
	if (sscanf_s(timestamp.c_str(), "%4d%2d%2d%2d%2d", &year, &month, &day, &hour, &minute) != 5) {
		return false;
	}

	// Basic range checks
	if (month < 1 || month > 12) return false;
	if (day < 1 || day > 31) return false;
	if (hour < 0 || hour > 23) return false;
	if (minute < 0 || minute > 59) return false;

	return true;
}

// Add 2 hours to starttime stamp, duration for each match is fixed to two hours
string calculateEndTimestamp(string starttimestamp) {
	int year, month, day, hour, minute;
	if (sscanf_s(starttimestamp.c_str(), "%4d%2d%2d%2d%2d", &year, &month, &day, &hour, &minute) != 5) {
		return "";
	}

	// Create tm structure
	tm timeStruct = {};
	timeStruct.tm_year = year - 1900;
	timeStruct.tm_mon = month - 1;
	timeStruct.tm_mday = day;
	timeStruct.tm_hour = hour;
	timeStruct.tm_min = minute;

	// Convert to time_t and add 2 hours
	time_t startTime = mktime(&timeStruct);
	startTime += 2 * 3600;

	// Convert back to tm structure
	tm newTime;
	localtime_s(&newTime, &startTime);

	// Format new timestamp as "yyyymmddhhmm"
	stringstream ss;
	ss << put_time(&newTime, "%Y%m%d%H%M");

	return ss.str();
}

void assignPlayersRecursive(Schedule* node, Player*& playerHead, const string& tournamentName, int tournamentLevel) {
	if (node == nullptr)
		return;

	assignPlayersRecursive(node->left, playerHead, tournamentName, tournamentLevel);

	// Check if schedule matches the criteria.
	if ((node->tournamentName == tournamentName) && (node->tournamentlevel == tournamentLevel) && (node->status == "Pending")) {
		// Find player 1
		Player* p1 = nullptr;
		while (playerHead != nullptr && playerHead->tournamentlevel != tournamentLevel) {
			cout << "Skipping " << playerHead->name << " (level " << playerHead->tournamentlevel << ")\n";
			playerHead = playerHead->nextaddress;
		}
		if (playerHead != nullptr) {
			p1 = playerHead;
			node->player1 = p1->name;
			playerHead = playerHead->nextaddress; // Move to the next player
		}

		// Find player 2
		Player* p2 = nullptr;
		while (playerHead != nullptr && playerHead->tournamentlevel != tournamentLevel) {
			cout << "Skipping " << playerHead->name << " (level " << playerHead->tournamentlevel << ")\n";
			playerHead = playerHead->nextaddress;
		}
		if (playerHead != nullptr) {
			p2 = playerHead;
			node->player2 = p2->name;
			playerHead = playerHead->nextaddress; // Move to the next player
		}

		cout << "Assigned Match: " << node->player1 << " vs. " << node->player2
			<< " in " << node->tournamentName << " (Level " << tournamentLevel << ")\n";
	}

	assignPlayersRecursive(node->right, playerHead, tournamentName, tournamentLevel);
}


void assignPlayersToSchedule(BST& scheduleTree, PlayerList& playerList, string tournamentName, int tournamentLevel) {
	Player* playerHead = playerList.getHead();

	if (playerHead == nullptr) {
		cout << "No players available in the list!\n";
		return;
	}

	assignPlayersRecursive(scheduleTree.getRoot(), playerHead, tournamentName, tournamentLevel);
}

void assignWinner(BST& schedule1, PlayerList& playerList, int no) {
	assignWinnerRecursive(schedule1.getRoot(), playerList.getHead(), no);
}

Schedule* assignWinnerRecursive(Schedule* current, Player* player, int no) {
	if (current == nullptr)
		return current;
	assignWinnerRecursive(current->left, player, no);
	if (current->no == no) {
		string winner, winningplayer;
		cout << "Assign a winner (Player 1/Player 2): ";
		getline(cin, winner);
		cin.clear();

		if (winner == "Player 1") {
			current->winners = current->player1;
			winningplayer = current->player1;
			current->status = "Completed";
		}
		else if (winner == "Player 2") {
			current->winners = current->player2;
			winningplayer = current->player2;
			current->status = "Completed";
		}
		else {
			cout << "Invalid input. Please enter Player 1 or Player 2.\n";
			return current;
		}

		Player* temp = player;
		while (temp != nullptr) {
			if (temp->name == winningplayer) {
				temp->tournamentlevel += 1;
				cout << "Updated " << temp->name << " to tournament level " << temp->tournamentlevel << endl;
				break;
			}
			temp = temp->nextaddress;
		}

		cout << "Match winner assigned successfully.\n";
		return current;
	}
	assignWinnerRecursive(current->right, player, no);
	return current;
}

bool loadFile(string filepath, BST& Schedule1) {
	ifstream file(filepath);

	if (!file.is_open()) {
		cout << "Error while opening file\n";
		return true;
	}

	string no, tournamentName, tournamentlevel, group, player1, player2, winners, location, starttimestamp, endtimestamp, status;

	string header;
	getline(file, header);

	while (getline(file, no, ',')) {
		getline(file, tournamentName, ',');
		getline(file, tournamentlevel, ',');
		getline(file, group, ',');
		getline(file, player1, ',');
		getline(file, player2, ',');
		getline(file, winners, ',');
		getline(file, location, ',');
		getline(file, starttimestamp, ',');
		getline(file, endtimestamp, ',');
		getline(file, status);

		int noint = stoi(no);
		int tournamentlevelint = stoi(tournamentlevel);

		Schedule1.insert(noint, tournamentName, tournamentlevelint, group, player1, player2, winners, location, starttimestamp, endtimestamp, status);
	}
	return false;
}

// This two functions are developed but will not be used
void writeScheduleToCSV(Schedule* node, ofstream& file) {
	if (node == nullptr)
		return;

	writeScheduleToCSV(node->left, file);

	std::ostringstream ss;
	ss << node->winners;  // Convert integer to string

	file << node->no << ","
		<< node->tournamentName << ","
		<< node->tournamentlevel << ","
		<< node->group << ","
		<< node->player1 << ","
		<< node->player2 << ","
		<< node->winners << ","
		<< node->location << ","
		<< node->starttimestamp << ","
		<< node->endtimestamp << ","
		<< node->status << "\n";

	writeScheduleToCSV(node->right, file);
}

void saveScheduleToFile(BST& Schedule1, string filename) {
	ofstream file(filename);
	if (!file.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	file << "No,TournamentName,TournamentLevel,Group,Player1,Player2,Winners,Location,StartTimestamp,EndTimestamp,Status\n";

	writeScheduleToCSV(Schedule1.getRoot(), file);

	file.close();
	cout << "Schedule data successfully saved to " << filename << endl;
}