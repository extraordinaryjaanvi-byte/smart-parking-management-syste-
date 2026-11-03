
#include <stdio.h>
#include <string.h>

#define TOTAL_SLOTS 10
#define MAX_HISTORY 100

// Structure for vehicle details
struct Vehicle {
    char number[20];
    char owner[50];
    char type[10];
    int occupied;
};

// Structure for parking history
struct History {
    char number[20];
    char owner[50];
    char type[10];
    int hours;
    float fee;
};

// Global variables
struct Vehicle parking[TOTAL_SLOTS];
struct History history[MAX_HISTORY];

int historyCount = 0;
float totalRevenue = 0;

// Read a number safely
int readNumber() {
    int number;
    char line[100];

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL)
            return 0;

        if (sscanf(line, "%d", &number) == 1)
            return number;

        printf("Please enter a valid number: ");
    }
}

// Read text safely
void readText(char text[], int size) {
    fgets(text, size, stdin);
    text[strcspn(text, "\n")] = '\0';
}

// Display parking slots
void displaySlots() {
    int available = 0;

    printf("\n========== PARKING SLOT STATUS ==========\n");

    for (int i = 0; i < TOTAL_SLOTS; i++) {
        printf("Slot %2d : ", i + 1);

        if (parking[i].occupied) {
            printf("OCCUPIED (%s)\n", parking[i].number);
        } else {
            printf("AVAILABLE\n");
            available++;
        }
    }

    printf("-----------------------------------------\n");
    printf("Available Slots: %d/%d\n",
           available, TOTAL_SLOTS);
}

// Park a vehicle
void parkVehicle() {
    int slot = -1;
    int typeChoice;

    for (int i = 0; i < TOTAL_SLOTS; i++) {
        if (!parking[i].occupied) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        printf("\nSorry! Parking is FULL.\n");
        return;
    }

    printf("\n========== VEHICLE ENTRY ==========\n");

    printf("Enter vehicle number: ");
    readText(parking[slot].number,
             sizeof(parking[slot].number));

    if (strlen(parking[slot].number) == 0) {
        printf("Vehicle number cannot be empty!\n");
        return;
    }

    // Check duplicate vehicle
    for (int i = 0; i < TOTAL_SLOTS; i++) {
        if (parking[i].occupied &&
            strcmp(parking[i].number,
                   parking[slot].number) == 0) {
            printf("This vehicle is already parked!\n");
            return;
        }
    }

    printf("Enter owner name: ");
    readText(parking[slot].owner,
             sizeof(parking[slot].owner));

    if (strlen(parking[slot].owner) == 0) {
        printf("Owner name cannot be empty!\n");
        return;
    }

    printf("\nSelect vehicle type:\n");
    printf("1. Car\n");
    printf("2. Bike\n");
    printf("Enter choice: ");

    typeChoice = readNumber();

    if (typeChoice == 1) {
        strcpy(parking[slot].type, "Car");
    }
    else if (typeChoice == 2) {
        strcpy(parking[slot].type, "Bike");
    }
    else {
        printf("Invalid vehicle type!\n");
        return;
    }

    parking[slot].occupied = 1;

    printf("\nVehicle parked successfully!\n");
    printf("Assigned Slot: %d\n", slot + 1);
    printf("Vehicle Number: %s\n",
           parking[slot].number);
}

// Vehicle exit and fee calculation
void vehicleExit() {
    char number[20];
    int slot = -1;
    int hours;
    float rate, fee;

    printf("\n========== VEHICLE EXIT ==========\n");
    printf("Enter vehicle number: ");
    readText(number, sizeof(number));

    for (int i = 0; i < TOTAL_SLOTS; i++) {
        if (parking[i].occupied &&
            strcmp(parking[i].number, number) == 0) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        printf("Vehicle not found!\n");
        return;
    }

    printf("Vehicle Found!\n");
    printf("Owner: %s\n", parking[slot].owner);
    printf("Type: %s\n", parking[slot].type);
    printf("Slot: %d\n", slot + 1);

    printf("\nEnter parking duration in hours: ");
    hours = readNumber();

    if (hours <= 0) {
        printf("Invalid parking duration!\n");
        return;
    }

    // Different rates for different vehicles
    if (strcmp(parking[slot].type, "Car") == 0) {
        rate = 30;
    } else {
        rate = 15;
    }

    fee = hours * rate;

    printf("\n========== PARKING RECEIPT ==========\n");
    printf("Vehicle Number : %s\n", parking[slot].number);
    printf("Owner Name     : %s\n", parking[slot].owner);
    printf("Vehicle Type   : %s\n", parking[slot].type);
    printf("Parking Hours  : %d\n", hours);
    printf("Rate Per Hour  : Rs. %.2f\n", rate);
    printf("-------------------------------------\n");
    printf("TOTAL FEE      : Rs. %.2f\n", fee);
    printf("=====================================\n");

    // Save history
    if (historyCount < MAX_HISTORY) {
        strcpy(history[historyCount].number,
               parking[slot].number);

        strcpy(history[historyCount].owner,
               parking[slot].owner);

        strcpy(history[historyCount].type,
               parking[slot].type);

        history[historyCount].hours = hours;
        history[historyCount].fee = fee;

        historyCount++;
    }

    totalRevenue += fee;

    // Free the parking slot
    parking[slot].occupied = 0;

    printf("\nVehicle exited successfully!\n");
    printf("Parking Slot %d is now available.\n",
           slot + 1);
}

// Search for a vehicle
void searchVehicle() {
    char number[20];
    int found = 0;

    printf("\nEnter vehicle number to search: ");
    readText(number, sizeof(number));

    for (int i = 0; i < TOTAL_SLOTS; i++) {
        if (parking[i].occupied &&
            strcmp(parking[i].number, number) == 0) {

            printf("\nVehicle Found!\n");
            printf("Owner: %s\n", parking[i].owner);
            printf("Vehicle: %s\n", parking[i].number);
            printf("Type: %s\n", parking[i].type);
            printf("Parking Slot: %d\n", i + 1);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Vehicle is not currently parked.\n");
    }
}

// Display parking history
void displayHistory() {
    printf("\n========== PARKING HISTORY ==========\n");

    if (historyCount == 0) {
        printf("No parking history available.\n");
        return;
    }

    for (int i = 0; i < historyCount; i++) {
        printf("\nRecord %d\n", i + 1);
        printf("Vehicle Number: %s\n",
               history[i].number);
        printf("Owner: %s\n", history[i].owner);
        printf("Type: %s\n", history[i].type);
        printf("Hours: %d\n", history[i].hours);
        printf("Fee: Rs. %.2f\n", history[i].fee);
    }
}

// Display revenue report
void revenueReport() {
    int occupied = 0;

    for (int i = 0; i < TOTAL_SLOTS; i++) {
        if (parking[i].occupied) {
            occupied++;
        }
    }

    printf("\n========== REVENUE REPORT ==========\n");
    printf("Total Parking Slots : %d\n", TOTAL_SLOTS);
    printf("Occupied Slots     : %d\n", occupied);
    printf("Available Slots    : %d\n",
           TOTAL_SLOTS - occupied);
    printf("Completed Sessions : %d\n", historyCount);
    printf("Total Revenue      : Rs. %.2f\n",
           totalRevenue);
    printf("====================================\n");
}

// Main function
int main() {
    int choice;

    printf("\n");
    printf("****************************************\n");
    printf("       SMART PARKING MANAGEMENT\n");
    printf("                SYSTEM\n");
    printf("****************************************\n");

    do {
        printf("\n\n");
        printf("========================================\n");
        printf("             MAIN MENU\n");
        printf("========================================\n");
        printf("1. View Parking Slots\n");
        printf("2. Park a Vehicle\n");
        printf("3. Vehicle Exit\n");
        printf("4. Search Vehicle\n");
        printf("5. Parking History\n");
        printf("6. Revenue Report\n");
        printf("7. Exit Program\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");

        choice = readNumber();

        switch (choice) {
            case 1:
                displaySlots();
                break;

            case 2:
                parkVehicle();
                break;

            case 3:
                vehicleExit();
                break;

            case 4:
                searchVehicle();
                break;

            case 5:
                displayHistory();
                break;

            case 6:
                revenueReport();
                break;

            case 7:
                printf("\nThank you for using Smart Parking!\n");
                printf("Have a safe journey!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 7);

    return 0;
}
