#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <Coordinates.h>



class Date
{
    //public-Bereich
    public:
        Date();
        virtual ~Date();

        //setter-Methode mit 3 Variablen
        Date(int year, int month, int day);

        //setter-Methode mit 4 Variablen
        Date(int year, int month, int day, int hour);

        //Funktionen, die die Werte des Typs zurückgeben
        int year();
        int month();
        int day();
        int hour();

//-------------------------------------------------------------------------------------
///////////////////////Memberfunktionen der Klasse Date////////////////////////////////
//-------------------------------------------------------------------------------------

        //Gibt das Datum aus (dd.mm.yyyy hh:00)
        void printDate();

        //Gibt true zurück wenn Schaltjahr, sonst false
        bool isLeapyear();

        //Gibt true zurück wenn Ende des Tages erreicht ist, sonst false
        bool endOfDay();

        //Gibt true zurück, wenn Ende des Monats erreicht ist, sonst false
        bool endOfMonth();

        //Gibt true zurück, wenn Ende das Jahres erreicht ist, sonst false
        bool endOfYear();

        //Funktion die die zeitliche Lage zweier Zeitpunkte zueinander bewertet
        //gibt 0 zurück, wenn Daten gleich sind
        //gibt 1 zurück, wenn this.date später liegt
        //gibt -1 zurück, wenn this.date früher liegt
        int compareToHour(Date cmpDate);

        //Funktion die die zeitliche Lage zweier Zeitpunkte zueinander bewertet
        //gibt 0 zurück, wenn Daten gleich sind
        //gibt 1 zurück, wenn this.date später liegt
        //gibt -1 zurück, wenn this.date früher liegt
        int compareToDay(Date cmpDate);

        //Setzt Date auf die erste Stunde (hour = 0) des nächsten Tages
        void nextDay();

        //Setzt Date auf die nächste Stunde
        void nextHour();


    //protected-Bereich
    protected:
    //private-Bereich
    private:
        short year_;
        short month_;
        short day_;
        short hour_;

};


#endif // DATE_H
