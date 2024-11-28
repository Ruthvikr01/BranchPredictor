#ifndef HYBRIDPREDICTOR_H
#define HYBRIDPREDICTOR_H

#include "PApPredictor.h"
#include "GApPredictor.h"
#include <unordered_map>
#include <string>

class HybridPredictor {
private:
    PApPredictor pap;// Local predictor
    GApPredictor gap;// Global predictor
    std::unordered_map<std::string, int> selectorTable; // 2-bit saturating counter table

public:
    HybridPredictor(int bhrSize, int phtSize);

    // Predict branch outcome
    int predict(const std::string& address);

    // Update the predictors and selector table
    void update(const std::string& address, int outcome);
};

#endif
