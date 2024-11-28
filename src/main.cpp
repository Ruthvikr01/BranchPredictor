#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <string>
#include <sstream>
#include "PApPredictor.h"
#include "GApPredictor.h"
#include "HybridPredictor.h"

using namespace std;

// Function to read the trace file
vector<pair<string, int>> readTraceFile(const string& filename) {
    vector<pair<string, int>> traceData;
    ifstream file(filename);

    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string address;
        string outcomeStr;

        if (iss >> address >> outcomeStr) {
            int outcome = (outcomeStr == "+") ? 1 : 0;  // Convert '+' to 1, '-' to 0
            traceData.push_back({address, outcome});
        }
    }

    file.close();

    if (traceData.empty()) {
        cout << "Warning: No data was read from the trace file." << endl;
    }

    return traceData;
}

int main() {
    string traceFile = "/home/ruthvik01/Downloads/CAProject/gccSmall.trace";
    auto traceData = readTraceFile(traceFile);


    PApPredictor pap(12, 4096);          // 12-bit BHR, 4096-entry PHT
    GApPredictor gap(12, 4096);          // 12-bit BHR, 4096-entry PHT
    HybridPredictor hybrid(12, 4096); // Hybrid predictor combining PAp and GAp

    // Open output file for writing
    ofstream outputFile("HybridPredictionTrace.txt");
    if (!outputFile) {
        cerr << "Error: Unable to open output file for writing." << endl;
        return 1;
    }

    int correctPredictionsHybrid = 0;
    int correctPredictionsPap = 0;
    int correctPredictionsGap = 0;
    int totalPredictions = 0;

    for (const auto& [address, outcome] : traceData) {
        int papPrediction = pap.predict(address);
        int gapPrediction = gap.predict(address);
        int hybridPrediction = hybrid.predict(address);
        string originalOutcomeStr = (outcome == 1) ? "+" : "-";
        string hybridPredictionStr = (hybridPrediction == 1) ? "+" : "-";
        int correctnessHybrid = (hybridPrediction == outcome) ? 1 : 0;
        int correctnessPap = (papPrediction == outcome) ? 1 : 0;
        int correctnessGap = (gapPrediction == outcome) ? 1 : 0;
        if (correctnessHybrid == 1) {
            correctPredictionsHybrid++;
        }
        if (correctnessPap == 1) {
            correctPredictionsPap++;
        }
        if (correctnessGap == 1) {
            correctPredictionsGap++;
        }

        outputFile << address << "\t" << originalOutcomeStr << "\t" 
                   << hybridPredictionStr << "\t" << correctnessHybrid << "\n";

        // Update predictors
        pap.update(address, outcome);
        gap.update(address, outcome);
        hybrid.update(address, outcome);
        totalPredictions++;
    }

    outputFile.close();
    cout << "Hybrid prediction trace written to HybridPredictionTrace.txt." << endl;

    // Calculate accuracy and miscalculation percentages for all predictors
    if (totalPredictions > 0) {
        // Hybrid Predictor
        double accuracyHybrid = (correctPredictionsHybrid / (double)totalPredictions) * 100.0;
        double miscalculationHybrid = 100.0 - accuracyHybrid;

        // PAp Predictor
        double accuracyPap = (correctPredictionsPap / (double)totalPredictions) * 100.0;
        double miscalculationPap = 100.0 - accuracyPap;

        // GAp Predictor
        double accuracyGap = (correctPredictionsGap / (double)totalPredictions) * 100.0;
        double miscalculationGap = 100.0 - accuracyGap;

        cout << "\nPAp Predictor Results:" << endl;
        cout << "-------------------------" << endl;
        cout << "Total Predictions: " << totalPredictions << endl;
        cout << "Correct Predictions: " << correctPredictionsPap << endl;
        cout << "Accuracy: " << accuracyPap << "%" << endl;
        cout << "Miscalculation Rate: " << miscalculationPap << "%" << endl;

        cout << "\nGAp Predictor Results:" << endl;
        cout << "-------------------------" << endl;
        cout << "Total Predictions: " << totalPredictions << endl;
        cout << "Correct Predictions: " << correctPredictionsGap << endl;
        cout << "Accuracy: " << accuracyGap << "%" << endl;
        cout << "Miscalculation Rate: " << miscalculationGap << "%" << endl;

        cout << "\nHybrid Predictor Results:" << endl;
        cout << "-------------------------" << endl;
        cout << "Total Predictions: " << totalPredictions << endl;
        cout << "Correct Predictions: " << correctPredictionsHybrid << endl;
        cout << "Accuracy: " << accuracyHybrid << "%" << endl;
        cout << "Miscalculation Rate: " << miscalculationHybrid << "%" << endl;
    } else {
        cout << "No predictions were made. Check the trace file content." << endl;
    }

    return 0;
}
