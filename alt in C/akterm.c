#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double pi = 3.14159265359;

//Programm zum Bestimmen und zur Ausgabe einer .AKTERM
//Als Inputparameter wird eine Zeitreihe der Wind- sowie der Bewölkungs-
//daten benötigt.
//Weiterhin wird aus einer Datei gelesen für welchen Zeitraum die .AKTERM
//erstellt werden soll.


//gibt Rundungswert von x als Intger zurück
int intround(double x){

    if(x >= 0.0){
        return (int)(x+.5);
    }else{
        return (int)(x-.5);
    }

}

//Datentyp für Datum
typedef struct Date{

    int hour;
    int day;
    int month;
    int year;

} Date;

//Datentyp für Zeit
typedef struct Time{

    int hour;
    int minute;

} Time;

//Datentyp für Koordinaten
typedef struct Coord{

    int bg;
    int bm;
    int bs;
    int lg;
    int lm;
    int ls;

} Coord;

//Funktion, um Datum mit Werten zu belegen
Date newDate(int hour, int day, int month, int year){

    Date date;
    date.hour = hour;
    date.day = day;
    date.month = month;
    date.year = year;
    return date;

}

//Funktion, um Koordinaten zu setzen
Coord newCoord(int bg, int bm, int bs, int lg, int lm, int ls){

    Coord new;

    new.bg = bg;
    new.bm = bm;
    new.bs = bs;
    new.lg = lg;
    new.lm = lm;
    new.ls = ls;

    return new;

}

//Funktion, um Koordinaten als Dezimalkoordinaten zurückzugeben (Breitengrad)
double decicoord_bg(Coord co){

    return (double)co.bg + ((double)co.bm)/60. + ((double)co.bs)/3600. ;

}

//Funktion, um Koordinaten als Dezimalkoordinaten zurückzugeben (Längengrad)
double decicoord_lg(Coord co){

    return (double)co.lg + ((double)co.lm)/60. + ((double)co.ls)/3600. ;

}

//Funktion um Lage zweier Daten zueinander zu vergleichen
//gibt 0 zurück, wenn Daten gleich
//gibt 1 zurück, wenn date1 früher liegt als date2
//gibt -1 zurück, wenn date2 früher liegt als date1
int cmpDates(Date date1, Date date2){

    if(date1.year == date2.year){
        if(date1.month == date2.month){
            if(date1.day == date2.day){
                if(date1.hour == date2.hour){

                    return 0;

                }else if(date1.hour < date2.hour){

                    return 1;

                }else{

                    return -1;

                }

            }else if(date1.day < date2.day){

                return 1;

            }else{

                return -1;

            }
        }else if(date1.month < date2.month){

            return 1;

        }else{

            return -1;

        }
    }else if(date1.year < date2.year){

        return 1;

    }else{

        return -1;

    }

}

//Datum im Format dd.mm.yyyy hh:mm ausgeben
void printDate(Date date){

    printf("%02d.%02d.%4d %02d:00\n", date.day, date.month, date.year, date.hour);

}

//Zeit im Format hh:mm ausgeben
void printTime(Time time){

    printf("%02d:%02d\n", time.hour, time.minute);

}

//Funktion zur überprüfung ob Jahr Schaltjahr ist
//gibt 0 zurück, wenn kein Schaltjahr, bei Schaltjahr 1
int isLeapyear(int year){

    if(year % 4 == 0){

        if(year % 100 == 0){

            if(year % 400 == 0){
                return 1;
            }else{
                return 0;
            }

        }else{
            return 1;
        }

    }else{
        return 0;
    }

}

//Funktion zur Überprüfung, ob Tag am Ende des Monats liegt
//gibt 0 zurück wenn nicht, 1 wenn schon
int endOfMonth(Date date){

    if((date.day == 31) && (date.month == 1 || date.month == 3 || date.month == 5 ||
        date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12)){

        return 1;

    }else if((date.day == 30) && (date.month == 4 || date.month == 6 || date.month == 9 ||
        date.month == 11)){

        return 1;

    }else if((date.day == 28) && (date.month == 2) && !isLeapyear(date.year)){

        return 1;

    }else if((date.day == 29) && (date.month == 2) && isLeapyear(date.year)){

        return 1;

    }else{

        return 0;

    }

}

//Funktion zur Überprüfung, ob Tag am Ende des Jahres liegt
//gibt 0 zurück wenn nicht, 1 wenn schon
int endOfYear(Date date){

    if(endOfMonth(date) && date.month == 12){

        return 1;

    }else{

        return 0;

    }

}

//Funktion zur Rückgabe des darauffolgenden Tages
Date nextDay(Date date){

    Date nextDate;
    nextDate.hour = date.hour;

    if(endOfMonth(date)){
        if(endOfYear(date)){

            nextDate.day = 1;
            nextDate.month = 1;
            nextDate.year = date.year + 1;

        }else{

            nextDate.day = 1;
            nextDate.month = date.month + 1;
            nextDate.year = date.year;

        }
    }else{

        nextDate.day = date.day + 1;
        nextDate.month = date.month;
        nextDate.year = date.year;

    }

    return nextDate;

}

//Funktion zur Rückgabe der darauffolgenden Stunde
Date nextHour(Date date){

    Date nextHour;

    if(date.hour == 23){
        nextHour = nextDay(date);
        nextHour.hour = 0;
    }else{
        nextHour = date;
        nextHour.hour = nextHour.hour+1;
    }
    return nextHour;
}

//Funktion zur Berechnung des Sonnenaufgangs
Time sunrise_time(Date date, Coord coords){

    Time sunrise;
    double aufgang_oz;
    double aufgang;
    date.hour = 0;
    Date test_date;
    int days_since_2k = 0;
    double T;
    double M;
    double L;
    double e;
    double DK;
    double RA;
    double RAm;
    double ZG;
    double ZD;
    double B;
    double h;
    int timezone;

    //Winterzeit
    if((date.month < 3) ||
       ((date.month == 3) && (date.day <= 28)) ||
       ((date.month == 10) && (date.day > 28))  ||
       (date.month > 10)){

          timezone = 1;
    //Sommerzeit
    }else{

          timezone = 2;

    }


    B = decicoord_bg(coords);

    test_date = newDate(0, 1, 1, 2000);
    while(cmpDates(test_date, date) == 1){
        days_since_2k++;
        test_date = nextDay(test_date);
    }
    T = (double)days_since_2k/36525.;
    M = 2.*pi*(0.993133+99.997361*T);
    //M in Intervall zwischen 0 und 2pi bringen
    while(M > 2.*pi){
       M = M - 2.*pi;
    }
    while(M < 0.){
       M = M + 2.*pi;
    }
    L = 2.*pi*(0.7859453+M/(2.*pi)+(6893.*sin(M)+72.*sin(2.*M)+6191.2*T)/1296000.);
    //L in Intervall zwischen 0 und 2pi bringen
    while(L > 2.*pi){
       L = L - 2.*pi;
    }
    while(L < 0.){
       L = L + 2.*pi;
    }

    e = 2.*pi*(23.43929111+(-46.815*T-0.00059*T*T+0.001813*T*T*T)/3600.)/360.;
    DK = asin(sin(e)*sin(L));
    RA = atan(tan(L)*cos(e));
    if(RA < 0.0){
        RA = RA + pi;
    }
    if(L > pi){
        RA = RA + pi;
    }



    RAm = 18.71506921 + 2400.0513369 * T + (2.5862*pow(10.0, -5.0) - 1.72 *
          pow(10.0,-9.0)*T)*T*T;
    while(RAm > 24.0){
       RAm = RAm - 24.0;
    }
    while(RAm < 0.0){
       RAm = RAm + 24.0;
    }
    //Zeitgleichung berechnen
    ZG = 1.0027379 * (RAm - (RA * 3.81972));

    h = -0.0145;
    //Zeitdifferenz brechnen
    ZD = 12.0 * acos((sin(h)-sin(B/57.29578)*sin(DK))/
         (cos(B/57.29578)*cos(DK)))/pi;

    aufgang_oz = 12.0 - ZD - ZG;
    aufgang = aufgang_oz - ((double)coords.lg/15.0) + timezone;

    sunrise.hour = (int)aufgang;
    sunrise.minute = (int)((aufgang-(double)sunrise.hour)*60.0);

    return sunrise;

}

//Funktion zur Berechnung des Sonnenuntergangs
Time sunset_time(Date date, Coord coords){

    Time sunset;
    double untergang_oz;
    double untergang;
    date.hour = 0;
    Date test_date;
    int days_since_2k = 0;
    double T;
    double M;
    double L;
    double e;
    double DK;
    double RA;
    double RAm;
    double ZG;
    double ZD;
    double B;
    double h;
    int timezone;

    //Winterzeit
    if((date.month < 3) ||
       ((date.month == 3) && (date.day <= 28)) ||
       ((date.month == 10) && (date.day > 28))  ||
       (date.month > 10)){

          timezone = 1;
    //Sommerzeit
    }else{

          timezone = 2;

    }


    B = decicoord_bg(coords);

    test_date = newDate(0, 1, 1, 2000);
    while(cmpDates(test_date, date) == 1){
        days_since_2k++;
        test_date = nextDay(test_date);
    }
    T = (double)days_since_2k/36525.;
    M = 2.*pi*(0.993133+99.997361*T);
    //M in Intervall zwischen 0 und 2pi bringen
    while(M > 2.*pi){
       M = M - 2.*pi;
    }
    while(M < 0.){
       M = M + 2.*pi;
    }
    L = 2.*pi*(0.7859453+M/(2.*pi)+(6893.*sin(M)+72.*sin(2.*M)+6191.2*T)/1296000.);
    //L in Intervall zwischen 0 und 2pi bringen
    while(L > 2.*pi){
       L = L - 2.*pi;
    }
    while(L < 0.){
       L = L + 2.*pi;
    }

    e = 2.*pi*(23.43929111+(-46.815*T-0.00059*T*T+0.001813*T*T*T)/3600.)/360.;
    DK = asin(sin(e)*sin(L));
    RA = atan(tan(L)*cos(e));
    if(RA < 0.0){
        RA = RA + pi;
    }
    if(L > pi){
        RA = RA + pi;
    }


    RAm = 18.71506921 + 2400.0513369 * T + (2.5862*pow(10.0, -5.0) - 1.72 *
          pow(10.0,-9.0)*T)*T*T;
    while(RAm > 24.0){
       RAm = RAm - 24.0;
    }
    while(RAm < 0.0){
       RAm = RAm + 24.0;
    }
    //Zeitgleichung berechnen
    ZG = 1.0027379 * (RAm - (RA * 3.81972));

    h = -0.0145;
    //Zeitdifferenz brechnen
    ZD = 12.0 * acos((sin(h)-sin(B/57.29578)*sin(DK))/
         (cos(B/57.29578)*cos(DK)))/pi;

    untergang_oz = 12.0 + ZD - ZG;
    untergang = untergang_oz - ((double)coords.lg/15.0) + timezone;

    sunset.hour = (int)untergang;
    sunset.minute = (int)((untergang-(double)sunset.hour)*60.0);

    return sunset;

}

//Funktion, um Tag-Ausbreitungsklasse zu berechnen
int km_day(double ff, int cloudy){

   int km = 9;

   //Einteilung nach Bedeckungsgrad
     switch(cloudy){

         case 0: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 5;
                 }else if(ff <= 3.5){
                     km = 5;
                 }else if(ff <= 4.5){
                     km = 5;
                 }else if(ff > 4.5){
                     km = 4;
                 }
             break;
         case 1: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 5;
                 }else if(ff <= 3.5){
                     km = 5;
                 }else if(ff <= 4.5){
                     km = 5;
                 }else if(ff > 4.5){
                     km = 4;
                 }
             break;
         case 2: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 5;
                 }else if(ff <= 3.5){
                     km = 5;
                 }else if(ff <= 4.5){
                     km = 5;
                 }else if(ff > 4.5){
                     km = 4;
                 }
             break;
         case 3: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 5;
                 }else if(ff <= 3.5){
                     km = 5;
                 }else if(ff <= 4.5){
                     km = 4;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 4: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 5;
                 }else if(ff <= 3.5){
                     km = 5;
                 }else if(ff <= 4.5){
                     km = 4;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 5: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 5;
                 }else if(ff <= 3.5){
                     km = 5;
                 }else if(ff <= 4.5){
                     km = 4;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 6: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 4;
                 }else if(ff <= 3.5){
                     km = 4;
                 }else if(ff <= 4.5){
                     km = 4;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 7: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 4;
                 }else if(ff <= 3.5){
                     km = 4;
                 }else if(ff <= 4.5){
                     km = 4;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 8: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 5;
                 }else if(ff <= 2.5){
                     km = 4;
                 }else if(ff <= 3.5){
                     km = 4;
                 }else if(ff <= 4.5){
                     km = 4;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
     }

     return km;

}

//Funktion, um Nacht-Ausbritungsklasse zu berechnen
int km_night(double ff, int cloudy){

   int km = 9;

   //Einteilung nach Bedeckungsgrad
     switch(cloudy){

         case 0: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 1: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 2: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 3: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 4: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 5: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 6: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 1;
                 }else if(ff <= 2.5){
                     km = 1;
                 }else if(ff <= 3.5){
                     km = 2;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 7: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 2;
                 }else if(ff <= 2.5){
                     km = 2;
                 }else if(ff <= 3.5){
                     km = 3;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
         case 8: //Einteilung nach Windgeschwindigkeit
                 if(ff <= 1.5){
                     km = 2;
                 }else if(ff <= 2.5){
                     km = 2;
                 }else if(ff <= 3.5){
                     km = 3;
                 }else if(ff <= 4.5){
                     km = 3;
                 }else if(ff > 4.5){
                     km = 3;
                 }
             break;
     }

     return km;

}

//Funktion zur Berechnung und Zurückgabe der Ausbreitungsklasse nach Klug/Manier
//VDI Richtlinie 3782.1
//Erklärung zu km:
//1 = I, 2 = II, 3 = III.1, 4 = III.2, 5 = IV, 6 = V
int klug_manier(Date date, Coord coords, double ff, int cloudy){

    int km;
    int km_daytime;
    int km_nighttime;
    Time sunset;
    Time sunrise;

    sunrise = sunrise_time(date, coords);
    sunset = sunset_time(date, coords);

    //Überprüfen, ob Tag- oder Nachtstunde
    //Tag
    if(date.hour > (sunrise.hour+1) && date.hour < sunset.hour){

        km = km_day(ff, cloudy);

    //Nacht
    }else{

        km = km_night(ff, cloudy);

    }

    //km nach Tabelle bestimmt. Sonderregeln/Modifikationen folgen
    //VDI 3782.1 Anhang A, Satz a)
    if((date.month >= 6 && date.month <= 8) && (date.hour >= 10 && date.hour <= 16)
        && (km < 6) && ((cloudy <= 6) || ((cloudy <= 7) && (ff < 2.5)))){
        km++;
    }else if((date.month >= 6 && date.month <= 8) && (date.hour >= 12 && date.hour <= 15)
              && (cloudy <= 5) && (km < 6)){
        km++;
    }//VDI 3782.1 Anhang A Satz b)
    else if((date.month == 5 || date.month == 9) && (date.hour >= 11 && date.hour <= 15)
             && (cloudy <= 6) && (km < 6)){
        km++;
    }

    //VDI 3782.1 Anhang A Satz c)
    //Anpassung nahe Sonnenauf- bzw. untergang

    //prüfen ob Zeit in SA+1 bis SA+2 liegt
    if(((sunrise.minute == 0) && (date.hour == sunrise.hour+1)) ||
       ((date.hour == sunrise.hour+2) && (sunrise.minute != 0))){
         //ja -> anpassen
         km_daytime = km_day(ff, cloudy);
         km_nighttime = km_night(ff, cloudy);
         if((km_nighttime == 1) && (km_daytime == 5)){
            if(((date.month >= 3) || (date.month <= 11)) && (ff > 1.0)){
               km = 2;
            }else{
               km = 1;
            }
         }else if((km_nighttime == 1) && (km_daytime == 4)){
            km = 2;
         }else if((km_nighttime == 2) && (km_daytime == 5)){
            km = 2;
         }else if((km_nighttime == 2) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 5)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 3)){
            km = 3;
         }
    //überprüfen ob Zeit im SA+2 bis SA+3 liegt
    }else if(((sunrise.minute == 0) && (date.hour == sunrise.hour+2)) ||
       ((date.hour == sunrise.hour+3) && (sunrise.minute != 0))){
          //ja -> anpassen
         km_daytime = km_day(ff, cloudy);
         km_nighttime = km_night(ff, cloudy);
         if((km_nighttime == 1) && (km_daytime == 5)){
            km = 2;
         }else if((km_nighttime == 1) && (km_daytime == 4)){
            km = 2;
         }else if((km_nighttime == 2) && (km_daytime == 5)){
            km = 3;
         }else if((km_nighttime == 2) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 5)){
            km = 4;
         }else if((km_nighttime == 3) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 3)){
            km = 3;
         }
      //überprüfen ob Zeit in SU-2 bis SU-1 liegt
      }else if(((sunset.minute == 0) && (date.hour == sunset.hour-2)) ||
       ((date.hour == sunset.hour-1) && (sunset.minute != 0))){
         //ja -> anpassen
         km_daytime = km_day(ff, cloudy);
         km_nighttime = km_night(ff, cloudy);
         if((km_nighttime == 1) && (km_daytime == 5)){
            km = 2;
         }else if((km_nighttime == 1) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 2) && (km_daytime == 5)){
            km = 3;
         }else if((km_nighttime == 2) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 5)){
            km = 4;
         }else if((km_nighttime == 3) && (km_daytime == 4)){
            km = 4;
         }else if((km_nighttime == 3) && (km_daytime == 3)){
            km = 3;
         }
      //überprüfen ob Zeit in SU-1 bis SU liegt
      }else if(((sunset.minute == 0) && (date.hour == sunset.hour-1)) ||
       ((date.hour == sunset.hour) && (sunset.minute != 0))){
         //ja -> anpassen
         km_daytime = km_day(ff, cloudy);
         km_nighttime = km_night(ff, cloudy);
         if((km_nighttime == 1) && (km_daytime == 5)){
            if((date.month == 1 || date.month == 2 || date.month == 12)
               && (ff <= 1.0) && (cloudy <= 6)){
                  km = 1;
            }else{
                  km = 2;
            }
         }else if((km_nighttime == 1) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 2) && (km_daytime == 5)){
            km = 2;
         }else if((km_nighttime == 2) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 5)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 4)){
            km = 4;
         }else if((km_nighttime == 3) && (km_daytime == 3)){
            km = 3;
         }
      //überprüfen ob Zeit in SU bis SU+1 liegt
      }else if(((sunset.minute == 0) && (date.hour == sunset.hour)) ||
       ((date.hour == sunset.hour+1) && (sunset.minute != 0))){
         //ja -> anpassen
         km_daytime = km_day(ff, cloudy);
         km_nighttime = km_night(ff, cloudy);
         if((km_nighttime == 1) && (km_daytime == 5)){
            if(((date.month >= 3) || (date.month <= 11)) && (ff > 1.0)){
               km = 2;
            }else{
               km = 1;
            }
         }else if((km_nighttime == 1) && (km_daytime == 4)){
            if(((date.month >= 3) || (date.month <= 11)) && (ff > 1.0)){
               km = 2;
            }else{
               km = 1;
            }
         }else if((km_nighttime == 2) && (km_daytime == 5)){
            km = 2;
         }else if((km_nighttime == 2) && (km_daytime == 4)){
            km = 2;
         }else if((km_nighttime == 3) && (km_daytime == 5)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 4)){
            km = 3;
         }else if((km_nighttime == 3) && (km_daytime == 3)){
            km = 3;
         }
      }

    //VDI 3782.1 Anhang A Satz d)
    //Anpassung für Wintermonate
    if((date.month == 1 || date.month == 2 || date.month == 12) &&
       (km == 5)){
          km = 4;
    }

    return km;

}

//Funktion für km bei fehlenden Wolkendaten
int klug_manier_error(double ff){

   int km;
   if(ff <= 2.5){
      km = 1;
   }else if(ff > 2.5 && ff <= 3.5){
      km = 2;
   }else if(ff > 3.5){
      km = 3;
   }

   return km;

}

//////////////////////////////////MAIN///////////////////////////////////////////
int main(void){

    FILE *fp_in;
    FILE *fp_in2;
    FILE *fp_out;
    char file_input_wind[100];
    char file_input_clouds[100];
    char file_output[100];
    char station_name[30];
    char station_id[10];
    int c1, c2, c3, c4, c5, c6;
    long i;
    double height_station, height_ane;
    double z0s;
    double h_ref;
    Date date_from;
    Date date_to;
    Coord coordinates;

    fp_in = fopen("input_akterm.txt", "r");
    fscanf(fp_in,"%*s %s %s", station_name, station_id);
    fscanf(fp_in,"%*s %*s %*s %*s %d %d %d", &date_from.day, &date_from.month,
                                             &date_from.year);
    date_from.hour = 0;
    fscanf(fp_in,"%*s %*s %*s %s", file_input_wind);
    fscanf(fp_in,"%*s %*s %*s %s", file_input_clouds);
    fscanf(fp_in,"%*s %*s %*s %s", file_output);
    fscanf(fp_in,"%*s %*s %d %d %d", &c1, &c2, &c3);
    fscanf(fp_in,"%*s %d %d %d", &c4, &c5, &c6);
    fscanf(fp_in,"%*s %*s %*s %lf", &height_station);
    fscanf(fp_in,"%*s %*s %*s %lf", &height_ane);
    fscanf(fp_in,"%*s %*s %*s %*s %*s %lf", &z0s);
    fscanf(fp_in,"%*s %*s %lf", &h_ref);

    fclose(fp_in);
    //config-Datei eingelesen

    date_to = date_from;
    date_to.hour = 23;
    for(i=1;i<365;i++){
        date_to = nextDay(date_to);
    }

    coordinates = newCoord(c1, c2, c3, c4, c5, c6);

/////////////////////////////////////////////////////////////////////////////////
    //Anemometerhöhen berechnen
    int ha[9];
    double z0a[9];
    double d0a;
    double d0s;
    double ps;

    z0a[0] = 0.01;
    z0a[1] = 0.02;
    z0a[2] = 0.05;
    z0a[3] = 0.10;
    z0a[4] = 0.20;
    z0a[5] = 0.50;
    z0a[6] = 1.00;
    z0a[7] = 1.50;
    z0a[8] = 2.00;

    d0s = 6.0*z0s;

    ps = (log((height_ane - d0s) / z0s)) / (log((h_ref - d0s) / z0s));

    for(i=0;i<9;i++){

        d0a = 6.0 * z0a[i];
        ha[i] = intround((d0a + z0a[i] * pow(((h_ref - d0a) / z0a[i]), ps)) * 10.0);
        if(ha[i] < 40){
            ha[i] = 40;
        }

    }


/////////////////////////////////////////////////////////////////////////////////
    //.AKTERM Header erstellen
    fp_out = fopen(file_output, "w");
    fprintf(fp_out,"* AKTERM_Zeitreihe, Barth&Bitter GmbH, Hannover\n"
                   "* Zeitraum %02d.%02d.%04d bis %02d.%02d.%04d\n"
                   "* Station %s %s, %.4f°N, %.4f°E, %.1f m ü NN\n"
                   "* Standortparameter ha=%.1f m, z0s=%.3f m\n"
                   "+ Anemometerhoehen (0.1 m):%5d%5d%5d%5d%5d%5d%5d%5d%5d\n",
                   date_from.day, date_from.month, date_from.year,
                   date_to.day, date_to.month, date_to.year, station_id, station_name,
                   decicoord_bg(coordinates), decicoord_lg(coordinates),
                   height_station, height_ane, z0s,
                   ha[0],ha[1],ha[2],ha[3],ha[4],ha[5],ha[6],ha[7],ha[8]);

    //Header fertig
/////////////////////////////////////////////////////////////////////////////////
    //.AKTERM erstellen
    int qdd;
    int qff;
    int km;
    int cloudy;
    double ff_in;
    int dd_in;
    Date date_expected;
    Date date_ff_in;
    Date date_cl_in;

    //QDD und QFF nicht variabel. Annahme, dass DD und FF immer in ° bzw. m/s kommen
    qdd = 2;
    qff = 3;

    //Zeiger in Inputdateien richtig setzen
    fp_in = fopen(file_input_wind, "r");
    fp_in2 = fopen(file_input_clouds, "r");
    //Wind-Datei
    date_expected = date_from;
    fscanf(fp_in, "%*s %*s %*s %*s %*s");
    fscanf(fp_in, "%*5s %4d %2d %2d %2d %*s %*s %*s %lf %*1s %d %*s",
                  &date_ff_in.year,&date_ff_in.month,&date_ff_in.day,&date_ff_in.hour,
                  &ff_in, &dd_in);
    while(cmpDates(date_ff_in,date_expected) == 1){
        fscanf(fp_in, "%*5s %4d %2d %2d %2d %*s %*s %*s %lf %*1s %d %*s",
                  &date_ff_in.year,&date_ff_in.month,&date_ff_in.day,&date_ff_in.hour,
                  &ff_in, &dd_in);
    }
    //Programmabbruch, wenn date_from nicht in Wind-Datei
    if(cmpDates(date_ff_in,date_expected) == -1){
        printf("Startdatum nicht in Winddatei enthalten...\n");
        return 1;
    }

    //Wolken-Datei
    fscanf(fp_in2, "%*s %*s %*s %*s");
    fscanf(fp_in2, "%*5s %4d %2d %2d %2d %*s %*s %d %*s",
                   &date_cl_in.year,&date_cl_in.month,&date_cl_in.day,&date_cl_in.hour,
                   &cloudy);
    while(cmpDates(date_cl_in,date_expected) == 1){
        fscanf(fp_in2, "%*5s %4d %2d %2d %2d %*s %*s %d %*s",
                   &date_cl_in.year,&date_cl_in.month,&date_cl_in.day,&date_cl_in.hour,
                   &cloudy);
    }
    //Programmabbruch, wenn date_from nicht in Wolken-Datei
    if(cmpDates(date_cl_in,date_expected) == -1){
        printf("Startdatum nicht in Wolkendatei enthalten...\n");
        return 1;
    }

    int error_count_ff = 0;
    int error_count_cl = 0;

    //Zeiger in Input-Dateien stehen auf date_from
    for(i = 0; i < 8760; i++){
        if(cloudy == -1 || cloudy == -999){
            error_count_cl++;
        }
        if(ff_in < 0.0 || ff_in == -999){
            error_count_ff++;
        }

        //Ausbreitungsklasse brechnen nach Klug/Manier
        //dazu brauchst du: Windgeschwindigkeit, Wolke, Datum/Zeit
        if((cmpDates(date_ff_in, date_expected) == 0) &&
           (cmpDates(date_cl_in, date_expected) == 0)){
            //beide Dateien enthalten Daten. Super!
            //km berechnen, printen und aus beiden Dateien neue Zeile auslesen

            if(cloudy == -1 || cloudy == -999){
                km = klug_manier_error(ff_in);
            }else{
                km = klug_manier(date_expected, coordinates, ff_in, cloudy);
            }

            fprintf(fp_out,"AK %5s %4d %02d %02d %02d 00 %1d %1d %3d %3d 1 %1d"
                               " 1 -999 9\n", station_id, date_expected.year,
                               date_expected.month, date_expected.day,
                               date_expected.hour, qdd, qff, dd_in,
                               intround(ff_in*10.0), km);

            fscanf(fp_in, "%*5s %4d %2d %2d %2d %*s %*s %*s %lf %*1s %d %*s",
                  &date_ff_in.year,&date_ff_in.month,&date_ff_in.day,&date_ff_in.hour,
                  &ff_in, &dd_in);

            fscanf(fp_in2, "%*5s %4d %2d %2d %2d %*s %*s %d %*s",
                       &date_cl_in.year,&date_cl_in.month,&date_cl_in.day,&date_cl_in.hour,
                       &cloudy);

        }else{
            //Fehlende Zeile in einer der Dateien
            printf("Fehler in Datei!\n");

            if(cmpDates(date_ff_in, date_expected) == -1){
                //Zeile in Wind-Datei fehlt
                printf("Winddatei-Fehler\n");
                km = 9;
                error_count_ff++;
                fprintf(fp_out,"AK %5s %4d %02d %02d %02d 00 %1d %1d 999 999 1 %1d"
                               " 1 -999 9\n", station_id, date_expected.year,
                               date_expected.month, date_expected.day,
                               date_expected.hour, qdd, qff, km);

                //prüfen, ob Wolken-Datei korrekt, dann Zeile auslesen
                if(cmpDates(date_cl_in, date_expected) == 0){
                    fscanf(fp_in2, "%*5s %4d %2d %2d %2d %*s %*s %d %*s",
                       &date_cl_in.year,&date_cl_in.month,&date_cl_in.day,&date_cl_in.hour,
                       &cloudy);
                }else{
                    printf("Wolkendatei-Fehler\n");
                   error_count_cl++;
                }
            }else{
                //Zeile in Wind-Datei korrekt
                //d.h. fehlende Zeile in Wolken-Datei
                printf("Wolkendatei-Fehler\n");
                error_count_cl++;
                km = klug_manier_error(ff_in);
                fprintf(fp_out,"AK %5s %4d %02d %02d %02d 00 %1d %1d %3d %3d 1 %1d"
                               " 1 -999 9\n", station_id, date_expected.year,
                               date_expected.month, date_expected.day,
                               date_expected.hour, qdd, qff, dd_in,
                               intround(ff_in*10.0), km);

                fscanf(fp_in, "%*5s %4d %2d %2d %2d %*s %*s %*s %lf %*1s %d %*s",
                  &date_ff_in.year,&date_ff_in.month,&date_ff_in.day,&date_ff_in.hour,
                  &ff_in, &dd_in);
            }

        }

        date_expected = nextHour(date_expected);

    }

    printf("Fehler in Wolkendatei: %d\n", error_count_cl);
    printf("Fehler in Winddatei: %d\n", error_count_ff);


    fclose(fp_out);

    return 0;
}
