#ifndef GAPPREDICTOR_H
#define GAPPREDICTOR_H

#include <vector>
#include <string>

class GApPredictor {
private:
    int bhrSize;               // Size of the global branch history register
    int phtSize;               // Size of the pattern history table
    int globalBHR;             // Global branch history register
    std::vector<int> PHT;      // Pattern history table (2-bit saturating counters)

public:
    GApPredictor(int bhrSize, int phtSize);
    int predict(const std::string& address); // Predict branch outcome
    void update(const std::string& address, int outcome); // Update predictor state
};

#endif
