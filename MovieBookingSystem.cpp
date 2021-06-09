/**
 * @author Furkan Güzelant ID: 21901515
 * @version 21.03.2021

*/
#include "MovieBookingSystem.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

    MovieBookingSystem::MovieBookingSystem() {
        movies = NULL;
        movieCount = 0;
        reservationList = NULL;
        reservationNumber = 0;
        reservCode = 0;
    }

    MovieBookingSystem::~MovieBookingSystem() {
        for(int i = 0; i < movieCount; i++) {
            int radius = movies[i].getRadius();
            for(int j = 0; j < ((numOfRow - 1) / (radius + 1) + 1); j++) {
                delete[] movies[i].getRows()[j];
        }
        delete[] movies[i].getRows();
        }
        delete [] reservationList;
        delete[] movies;
    }

    void MovieBookingSystem:: addMovie( const long movieID, const int audienceRadius ) {
        bool validId = true;
        for(int i = 0; i < movieCount; i++) {
            if(movieID == movies[i].getId()) {
                validId = false;
                movies[i].displayMovieInfo();
                cout << "already exits" << endl;
                return;
            }
        }
        if(validId && audienceRadius >= 0 && audienceRadius < 8) {
            Movie* temp;
            movieCount++;
            temp = new Movie[movieCount];

            for(int i = 0; i < movieCount - 1; i++) {
                temp[i] = movies[i];
            }
            Movie m(movieID, audienceRadius);
            temp[movieCount - 1] = m;

            delete[] movies;
            movies = temp;
            movies[movieCount - 1].displayMovieInfo();
            cout << "has been added." << endl;
        }
        else {
            cout << "Invalid input! Audience radius should be in range [0,7]." << endl;
            }

    }

    void MovieBookingSystem::cancelMovie( const long movieID) {
        bool found = false;
        Movie* temp = NULL;
        Movie* m = NULL;

        int index = 0;
        while(!found && index < movieCount) {
            if(movieID == movies[index].getId()) {
                m = &movies[index];
                found = true;
            }
            index++;
        }

        if(found) {

            int counter = 0;

            if(reservationNumber > 0) {
                for(int i = 0; i < reservationNumber; i++) {
                    if(reservationList[i].getMovie()->getId() != movieID) {
                        counter++;
                    }
                }

                if(counter > 0) {
                    Reservation* templist = new Reservation[counter];
                    int k = 0;
                    for(int i = 0; i < reservationNumber; i++) {
                        if(reservationList[i].getMovie()->getId() != movieID) {
                            templist[k] = reservationList[i];
                            k++;
                        }
                    }
                    reservationNumber = counter;
                    delete[] reservationList;
                    reservationList = templist;
                }

                else {
                    delete[] reservationList;
                    reservationList = NULL;
                    reservationNumber = 0;
                }
            }


            movieCount--;
            if(movieCount > 0) {

                temp = new Movie[movieCount];
                int j = 0;
                for(int i = 0; i < movieCount + 1; i++) {
                    if(movies[i].getId() != movieID) {
                        temp[j] = movies[i];
                        j++;
                    }
                }
            }
            m->displayMovieInfo();
            cout << " has been canceled." << endl;

            for(int i = 0; i < (numOfRow - 1) / (m->getRadius() + 1) + 1; i++) {
                delete[] m->getRows()[i];
            }

            delete[] m->getRows();
            delete[] movies;

            if(movieCount > 0)
                movies = temp;
            else
                movies = NULL;
         }

         else {
            time_t t = movieID;
            string time = ctime(&t);
            time = time.substr(0, time.size() - 1); //To delete the newline character
            cout << "Movie at " << time << " does not exist" << endl;
         }
    }

    void MovieBookingSystem:: showAllMovies() {
        if(movieCount == 0) {
            cout << "No movie on show" << endl;
        }
        else {
            cout << "Movies on show: " << endl;
            for(int i = 0; i < movieCount; i++) {
                movies[i].displayMovieInfo();
                cout << "(" << movies[i].getSeat() << " available seats)" << endl;
            }
        }
    }

    void MovieBookingSystem::showMovie( const long movieID) {
        bool found = false;
        Movie* m;
        int i = 0;
         while(!found && i < movieCount) {
            long id = movies[i].getId();
            if(movieID == id) {
                m = &movies[i];
                found = true;
            }
            i++;
         }
         if(found) {
            m->displayMovieSeats();
         }
         else {
            time_t t = movieID;
            string time = ctime(&t);
            time = time.substr(0, time.size() - 1) + " "; //To delete the newline character
            cout << "Movie at " << time << "does not exist" << endl;
         }
     }

    int MovieBookingSystem::makeReservation( const long movieID, const int row, const char col) {


        char** seats;
        int r;
        Movie* m;
        Reservation* temp;

        bool found = false;
        int i = 0;
        while(!found && i < movieCount) {
            long id = movies[i].getId();
            if(movieID == id) {
                m = &movies[i];
                seats = m->getRows();
                r = m->getRadius();
                found = true;
            }
            i++;
         }

         if(found) {

            if((col - 'A') % (r + 1) == 0 && (row - 1) % (r + 1) == 0) { //Made use of ASCII values
                char& seatStatus = seats[(row - 1) / (r + 1)][(col - 'A') / (r + 1)];
                if(seatStatus == 'o') {
                    seatStatus = 'x';
                    m->setSeat(m->getSeat() - 1);

                    stringstream ss;
                    string s;

                    ss << col << row;
                    ss >> s;

                    reservationNumber++;
                    temp = new Reservation[reservationNumber];
                        for(int i = 0; i < reservationNumber - 1; i++) {
                            temp[i] = reservationList[i];
                    }
                    reservCode++;

                    Reservation r = Reservation(reservCode, s, m);
                    temp[reservationNumber - 1] = r;

                    if(reservationList != NULL) {
                        delete[] reservationList;
                    }
                    reservationList = temp;
                    cout << "Reservation done for " << s << " in ";
                    m->displayMovieInfo();
                    cout << endl;

                    return reservCode;
                }
                else {
                cout << "Seat " << col << row << " is not available in ";
                    m->displayMovieInfo();
                    cout << endl;
                }
            }
            else {
            cout << "Seat " << col << row << " is not occupiable in ";
            m->displayMovieInfo();
            cout << endl;
            }
         }
         else {
            time_t t = movieID;
            string time = ctime(&t);
            time = time.substr(0, time.size() - 1); //To delete the newline character
            cout << "Movie at " << time << " does not exist" << endl;
            return -1;
         }
         return 0;
    }

    void MovieBookingSystem::cancelReservations( const int numRes,const int* resCode) {
        int res = numRes;

        for(int i = 0; i < numRes; i++) {
            for(int j = 0; j < reservationNumber; j++) {
                if(reservationList[j].getCodeId() == resCode[i]) {
                    res--;
                }
            }
        }

        if(res == 0) {

            Movie* m;
            string seat;
            Reservation* temp;

            for(int i = 0; i < numRes; i++) {
               for(int j = 0; j < reservationNumber; j++) {
                    if(reservationList[j].getCodeId() == resCode[i]) {
                        m = reservationList[j].getMovie();
                        seat = reservationList[j].getSeat();
                        char col = seat[0];

                        int row;
                        stringstream ss(seat.substr(1));
                        ss >> row;

                        int r = m->getRadius();

                        m->getRows()[(row - 1) / (r + 1)][(col - 'A') / (r + 1)] = 'o';
                        m->setSeat(m->getSeat() + 1);

                        cout << "Reservation on Code " << resCode[i] << " is canceled " << "Seat: " << seat << " in ";
                        m->displayMovieInfo();
                        cout << endl;
                    }
               }
            }

               if(reservationNumber == numRes) {
                    delete[] reservationList;
                    reservationList = NULL;
                    reservationNumber = 0;
                }

                else {
                    bool cancel = false;
                    int k = 0;
                    temp = new Reservation[reservationNumber - numRes];

                    for(int i = 0; i < reservationNumber; i++) {
                        for(int j = 0; j < numRes; j++) {
                            if(reservationList[i].getCodeId() == resCode[j]) {
                                cancel = true;
                            }
                        }
                        if(!cancel) {
                            temp[k] = reservationList[i];
                            k++;
                        }
                        cancel = false;
                    }

                    reservationNumber = reservationNumber - numRes;
                    delete[] reservationList;
                    reservationList = temp;
                }
        }
        else {
            cout << "Some codes do not exist. Cancelation is failed." << endl;
        }
    }

    void MovieBookingSystem::showReservation( const int resCode) {
        Reservation* res = NULL;
        bool found = false;
        int i = 0;

        while(!found && i < reservationNumber) {
            if(resCode == reservationList[i].getCodeId()) {
                res = &reservationList[i];
                found = true;
            }
            i++;
        }
        if(found) {
            cout << "Reservation with Code " << res->getCodeId() << ": Seat " << res->getSeat() << " ";
            res->getMovie()->displayMovieInfo();
        }
        else {
            cout << "No reservation with code " << resCode << endl;
        }

    }

    Movie::Movie() {
        id = 0;
        radius = 0;
    }

    Movie::Movie(const long movieId, const int r) {
        const int rowNumber = ((MovieBookingSystem::numOfRow - 1) / (r + 1) + 1);
        const int columnNumber = ((MovieBookingSystem::numOfColumn - 1) / (r + 1) + 1);
        id = movieId;
        radius = r;

        time_t t = id;
        time = ctime(&t);
        time = time.substr(0, time.size() - 1) + " "; //To delete the newline character
        seat = rowNumber * columnNumber;
        rows = new char*[rowNumber];

        for (int j = 0; j < rowNumber; j++) {
            columns = new char [columnNumber];
            for(int i = 0; i < columnNumber; i++) {
                columns[i] = 'o';
            }
            rows[j] = columns;
        }

    }

    long Movie:: getId() const {
        return id;
    }

    int Movie::getRadius() const {
        return radius;
    }

    char** Movie::getRows() const {
        return rows;
    }

    int Movie::getSeat() const {
        return seat;
    }

    void Movie::setSeat(const int s) {
        seat = s;
    }

    void Movie::displayMovieInfo() {
        cout << "Movie at " << time;
    }

    void Movie::displayMovieSeats() {
        const int rowNumber = ((MovieBookingSystem::numOfRow - 1) / (radius + 1) + 1);
        const int columnNumber = ((MovieBookingSystem::numOfColumn - 1) / (radius + 1) + 1);

        displayMovieInfo();
        cout << "has " << seat << " available seats" << endl;
        string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
         cout << "   ";
         for(int i = 0; i < 26; i = i + radius + 1) {
            cout << letters[i] << " ";
         }
         cout << endl;

         for (int j = 0; j < rowNumber; j++) {
                int rowNo = j * (radius + 1) + 1;
                if(rowNo < 10)
                    cout << " ";
                cout << rowNo << " ";
            for(int i = 0; i < columnNumber; i++) {
                cout << rows[j][i] << " ";
            }
            cout << endl;
        }
    }

    Reservation::Reservation() {
    }

    Reservation::Reservation(int cId, string s, Movie* m) {
        codeId = cId;
        seat = s;
        movie = m;
    }

    int Reservation::getCodeId() const {
        return codeId;
    }

    string Reservation::getSeat() const {
        return seat;
    }

    Movie* Reservation:: getMovie() const {
        return movie;
    }




