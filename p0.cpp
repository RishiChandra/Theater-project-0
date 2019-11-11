// Name: Rishi Chandra
// NetID: rxc170008
// List of libraries to include
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>      // to use std::setprecision when displaying the report

using namespace std; // To not have to use std:: before many commands

void display(int num_rows, int num_cols, char theater[10][26]); // Prototype for function to display the theater

bool available(char theater[10][26], int row, int seat, int total); // Prototype for function to see if the requested seats are open

int best_available(char theater[10][26], int row, int total, int num_cols); // Prototype for function to find the best available seats

void reserve(char thater[10][26], int row, int seat, int adult, int child, int senior); // Prototype for function to reserve the seats

void report(int num_rows, int num_cols, char theater[10][26]); // Prototype for function to display the final report

int main(){
    string name; // Var to store the next line form the text file
    int num_rows = 0; // Var to store number of rows since the memory of the 2d array is not dynamically allocated
    int num_cols; // Var to store number of cols
    ifstream input("A1.txt"); // Var to read the text file

    if (!input.good()){
        cout << "Missing file or invalid filename" << endl; // validate file
    }

    // Loop over the file to find the size of the array
    while(!input.eof()){
        getline(input, name);
        num_rows++; // Increment the number of rows for each line in the theater file
    }
    num_cols = name.size(); // Get the number of cols based on the number of characters in the line
    input.seekg(0); // Reset the flag for reading the file

    char theater[10][26]; // Create the 2d array to store the theater information
    for(int i = 0; i < num_rows; i++){ // Loop over the number of rows
        getline(input, name); // Get the line in the row and store it in name via pass by reference
        for(int j = 0; j < num_cols; j++){ // Loop over the number of cols
            theater[i][j] = name.at(j); // Store the character in the theater array
        }
    }
    input.close(); // Close the file so there will be stream errors later

    // Create UI
    int exit; // Var to store the users decision
    while (true){ // Infinite while loop until the user decides to quit
        cout << "1. Reserve Seats" << endl << "2. Exit" << endl; // Prompt the user
        cin >> exit; // Store user input
        if (exit == 2){ // If the user decides to quit
            break;
        }
        display(num_rows, num_cols, theater); // Display the theater so the user can decide on seats to reserve

        // Initialize vars to store the users desired seat info
        int row;
        char seat_letter;
        int adult;
        int child;
        int senior;
        bool valid = true;

        // Prompt the user and store responses
        cout << "What row number would you like?" << endl;
        cin >> row;
        while (row > num_rows - 1){
            cout << "Requested seats are out of bounds, please enter valid input" << endl;
            cin >> row;
        }
        cout << "What starting seat letter would you like?" << endl;
        cin >> seat_letter;
        seat_letter = toupper(seat_letter); // Ensure the program works for lower case input
        int seat = seat_letter - '@'; // Use ASCII conversion tables to turn the seat letter to an int
        while (seat > num_cols){
            cout << "Requested seats are out of bounds, please enter valid input" << endl;
            cin >> seat_letter;
            seat_letter = toupper(seat_letter); // Ensure the program works for lower case input
            seat = seat_letter - '@';
        }
        cout << "How many adult tickets would you like?" << endl;
        cin >> adult;
        while (adult < 0) {
            cout << "Invalid input, please enter valid input" << endl;
            cin >> adult;
        }
        cout << "How many child tickets would you like?" << endl;
        cin >> child;
        while (child < 0) {
            cout << "Invalid input, please enter valid input" << endl;
            cin >> child;
        }
        cout << "How many senior tickets would you like?" << endl;
        cin >> senior;
        while (senior < 0){
            cout << "Invalid input, please enter valid input" << endl;
            cin >> senior;
        }

        int total = adult + child + senior; // Var to store the total number of seats for ease
        if ((seat + total - 1) > num_cols) {
            cout << "Requested seats are out of bounds, please enter valid input" << endl;
        }
        // If the input is valid
        else{
            bool work = available(theater, row, seat, total); // Check if the requested seats are available
            if (work){
                    reserve(theater, row, seat, adult, child, senior); // Reserve the validated seats
            }
            else{
                int best_seat = best_available(theater, row, total, num_cols); //Find the next best seats
                if (best_seat != -1){ // If there are better seats prompt the user to agree or not
                    cout << "The seats you selected are not available. The next best available seats start at seat letter ";
                    cout << static_cast<char>('A' + best_seat) << ". Would you like these seats?" << endl;
                    cout << "Enter Y for yes and N for no" << endl;
                    char agree;
                    cin >> agree;
                    if (agree == 'Y' || agree == 'y'){
                        reserve(theater, row, best_seat + 1, adult, child, senior); // Reserve the best seats if the user agrees
                    }
                }
                else { // Inform the user there are no seats available in the row
                    cout << "There are no available seats in the row you selected" << endl;
                }
            }

        }

    }
    report(num_rows, num_cols, theater);// Display the final report

    ofstream output("A1.txt"); // Open the file up to update it for the new theater

    for (int i = 0; i < num_rows; i++){ // Loop through all the rows
        string in = ""; // Var to store the row
        for (int j = 0; j < num_cols; j++){ // Loop through all the cols
            in += theater[i][j];
        }
        output << in; // Write the new row to the file
        if (i != num_rows - 1){ // End the line except if its the last row
            output << endl;
        }
    }

    output.close(); // Close the file so there are no streaming error


    return 0;
}

// Method to reserve the seats
void reserve(char theater[10][26], int row, int seat, int adult, int child, int senior){
    // Start with the adult seats first
    for (int i = 0; i < adult; i++){ // Loop through the adult seats to reserve
            theater[row][seat - 1 + i] = 'A'; // Mark the adult seats as adult
    }
    for (int i = 0; i < child; i++) { // Loop through the child seats to reserve
        theater[row][seat - 1 + adult + i] = 'C'; // Mark the child seats, which come after the adult seats, as child
    }
    for (int i = 0; i < senior; i++){ //Loop through the senior seats to reserve
        theater[row][seat - 1 + adult + child + i] = 'S'; // Mark the senior seats, which come after the adult and child seats, as senior
    }
}

// Method to find the best seats in the row
int best_available(char theater[10][26], int row, int total, int num_cols){
    int small = 1000000; // Initialize the var to store the smallest dist as a large number for now
    int seat = -1; // Initialize the index of the best seat, if there are no seats then the index will be -1
    for (int i = 0; i < num_cols - total + 1; i++ ){ // Loop through all the seats in the row
        if (available(theater, row,i + 1, total)){ // Check if the number of seats are available
            int dist = abs((i + (total-1)/2) - (num_cols+1)/2); // Find the distance of the first seat from the middle of the row
            if (dist < small){ // If the seat is closer than the previous closest, update the information
                small = dist;
                seat = i;
            }
        }
    }
    return seat;

}

// Method to see if the desired seats are available
bool available(char theater[10][26], int row, int seat, int total){
    for (int i = 0; i < total; i++){ // Loop through the number of desired seats
        if (theater[row][seat + i - 1] != '.'){ // If one of the seats is taken then return false
            return false;
        }
    }
    return true;
}

// Method to display the final report of th theater
void report(int num_rows, int num_cols, char theater[10][26]){
    // Initialize the vars to store the theater stats
    int seats = 0;
    int sold = 0;
    int adult = 0;
    int child = 0;
    int senior = 0;

    // Loop through all the rows and cols
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            seats++; // Count the total number of seats
            char temp = theater[i][j];
            if (temp != '.'){ // If the seat has been sold
                sold++; // Count the total number of sold seats
                if (temp == 'A'){
                    adult++; // Count the total number of adult seats
                }
                else if(temp == 'C'){
                    child++; // Count the total number of child seats
                }
                else if(temp == 'S'){
                    senior++; // Count the total number of senior seats
                }
            }
        }
    }

    // Display the formatted report
    cout << endl << "Final Report:" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "|Total seats: \t\t\t|" << seats << "\t\t|" << endl;
    cout << "|Total seats sold: \t\t|" << sold << "\t\t|" << endl;
    cout << "|Total adult seats sold: \t|" << adult << "\t\t|" << endl;
    cout << "|Total child seats sold: \t|" << child << "\t\t|" << endl;
    cout << "|Total senior seats sold: \t|" << senior << "\t\t|" << endl;
    double sales = 10 * adult + 5 * child + 7.5 * senior; // Calculate the total sales using the given ticket prices
    cout << "|Total ticket sales: \t\t|$" << std::fixed << setprecision(2) << sales << "\t|" << endl; // Format the sales
    cout << "-------------------------------------------------" << endl;
}

// Method to display the auditorium to the user
void display(int num_rows, int num_cols, char theater[10][26]){
    // Print the ABC column for the user
    cout << "  ";
    for (int i = 0; i < num_cols; i++){
        cout << static_cast<char>('A' + i);
    }

    cout << endl;

    // Loop through the rows and cols
    for(int i = 0; i < num_rows; i++){
        cout << i << " ";
        for(int j = 0; j < num_cols; j++){
            char temp = theater[i][j];
            if (temp != '.'){
                cout << '#'; // If the seat is occupied represent it with a #
            }
            else{
                cout << temp; // If the seat is not occupied the represent it with the default .
            }
        }
        cout << endl;
    }
}


