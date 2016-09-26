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
        string input_file{"S:\\Datenban\\Wetter\\Programme\\akterm\\input_akterm.txt"};

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
	int nr_of_sits{8760};
    if(leapPeriod(date_start, date_end))
    {
		nr_of_sits = 8784;
	}
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
    
    ////////////cl einlesen/////////////////
    read_line_cl(file_in_cl, date_read_cl, cl);
    for(int i = 0; i < nr_of_sits; i++)
    {
		while(date_read_cl.compareToHour(date_expected) < 0)
        {
            read_line_cl(file_in_cl, date_read_cl, cl);
        }
		if(date_read_cl.compareToHour(date_expected) > 0)
		{
			//Datenlücke in cl-Datei
			missing_data_cl++;
			sits[i].save_cl(999);
		}
		else
		{
			//Daten vorhanden
			if(cl < 0 || cl > 8)
			{
				false_data_cl++;
				sits[i].save_cl(999);
			}
			else
			{
				sits[i].save_cl(cl);
			}
		}
		date_expected.nextHour();
	}
	//Datenlücken (999er) interpolieren
	{
		int gap_size{0};
		int i_start{0};
		int i_end{0};
		int cl_start{0};
		int cl_end{0};
		
		if(sits[0].ret_cl() == 999)
		{
			cout << "Erster Wert in Bewoelkung ist Fehler....\n";
			for(int i = 1; i < nr_of_sits; i++)
			{
				if(sits[i].ret_cl() != 999)
				{
					for(int j = 0; j < i; j++)
					{
						sits[j] = Situation(sits[i].ret_date(), sits[i].ret_ff(),
						        sits[i].ret_dd(), sits[i].ret_cl(), sits[i].ret_km());
					}
					break;
				}
			}
		}
		
		for(int i = 1; i < nr_of_sits; i++)
		{
			if(sits[i].ret_cl() == 999)
			{
				i_start = i-1;
				cl_start = sits[i-1].ret_cl();
				while(sits[i].ret_cl() == 999)
				{
					i++;
				}
				i_end = i;
				cl_end = sits[i].ret_cl();
				
				//Werte in Lücke interpoliert auffüllen
				int cl_dif{cl_end-cl_start};
				gap_size = i_end-i_start;
				for(int j = i_start+1; j < i_end; j++)
				{
					int cl_interpol{static_cast<int>(cl_start + (cl_dif*((j-i_start)/gap_size)))};
					sits[j].save_cl(cl_interpol);
				}
			}
		}
	}
    
    ///////////ff einlesen/////////////////
    date_expected = date_start;
    read_line_ff(file_in, date_read_ff, dd, ff);
    for(int i = 0; i < nr_of_sits; i++)
    {
        while(date_read_ff.compareToHour(date_expected) < 0)
        {
            read_line_ff(file_in, date_read_ff, dd, ff);
        }
        if(date_read_ff.compareToHour(date_expected) > 0)
        {
            //Datenlücke in ff-Datei!!!
            missing_data_ff++;
            sits[i].save_others(date_expected, 999, 999);
        }
        else
        {
            //Daten ff vorhanden
			if(ff == 999 || dd == 999)
			{
				false_data_ff++;
			}
			sits[i].save_others(date_expected, ff, dd);
        }
        
		sits[i].km(place);

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
