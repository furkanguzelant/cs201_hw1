/**
 * @author Furkan Güzelant ID: 21901515
 * @version 21.03.2021

*/
#ifndef __MOVIEBOOKINGSYSTEM_H
#define __MOVIEBOOKINGSYSTEM_H
#include <string>
using namespace std;
class Movie;
class Reservation;
class MovieBookingSystem{
public:
    const static int numOfRow = 30;
    const static int numOfColumn = 26;
    MovieBookingSystem();
    ~MovieBookingSystem();


    void addMovie( const long movieID, const int audienceRadius );
    void cancelMovie( const long movieID);
    void showAllMovies();
    void showMovie( const long movieID);
    int makeReservation( const long movieID, const int row, const char col);
    void cancelReservations( const int numRes,const int* resCode);
    void showReservation( const int resCode);
private:
    Movie* movies;
    int movieCount;
    Reservation* reservationList;
    int reservationNumber;
    int reservCode;
};

class Movie{
public:

    Movie();
    Movie(const long movieId, const int r);
    long getId() const;
    int getRadius() const;
    char** getRows() const;
    int getSeat() const;
    void setSeat(const int s);
    void displayMovieInfo();
    void displayMovieSeats();

private:
    int radius;
    long id;
    char** rows;
    char* columns;
    int seat;
    string time;
};

class Reservation{
public:
    Reservation();
    Reservation(int cId, string s, Movie* m);
    int getCodeId() const;
    string getSeat() const;
    Movie* getMovie() const;
private:
    int codeId;
    string seat;
    Movie* movie;
};
#endif
