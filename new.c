#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FLIGHTS 10
#define MAX_CLASSES 3 // Economy, Business, First
#define MAX_BOOKINGS 100

// Structure to store flight details
struct Flight {
    int flightNumber;
    char origin[50];
    char destination[50];
    int capacity[MAX_CLASSES];
    int availableSeats[MAX_CLASSES];
    double prices[MAX_CLASSES]; // Prices for Economy, Business, First
    char departureTime[10];
    char arrivalTime[10];
};

// Structure for linked list node for booking
struct BookingNode {
    int bookingID;
    char passengerName[50];
    int age;
    char gender[10];
    int flightNumber;
    int classType; // 0: Economy, 1: Business, 2: First
    char password[20]; // Password for cancellation
    struct BookingNode* next;
};

// Array of flights
struct Flight flights[MAX_FLIGHTS];
int flightCount = 3;
int bookingCount = 0;

// Head of the linked list for bookings
struct BookingNode* head = NULL;

// Class labels
const char* classNames[] = {"Economy", "Business", "First"};

// Function to display flight schedule in a nice format
void displayFlights() {
    printf("\n+------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                 Flight Schedule                                                             |\n");
    printf("+------------------------------------------------------------------------------------------------------------+\n");
    printf("| Flight No | Origin     | Destination  | Eco Seats | Eco Price | Bus Seats | Bus Price | First Seats | First Price | Departure  | Arrival    |\n");
    printf("+------------------------------------------------------------------------------------------------------------+\n");
    for (int i = 0; i < flightCount; i++) {
        printf("| %-9d | %-10s | %-12s | %-9d | $%-8.2f | %-9d | $%-8.2f | %-11d | $%-10.2f | %-9s | %-10s |\n",
               flights[i].flightNumber, flights[i].origin, flights[i].destination,
               flights[i].availableSeats[0], flights[i].prices[0], flights[i].availableSeats[1], flights[i].prices[1],
               flights[i].availableSeats[2], flights[i].prices[2], flights[i].departureTime, flights[i].arrivalTime);
    }
    printf("+------------------------------------------------------------------------------------------------------------+\n");
}

// Function to book a seat on a flight
void bookSeat() {
    char passengerName[50], gender[10], password[20];
    int age, flightNumber, classType;

    printf("\nEnter Passenger Name: ");
    scanf("%s", passengerName);
    printf("Enter Age: ");
    scanf("%d", &age);
    printf("Enter Gender (M/F): ");
    scanf("%s", gender);
    printf("Enter Flight Number: ");
    scanf("%d", &flightNumber);

    // Find the flight
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNumber == flightNumber) {
            printf("\nAvailable Classes and Prices:\n");
            printf("0 - Economy: $%.2f, 1 - Business: $%.2f, 2 - First: $%.2f\n",
                   flights[i].prices[0], flights[i].prices[1], flights[i].prices[2]);

            printf("Select Class (0 - Economy, 1 - Business, 2 - First): ");
            scanf("%d", &classType);

            if (classType < 0 || classType >= MAX_CLASSES) {
                printf("\nInvalid class selection.\n");
                return;
            }

            if (flights[i].availableSeats[classType] > 0) {
                struct BookingNode* newBooking = (struct BookingNode*)malloc(sizeof(struct BookingNode));
                newBooking->bookingID = ++bookingCount;
                strcpy(newBooking->passengerName, passengerName);
                newBooking->age = age;
                strcpy(newBooking->gender, gender);
                newBooking->flightNumber = flightNumber;
                newBooking->classType = classType;

                // Ask for password
                printf("Create a Password for this booking: ");
                scanf("%s", password);
                strcpy(newBooking->password, password);

                newBooking->next = NULL;

                // Add the booking to the linked list
                if (head == NULL) {
                    head = newBooking;
                } else {
                    struct BookingNode* temp = head;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = newBooking;
                }

                // Reduce available seats
                flights[i].availableSeats[classType]--;
                printf("\nBooking Successful!\n");
                printf("\nBooking ID: %d\n", newBooking->bookingID);
                printf("Passenger: %s, Age: %d, Gender: %s\n", passengerName, age, gender);
                printf("Flight Number: %d, Class: %s\n", flightNumber, classNames[classType]);
                printf("Departure: %s | Arrival: %s\n", flights[i].departureTime, flights[i].arrivalTime);
                printf("Total Price: $%.2f\n", flights[i].prices[classType]);
                return;
            } else {
                printf("\nSorry, no available seats in %s class for this flight.\n", classNames[classType]);
                return;
            }
        }
    }
    printf("\nInvalid Flight Number.\n");
}

// Function to cancel a booking by booking ID and password
void cancelBooking() {
    int bookingID;
    char password[20];

    printf("\nEnter your Booking ID to cancel: ");
    scanf("%d", &bookingID);

    // Find the booking
    struct BookingNode* temp = head;
    while (temp != NULL) {
        if (temp->bookingID == bookingID) {
            printf("Enter your password to confirm cancellation: ");
            scanf("%s", password);

            if (strcmp(temp->password, password) == 0) {
                // Find the corresponding flight and update available seats
                struct Flight* flight = NULL;
                for (int i = 0; i < flightCount; i++) {
                    if (flights[i].flightNumber == temp->flightNumber) {
                        flight = &flights[i];
                        break;
                    }
                }
                if (flight != NULL) {
                    flight->availableSeats[temp->classType]++; // Increase available seats
                    printf("Booking canceled successfully.\n");
                }

                // Remove the booking from the linked list
                if (head == temp) {
                    head = temp->next;
                } else {
                    struct BookingNode* prev = head;
                    while (prev->next != temp) {
                        prev = prev->next;
                    }
                    prev->next = temp->next;
                }
                free(temp);
                return;
            } else {
                printf("Incorrect password. Cancellation failed.\n");
                return;
            }
        }
        temp = temp->next;
    }
    printf("\nBooking not found.\n");
}

// Function to check available seats for a flight
void checkAvailableSeats() {
    int flightNumber;
    printf("\nEnter Flight Number to Check Available Seats: ");
    scanf("%d", &flightNumber);

    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNumber == flightNumber) {
            printf("\nAvailable Seats for Flight %d:\n", flightNumber);
            for (int j = 0; j < MAX_CLASSES; j++) {
                printf("%s: %d\n", classNames[j], flights[i].availableSeats[j]);
            }
            return;
        }
    }
    printf("\nFlight not found.\n");
}

// Function to view all booked tickets in a formatted table
void viewBookedTickets() {
    struct BookingNode* temp = head;
    if (temp == NULL) {
        printf("\nNo bookings available.\n");
        return;
    }

    printf("\n+------------------------------------------------------------------------------------------------------------+\n");
    printf("|                          Booked Tickets Overview                                                           |\n");
    printf("+------------------------------------------------------------------------------------------------------------+\n");
    printf("| Booking ID | Passenger Name  | Age | Gender | Flight Number | Class   | Departure Time | Arrival Time   | Price   |\n");
    printf("+------------------------------------------------------------------------------------------------------------+\n");

    while (temp != NULL) {
        struct Flight* flight = NULL;
        for (int i = 0; i < flightCount; i++) {
            if (flights[i].flightNumber == temp->flightNumber) {
                flight = &flights[i];
                break;
            }
        }

        if (flight != NULL) {
            printf("| %-11d | %-15s | %-3d | %-6s | %-13d | %-7s | %-15s | %-14s | $%-6.2f |\n",
                   temp->bookingID, temp->passengerName, temp->age, temp->gender,
                   temp->flightNumber, classNames[temp->classType],
                   flight->departureTime, flight->arrivalTime, flight->prices[temp->classType]);
        }
        temp = temp->next;
    }

    printf("+------------------------------------------------------------------------------------------------------------+\n");
}

int main() {
    // Initialize flight details
    flights[0] = (struct Flight){101, "New York", "San Francisco", {120, 40, 15}, {120, 40, 15}, {150.0, 400.0, 900.0}, "09:00", "13:00"};
    flights[1] = (struct Flight){102, "Los Angeles", "Chicago", {120, 40, 15}, {120, 40, 15}, {150.0, 400.0, 900.0}, "11:00", "15:30"};
    flights[2] = (struct Flight){103, "Miami", "Dallas", {100, 30, 10}, {100, 30, 10}, {160.0, 400.0, 850.0}, "12:00", "16:00"};

    int choice;
    while (1) {
        printf("\n1. Display Flight Schedule\n");
        printf("2. Book a Flight\n");
        printf("3. Cancel a Booking\n");
        printf("4. Check Available Seats\n");
        printf("5. View Booked Tickets\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayFlights();
                break;
            case 2:
                bookSeat();
                break;
            case 3:
                cancelBooking();
                break;
            case 4:
                checkAvailableSeats();
                break;
            case 5:
                viewBookedTickets();
                break;
            case 6:
                printf("Thank you for using the Flight Booking System!\n");
                exit(0);
        }
    }
    return 0;
}
