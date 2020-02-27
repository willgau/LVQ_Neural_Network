/*
Fichier : FileParser.h

Auteur: William Gauvin

Description : Lis les bases de données
*/

#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <deque>


using namespace std;

//Fonction public pour lire les fichiers
vector<double> LoadConfig();
deque<vector<vector<double>>> LoadTrain(vector<double>);
deque<vector<vector<double>>> LoadTest(vector<double>);
deque<vector<vector<double>>> LoadVC(vector<double>);
