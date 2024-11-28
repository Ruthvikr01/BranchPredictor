#include "HybridPredictor.h"

HybridPredictor::HybridPredictor(int bhrSize, int phtSize)
    : pap(bhrSize, phtSize), gap(bhrSize, phtSize) {}


int HybridPredictor::predict(const std::string& address) {
    if (selectorTable.find(address) == selectorTable.end()) {
        selectorTable[address] = 2;  // Default to PAp (2-bit saturating counter neutral)
    }

    int papPrediction = pap.predict(address);
    int gapPrediction = gap.predict(address);

    // Use the selector table to choose between PAp and GAp
    return (selectorTable[address] >= 2) ? papPrediction : gapPrediction;
}


void HybridPredictor::update(const std::string& address, int outcome) {
    int papPrediction = pap.predict(address);
    int gapPrediction = gap.predict(address);

    // Update the PAp and GAp predictors
    pap.update(address, outcome);
    gap.update(address, outcome);

    // Update the selector table based on the predictors' performance
    if (papPrediction == outcome && gapPrediction != outcome) {
        if (selectorTable[address] < 3) {
            selectorTable[address]++;
        }
    } else if (gapPrediction == outcome && papPrediction != outcome) {
        if (selectorTable[address] > 0) {
            selectorTable[address]--;
        }
    }
}
