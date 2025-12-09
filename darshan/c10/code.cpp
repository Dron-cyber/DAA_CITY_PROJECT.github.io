#include <bits/stdc++.h>
using namespace std;

/*
 Water Theft Detection Logic:
 1) High Flow  + Low Pressure → Theft
 2) Unauthorized Flow (during blocked hours) → Theft
 3) Sudden Pressure Drop → Possible Tampering
*/

class WaterTheftDetector {
private:
   
    const float MAX_ALLOWED_FLOW = 5.0;  // L/min
    const float MIN_PRESSURE = 2.0;      // bar
    const float MAX_ALLOWED_DROP = 1.0;  // sudden drop bar

    
    float prevPressure = 3.0;
    bool theft = false;

public:
    
    bool isBlockedTime(int hour) {
        return (hour >= 22 || hour <= 5);
    }

    bool detect(float flow, float pressure, int hour) {
        theft = false;

       
        if (flow > MAX_ALLOWED_FLOW && pressure < MIN_PRESSURE) {
            cout << "Rule 1 Triggered: High flow + Low pressure\n";
            theft = true;
        }

       
        if (flow > 0 && isBlockedTime(hour)) {
            cout << " Rule 2 Triggered: Flow in restricted time\n";
            theft = true;
        }

       
        if (prevPressure - pressure > MAX_ALLOWED_DROP) {
            cout << " Rule 3 Triggered: Sudden pressure drop\n";
            theft = true;
        }

        prevPressure = pressure;
        return theft;
    }
};

int main() {
    WaterTheftDetector system;

   
    vector<float> flowData     = {1.2, 2.3, 6.5, 7.1, 0.0, 4.8};
    vector<float> pressureData = {3.1, 3.0, 1.4, 1.3, 0.8, 3.2};
    vector<int>    timeData    = {10, 21, 23, 1, 3, 14};  // hours

    cout << "==== Real-Time Water Theft Detection Simulation ====\n\n";

    for (int i = 0; i < flowData.size(); i++) {
        float flow = flowData[i];
        float pressure = pressureData[i];
        int hour = timeData[i];

        cout << "Minute " << i+1
             << " | Flow: " << flow
             << " L/min | Pressure: " << pressure
             << " bar | Hour: " << hour << "\n";

        bool theft = system.detect(flow, pressure, hour);

        if (theft) {
            cout << " ALERT: Possible WATER THEFT detected!\n\n";
        } else {
            cout << " Normal conditions\n\n";
        }
    }

    return 0;
}
