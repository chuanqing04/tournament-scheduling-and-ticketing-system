#include "MatchTracking.hpp"

#include <iostream>

MatchStack::MatchStack() : top(nullptr) {}

MatchStack::~MatchStack() {
    StackNode* current = top;
    while (current != nullptr) {
        StackNode* next = current->next;
        delete current;
        current = next;
    }
    top = nullptr;
}

void MatchStack::push(const MatchResult& match) {
    StackNode* newNode = new StackNode{ match, top };
    top = newNode;
    std::cout << "Match recorded: " << match.tournamentName << ", " << match.player1
        << " vs " << match.player2 << std::endl;
}

MatchResult MatchStack::pop() {
    if (isEmpty()) {
        std::cerr << "Error: Stack is empty.\n";
        return {}; // Return a default MatchResult
    }
    StackNode* temp = top;
    MatchResult result = temp->data;
    top = top->next;
    delete temp;
    return result;
}

MatchResult MatchStack::peek() const {
    if (isEmpty()) {
        std::cerr << "Error: Stack is empty.\n";
        return {}; // Return a default MatchResult
    }
    return top->data;
}

bool MatchStack::isEmpty() const {
    return top == nullptr;
}

void MatchStack::displayHistory() const {
    if (isEmpty()) {
        std::cout << "No match history available.\n";
        return;
    }
    std::cout << "\n--- Match History (Most Recent First) ---\n";
    StackNode* current = top;
    while (current != nullptr) {
        const MatchResult& match = current->data;
        std::cout << "Match: " << match.tournamentName << "\n"
            << "  Players: " << match.player1 << " vs " << match.player2 << "\n"
            << "  Winner: " << match.winners << "\n"
            << "  Score: " << match.player1Set1Score << "-" << match.player2Set1Score << ", " << match.player1Set2Score << "-" << match.player2Set2Score << ", " << match.player1Set3Score << "-" << match.player2Set3Score << "\n"
            << "  Location: " << match.location << " Time:" << match.startTimestamp << "-" << match.endTimestamp << "\n"
            << std::endl;
        current = current->next;
    }
    std::cout << "--- End of History ---\n";
}

// Function to record match result (Key Action 1 and 2)
void MatchStack::recordMatchResult(int matchNo, std::string tournamentName, int tournamentLevel, std::string group, std::string player1, std::string player2, std::string winners, int player1Set1Score, int player2Set1Score, int player1Set2Score, int player2Set2Score, int player1Set3Score, int player2Set3Score, std::string location, std::string startTimestamp, std::string endTimestamp, std::string status) {
    MatchResult result;
    result.matchNo = matchNo;
    result.tournamentName = tournamentName;
    result.tournamentLevel = tournamentLevel;
    result.group = group;
    result.player1 = player1;
    result.player2 = player2;
    result.winners = winners;
    result.player1Set1Score = player1Set1Score;
    result.player2Set1Score = player2Set1Score;
    result.player1Set2Score = player1Set2Score;
    result.player2Set2Score = player2Set2Score;
    result.player1Set3Score = player1Set3Score;
    result.player2Set3Score = player2Set3Score;
    result.location = location;
    result.startTimestamp = startTimestamp;
    result.endTimestamp = endTimestamp;
    result.status = status;
    push(result); // Use the push function of the MatchStack
}


