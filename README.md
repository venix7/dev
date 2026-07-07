# Parking Lot Management System (C++)

A Command Line Interface (CLI) based Parking Lot Management System developed in C++ that simulates a real-world single-location parking facility. The system automatically allocates parking slots based on vehicle type, tracks entry and exit times, calculates parking fees based on parking duration, and persists active parking records using file handling.

## Features

- Park Cars, Bikes, and Trucks
- Automatic slot allocation based on vehicle type
- Prevent duplicate vehicle entries
- Track entry time using `std::chrono`
- Calculate parking fees with partial-hour rounding
- View all parked vehicles
- View available parking slots
- Persistent storage using `parking_records.txt`
- Menu-driven CLI
- Input validation and error handling

## Tech Stack

- C++17
- Standard Template Library (STL)
- Object-Oriented Programming (OOP)
- `std::chrono`
- File Handling (`fstream`)

## Data Structures Used

- `vector<ParkingSlot>` — Stores all parking slots
- `unordered_map<string, Vehicle>` — Stores active parked vehicles
- `unordered_map<string, int>` — Maps vehicle numbers to parking slot indices for O(1) lookup

## Fee Calculation

| Vehicle | Rate |
|---------|------|
| Bike | ₹10 for first hour + ₹5 per additional hour |
| Car | ₹20 for first hour + ₹10 per additional hour |
| Truck | ₹30 for first hour + ₹15 per additional hour |

- Parking duration is calculated using `std::chrono`.
- Partial hours are rounded up to the next hour.

## Project Structure

```
parking-lot-management-system/
│
├── main.cpp
├── README.md
├── .gitignore
├── .vscode/
│   └── tasks.json
└── parking_records.txt (generated automatically)
```

## How to Build

Compile:

```bash
g++ main.cpp -o main
```

Run:

### Windows

```powershell
.\main.exe
```

### Linux / macOS

```bash
./main
```

## Edge Cases Handled

- Duplicate vehicle numbers
- Parking when no slots are available
- Exiting a vehicle that is not parked
- Invalid vehicle type
- Invalid menu selection
- Partial-hour billing
- Persistent records across program restarts

## Object-Oriented Design

The project is built around three core classes:

- **Vehicle** – Stores vehicle details and entry time.
- **ParkingSlot** – Represents an individual parking slot.
- **ParkingLot** – Manages slot allocation, parking, exits, fee calculation, and file persistence.

## Concepts Demonstrated

- Object-Oriented Programming
- Classes and Constructors
- Enums
- STL Containers (`vector`, `unordered_map`)
- Hash-based lookups
- File I/O
- Time handling using `std::chrono`
- Menu-driven CLI application
- Input validation
- Basic system design

## Future Improvements

- Multiple parking floors
- Different parking rates by time of day
- Admin authentication
- Daily transaction logs
- Search and filter parked vehicles
- Unit testing
