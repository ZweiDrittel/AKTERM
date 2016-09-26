#include "Date.h"
#include "Coordinates.h"
#include "Situation.h"
#include "file_creation.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;



//wichtig!!!!:
//Fehlerwerte in Messdaten jeweils mit 999 kennzeichen
//Fehlerwert für km ist 9
int main()
{
    string station_name;
    string station_id;
    Date date_start;
    Date date_end;
    string fn_in_ff;
    string fn_in_cl;
    string fn_out;
    Coordinates place;
    double h_stat;
    double h_as;
    double z0s;
    double h_ref;

    //Input-info-Datei mit Eckdaten einlesen
    ifstream file_in;

    {
        string input_file{"S:\\Datenban\\Wetter_neu\\Programme\\akterm\\input_akterm.txt"};

        //Input-Datei verarbeiten
        read_from_input(file_in, input_file, station_name, station_id,
                        date_start, date_end, fn_in_ff, fn_in_cl, fn_out,
                        place, h_stat, h_as, z0s, h_ref);
    }

    //Input-info-Datei eingelesen

    //input-Datei einlesen (nur richtige Stellen)
    ifstream file_in_ff;
    ifstream file_in_cl;
    ofstream file_out;
    Date date_ff;
    Date date_cl;
    file_in.open(fn_in_ff, ios::in);
    file_in_cl.open(fn_in_cl, ios::in);
    file_out.open(fn_out, ios::out);

    ////////////////////////////////////////////////////////////////////
    ////////////////////////Header erstellen////////////////////////////
    ////////////////////////////////////////////////////////////////////
    create_header(file_out, date_start, date_end, station_id, station_name,
                  place, h_stat, h_as, z0s, h_ref);
    ////////////////////////////////////////////////////////////////////
    ////////////////////////Header erstellt/////////////////////////////
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //////////Datei einlesen, kms berechnen und alles ausgeben//////////
    ////////////////////////////////////////////////////////////////////
    const int nr_of_sits {8760};
    Situation sits[nr_of_sits];
    string line_ff;
    string line_cl;
    getline(file_in, line_ff);
    getline(file_in_cl, line_cl);
    Date date_expected = date_start;
    int missing_data_ff{0};
    int missing_data_cl{0};
    int false_data_ff{0};
    int false_data_cl{0};

    int dd;
    double ff;
    int cl;
    Date date_read_ff;
    Date date_read_cl;

    read_line_ff(file_in, date_read_ff, dd, ff);
    read_line_cl(file_in_cl, date_read_cl, cl);

    for(int i = 0; i < nr_of_sits; i++)
    {
        while(date_read_ff.compareToHour(date_expected) < 0)
        {
            read_line_ff(file_in, date_read_ff, dd, ff);
        }
        while(date_read_cl.compareToHour(date_expected) < 0)
        {
            read_line_cl(file_in_cl, date_read_cl, cl);
        }
        if(date_read_ff.compareToHour(date_expected) > 0)
        {
            //Datenlücke in ff-Datei!!!
            missing_data_ff++;
            if(date_read_cl.compareToHour(date_expected) > 0)
            {
                //zusätzlich Datenlücke in cl-Datei!!!
                missing_data_cl++;
                sits[i] = Situation(date_expected, 999, 999, 999, 9);
            }
            else
            {
                //nur in ff-Datei Datenlücke
                if(cl == 999)
                {
                    false_data_cl++;
                }
                sits[i] = Situation(date_expected, 999, 999, cl, 9);
            }
        }
        else
        {
            //Daten ff vorhanden
            if(date_read_cl.compareToHour(date_expected) > 0)
            {
                //Datenlücke in cl-Datei!!!
                missing_data_cl++;
                if(ff == 999 || dd == 999)
                {
                    false_data_ff++;
                }
                sits[i] = Situation(date_expected, ff, dd, 999);
                sits[i].km(place);
            }
            else
            {
                //alle Daten am Start!
                if(cl == 999)
                {
                    false_data_cl++;
                }
                if(ff == 999 || dd == 999)
                {
                    false_data_ff++;
                }
                sits[i] = Situation(date_expected, ff, dd, cl);
                sits[i].km(place);
            }
        }

        create_akt_line(file_out, station_id, sits[i]);
        date_expected.nextHour();

    }

    cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "++++++++++++Information ueber Datenfehler++++++++++++++\n";
    cout << "Fehldaten Wind: " << missing_data_ff << "\n";
    cout << "Fehldaten Wolken: " << missing_data_cl << "\n";
    cout << "Fehlerhafte Messwerte Wind: " << false_data_ff << "\n";
    cout << "Fehlerhafte Messwerte Wolken: " << false_data_cl << "\n";
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    file_in.close();
    file_in_cl.close();
    file_out.close();

	wait();
    return 0;
}
