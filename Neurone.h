/*
Fichier: Neurone.h

Auteur: William Gauvin
David Barrière
Charles Masson

Description : Ce fichier contient les déclarations de fonctions
pour la classe Neurone.

Définition : Le neurone ici est un terme plus général, le neurone
dans un réseau LVQ est une "classe" qui contient un ou plusieurs
représentant pour calculer les distances pour trouver la classe
que l'entré donnée est associé.
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
Classe du neurone ou de la "classe" qui contient les représentants de cette
classe d'appartenance pour faire l'apprentissage sur les entrées.
*/
class Neurone {
public:
	//Constructeur de la classe Neurone
	Neurone(vector<vector<double>> &Poids_Init, vector<double> &config);

	//Met à jour les poids selon si la classe d'appartenance est la même que celle calculé
	void Update_Poids_Pareil(vector<double> &Input, int Numero_Representant);
	void Update_Poids_Different(vector<double> &Input, int Numero_Representant);

	//Fonction pour avoir ou mettre la valeur du taux d'apprentissage
	void Set_Alpha(double val) { alpha = val; }
	double Get_Alpha() { return alpha; }

	//Fonction pour avoir un ou tous les représentants du neurones
	vector<double> Get_X_Representant(int val) { return Representant[val]; }
	vector<vector<double>> Get_All_Representant() { return Representant; }

	//Retourne le total de représentant pour le neurone ou la "classe"
	int Get_Total_of_Representant() { return Total_of_Representant; }

	//Met le nombre total de représentant à la dimension du vecteur de Representant
	void Set_Total_of_Representant() { Total_of_Representant = Representant.size(); }

	//Remplace les représentants par les nouveaux
	void Set_All_Representant(vector<vector<double>> &val) { Representant = val; }

private:
	//Représentant pour le neurone ou la "classe"
	vector<vector<double>> Representant;

	//Nombre total de représentant
	int Total_of_Representant;

	//Taux d'apprentissage
	double alpha;
};
