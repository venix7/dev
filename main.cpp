#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

enum class VehicleType {
    BIKE = 1,
    CAR,
    TRUCK
};

string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::BIKE:
            return "Bike";
        case VehicleType::CAR:
            return "Car";
        case VehicleType::TRUCK:
            return "Truck";
    }
    return "";
}

class Vehicle {
public:
    string number;
    VehicleType type;
    chrono::system_clock::time_point entryTime;

    Vehicle(const string& num, VehicleType t)
    : number(num), type(t), entryTime(chrono::system_clock::now()) {}

    Vehicle(const string& num, VehicleType t, chrono::system_clock::time_point time)
    : number(num), type(t), entryTime(time) {}

    Vehicle() = default;
};

class ParkingSlot {
public:
    int slotId;
    VehicleType slotType;
    bool occupied;

    ParkingSlot(int id, VehicleType type)
        : slotId(id), slotType(type), occupied(false) {}
};

class ParkingLot {
private:
    vector<ParkingSlot> slots;

    unordered_map<string, Vehicle> activeVehicles;
    unordered_map<string, int> vehicleToSlot;

public:
    ParkingLot() {
        int id = 1;

        for (int i = 0; i < 5; i++)
            slots.emplace_back(id++, VehicleType::BIKE);

        for (int i = 0; i < 10; i++)
            slots.emplace_back(id++, VehicleType::CAR);

        for (int i = 0; i < 3; i++)
            slots.emplace_back(id++, VehicleType::TRUCK);

        loadFromFile();
    }

    int findAvailableSlot(VehicleType type) {

        for (int i = 0; i < slots.size(); i++) {

            if (!slots[i].occupied && slots[i].slotType == type)
                return i;
        }

        return -1;
    }

    double calculateFee(const Vehicle& vehicle) {

        auto exitTime = chrono::system_clock::now();

        auto minutes = chrono::duration_cast<chrono::minutes>(
                           exitTime - vehicle.entryTime)
                           .count();

        int hours = (minutes + 59) / 60;

        if (hours == 0)
            hours = 1;

        double fee = 20;

        if (hours > 1)
            fee += (hours - 1) * 10;

        if (vehicle.type == VehicleType::BIKE)
            fee *= 0.5;

        else if (vehicle.type == VehicleType::TRUCK)
            fee *= 1.5;

        return fee;
    }

    void parkVehicle() {

        string number;
        int choice;

        cout << "Vehicle Number : ";
        cin >> number;

        if (activeVehicles.find(number) != activeVehicles.end()) {

            cout << "Vehicle already parked.\n";
            return;
        }

        cout << "Vehicle Type\n";
        cout << "1. Bike\n";
        cout << "2. Car\n";
        cout << "3. Truck\n";

        cin >> choice;

        if (choice < 1 || choice > 3) {

            cout << "Invalid vehicle type.\n";
            return;
        }

        VehicleType type = static_cast<VehicleType>(choice);

        int slotIndex = findAvailableSlot(type);

        if (slotIndex == -1) {

            cout << "No slot available.\n";
            return;
        }

        activeVehicles.emplace(number, Vehicle(number, type));
        vehicleToSlot[number] = slotIndex;

        slots[slotIndex].occupied = true;

        cout << "Vehicle parked successfully.\n";
        cout << "Assigned Slot : " << slots[slotIndex].slotId << "\n";

        saveToFile();
    }

    void exitVehicle() {

        string number;

        cout << "Vehicle Number : ";
        cin >> number;

        auto it = activeVehicles.find(number);

        if (it == activeVehicles.end()) {

            cout << "Vehicle not found.\n";
            return;
        }

        Vehicle vehicle = it->second;

        double fee = calculateFee(vehicle);

        int slotIndex = vehicleToSlot[number];

        slots[slotIndex].occupied = false;

        activeVehicles.erase(it);
        vehicleToSlot.erase(number);

        cout << fixed << setprecision(2);
        cout << "Parking Fee : Rs" << fee << "\n";
        cout << "Vehicle exited successfully.\n";

        saveToFile();
    }

    void displayVehicles() {

        if (activeVehicles.empty()) {

            cout << "No vehicles parked.\n";
            return;
        }

        cout << "\n------ Parked Vehicles ------\n";

        for (const auto& pair : activeVehicles) {

            cout << "Number : "
                 << pair.second.number
                 << " | Type : "
                 << vehicleTypeToString(pair.second.type)
                 << " | Slot : "
                 << slots[vehicleToSlot[pair.first]].slotId
                 << "\n";
        }
    }

    void displayAvailableSlots() {

        cout << "\n------ Available Slots ------\n";

        bool found = false;

        for (const auto& slot : slots) {

            if (!slot.occupied) {

                found = true;

                cout << "Slot "
                     << setw(2)
                     << slot.slotId
                     << " ("
                     << vehicleTypeToString(slot.slotType)
                     << ")\n";
            }
        }

        if (!found)
            cout << "Parking lot is full.\n";
    }

    void saveToFile() {

        ofstream file("parking_records.txt");

        for (const auto& pair : activeVehicles) {

            const Vehicle& vehicle = pair.second;

            time_t entry =
                chrono::system_clock::to_time_t(vehicle.entryTime);

            file << vehicle.number << " "
                << static_cast<int>(vehicle.type) << " "
                << vehicleToSlot.at(vehicle.number) << " "
                << entry << "\n";
        }
    }

    void loadFromFile() {

        ifstream file("parking_records.txt");

        if (!file)
            return;

        string number;
        int type;
        int slotIndex;
        time_t entry;

        while (file >> number >> type >> slotIndex >> entry) {

            auto tp =
                chrono::system_clock::from_time_t(entry);

            Vehicle vehicle(
                number,
                static_cast<VehicleType>(type),
                tp
            );

            activeVehicles.emplace(number, vehicle);

            vehicleToSlot[number] = slotIndex;

            slots[slotIndex].occupied = true;
        }
    }
};

void menu() {

    cout << "\n========== Parking Lot ==========\n";

    cout << "1. Park Vehicle\n";
    cout << "2. Exit Vehicle\n";
    cout << "3. Display Parked Vehicles\n";
    cout << "4. Display Available Slots\n";
    cout << "5. Exit\n";

    cout << "Choice : ";
}

int main() {

    ParkingLot parkingLot;

    int choice;

    while (true) {

        menu();

        cin >> choice;

        if (cin.fail()) {

            cin.clear();
            cin.ignore(1000, '\n');

            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {

            case 1:
                parkingLot.parkVehicle();
                break;

            case 2:
                parkingLot.exitVehicle();
                break;

            case 3:
                parkingLot.displayVehicles();
                break;

            case 4:
                parkingLot.displayAvailableSlots();
                break;

            case 5:
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }
}