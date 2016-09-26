#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>
#include <stdio.h>
#include <iomanip>

class Coordinates
{
    public:
        Coordinates();
        virtual ~Coordinates();

        //Konstruktor, wenn Koordinaten in Stunden, Minuten, Sekunden eingegeben werden
        Coordinates(int la_h, int la_m, int la_s, int lo_h, int lo_m, int lo_s);

        //Konstruktor, wenn Koordinaten in Dezimalgrad eingegeben werden
        Coordinates(double lat, double lon);
        //Funktion, die Koordinaten in der Form hh°mm'ss" ausgibt
        void printCoords_deg();
        //Funktion, die Koordinaten in Dezimalform ausgibt
        void printCoords_dezi();

        //Funktion, die Breitengrad als Dezimalwert zurückliefert
        double lati_dezi();
        //Funktion, die Längengrad als Dezimalwert zurückliefert
        double longi_dezi();


    protected:
    private:
        int lati_hour_;
        int lati_minute_;
        int lati_second_;
        int longi_hour_;
        int longi_minute_;
        int longi_second_;
};

#endif // COORDINATES_H
