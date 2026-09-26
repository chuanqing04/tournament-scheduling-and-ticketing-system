#ifndef PLAYERWITHDRAWAL_HPP
#define PLAYERWITHDRAWAK_HPP

#include "Player.hpp"
#include "TournamentScheduling.hpp"

using namespace std;

struct stacknodeP {
    Player* player;
    stacknodeP* next;
};

class StackLinkedListP {
private:
    stacknodeP* top;  // Top of the stack
    int size = 0;

public:
    StackLinkedListP() {
        top = nullptr;
        size = 0;
    }

    ~StackLinkedListP() {
        cout << "Stack Linked List is deleted!" << endl;
        while (!isEmpty()) {
            Pop();
        }
    }

    bool isEmpty() {
        if (top == nullptr)
            return true;
        return false;
    }

    void Push(Player* player) {
        stacknodeP* newNode = new stacknodeP{ player, top };  // Create new node
        top = newNode;  // Move top to new node
        size++;
    }

    // For undo withdrawal
    void Pop() {
        if (!isEmpty()) {
            stacknodeP* temp = top;
            cout << "Undo withdrawal for: " << temp->player->name << endl;
            top = top->next;  // Move top to next node
            delete temp;  // Delete old top node
            size--;
        }
        else {
            cout << "Stack underflow! No withdrawals to undo." << endl;
        }
    }

    Player* Peek() {
        if (!isEmpty()) {
            return top->player;
        }
        cout << "Stack underflow!" << endl;
        return nullptr;
    }
};

class PlayerWithdrawal {
private:
    PlayerList& playerList;  // Store reference to PlayerList
    BST& tournamentSchedule;
    StackLinkedListP withdrawnPlayers;  // Stack for tracking withdrawn players

public:
    PlayerWithdrawal(PlayerList& list, BST& schedule) :playerList(list), tournamentSchedule(schedule) {}

    bool withdrawPlayer(PlayerList& playerList, string playerName, const string& reason) {
        Player* player = playerList.getHead();
        while (player != nullptr) {
            if (player->name == playerName) {
                if (player->withdrawstatus == 1) {
                    cout << "Player " << player->name << " has already withdrawn." << endl;
                    return false;
                }
                player->withdrawstatus = 1;
                player->withdrawreason = reason;
                withdrawnPlayers.Push(player);  // Push to stack

                cout << "Player " << player->name << " withdrawn due to: " << reason << endl;

                return true;
            }
            player = player->nextaddress;
        }
        return false;
    }

    void undoWithdrawal() {
        if (!withdrawnPlayers.isEmpty()) {
            Player* player = withdrawnPlayers.Peek();
            player->withdrawstatus = 0;
            player->withdrawreason = "";
            withdrawnPlayers.Pop();  // Remove from stack
        }
        else {
            cout << "No withdrawals to undo." << endl;
        }
    }

    Schedule* findMatchByPlayer(string playerName) {
        return findMatchByPlayerRecursive(tournamentSchedule.getRoot(), playerName);
    }

    Schedule* findMatchByPlayerRecursive(Schedule* node, string playerName) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->player1 == playerName || node->player2 == playerName) {
            return node;
        }

        Schedule* leftSearch = findMatchByPlayerRecursive(node->left, playerName);
        if (leftSearch) return leftSearch;

        return findMatchByPlayerRecursive(node->right, playerName);
    }

    void updateWithdrawal(string withdrawnPlayerName) {
        Schedule* match = findMatchByPlayer(withdrawnPlayerName);
        if (!match) {
            cout << "Player withdrawn." << endl;
            return;
        }

        string winnerName;
        string loserName;
        if (match->player1 == withdrawnPlayerName) {
            cout << "Player " << withdrawnPlayerName << " withdrawn.\n";
            winnerName = match->player2;
            loserName = match->player1;
        }
        else if (match->player2 == withdrawnPlayerName) {
            cout << "Player " << withdrawnPlayerName << " withdrawn." << endl;
            winnerName = match->player1;
            loserName = match->player2;
        }

        Player* player = playerList.getHead();

        int tournamentlvl = match->tournamentlevel;
        string substitute = findSubstitutePlayer(tournamentlvl, winnerName);
        if (substitute != "none") {
            cout << "Substituting withdrawn player with " << substitute << "." << endl;
            if (match->player1 == withdrawnPlayerName)
                match->player1 = substitute;
            else if (match->player2 == withdrawnPlayerName)
                match->player2 = substitute;
            string temp1 = match->starttimestamp;
            string temp2 = match->endtimestamp;
            match->starttimestamp = postpone(temp1);
            match->endtimestamp = postpone(temp2);
            cout << "Match is postponed by one day." << endl;
            return;
        }
        cout << "No substitute found." << endl;
        while (player != nullptr) {
            if (player->name == winnerName) {
                match->winners = winnerName;  // Winner is marked
                player->tournamentlevel += 1;
                cout << "Updated: " << winnerName << " (ID: " << player->ID << ") is now marked as winner." << endl;
                break;
            }
            player = player->nextaddress;
        }
    }

    string findSubstitutePlayer(int tournamentlvl, string name) {
        Player* player = playerList.getHead();
        while (player != nullptr) {
            if (player->withdrawstatus == 0 && player->tournamentlevel == tournamentlvl && player->name != name) {
                return player->name;
            }
            player = player->nextaddress;
        }
        return "none";
    }

    string postpone(string oldTime) {
        struct tm timeStruct = {};
        istringstream ss(oldTime);
        ss >> get_time(&timeStruct, "%Y%m%d%H%M");

        time_t rawTime = mktime(&timeStruct);
        rawTime += 24 * 60 * 60;

        struct tm newTime;
        localtime_s(&newTime, &rawTime);

        ostringstream newTimestamp;
        newTimestamp << put_time(&newTime, "%Y%m%d%H%M");

        return newTimestamp.str();
    }
};

#endif