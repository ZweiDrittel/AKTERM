#include "file_creation.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include "Date.h"
#include "Coordinates.h"
#include "Situation.h"

using namespace std;

void create_header(ofstream &file_out, Date &date_start, Date &date_end,
                   string station_id, string station_name, Coordinates &place,
                   double h_stat, double h_as, double z0s, double h_ref)
{
    file_out << "* AKTERM_Zeitreihe, Barth&Bitter GmbH, Hannover\n";
    file_out << "* Zeitraum " << setw(2) << setfill('0') << date_start.month();
    file_out << "/" << setw(4) << date_start.year() << " bis ";
    file_out << setw(2) << date_end.month() << "/" << setw(4) << date_end.year() << "\n";
    file_out << "* Station " << station_id << " " << station_name << ", ";
    file_out << place.lati_dezi() << "°N " << place.longi_dezi() << "°E, ";
    file_out << h_stat << " m ü NN\n";
    file_out << "* Standortparameter ha = " << h_as << " m, z0s = " << z0s << " m\n";
    {
        double ha[9];
        double z0[9]{0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 1.5, 2.0};
        double d0[9];
        double d0s {6*z0s};
        double ps;
        ps = (log((h_as-d0s)/z0s))/(log((h_ref-d0s)/z0s));

        for(int i = 0; i < 9; i++)
        {
            d0[i] = 6*z0[i];
        }

        file_out << "+ Anemometerhoehen (0.1 m):";

        for(int i = 0; i < 9; i++)
        {
            ha[i] = d0[i] + z0[i] * pow((h_ref-d0[i])/z0[i], ps);
            ha[i] = (ha[i] < 4.0) ? 4.0 : ha[i];
            file_out << setw(5) << setfill(' ') << static_cast<int>(round(ha[i]*10.0));
        }
        file_out << "\n";
    }
}

void read_from_input(ifstream &file_in, string &input_file, string &station_name,
                     string &station_id, Date &date_start, Date &date_end,
                     string &fn_in_ff, string &fn_in_cl, string &fn_out,
                     Coordinates &place, double &h_stat, double &h_as,
                     double &z0s, double &h_ref)
{
    file_in.open(input_file, ios::in);

    //Prüfung, ob Datei korrekt geöffnet werden konnte
    if(!file_in)
    {
        cerr << "Eingabedatei \"input_akterm\" kann nicht geöffnet werden...\n";
        return;
    }
    else
    {
        string dummy_str;
        file_in >> dummy_str >> station_name >> station_id;
        file_in >> dummy_str >> dummy_str >> dummy_str >> dummy_str;
        {
            int dummy_year;
            int dummy_month;
            int dummy_day;
            file_in >> dummy_day >> dummy_month >> dummy_year;
            date_start = Date(dummy_year, dummy_month, dummy_day, 00);
            date_end = Date(dummy_year, dummy_month, dummy_day, 23);
            //Enddatum richtig setzen
            for(int i = 0; i<364; i++)
            {
                date_end.nextDay();
            }
            date_end = Date(date_end.year(),date_end.month(),date_end.day(),23);
        }
        file_in >> dummy_str >> dummy_str >> dummy_str >> fn_in_ff;
        file_in >> dummy_str >> dummy_str >> dummy_str >> fn_in_cl;
        file_in >> dummy_str >> dummy_str >> dummy_str >> fn_out;
        {
            int x1, x2, x3;
            int y1, y2, y3;
            file_in >> dummy_str >> dummy_str >> y1 >> y2 >> y3;
            file_in >> dummy_str >> x1 >> x2 >> x3;
            place = Coordinates(y1, y2, y3, x1, x2, x3);
        }
        file_in >> dummy_str >> dummy_str >> dummy_str >> h_stat;
        file_in >> dummy_str >> dummy_str >> dummy_str >> h_as;
        file_in >> dummy_str >> dummy_str >> dummy_str >> dummy_str >> dummy_str;
        file_in >> z0s;
        file_in >> dummy_str >> dummy_str >> h_ref;
    }
    file_in.close();


}


bool is_semi(char c)
{
    return c == ';' ? true : false;
}

bool leapPeriod(Date &date_start, Date &date_end)
{
	//Zeitraum durchlaufen und guckenm ob 29.02. getroffen wird
	Date date_dummy{date_start};
	while(date_dummy.compareToDay(date_end) < 1)
	{
		//gucken ob 29.02.
		if(date_dummy.month() == 2 && date_dummy.day() == 29)
		{
			return true;
		}
		date_dummy.nextDay();
	}
	return false;
}

void read_line_ff(ifstream &file_in, Date &dummy_date, int &dd, double &ff)
{
    int year;
    int month;
    int day;
    int hour;
    string date_str{};
    string dd_str{};
    string ff_str{};
    string line_ff;
    int semi_counter {0};

    getline(file_in, line_ff);

    for(auto c : line_ff)
    {
        if(is_semi(c))
        {
            semi_counter++;
        }

        if((semi_counter == 1) && (!is_semi(c)))
        {
            date_str += c;
        }
        if((semi_counter == 4) && (!is_semi(c)))
        {
            ff_str += c;
        }
        if((semi_counter == 5) && (!is_semi(c)))
        {
            dd_str += c;
        }
    }

    dd = stoi(dd_str);
    ff = stod(ff_str);
    year = stoi(date_str.substr(0,4));
    month = stoi(date_str.substr(4,2));
    day = stoi(date_str.substr(6,2));
    hour = stoi(date_str.substr(8,2));
    dummy_date = Date(year, month, day, hour);
    
    if(dd == -999 || dd == -1)
    {
		dd = 999;
	}
	if(ff == -999 || ff == -1)
	{
		ff = 999;
	}
    
}

void read_line_cl(ifstream &file_in_cl, Date &dummy_date, int &cl)
{
    int year;
    int month;
    int day;
    int hour;
    string date_str{};
    string cl_str{};
    string line_cl;
    int semi_counter {0};

    getline(file_in_cl, line_cl);

    for(auto c : line_cl)
    {
        if(is_semi(c))
        {
            semi_counter++;
        }

        if((semi_counter == 1) && (!is_semi(c)))
        {
            date_str += c;
        }
        if((semi_counter == 3) && (!is_semi(c)))
        {
            cl_str += c;
        }
    }

    cl = stoi(cl_str);
    year = stoi(date_str.substr(0,4));
    month = stoi(date_str.substr(4,2));
    day = stoi(date_str.substr(6,2));
    hour = stoi(date_str.substr(8,2));
    dummy_date = Date(year, month, day, hour);
}

void create_akt_line(ofstream &file_out, string station_id, Situation &sit)
{
    file_out << "AK " << setw(5) << setfill('0') << station_id << " ";
    file_out << setw(4) << setfill('0') << sit.ret_year() << " ";
    file_out << setw(2) << setfill('0') << sit.ret_month() << " ";
    file_out << setw(2) << setfill('0') << sit.ret_day() << " ";
    file_out << setw(2) << setfill('0') << sit.ret_hour() << " ";
    file_out << "00 1 1 ";
    file_out << setw(3) << setfill(' ') << sit.ret_dd() << " ";
    if(10.0*sit.ret_ff() > 999)
    {
        file_out << setw(3) << setfill(' ') << "999" << " ";
    }
    else
    {
        file_out << setw(3) << setfill(' ') << (10.0*sit.ret_ff()) << " ";
    }
    file_out << "1 " << sit.ret_km() << " 1 -999 9\n";
}


void wait() 
{ 
    // Löscht etwaige Fehlerzustände, die das Einlesen verhindern könnten 
    cin.clear(); 
    // Ignoriert soviele Zeichen im Puffer wie im Puffer vorhanden sind 
    // (= ignoriert alle Zeichen die derzeit im Puffer sind) 
    cin.ignore(cin.rdbuf()->in_avail()); 
    // Füge alle eingelesenen Zeichen in den Puffer bis ein Enter gedrückt wird 
    // cin.get() liefert dann das erste Zeichen aus dem Puffer zurück, welches wir aber ignorieren (interessiert uns ja nicht) 
    cin.get(); 
} 
