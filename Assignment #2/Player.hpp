/*Load PLayer file in linked list for usage in
Tournament Scheduling.*/

#pragma once
// Forward declaration of Schedule if needed:
class Schedule;

#include "TournamentScheduling.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Player {
    int ID;
    string name;
    int elo;
    int withdrawstatus;
    string withdrawreason;
    int tournamentlevel;
    Player* nextaddress;
};


class PlayerList {
    Player* head = nullptr;
    int size = 0;
    string listname;

public:
    PlayerList(string listname) {
        this->listname = listname;
    }

    Player* getHead() {
        return head;
    }

    Player* CreateNewNode(int ID, string name, int elo, int withdrawstatus, string withdrawreason, int tournamentlevel) {
        Player* newnode = new Player();
        newnode->ID = ID;
        newnode->name = name;
        newnode->elo = elo;
        newnode->withdrawstatus = withdrawstatus;
        newnode->withdrawreason = withdrawreason;
        newnode->tournamentlevel = tournamentlevel;
        newnode->nextaddress = nullptr;
        return newnode;
    }

    void InsertAtEndList(int ID, string name, int elo, int withdrawstatus, string withdrawreason, int tournamentlevel) {
        Player* newnode = CreateNewNode(ID, name, elo, withdrawstatus, withdrawreason, tournamentlevel);
        if (head == nullptr) {
            head = newnode;
        }
        else {
            Player* temp = head;
            while (temp->nextaddress != nullptr) {
                temp = temp->nextaddress;
            }
            temp->nextaddress = newnode;
        }
        size++;
    }

    void DeleteAtFirstList() {
        if (head == nullptr)
            return;
        Player* temp = head;
        head = head->nextaddress;
        delete temp;
        size--;
    }

    void DisplayList() {
        Player* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->ID << ", Name: " << temp->name << ", ELO: " << temp->elo << ", Withdraw Status: " << temp->withdrawstatus << ", Withdraw Reason: " << temp->withdrawreason << ", Tournament level: " << temp->tournamentlevel << "\n";
            temp = temp->nextaddress;
        }
    }

    void sortPlayersByElo() {
        if (head == nullptr)
            return;

        bool swapped;
        do {
            swapped = false;
            Player* current = head;
            Player* prev = nullptr;

            while (current->nextaddress) {
                if (current->elo < current->nextaddress->elo) {
                    Player temp = *current;
                    *current = *current->nextaddress;
                    *current->nextaddress = temp;

                    Player* tempNext = current->nextaddress->nextaddress;
                    current->nextaddress->nextaddress = current;
                    current->nextaddress = tempNext;

                    swapped = true;
                }
                prev = current;
                current = current->nextaddress;
            }
        } while (swapped);
    }
};

bool playermain(string filename, PlayerList& Players) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error while opening file\n";
        return true;
    }

    string ID, name, eloStr, withdrawreason, tournamentlevel, withdrawstatus;

    string header;
    getline(file, header);

    while (getline(file, ID, ',')) {
        getline(file, name, ',');
        getline(file, eloStr, ',');
        getline(file, withdrawstatus, ',');
        getline(file, withdrawreason, ',');
        getline(file, tournamentlevel, '\n');

        int noID = stoi(ID);
        int elo = stoi(eloStr);
        int tournamentlevelint = stoi(tournamentlevel);
        int withdrawstatusint = stoi(withdrawstatus);

        Players.InsertAtEndList(noID, name, elo, withdrawstatusint, withdrawreason, tournamentlevelint);
    }
    Players.sortPlayersByElo();
    return false;
}

void savePlayersToFile(PlayerList& playerList, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    file << "ID,Name,ELO,WithdrawStatus,WithdrawReason,TournamentLevel\n";

    Player* temp = playerList.getHead();
    while (temp != nullptr) {
        file << temp->ID << ","
            << temp->name << ","
            << temp->elo << ","
            << temp->withdrawstatus << ","
            << temp->withdrawreason << ","
            << temp->tournamentlevel << "\n";
        temp = temp->nextaddress;
    }

    file.close();
    cout << "Player data successfully saved to " << filename << endl;
}