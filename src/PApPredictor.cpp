#include "PApPredictor.h"
#include <vector>
#include <cmath>

PApPredictor::PApPredictor(int bhrSize, int phtSize) : bhrSize(bhrSize), phtSize(phtSize) {}


int PApPredictor::predict(const std::string& address) {

    if (BHR.find(address) == BHR.end()) {
        BHR[address] = 0; 
        PHT[address] = std::vector<int>(1 << bhrSize, 2); // Initialize 2-bit counters to weakly taken
    }

    // Use the BHR to index into the PHT
    int historyIndex = BHR[address];
    return (PHT[address][historyIndex] >= 2) ? 1 : 0; // Taken if counter >= 2
}

// Update branch history and pattern table
void PApPredictor::update(const std::string& address, int outcome) {
    if (BHR.find(address) == BHR.end()) {
        BHR[address] = 0;
        PHT[address] = std::vector<int>(1 << bhrSize, 2);
    }

    int historyIndex = BHR[address];

    // Update PHT counter based on the outcome
    if (outcome == 1 && PHT[address][historyIndex] < 3) {
        PHT[address][historyIndex]++; // Strengthen "taken"
    } else if (outcome == 0 && PHT[address][historyIndex] > 0) {
        PHT[address][historyIndex]--; // Strengthen "not taken"
    }

    // Update BHR with the new outcome
    BHR[address] = ((BHR[address] << 1) | outcome) & ((1 << bhrSize) - 1); // Maintain BHR size
}
