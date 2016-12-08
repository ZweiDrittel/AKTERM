#include "Situation.h"

Situation::Situation()
{
    //ctor
}

Situation::Situation(Date datum, double ff, int dd, int cl, short int km)
{
    datum_ = datum;
    ff_ = ff;
    dd_ = dd;
    cl_ = cl;
    km_ = km;
}

Situation::Situation(Date datum, double ff, int dd, int cl)
{
    datum_ = datum;
    ff_ = ff;
    dd_ = dd;
    cl_ = cl;
}

double Situation::ret_ff()
{
    return ff_;
}
int Situation::ret_dd()
{
    return dd_;
}
int Situation::ret_cl()
{
    return cl_;
}
short int Situation::ret_km()
{
    return km_;
}
Date Situation::ret_date()
{
	return datum_;
}
short int Situation::ret_year()
{
    return datum_.year();
}
short int Situation::ret_month()
{
    return datum_.month();
}
short int Situation::ret_day()
{
    return datum_.day();
}
short int Situation::ret_hour()
{
    return datum_.hour();
}
void Situation::printDate()
{
    datum_.printDate();
}

void Situation::save_cl(int cl)
{
	cl_ = cl;
}

void Situation::save_others(Date datum, double ff, int dd)
{
	datum_ = datum;
	ff_ = ff;
	dd_ = dd;
}

void Situation::km(Coordinates coord)
{
    //Fehler in ff_
    if(ff_ == 999)
    {
        km_ = 9;
        return;
    }
    //Fehler in cl_
    if(cl_ == 999)
    {
        km_ = km_cl_err();
        return;
    }

    const double sr {sunrise(coord)};
    const double ss {sunset(coord)};
    short int km;
    bool nighttime;

    //prüfen, ob Zeitpunkt in der Nacht oder am Tage liegt
    nighttime = ((datum_.hour() < sr) || sa_01(sr) || (datum_.hour() > ss)) ? true : false;
    //km setzen. Veränderungen folgen
    km = nighttime ? km_night() : km_day();
    //Veränderung von km nach Anhang A Nummer a)
    km = km_var_a(km);
    //Veränderung von km nach Anhang A Nummer b)
    km = km_var_b(km);
    //Veränderung von km nach Anhang A Nummer c)
    km = km_var_c(km, sr, ss);
    //Veränderung von km nach Anhang A Nummer d)
    km = km_var_d(km);

    km_ = km;

}

short int Situation::km_cl_err()
{
    if(ff_ < 2.5)
    {
        return 1;
    }
    else if(ff_ < 3.5)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

short int Situation::km_night()
{
    switch(cl_)
    {
        case 0 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 1 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 2 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 3 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 4 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 5 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 6 :
            if(ff_ <= 1.5)
            {
                return 1;
            }
            else if(ff_ <= 2.5)
            {
                return 1;
            }
            else if(ff_ <= 3.5)
            {
                return 2;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 7 :
            if(ff_ <= 1.5)
            {
                return 2;
            }
            else if(ff_ <= 2.5)
            {
                return 2;
            }
            else if(ff_ <= 3.5)
            {
                return 3;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        case 8 :
            if(ff_ <= 1.5)
            {
                return 2;
            }
            else if(ff_ <= 2.5)
            {
                return 2;
            }
            else if(ff_ <= 3.5)
            {
                return 3;
            }
            else if(ff_ <= 4.5)
            {
                return 3;
            }
            else
            {
                return 3;
            }
            break;

        default :
            return 9;
    }
}

short int Situation::km_day()
{
    switch(cl_)
    {
        case 0 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 5;
            }
            else if(ff_ <= 3.5)
            {
                return 5;
            }
            else if(ff_ <= 4.5)
            {
                return 5;
            }
            else
            {
                return 4;
            }
            break;

        case 1 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 5;
            }
            else if(ff_ <= 3.5)
            {
                return 5;
            }
            else if(ff_ <= 4.5)
            {
                return 5;
            }
            else
            {
                return 4;
            }
            break;

        case 2 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 5;
            }
            else if(ff_ <= 3.5)
            {
                return 5;
            }
            else if(ff_ <= 4.5)
            {
                return 5;
            }
            else
            {
                return 4;
            }
            break;

        case 3 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 5;
            }
            else if(ff_ <= 3.5)
            {
                return 5;
            }
            else if(ff_ <= 4.5)
            {
                return 4;
            }
            else
            {
                return 3;
            }
            break;

        case 4 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 5;
            }
            else if(ff_ <= 3.5)
            {
                return 5;
            }
            else if(ff_ <= 4.5)
            {
                return 4;
            }
            else
            {
                return 3;
            }
            break;

        case 5 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 5;
            }
            else if(ff_ <= 3.5)
            {
                return 5;
            }
            else if(ff_ <= 4.5)
            {
                return 4;
            }
            else
            {
                return 3;
            }
            break;

        case 6 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 4;
            }
            else if(ff_ <= 3.5)
            {
                return 4;
            }
            else if(ff_ <= 4.5)
            {
                return 4;
            }
            else
            {
                return 3;
            }
            break;

        case 7 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 4;
            }
            else if(ff_ <= 3.5)
            {
                return 4;
            }
            else if(ff_ <= 4.5)
            {
                return 4;
            }
            else
            {
                return 3;
            }
            break;

        case 8 :
            if(ff_ <= 1.5)
            {
                return 5;
            }
            else if(ff_ <= 2.5)
            {
                return 4;
            }
            else if(ff_ <= 3.5)
            {
                return 4;
            }
            else if(ff_ <= 4.5)
            {
                return 4;
            }
            else
            {
                return 3;
            }
            break;

        default :
            return 9;
    }
}

short int Situation::km_var_a(short int km)
{
    if(datum_.month() >= 6 && datum_.month() <= 8)
    {
        if(datum_.hour() >= 10 && datum_.hour() <= 16)
        {
            if(km < 6)
            {
                if(cl_ <= 6 || (cl_ == 7 && ff_ < 2.5))
                {
                    km++;
                }
            }
        }
        if(datum_.hour() >= 12 && datum_.hour() <= 15)
        {
            if(km < 6)
            {
                if(cl_ <= 5)
                {
                    km++;
                }
            }
        }
    }
    return km;
}

short int Situation::km_var_b(short int km)
{
    if(datum_.month() == 5 || datum_.month() == 9)
    {
        if(datum_.hour() >= 11 && datum_.hour() <= 15)
        {
            if(km < 6)
            {
                if(cl_ <= 6)
                {
                    km++;
                }
            }
        }
    }
    return km;
}

short int Situation::km_var_c(short int km, int sr, int ss)
{
    if(sa_12(sr))
    {
        int km_n = km_night();
        int km_d = km_day();

        if(datum_.month() >= 3 && datum_.month() <= 11 && ff_ > 1.0)
        {
            km = (km_n == 1 && km_d == 5) ? 2 : km;
        }
        else
        {
            km = (km_n == 1 && km_d == 5) ? 1 : km;
        }
        km = (km_n == 1 && km_d == 4) ? 2 : km;
        km = (km_n == 2 && km_d == 5) ? 2 : km;
        km = (km_n == 2 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 5) ? 3 : km;
        km = (km_n == 3 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 3) ? 3 : km;
    }
    else if(sa_23(sr))
    {
        int km_n = km_night();
        int km_d = km_day();

        km = (km_n == 1 && km_d == 5) ? 2 : km;
        km = (km_n == 1 && km_d == 4) ? 2 : km;
        km = (km_n == 2 && km_d == 5) ? 3 : km;
        km = (km_n == 2 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 5) ? 4 : km;
        km = (km_n == 3 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 3) ? 3 : km;
    }
    else if(su_21(ss))
    {
        int km_n = km_night();
        int km_d = km_day();

        km = (km_n == 1 && km_d == 5) ? 2 : km;
        km = (km_n == 1 && km_d == 4) ? 3 : km;
        km = (km_n == 2 && km_d == 5) ? 3 : km;
        km = (km_n == 2 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 5) ? 4 : km;
        km = (km_n == 3 && km_d == 4) ? 4 : km;
        km = (km_n == 3 && km_d == 3) ? 3 : km;
    }
    else if(su_10(ss))
    {
        int km_n = km_night();
        int km_d = km_day();

        if((datum_.month() == 1 || datum_.month() == 2 || datum_.month() == 12)
           && ff_ <= 1.0 && cl_ <= 6)
        {
            km = (km_n == 1 && km_d == 5) ? 1 : km;
        }
        else
        {
            km = (km_n == 1 && km_d == 5) ? 2 : km;
        }
        km = (km_n == 1 && km_d == 4) ? 3 : km;
        km = (km_n == 2 && km_d == 5) ? 2 : km;
        km = (km_n == 2 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 5) ? 3 : km;
        km = (km_n == 3 && km_d == 4) ? 4 : km;
        km = (km_n == 3 && km_d == 3) ? 3 : km;
    }
    else if(su_01(ss))
    {
        int km_n = km_night();
        int km_d = km_day();

        if(datum_.month() >= 3 && datum_.month() <= 11 && ff_ > 1.0)
        {
            km = (km_n == 1 && km_d == 5) ? 2 : km;
        }
        else
        {
            km = (km_n == 1 && km_d == 5) ? 1 : km;
        }
        if(datum_.month() >= 3 && datum_.month() <= 11 && ff_ > 1.0)
        {
            km = (km_n == 1 && km_d == 4) ? 2 : km;
        }
        else
        {
            km = (km_n == 1 && km_d == 4) ? 1 : km;
        }
        km = (km_n == 2 && km_d == 5) ? 2 : km;
        km = (km_n == 2 && km_d == 4) ? 2 : km;
        km = (km_n == 3 && km_d == 5) ? 3 : km;
        km = (km_n == 3 && km_d == 4) ? 3 : km;
        km = (km_n == 3 && km_d == 3) ? 3 : km;
    }
    return km;
}

short int Situation::km_var_d(short int km)
{
    if(datum_.month() == 1 || datum_.month() == 2 || datum_.month() == 12)
    {
        if(km == 5)
        {
            km--;
        }
    }
    return km;
}


bool Situation::winterzeit()
{
    if((datum_.month() < 3) || (datum_.month() == 3 && datum_.day() <= 28)
       || (datum_.month() > 10) || (datum_.month() == 10 && datum_.day() > 28))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Situation::sa_01(const double sr)
{
    const int sunrise_hour {static_cast<int>(sr)};
    const int sunrise_minute {static_cast<int>((sr-sunrise_hour)*60.0)};

    if(sunrise_minute == 0)
    {
        return (datum_.hour() == sunrise_hour) ? true : false;
    }
    else
    {
        return (datum_.hour() == sunrise_hour+1) ? true : false;
    }
}

bool Situation::sa_12(const double sr)
{
    const int sunrise_hour {static_cast<int>(sr)};
    const int sunrise_minute {static_cast<int>((sr-sunrise_hour)*60.0)};

    if(sunrise_minute == 0)
    {
        return (datum_.hour() == sunrise_hour+1) ? true : false;
    }
    else
    {
        return (datum_.hour() == sunrise_hour+2) ? true : false;
    }
}

bool Situation::sa_23(const double sr)
{
    const int sunrise_hour {static_cast<int>(sr)};
    const int sunrise_minute {static_cast<int>((sr-sunrise_hour)*60.0)};

    if(sunrise_minute == 0)
    {
        return (datum_.hour() == sunrise_hour+2) ? true : false;
    }
    else
    {
        return (datum_.hour() == sunrise_hour+3) ? true : false;
    }
}

bool Situation::su_21(const double ss)
{
    const int sunset_hour {static_cast<int>(ss)};
    const int sunset_minute {static_cast<int>((ss-sunset_hour)*60.0)};

    if(sunset_minute == 0)
    {
        return (datum_.hour() == sunset_hour-2) ? true : false;
    }
    else
    {
        return (datum_.hour() == sunset_hour-1) ? true : false;
    }
}

bool Situation::su_10(const double ss)
{
    const int sunset_hour {static_cast<int>(ss)};
    const int sunset_minute {static_cast<int>((ss-sunset_hour)*60.0)};

    if(sunset_minute == 0)
    {
        return (datum_.hour() == sunset_hour-1) ? true : false;
    }
    else
    {
        return (datum_.hour() == sunset_hour) ? true : false;
    }
}

bool Situation::su_01(const double ss)
{
    const int sunset_hour {static_cast<int>(ss)};
    const int sunset_minute {static_cast<int>((ss-sunset_hour)*60.0)};

    if(sunset_minute == 0)
    {
        return (datum_.hour() == sunset_hour) ? true : false;
    }
    else
    {
        return (datum_.hour() == sunset_hour+1) ? true : false;
    }
}

double Situation::T_f()
{
    int counter {0};
    Date countDate;
    countDate = Date(2000, 1, 1);
    while(datum_.compareToDay(countDate) != 0)
    {
        countDate.nextDay();
        counter++;
    }
    return static_cast<double>(counter) / 36525.0;
}

double Situation::M_f(double T)
{
    return 2.0*PI*(0.993133+99.997361*T);
}

double Situation::L_f(double M, double T)
{
    return 2.0*PI*(0.7859453+M/(2.0*PI)+(6893.0*sin(M)+
           72.0*sin(2.0*M)+6191.2*T)/1296000.0);
}

double Situation::e_f(double T)
{
    return 2.0*PI*(23.43929111+(-46.8150*T-0.00059*T*T+
           0.001813*T*T*T)/3600.0)/360.0;
}

double Situation::DK_f(double e, double L)
{
    return asin(sin(e)*sin(L));
}

double Situation::RA_f(double L, double e)
{
    return atan(tan(L)*cos(e));
}

double Situation::RAm_f(double T)
{
    double RAm {0.0};
    RAm = 18.71506921+2400.0513369*T+(2.5862*pow(10.0, -5)-
           1.72*pow(10, -9)*T)*T*T;
    while(RAm > 24.0)
    {
        RAm -= 24.0;
    }
    while(RAm < 0.0)
    {
        RAm += 24.0;
    }
    return RAm;
}

double Situation::Zeitgl_f(double RA, double RAm)
{

    return 1.0027379*(RAm-RA);
}

double Situation::Zeitdiff_f(double h, double B, double DK)
{
    B /= 57.29578;
    return 12.0*acos((sin(h)-sin(B)*sin(DK))/(cos(B)*cos(DK)))/PI;
}

double Situation::sunrise(Coordinates coord)
{
    int timezone;
    double T {T_f()};
    double M {M_f(T)};
    double L {L_f(M, T)};
    double e {e_f(T)};
    double DK {DK_f(e, L)};
    double RA {RA_f(L, e)};
    double RAm {RAm_f(T)};
    double h {-0.0145};

    timezone = winterzeit() ? 1.0 : 2.0;

    //Korrekturen der Parameter
    if(RA < 0.0)
    {
        RA += PI;
    }
    if(L > PI)
    {
        RA += PI;
    }

    double Zeitgleichung {Zeitgl_f(RA*3.81972, RAm)};
    double Zeitdifferenz {Zeitdiff_f(h, coord.lati_dezi(), DK)};
    double AufgangOrt {12.0-Zeitdifferenz-Zeitgleichung};
    return AufgangOrt - coord.longi_dezi()/15.0 + timezone;

}

double Situation::sunset(Coordinates coord)
{

    int timezone;
    double T {T_f()};
    double M {M_f(T)};
    double L {L_f(M, T)};
    double e {e_f(T)};
    double DK {DK_f(e, L)};
    double RA {RA_f(L, e)};
    double RAm {RAm_f(T)};
    double h {-0.0145};

    timezone = winterzeit() ? 1.0 : 2.0;

    //Korrekturen der Parameter
    if(RA < 0.0)
    {
        RA += PI;
    }
    if(L > PI)
    {
        RA += PI;
    }

    double Zeitgleichung {Zeitgl_f(RA*3.81972, RAm)};
    double Zeitdifferenz {Zeitdiff_f(h, coord.lati_dezi(), DK)};
    double UntergangOrt {12.0+Zeitdifferenz-Zeitgleichung};
    return UntergangOrt - coord.longi_dezi()/15.0 + timezone;

}

Situation::~Situation()
{
    //dtor
}
