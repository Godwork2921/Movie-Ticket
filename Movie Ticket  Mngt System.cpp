#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

class Movie {
public:
    string title;
    string genre;
    int year;
    double rating;
    double price;

    Movie(const string& title, const string& genre, int year, double rating, double price)
        : title(title), genre(genre), year(year), rating(rating), price(price) {}

    string getTitle() const { return title; }
    string getGenre() const { return genre; }
    int getYear() const { return year; }
    double getRating() const { return rating; }
    double getPrice() const { return price; }
};

struct Booking {
    string username;
    string movieTitle;
};

class MovieTicketSystem {
private:
    string adminPassword = "123456";
    string usersFilePath = "users.txt";
    string moviesFilePath = "movies.txt";
    vector<Movie> movies;
    vector<Booking> bookings;
    string currentUser;

    void displayWelcome() {
        cout << R"(
                    /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
                    |    WELCOME TO ABC MOVIES STORE |
                    |                                |
                    /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                    _________________________________
                    |                               |
                    |_____1.Admin Access____________|
                    |                               |
                    |_____2.User Access_____________|
                    |                               |
                    |_____3.Exit____________________|
                    |                               |
                    |_______________________________|
        )" << endl;
    }

    void adminLoginInterface() {
        system("cls");
        string password;
        cout << "Admin Login:\nEnter password: ";
        cin >> password;

        if (password == adminPassword) {
            cout << "Admin login successful!\n";
            adminMenu();
        } else {
            cout << "Incorrect password.\n";
            system("pause");
        }
    }

    void userLoginRegisterInterface() {
        system("cls");
        int userChoice;
        cout << "User Login/Register:\n1. Login\n2. Register\nEnter choice: ";
        cin >> userChoice;

        if (userChoice == 1) userLogin();
        else if (userChoice == 2) userRegistration();
        else {
            cout << "Invalid choice.\n";
            system("pause");
        }
    }

    void userRegistration() {
        string username, password;
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter new password: ";
        cin >> password;

        ofstream userFile(usersFilePath, ios::app);
        if (userFile.is_open()) {
            userFile << username << "," << password << endl;
            userFile.close();
            cout << "Registration successful!\n";
            system("pause");
            userLogin();
        } else {
            cout << "Error registering user.\n";
            system("pause");
        }
    }

    void userLogin() {
        string username, password, storedUsername, storedPassword;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream userFile(usersFilePath);
        if (userFile.is_open()) {
            string line;
            while (getline(userFile, line)) {
                stringstream ss(line);
                getline(ss, storedUsername, ',');
                getline(ss, storedPassword);
                if (username == storedUsername && password == storedPassword) {
                    userFile.close();
                    cout << "User login successful!\n";
                    currentUser = username;
                    userMenu();
                    return;
                }
            }
            userFile.close();
            cout << "Invalid username or password.\n";
            system("pause");
        } else {
            cout << "User file not found.\n";
            system("pause");
        }
    }

    void adminMenu() {
        system("cls");
        int choice;
        do {
            cout << "*** ADMIN MENU ***\n\n";
            cout << "1) Add Movie\n2) View Movie List\n3) View Booked Movies\n4) Remove Movie\n5) Main Menu\n\nSelect any option: ";
            cin >> choice;

            switch (choice) {
            case 1: addMovie(); viewMovieList(); break;
            case 2: viewMovieList(); break;
            case 3: viewBooked(); break;
            case 4: removeMovie(); break;
            case 5: return;
            default: cout << "Invalid choice.\n";
            }
            system("cls");
        } while (true);
    }

    void userMenu() {
        system("cls");
        int choice;
        do {
            cout << "*** USER MENU ***\n\n";
            cout << "1) View Movie List\n2) Book Ticket(s)\n3) View Booked History\n4) Main Menu\n\nSelect any option: ";
            cin >> choice;

            switch (choice) {
            case 1: viewMovieList(); break;
            case 2: bookTicketsByUser(); break;
            case 3: viewBookedHistory(); break;
            case 4: return;
            default: cout << "Invalid choice.\n";
            }
            system("cls");
        } while (true);
    }

    void addMovie() {
        string title, genre;
        int year;
        double rating, price;

        cout << "Enter movie title: "; cin.ignore(); getline(cin, title);
        cout << "Enter movie genre: "; getline(cin, genre);
        cout << "Enter year: "; cin >> year;
        cout << "Enter rating: "; cin >> rating;
        cout << "Enter price: "; cin >> price;

        movies.emplace_back(title, genre, year, rating, price);
        saveMoviesToFile();
        cout << "Movie added successfully.\n";
        system("pause");
    }

    void viewMovieList() {
        loadMoviesFromFile();
        if (movies.empty()) cout << "No movies available.\n";
        else {
            cout << "Code Name\tGenre\tYear\tRating\tPrice\n";
            for (size_t i = 0; i < movies.size(); ++i) {
                cout << setw(4) << i + 1 << "\t" << setw(15) << movies[i].getTitle() << "\t"
                     << setw(5) << movies[i].getGenre() << "\t" << setw(4) << movies[i].getYear() << "\t"
                     << setw(6) << fixed << setprecision(1) << movies[i].getRating() << "\t"
                     << setw(5) << fixed << setprecision(0) << movies[i].getPrice() << endl;
            }
        }
        system("pause");
    }

    void viewBooked() {
        if (bookings.empty()) cout << "No bookings available.\n";
        else {
            cout << "Code Name\tGenre\tYear\tRating\tPrice\tUser\n";
            for (size_t i = 0; i < bookings.size(); ++i) {
                for (const auto& movie : movies) {
                    if (bookings[i].movieTitle == movie.getTitle()) {
                        cout << setw(4) << i + 1 << "\t" << setw(15) << movie.getTitle() << "\t"
                             << setw(5) << movie.getGenre() << "\t" << setw(4) << movie.getYear() << "\t"
                             << setw(6) << fixed << setprecision(1) << movie.getRating() << "\t"
                             << setw(5) << fixed << setprecision(0) << movie.getPrice() << "\t"
                             << bookings[i].username << endl;
                    }
                }
            }
        }
        system("pause");
    }

    void removeMovie() {
        if (movies.empty()) cout << "No movies available to remove.\n";
        else {
            viewMovieList();
            int choice;
            cout << "Enter the number of the movie to remove: "; cin >> choice;
            if (choice >= 1 && choice <= movies.size()) {
                movies.erase(movies.begin() + choice - 1);
                saveMoviesToFile();
                cout << "Movie removed successfully.\n";
            } else cout << "Invalid movie number.\n";
        }
        system("pause");
    }

    void bookTicketsByUser() {
        viewMovieList();
        if (movies.empty()) return;

        int numTickets;
        cout << "How many tickets do you want to book? ";
        cin >> numTickets;

        for (int i = 0; i < numTickets; ++i) {
            int movieChoice;
            cout << "Enter the code of the movie for ticket " << i + 1 << ": ";
            cin >> movieChoice;

            if (movieChoice >= 1 && movieChoice <= movies.size()) {
                bookTicket(currentUser, movies[movieChoice - 1].getTitle());
                cout << "Ticket " << i + 1 << " booked successfully.\n";
            } else {
                cout << "Invalid movie code for ticket " << i + 1 << ".\n";
                --i;
            }
        }
        system("pause");
    }

    void viewBookedHistory() {
        cout << "Your Booked History:\n";
        bool booked = false;
        for (const auto& booking : bookings) {
            if (booking.username == currentUser) {
                cout << booking.movieTitle << endl;
                booked = true;
            }
        }
        if (!booked) cout << "You have not booked any tickets yet.\n";
        system("pause");
    }

    void bookTicket(const string& username, const string& movieTitle) {
        Booking booking;
        booking.username = username;
        booking.movieTitle = movieTitle;
        bookings.push_back(booking);
    }

    void saveMoviesToFile() {
        ofstream movieFile(moviesFilePath);
        if (movieFile.is_open()) {
            for (const auto& movie : movies) {
                movieFile << movie.getTitle() << "," << movie.getGenre() << "," << movie.getYear() << ","
                          << movie.getRating() << "," << movie.getPrice() << endl;
            }
            movieFile.close();
        }
    }

    void loadMoviesFromFile() {
        movies.clear();
        ifstream movieFile(moviesFilePath);
        if (movieFile.is_open()) {
            string line;
            while (getline(movieFile, line)) {
                stringstream ss(line);
                string title, genre;
                int year;
                double rating, price;

                getline(ss, title, ',');
                getline(ss, genre, ',');
                ss >> year; ss.ignore();
                ss >> rating; ss.ignore();
                ss >> price;

                movies.emplace_back(title, genre, year, rating, price);
            }
            movieFile.close();
        }
    }

public:
    void run() {
        loadMoviesFromFile();
        displayWelcome();
        int choice;
        do {
            cout << "\n\n\t\t\t Your choice: \t"; cin >> choice;
            switch (choice) {
            case 1: adminLoginInterface(); break;
            case 2: userLoginRegisterInterface(); break;
            case 3: cout << "Exiting...\n"; return;
            default: cout << "Invalid choice.\n"; system("pause");
            }
            system("cls");
            displayWelcome();
        } while (true);
    }
};

int main() {
    MovieTicketSystem system;
    system.run();
    return 0;
}
