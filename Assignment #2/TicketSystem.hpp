#ifndef TICKETSYSTEM_HPP
#define TICKETSYSTEM_HPP

#include <iostream>
#include <queue>
#include <string>
#include <tuple>  
#include <iomanip>
#include <unordered_map>
#include "TournamentScheduling.hpp"  // Include BST structure

using namespace std;

struct MatchNode {
    int matchID;
    string player1;
    string player2;
    string location;
    string round;
    string status;
    MatchNode* next;

    MatchNode(int id, string p1, string p2, string loc, string r, string s)
        : matchID(id), player1(p1), player2(p2), location(loc), round(r), status(s), next(nullptr) {
    }
};

struct TicketNode {
    int ticket_id;
    string name;
    string ticket_type;
    int match_id;
    TicketNode* next;

    TicketNode(int id, string n, string type, int m_id)
        : ticket_id(id), name(n), ticket_type(type), match_id(m_id), next(nullptr) {
    }
};

class TicketList {
private:
    TicketNode* head;

public:
    TicketList() : head(nullptr) {}

    void addTicket(int ticket_id, string name, string ticket_type, int match_id) {
        TicketNode* newTicket = new TicketNode(ticket_id, name, ticket_type, match_id);
        if (!head) {
            head = newTicket;
        }
        else {
            TicketNode* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newTicket;
        }
    }

    TicketNode* getHead() const { return head; }
};

class TicketQueue {
private:
    priority_queue<tuple<int, int, string>> queue;
    TicketList ticketList;
    BST* scheduleTree; // Pointer to BST structure
    MatchNode* matchHead;
    int ticketCounter = 101;
    std::queue<std::pair<int, std::string>> insideVenueQueue;
    unordered_map<string, int> arenaCapacity = { {"Arena A", 200}, {"Arena B", 200}, {"Arena C", 200} };
    unordered_map<string, int> currentArenaOccupancy;
    unordered_map<int, int> ticketCount;
    const int MAX_TICKETS_PER_MATCH = 200;

    string getTournamentLevelName(int level) {
        switch (level) {
        case 1: return "Qualifiers";
        case 2: return "Knockouts";
        case 3: return "Quarterfinals";
        case 4: return "Semifinals";
        case 5: return "Finals";
        default: return "Unknown";
        }
    }
    void displayMatchesFromBST(Schedule* node) {
        if (node == nullptr) return;

        displayMatchesFromBST(node->left);

        cout << "| " << setw(8) << right << node->no << " | "
            << setw(20) << left << node->player1 << " | "
            << setw(20) << left << node->player2 << " | "
            << setw(10) << left << node->location << " | "
            << setw(14) << left << getTournamentLevelName(node->tournamentlevel) << " | "
            << setw(10) << left << node->status << " |\n";

        displayMatchesFromBST(node->right);
    }

public:
    TicketQueue(BST* schedule) : scheduleTree(schedule) {}

    void displayAllTickets() {
        cout << "\nPurchased Tickets (Grouped by Ticket Type):\n";
        cout << "| Ticket ID  | Name             | Ticket Type   | Match ID |\n";
        cout << "|------------|-----------------|--------------|----------|\n";

        if (!ticketList.getHead()) {
            cout << "No tickets have been purchased yet.\n";
            return;
        }

        // Separate linked lists for each ticket type
        TicketNode* vipHead = nullptr, * vipTail = nullptr;
        TicketNode* earlyBirdHead = nullptr, * earlyBirdTail = nullptr;
        TicketNode* standardHead = nullptr, * standardTail = nullptr;

        TicketNode* current = ticketList.getHead();

        // **Step 1: Categorize Tickets into Separate Linked Lists**
        while (current) {
            TicketNode* newNode = new TicketNode(*current);  // Create a copy

            if (current->ticket_type == "VIP") {
                if (!vipHead) vipHead = vipTail = newNode;
                else {
                    vipTail->next = newNode;
                    vipTail = newNode;
                }
            }
            else if (current->ticket_type == "Early Bird") {
                if (!earlyBirdHead) earlyBirdHead = earlyBirdTail = newNode;
                else {
                    earlyBirdTail->next = newNode;
                    earlyBirdTail = newNode;
                }
            }
            else if (current->ticket_type == "Standard") {
                if (!standardHead) standardHead = standardTail = newNode;
                else {
                    standardTail->next = newNode;
                    standardTail = newNode;
                }
            }
            current = current->next;
        }

        // **Step 2: Display Tickets in Order (VIP → Early Bird → Standard)**
        printTickets(vipHead);
        printTickets(earlyBirdHead);
        printTickets(standardHead);

        // **Step 3: Free Memory (Avoid Memory Leaks)**
        deleteList(vipHead);
        deleteList(earlyBirdHead);
        deleteList(standardHead);
    }

    TicketNode* sortTickets(TicketNode* head) {
        if (!head || !head->next) return head;

        bool swapped;
        TicketNode* ptr1;
        TicketNode* lptr = nullptr;

        do {
            swapped = false;
            ptr1 = head;

            while (ptr1->next != lptr) {
                if (ptr1->ticket_id > ptr1->next->ticket_id) {
                    swap(ptr1->ticket_id, ptr1->next->ticket_id);
                    swap(ptr1->name, ptr1->next->name);
                    swap(ptr1->ticket_type, ptr1->next->ticket_type);
                    swap(ptr1->match_id, ptr1->next->match_id);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);

        return head;
    }
    void printTickets(TicketNode* head) {
        TicketNode* current = head;
        while (current) {
            cout << "| " << setw(10) << left << current->ticket_id << " | "
                << setw(15) << left << current->name << " | "
                << setw(12) << left << current->ticket_type << " | "
                << setw(8) << left << current->match_id << " |\n";
            current = current->next;
        }
    }

    void deleteList(TicketNode* head) {
        while (head) {
            TicketNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void displayMatches() {
        cout << "\n Tennis Matches List:\n";
        cout << "| Match ID | Player 1             | Player 2             | Location   | Tournament     | Status     |\n";
        cout << "|----------|----------------------|----------------------|------------|----------------|------------|\n";
        displayMatchesFromBST(scheduleTree->getRoot());
    }

    void purchaseTicket(string name, int ticketType, int matchID) {
        Schedule* match = scheduleTree->findScheduleByNo(scheduleTree->getRoot(), matchID);
        if (!match) {
            cout << "Match ID " << matchID << " not found!\n";
            return;
        }

        if (match->status == "Completed") {
            cout << "Ticket purchase failed: Match " << matchID << " is already closed.\n";
            return;
        }

        if (ticketCount.find(matchID) == ticketCount.end()) {
            ticketCount[matchID] = 0;
        }

        if (ticketCount[matchID] >= MAX_TICKETS_PER_MATCH) {
            cout << "Ticket purchase failed: Match " << matchID
                << " in " << match->location << " has reached its limit of " << MAX_TICKETS_PER_MATCH << ".\n";
            return;
        }

        string ticketTypeStr;
        switch (ticketType) {
        case 1: ticketTypeStr = "VIP"; break;
        case 2: ticketTypeStr = "Early Bird"; break;
        case 3: ticketTypeStr = "Standard"; break;
        default:
            cout << "Invalid ticket type! Choose 1 (VIP), 2 (Early Bird), or 3 (Standard).\n";
            return;
        }

        ticketCounter++;
        ticketList.addTicket(ticketCounter, name, ticketTypeStr, matchID);
        ticketCount[matchID]++;

        queue.push(make_tuple(ticketType, ticketCounter, name));

        cout << "Ticket successfully purchased! Ticket ID: " << ticketCounter
            << " for Match " << matchID << " at " << match->location
            << " (" << ticketTypeStr << " ticket)" << endl;
    }

    void randomTicketPurchase(int numTickets) {
        if (!scheduleTree || !scheduleTree->getRoot()) {
            cout << "No matches available for ticket purchase.\n";
            return;
        }

        srand(time(0));
        string names[] = { "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Hannah", "Ivan", "Jack" };
        int ticketTypes[] = { 1, 2, 3 };
        int nameSize = sizeof(names) / sizeof(names[0]);
        int ticketSize = sizeof(ticketTypes) / sizeof(ticketTypes[0]);

        // Step 1: Count number of matches
        int totalMatches = scheduleTree->CountNodes();
        if (totalMatches == 0) {
            cout << "No matches available for ticket purchase.\n";
            return;
        }

        Schedule** matches = new Schedule * [totalMatches];
        int index = 0;
        storeMatchesInArray(scheduleTree->getRoot(), matches, index);

        // Step 3: Randomly select matches for ticket purchase
        for (int i = 0; i < numTickets; i++) {
            int randomMatchIndex = rand() % totalMatches;
            Schedule* selectedMatch = matches[randomMatchIndex];

            if (!selectedMatch) continue;

            string matchStatus = selectedMatch->status;
            transform(matchStatus.begin(), matchStatus.end(), matchStatus.begin(), ::tolower);

            if (matchStatus == "completed") {
                cout << "Skipping match " << selectedMatch->no << " - already completed.\n";
                continue;
            }

            string randomName = names[rand() % nameSize];
            int randomTicketType = ticketTypes[rand() % ticketSize];

            cout << "\nSimulating Ticket Purchase...\n";
            cout << "Name: " << randomName << ", Ticket Type: " << randomTicketType
                << ", Match ID: " << selectedMatch->no << "\n";

            purchaseTicket(randomName, randomTicketType, selectedMatch->no);
        }
    }

    // Helper function to store BST nodes in an array
    void storeMatchesInArray(Schedule* node, Schedule* matches[], int& index) {
        if (!node) return;
        storeMatchesInArray(node->left, matches, index);
        matches[index++] = node;
        storeMatchesInArray(node->right, matches, index);
    }

    void processEntry(int numEntries) {
        if (queue.empty()) {
            cout << "No one is in the queue for entry.\n";
            return;
        }

        cout << "Processing " << numEntries << " spectators for entry.\n";

        int count = 0;
        while (!queue.empty() && count < numEntries) {
            tuple<int, int, string> topEntry = queue.top();
            int priority = get<0>(topEntry);
            int ticketID = get<1>(topEntry);
            string name = get<2>(topEntry);
            queue.pop();  // Remove from queue

            cout << "Processing Ticket ID: " << ticketID << " for " << name << "...\n";

            TicketNode* temp = ticketList.getHead();
            while (temp) {
                if (temp->ticket_id == ticketID) {
                    Schedule* match = scheduleTree->findScheduleByNo(scheduleTree->getRoot(), temp->match_id);
                    if (!match || match->status != "Open") {
                        cout << "Entry denied: Match " << temp->match_id << " is not open.\n";
                        return;
                    }

                    insideVenueQueue.push({ ticketID, name });
                    cout << name << " (Ticket ID: " << ticketID << ") has entered the venue.\n";
                    count++;
                    break;
                }
                temp = temp->next;
            }
        }
    }

    void processExit(int numExits) {
        if (insideVenueQueue.empty()) {
            cout << "No spectators inside the venue.\n";
            return;
        }

        int count = 0;
        while (count < numExits && !insideVenueQueue.empty()) {
            pair<int, string> frontEntry = insideVenueQueue.front();
            insideVenueQueue.pop();

            cout << frontEntry.second << " (Ticket ID: " << frontEntry.first << ") has exited the venue.\n";
            count++;
        }

        cout << count << " spectators exited the venue.\n";
    }

    void openMatch(int matchID) {
        Schedule* match = scheduleTree->findScheduleByNo(scheduleTree->getRoot(), matchID);
        if (!match) {
            cout << "Match ID " << matchID << " not found!\n";
            return;
        }
        match->status = "Open";
        cout << "Match " << matchID << " is now Open.\n";
    }
};

#endif // TICKETSYSTEM_HPP
