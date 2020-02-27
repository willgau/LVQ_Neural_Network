/*
Fichier: Neurone.cpp

Auteur: William Gauvin
		David Barrière
		Charles Masson

Description : Ce fichier contient les déclarations de fonctions
pour la classe Neurone.

Définition : Le neurone ici est un terme plus général, le neurone
dans un réseau LVQ est une "classe" qui contient un ou plusieurs
représentant pour calculer les distances pour trouver la classe
que l'entré donnée est associé.

Étape de l'algorithme:
	*Étape 4: Actualiser les poids
*/

#pragma once
//Include utile
#include "Neurone.h"
#include "Reseau.h"

/*
Étape 4 du réseau LVQ: Actualiser les poids

Entrée: Entrée (Input)
		Numéro du représentant gagnant

Description: Met à jour les poids si la classe du représentant
est la même que celle de l'entrée. Il y a donc le "+" dans
l'équation de mise à jour.
*/

void Neurone::Update_Poids_Pareil(vector<double> &Input, int Numero_Representant){
	
	//Variable tampon
	double Old_Poids = 0.0;

	//Met à jour le représentant
	for (auto i = 0; i < Representant[Numero_Representant].size(); i++) {
		Old_Poids = Representant[Numero_Representant][i];
		Representant[Numero_Representant][i] = Old_Poids + alpha*(Input[i] - Old_Poids);
	}
}
/*
Étape 4 du réseau LVQ: Actualiser les poids

Entrée: Entrée (Input)
		Numéro du représentant gagnant

Description: Met à jour les poids si la classe du représentant
n'est pas la même que celle de l'entrée. Il y a donc le "-" dans
l'équation de mise à jour.
*/
void Neurone::Update_Poids_Different(vector<double> &Input, int Numero_Representant){
	
	//Variable tampon
	double Old_Poids = 0.0;

	//Met à jour le représentant
	for (auto i = 0; i <Representant[Numero_Representant].size(); i++) {
		Old_Poids = Representant[Numero_Representant][i];
		Representant[Numero_Representant][i] = Old_Poids - alpha*(Input[i] - Old_Poids);
	}
}
/*
Constructeur de la classe neurone

Entrée/Sortie: Poids initiaux (Représentant)
			   Configuration

Description: Construit le neurone ou la "classe" d'appartenance 
avec le ou les représentants, le total de représentant puis le taux
d'apprentissage pour ce neurone

*/
Neurone::Neurone(vector<vector<double>> &Poids_Init, vector<double> &config) {

	//Représentant du neurone ou de la "classe"
	Representant = Poids_Init;

	//Nombre total de représentant
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