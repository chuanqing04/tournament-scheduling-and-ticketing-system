#include "TournamentScheduling.hpp"
#include "Player.hpp"
#include "PlayerWithdrawal.hpp"
#include "TicketSystem.hpp"

int main() {
	string filename1, filename2;
	cout << "File Import, enter your schedule file name:  ";
	getline(cin, filename1);
	cin.clear();

	BST Schedule1(filename1);
	if (loadFile(filename1, Schedule1))
		return 1;

	cout << "Enter player file: ";
	getline(cin, filename2);
	cin.clear();
	PlayerList playerList("Players");
	PlayerWithdrawal Withdraw(playerList, Schedule1);
	if (playermain(filename2, playerList))
		return 1;

	TicketQueue system(&Schedule1);

	//Main Menu
	while (true) {
		string option;
		cout <<
			"-------------------------------------------------------\n"
			"                      Main Menu\n"
			"-------------------------------------------------------\n"
			"1. Manage Schedule\n"
			"2. Manage Ticket System\n"
			"3. Manage Players\n"
			"4. Manage Match\n"
			"0. Exit\n"
			"Enter your option: ";
		cin >> option;
		cin.ignore();

		if (option == "1") {
			while (true) {
				cout <<
					"-------------------------------------------------------\n"
					"               Schedule Management Menu\n"
					"-------------------------------------------------------\n"
					"1. Add Schedule\n"
					"2. Delete Schedule\n"
					"3. Filter Schedule\n"
					"4. Display all Schedules\n"
					"5. Assign Winner\n"
					"0. Back to Main Menu\n"
					"Enter your option: ";
				cin >> option;
				cin.ignore();

				if (option == "1") {
					addSchedule(Schedule1);
				}
				else if (option == "2") {
					string tournamentID;
					cout << "Enter tournament ID to delete: ";
					getline(cin, tournamentID);
					cin.clear();

					bool isValid = true;
					for (char c : tournamentID) {
						if (!isdigit(c)) {
							isValid = false;
							break;
						}
					}

					if (!isValid) {
						cout << "Invalid input. Match ID must be a number.\n";
					}
					else {
						int tournamentIDint = stoi(tournamentID);
						Schedule1.removeNode(tournamentIDint);
					}
				}
				else if (option == "3") {
					string choice, query;
					cout << "Enter filter option (tournamentname, location, starttimestamp): ";
					getline(cin, choice);
					cin.clear();

					cout << "Enter Query: ";
					getline(cin, query);
					cin.clear();

					Schedule1.inorder(choice, query);
				}
				else if (option == "4") {
					Schedule1.inorder();
					cout << "Count: " << Schedule1.CountNodes() << endl;
				}
				else if (option == "5") {
					string no;
					cout << "Enter tournament match id: ";
					getline(cin, no);
					cin.clear();

					bool isValid = true;
					for (char c : no) {
						if (!isdigit(c)) {
							isValid = false;
							break;
						}
					}

					if (!isValid) {
						cout << "Invalid input. Match ID must be a number.\n";
					}
					else {
						int noint = stoi(no);
						assignWinner(Schedule1, playerList, noint);
					}
				}
				else if (option == "0") {
					break;
				}
				else {
					cout << "Invalid input." << endl;
				}
			}
		}
		else if (option == "2") {
			while (true) {
				cout <<
					"-------------------------------------------------------\n"
					"                Ticket Management Menu\n"
					"-------------------------------------------------------\n"
					"1. View Matches\n"
					"2. Purchase Ticket\n"
					"3. Process Entry\n"
					"4. Process Exit\n"
					"5. Display All Tickets\n"
					"6. Open Match\n"
					"0. Back to Main Menu\n"
					"Enter your option: ";
				cin >> option;
				cin.ignore();
				if (option == "1") {
					system.displayMatches();
				}
				else if (option == "2") {
					string path;
					cout << "\nChoose Purchase Type:\n";
					cout << "1. Manual\n";
					cout << "2. Random\n";
					cout << "Enter your option: ";
					cin >> path;
					if (path == "1")
					{
						string name;
						int matchID, ticketType;
						cout << "Enter your name: ";
						cin >> name;
						cout << "\nChoose Ticket Type:\n";
						cout << "1. VIP\n";
						cout << "2. EarlyBird\n";
						cout << "3. Standard\n";
						cout << "Enter your choice (1-3): ";
						cin >> ticketType;
						cout << "Enter match ID: ";
						cin >> matchID;
						system.purchaseTicket(name, ticketType, matchID);
					}
					else if (path == "2")
					{
						int num;
						cout << "Enter number of random tickets to purchase: ";
						cin >> num;
						system.randomTicketPurchase(num);
					}
					else
					{
						cout << "Invalid input." << endl;
					}
				}
				else if (option == "3") {
					int numEntries;
					cout << "Enter number of spectators to process for entry: ";
					cin >> numEntries;
					system.processEntry(numEntries);
				}
				else if (option == "4") {
					int numExits;
					cout << "Enter number of spectators to process for exit: ";
					cin >> numExits;
					system.processExit(numExits);
				}
				else if (option == "5") {
					system.displayAllTickets();
				}
				else if (option == "6") {
					int matchID;
					cout << "Enter MatchID to Open: ";
					cin >> matchID;
					system.openMatch(matchID);
				}
				else if (option == "0") {
					break;
				}
				else {
					cout << "Invalid input." << endl;
				}
			}
		}
		else if (option == "3") {
			while (true) {
				cout <<
					"-------------------------------------------------------\n"
					"                Player Management Menu\n"
					"-------------------------------------------------------\n"
					"1. Display all Players\n"
					"2. Assign Players\n"
					"3. Withdraw Players\n"
					"0. Back to Main Menu\n"
					"Enter your option: ";
				cin >> option;
				cin.ignore();

				if (option == "1") {
					playerList.DisplayList();
				}
				else if (option == "2") {
					string tournamentName, tournamentLevel;
					cout << "Enter tournament name: ";
					getline(cin, tournamentName);
					cin.clear();

					cout << "Enter tournament level: ";
					getline(cin, tournamentLevel);
					cin.clear();
					bool isNumeric = all_of(tournamentLevel.begin(), tournamentLevel.end(), ::isdigit);
					if (!isNumeric) {
						cout << "Invalid input. Please enter a number.\n";
						continue;
					}
					int tournamentLevelint = stoi(tournamentLevel);
					assignPlayersToSchedule(Schedule1, playerList, tournamentName, tournamentLevelint);
				}
				else if (option == "3") {
					string playerName;
					string reason;
					cout << "Enter Player Name to withdraw: ";
					getline(cin, playerName);
					cin.clear();
					cout << "Enter withdrawal reason: ";
					getline(cin, reason);
					cin.clear();

					bool Result;
					Result = Withdraw.withdrawPlayer(playerList, playerName, reason);
					if (Result == true) {
						while (true) {
							cout <<
								"Are you sure you want to proceed?\n"
								"1. Proceed\n"
								"2. Undo\n"
								"Enter your option: ";
							cin >> option;
							cin.ignore();

							if (option == "1") {
								Withdraw.updateWithdrawal(playerName);
								break;
							}
							else if (option == "2") {
								Withdraw.undoWithdrawal();
								break;
							}
							else {
								cout << "Invalid input." << endl;
							}
						}
					}
					else {
						cout << "Player Name not found." << endl;
						continue;
					}
				}
				else if (option == "0") {
					break;
				}
				else {
					cout << "Invalid input." << endl;
				}

			}
		}
		else if (option == "4") {
			while (true) {
				cout <<
					"-------------------------------------------------------\n"
					"                Match History Tracking\n"
					"-------------------------------------------------------\n"
					"1. Display Match History\n"
					"2. Record Match Result\n"
					"0. Back to Main Menu\n"
					"Enter your option: ";
				cin >> option;
				cin.ignore();

				if (option == "1") {
					Schedule1.displayMatchHistory();
				}
				else if (option == "2") {
					int matchNo;
					string tournamentName, player1, player2, winners, location, startTimestamp, endTimestamp, group, status;
					int tournamentLevel, player1Set1Score, player2Set1Score, player1Set2Score, player2Set2Score, player1Set3Score, player2Set3Score;

					cout << "Enter Match Number: ";
					cin >> matchNo;
					cin.ignore(); // Consume the newline character

					Schedule* foundSchedule = Schedule1.findScheduleByNo(Schedule1.getRoot(), matchNo);
					if (foundSchedule) {
						tournamentName = foundSchedule->tournamentName;
						tournamentLevel = foundSchedule->tournamentlevel;
						group = foundSchedule->group;
						player1 = foundSchedule->player1;
						player2 = foundSchedule->player2;
						location = foundSchedule->location;
						startTimestamp = foundSchedule->starttimestamp;
						endTimestamp = foundSchedule->endtimestamp;
						status = foundSchedule->status;

						cout << "Enter Set 1 Score - " << player1 << ": ";
						cin >> player1Set1Score;
						cout << "Enter Set 1 Score - " << player2 << ": ";
						cin >> player2Set1Score;
						cin.ignore();

						cout << "Enter Set 2 Score - " << player1 << ": ";
						cin >> player1Set2Score;
						cout << "Enter Set 2 Score - " << player2 << ": ";
						cin >> player2Set2Score;
						cin.ignore();

						cout << "Enter Set 3 Score - " << player1 << ": ";
						cin >> player1Set3Score;
						cout << "Enter Set 3 Score - " << player2 << ": ";
						cin >> player2Set3Score;
						cin.ignore();

						cout << "Enter winner: ";
						getline(cin, winners);
						Schedule1.recordMatch(matchNo, tournamentName, tournamentLevel, group, player1, player2, winners, player1Set1Score, player2Set1Score, player1Set2Score, player2Set2Score, player1Set3Score, player2Set3Score, location, startTimestamp, endTimestamp, status);
					}
					else {
						cout << "Not found schedule";
					}
				}
				else if (option == "0") {
					break;
				}
				else {
					cout << "Invalid input." << endl;
				}
			}
		}
		else if (option == "0")
			break;
		else {
			cout << "Invalid input." << endl;
		}
	};

	//saveScheduleToFile(Schedule1, filename1);
	//savePlayersToFile(playerList, filename2);
	return 0;
}
