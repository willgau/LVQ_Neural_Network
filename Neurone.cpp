/*
Fichier: Neurone.cpp

Auteur: William Gauvin
		David Barri�re
		Charles Masson

Description : Ce fichier contient les d�clarations de fonctions
pour la classe Neurone.

D�finition : Le neurone ici est un terme plus g�n�ral, le neurone
dans un r�seau LVQ est une "classe" qui contient un ou plusieurs
repr�sentant pour calculer les distances pour trouver la classe
que l'entr� donn�e est associ�.

�tape de l'algorithme:
	*�tape 4: Actualiser les poids
*/

#pragma once
//Include utile
#include "Neurone.h"
#include "Reseau.h"

/*
�tape 4 du r�seau LVQ: Actualiser les poids

Entr�e: Entr�e (Input)
		Num�ro du repr�sentant gagnant

Description: Met � jour les poids si la classe du repr�sentant
est la m�me que celle de l'entr�e. Il y a donc le "+" dans
l'�quation de mise � jour.
*/

void Neurone::Update_Poids_Pareil(vector<double> &Input, int Numero_Representant){
	
	//Variable tampon
	double Old_Poids = 0.0;

	//Met � jour le repr�sentant
	for (auto i = 0; i < Representant[Numero_Representant].size(); i++) {
		Old_Poids = Representant[Numero_Representant][i];
		Representant[Numero_Representant][i] = Old_Poids + alpha*(Input[i] - Old_Poids);
	}
}
/*
�tape 4 du r�seau LVQ: Actualiser les poids

Entr�e: Entr�e (Input)
		Num�ro du repr�sentant gagnant

Description: Met � jour les poids si la classe du repr�sentant
n'est pas la m�me que celle de l'entr�e. Il y a donc le "-" dans
l'�quation de mise � jour.
*/
void Neurone::Update_Poids_Different(vector<double> &Input, int Numero_Representant){
	
	//Variable tampon
	double Old_Poids = 0.0;

	//Met � jour le repr�sentant
	for (auto i = 0; i <Representant[Numero_Representant].size(); i++) {
		Old_Poids = Representant[Numero_Representant][i];
		Representant[Numero_Representant][i] = Old_Poids - alpha*(Input[i] - Old_Poids);
	}
}
/*
Constructeur de la classe neurone

Entr�e/Sortie: Poids initiaux (Repr�sentant)
			   Configuration

Description: Construit le neurone ou la "classe" d'appartenance 
avec le ou les repr�sentants, le total de repr�sentant puis le taux
d'apprentissage pour ce neurone

*/
Neurone::Neurone(vector<vector<double>> &Poids_Init, vector<double> &config) {

	//Repr�sentant du neurone ou de la "classe"
	Representant = Poids_Init;

	//Nombre total de repr�sentant
	Total_of_Representant = Poids_Init.size();

	//Taux d'apprentissage multiple actif
	if (config[10]) {
		int range = (int)((config[5] * config[11]) * 1000);
		alpha = (double)((rand() % range))/1000;
	}
	else {
		//Taux d'apprentissage identique
		alpha = config[5];
	}
}