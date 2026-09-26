#pragma once

#include <iostream>
#include <string>

// Structure to represent a match result
struct MatchResult {
    int matchNo;
    std::string tournamentName;
    int tournamentLevel;
    std::string group;
    std::string player1;
    std::string player2;
    std::string winners;
    int player1Set1Score;
    int player2Set1Score;
    int player1Set2Score;
    int player2Set2Score;
    int player1Set3Score;
    int player2Set3Score;
    std::string location;
    std::string startTimestamp;
    std::string endTimestamp;
    std::string status;
};

class MatchStack {
private:
    struct StackNode {
        MatchResult data;
        StackNode* next;
    };
    StackNode* top;

public:
    MatchStack();
    ~MatchStack();
    void push(const MatchResult& match);
    MatchResult pop();
    MatchResult peek() const;
    bool isEmpty() const;
    void displayHistory() const;

    // Function to record match result (Key Action 1)
    void recordMatchResult(int matchNo, std::string tournamentName, int tournamentLevel, std::string group, std::string player1, std::string player2, std::string winners, int player1Set1Score, int player2Set1Score, int player1Set2Score, int player2Set2Score, int player1Set3Score, int player2Set3Score, std::string location, std::string startTimestamp, std::string endTimestamp, std::string status);
};

