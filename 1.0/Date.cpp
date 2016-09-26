#include "Date.h"
#include <iomanip>

using namespace std;

Date::Date()
{
    //ctor
}

Date::Date(int year, int month, int day)
{
    year_ = year;
    month_ = month;
    day_ = day;
    hour_ = 0;
}

Date::Date(int year, int month, int day, int hour)
{
    year_ = year;
    month_ = month;
    day_ = day;
    hour_ = hour;
}

int Date::year()
{
    return year_;
}
int Date::month()
{
    return month_;
}
int Date::day()
{
    return day_;
}
int Date::hour()
{
    return hour_;
}

void Date::printDate()
{
    cout << setw(2) << setfill('0') << day_ << ".";
    cout << setw(2) << setfill('0') << month_ << "." << year_;
    cout << " " << hour_ << ":00";
}

bool Date::isLeapyear()
{
    if(year_ % 4 == 0)
    {
        if(year_ % 100 == 0)
        {
            //Eigenlich kein Schaltjahr
            if(year_ % 400 == 0)
            {
                //Außer hier
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            //Schaltjahr
            return true;
        }
    }
    else
    {
        //kein Schaltjahr
        return false;
    }
}

bool Date::endOfDay()
{
    return (hour_ == 23) ? true : false;
}

bool Date::endOfMonth()
{
    if(month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11)
    {
        //Monat hat 30 Tage
        if(day_ == 30)
        {
            return true;
        }
    }
    else if(month_ == 2)
    {
        //Februar
        if(isLeapyear())
        {
            if(day_ == 29)
            {
                return true;
            }
        }
        else
        {
            if(day_ == 28)
            {
                return true;
            }
        }
    }
    else
    {
        //Monat hat 31 Tage
        if(day_ == 31)
        {
            return true;
        }
    }
    return false;
}

bool Date::endOfYear()
{
    return (month_ == 12) ? true : false;
}


int Date::compareToHour(Date cmpDate)
{
    if(year_ == cmpDate.year())
    {
        if(month_ == cmpDate.month())
        {
            if(day_ == cmpDate.day())
            {
                if(hour_ == cmpDate.hour())
                {
                    return 0;
                }
                else if(hour_ < cmpDate.hour())
                {
                    return -1;
                }
                else
                {
                    return 1;
                }
            }
            else if(day_ < cmpDate.day())
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
        else if(month_ < cmpDate.month())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else if(year_ < cmpDate.year())
    {
        return -1;
    }
    else
    {
        return 1;
    }
}


int Date::compareToDay(Date cmpDate)
{
    if(year_ == cmpDate.year())
    {
        if(month_ == cmpDate.month())
        {
            if(day_ == cmpDate.day())
            {
                return 0;
            }
            else if(day_ < cmpDate.day())
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
        else if(month_ < cmpDate.month())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else if(year_ < cmpDate.year())
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void Date::nextDay()
{
    hour_ = 0;
    if(endOfMonth())
    {
        day_ = 1;
        if(endOfYear())
        {
            year_++;
            month_ = 1;
        }
        else
        {
            month_++;
        }
    }
    else
    {
        day_++;
    }
}

void Date::nextHour()
{
    if(endOfDay())
    {
        nextDay();
    }
    else
    {
        hour_++;
    }
}

Date::~Date()
{
    //dtor
}
