#ifndef SITUATION_H
#define SITUATION_H

#include "Date.h"
#include "Coordinates.h"

const double PI = 3.14159265359;

class Situation
{
    public:
        Situation();
        virtual ~Situation();

        //Konstruktor mit allen Parametern
        Situation(Date datum, double ff, int dd, int cl, short int km);

        //Konstruktor ohne km (sollte standard sein)
        Situation(Date datum, double ff, int dd, int cl);

        //Funktionen zur Rückgabe der Daten
        double ret_ff();
        int ret_dd();
        int ret_cl();
        short int ret_km();
        short int ret_year();
        short int ret_month();
        short int ret_day();
        short int ret_hour();
        void printDate();
        
        //Funktion, um cl zu speichern
        void save_cl(int cl);
        
        //Funktion, um alles außer cl zu speichern
        void save_others(Date datum, double ff, int dd);

        //Funktion zur Berechnung der Ausbreitungsklasse nach Klug-Manier
        void km(Coordinates coord);

    protected:
    private:
        Date    datum_;
        double  ff_;
        int     dd_;
        int     cl_;
        short int km_ {9};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////Ab hier Funktionen für km mit Variationen///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

        //km Berechnung bei fehlender Bewölkungsinformation
        short int km_cl_err();

        //Rückgabe der AK für Nacht
        //I = 1, II = 2, III.1 = 3, III.2 = 4, IV = 5, V = 6
        short int km_night();

        //Rückgabe der AK für Tag
        //I = 1, II = 2, III.1 = 3, III.2 = 4, IV = 5, V = 6
        short int km_day();

        //Korrektur nach VDI-Richtlinie 3782 Blatt 1, Anhang A Nummer a)
        short int km_var_a(short int km);

        //Korrektur nach VDI-Richtlinie 3782 Blatt 1, Anhang A Nummer b)
        short int km_var_b(short int km);

        //Korrektur nach VDI-Richtlinie 3782 Blatt 1, Anhang A Nummer c)
        short int km_var_c(short int km, int sr, int ss);

        //Korrektur nach VDI-Richtlinie 3782 Blatt 1, Anhang A Nummver d)
        short int km_var_d(short int km);


/////////////////////////////////////////////////////////////////////////////////////////
/////////////Ab hier Funktionen für sunset/sunrise///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

        //prüfen, ob Winterzeit oder Sommerzeit
        //Winterzeit gibt true zurück, Sommerzeit false
        bool winterzeit();

        //Funktion die zurückgibt, ob Zeitpunkt in SA+1-SA+2 liegt
        bool sa_01(const double sr);

        //Funktion die zurückgibt, ob Zeitpunkt in SA+1-SA+2 liegt
        bool sa_12(const double sr);

        //Funktion die zurückgibt, ob Zeitpunkt in SA+2-SA+3 liegt
        bool sa_23(const double sr);

        //Funktion die zurückgibt, ob Zeitpunkt in SU-2-SU-1 liegt
        bool su_21(const double ss);

        //Funktion die zurückgibt, ob Zeitpunkt in SU-1-SU liegt
        bool su_10(const double ss);

        //Funktion die zurückgibt, ob Zeitpunkt in SU-SU+1 liegt
        bool su_01(const double ss);

        //Funktion zur Berechnung der Tage-seit-01.01.2000-Funktion
        double T_f();

        //Funktion zur Berechnung des Parameters M
        double M_f(double T);

        //Funktion zur Berechnung des seit Frühlingsanfang zurückgelegten Wegs
        double L_f(double M, double T);

        //Funktion zur Berechnung der Neigung der Erdachse
        double e_f(double T);

        //Funktion zur Berechnung der Deklination
        double DK_f(double e, double L);

        //Funktion zur Berechnung der Rektaszension
        double RA_f(double L, double e);

        //Funktion zur Berechnung der mittleren Rektaszension
        double RAm_f(double T);

        //Funktion zur Berechnung der Zeitlgiehcung
        double Zeitgl_f(double RA, double RAm);

        //Funktion zur Berechnung der Zeitdifferenz
        double Zeitdiff_f(double h, double B, double DK);

        //Funktion die Sonnenaufgang berechnet
        double sunrise(Coordinates coord);

        //Funktion die Sonnenuntergang berechnet
        double sunset(Coordinates coord);

};

#endif // SITUATION_H
