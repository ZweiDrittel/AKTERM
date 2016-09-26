#ifndef CREATE_HEADER_H
#define CREATE_HEADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Date.h"
#include "Coordinates.h"
#include "Situation.h"

using namespace std;

//Funktion, um .AKTERM-Header zu erstellen
void create_header(ofstream &file_out, Date &date_start, Date &date_end,
                   string station_id, string station_name, Coordinates &place,
                   double h_stat, double h_as, double z0s, double h_ref);

//Funktion, um Input-Datei zu verarbeiten
void read_from_input(ifstream &file_in, string &input_file, string &station_name,
                     string &station_id, Date &date_start, Date &date_end,
                     string &fn_in_ff, string &fn_in_cl, string &fn_out,
                     Coordinates &place, double &h_stat, double &h_as,
                     double &z0s, double &h_ref);

//Funktion, die guckt, ob char ';' ist
bool is_semi(char c);

//Funktion, um Zeile aus Wind-input-Datei zu lesen
void read_line_ff(ifstream &file_in, Date &dummy_date, int &dd, double &ff);

//Funktion, um Zeile aus Wolken-input-Datei zu lesen
void read_line_cl(ifstream &file_in_cl, Date &dummy_date, int &cl);

//Funktion, um Zeile in .akterm auszugeben
void create_akt_line(ofstream &file_out, string station_id, Situation &sit);

//Funktion, die wartet bis man Enter drückt
void wait();

#endif // CREATE_HEADER_H
