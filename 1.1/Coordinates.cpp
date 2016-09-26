#include "Coordinates.h"
#include <iomanip>

using namespace std;

Coordinates::Coordinates()
{
    //ctor
}

Coordinates::Coordinates(int la_h, int la_m, int la_s, int lo_h, int lo_m, int lo_s)
{
    lati_hour_ = la_h;
    lati_minute_ = la_m;
    lati_second_ = la_s;
    longi_hour_ = lo_h;
    longi_minute_ = lo_m;
    longi_second_ = lo_s;
}

Coordinates::Coordinates(double lat, double lon)
{
    lati_hour_ = static_cast<int>(lat);
    lati_minute_ = static_cast<int>((lat - static_cast<double>(lati_hour_)) * 60.0);
    lati_second_ = static_cast<int>((lat - static_cast<double>(lati_hour_) -
                   (static_cast<double>(lati_minute_) / 60.0)) * 3600.0);
    longi_hour_ = static_cast<int>(lon);
    longi_minute_ = static_cast<int>((lon - static_cast<double>(longi_hour_)) * 60.0);
    longi_second_ = static_cast<int>((lon - static_cast<double>(longi_hour_) -
                   (static_cast<double>(longi_minute_) / 60.0)) * 3600.0);
}

void Coordinates::printCoords_deg()
{
    cout << setw(2) << setfill('0') << lati_hour_ << "°";
    cout << setw(2) << setfill('0') << lati_minute_ << "\'";
    cout << setw(2) << setfill('0') << lati_second_ << "\"N ";
    cout << setw(2) << setfill('0') << longi_hour_ << "°";
    cout << setw(2) << setfill('0') << longi_minute_ << "\'";
    cout << setw(2) << setfill('0') << longi_second_ << "\"E";
}

void Coordinates::printCoords_dezi()
{
    cout << setw(2) << setfill('0') << lati_hour_ << ".";
    printf("%4.0f",(lati_minute_/60.0*10000.0)+(lati_second_/3600.0*10000.0));
    cout << "°N ";
    cout << setw(2) << setfill('0') << longi_hour_ << ".";
    printf("%4.0f",(longi_minute_/60.0*10000.0)+(longi_second_/3600.0*10000.0));
    cout << "°E";
}

double Coordinates::lati_dezi()
{
    return static_cast<double>(lati_hour_) +
           (static_cast<double>(lati_minute_) / 60.0) +
           (static_cast<double>(lati_second_) / 3600.0);
}

double Coordinates::longi_dezi()
{
    return static_cast<double>(longi_hour_) +
           (static_cast<double>(longi_minute_) / 60.0) +
           (static_cast<double>(longi_second_) / 3600.0);
}

Coordinates::~Coordinates()
{
    //dtor
}
