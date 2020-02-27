/*
Fichier: Neurone.h

Auteur: William Gauvin
David Barri�re
Charles Masson

Description : Ce fichier contient les d�clarations de fonctions
pour la classe Neurone.

D�finition : Le neurone ici est un terme plus g�n�ral, le neurone
dans un r�seau LVQ est une "classe" qui contient un ou plusieurs
repr�sentant pour calculer les distances pour trouver la classe
que l'entr� donn�e est associ�.
*/
#pragma once
//Include utile
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <deque>
#include <string>
#include "FileParser.h"

using namespace std;

/*********************************** CLASSE DU NEURONE **************************/
/*
Classe du neurone ou de la "classe" qui contient les repr�sentants de cette
classe d'appartenance pour faire l'apprentissage sur les entr�es.
*/
class Neurone {
public:
	//Constructeur de la classe Neurone
	Neurone(vector<vector<double>> &Poids_Init, vector<double> &config);

	//Met � jour les poids selon si la classe d'appartenance est la m�me que celle calcul�
	void Update_Poids_Pareil(vector<double> &Input, int Numero_Representant);
	void Update_Poids_Different(vector<double> &Input, int Numero_Representant);

	//Fonction pour avoir ou mettre la valeur du taux d'apprentissage
	void Set_Alpha(double val) { alpha = val; }
	double Get_Alpha() { return alpha; }

	//Fonction pour avoir un ou tous les repr�sentants du neurones
	vector<double> Get_X_Representant(int val) { return Representant[val]; }
	vector<vector<double>> Get_All_Representant() { return Representant; }

	//Retourne le total de repr�sentant pour le neurone ou la "classe"
	int Get_Total_of_Representant() { return Total_of_Representant; }

	//Met le nombre total de repr�sentant � la dimension du vecteur de Representant
	void Set_Total_of_Representant() { Total_of_Representant = Representant.size(); }

	//Remplace les repr�sentants par les nouveaux
	void Set_All_Representant(vector<vector<double>> &val) { Representant = val; }

private:
	//Repr�sentant pour le neurone ou la "classe"
	vector<vector<double>> Representant;

	//Nombre total de repr�sentant
	int Total_of_Representant;

	//Taux d'apprentissage
	double alpha;
};
