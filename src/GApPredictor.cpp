#include "GApPredictor.h"
#include <cmath>

GApPredictor::GApPredictor(int bhrSize, int phtSize) 
    : bhrSize(bhrSize), phtSize(phtSize), globalBHR(0) {
    // Initialize the PHT with weakly taken states (2)
    PHT = std::vector<int>(1 << bhrSize, 2); // 2^bhrSize entries initialized to 2
}

int GApPredictor::predict(const std::string& address) {
    // Use the global BHR to index into the PHT
    int historyIndex = globalBHR;
    return (PHT[historyIndex] >= 2) ? 1 : 0; // Taken if counter >= 2
}

// Update predictor state
void GApPredictor::update(const std::string& address, int outcome) {
    int historyIndex = globalBHR;

    // Update PHT counter based on the outcome
    if (outcome == 1 && PHT[historyIndex] < 3) {
        PHT[historyIndex]++; // Strengthen "taken"
    } else if (outcome == 0 && PHT[historyIndex] > 0) {
        PHT[historyIndex]--; // Strengthen "not taken"
    }

    // Update global BHR with the new outcome
    globalBHR = ((globalBHR << 1) | outcome) & ((1 << bhrSize) - 1); // Maintain BHR size
}
