#ifndef PAPPREDICTOR_H
#define PAPPREDICTOR_H
#include <unordered_map>
#include <vector>
#include <string>

class PApPredictor {
private:
    int bhrSize; // Branch history register size
    int phtSize; // Pattern history table size
    std::unordered_map<std::string, std::vector<int>> PHT; // Pattern history table
    std::unordered_map<std::string, int> BHR; // Branch history register for each branch address

public:
    PApPredictor(int bhrSize, int phtSize);
    int predict(const std::string& address); // Predict outcome
    void update(const std::string& address, int outcome); // Update prediction tables
};

#endif
