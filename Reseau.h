/*
Fichier : Reseau.h

Auteur : William Gauvin
		 David Barrière
		 Charles Masson

Description: Contient la déclaration de la classe réseau

*/

#pragma once

//Include relatif au réseau
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <deque>
#include <string>
#include "FileParser.h"
#include "Neurone.h"
#include "Utilitaire.h"


/****************** CLASSE DU RÉSEAU LVQ ********************************/
/*
Classe du réseau LVQ, elle contient toutes les classes différentes
pour identifier à quelle classe apartient une entré. 

*/
class Reseau_LVQ {
public:
	//Constructeur pour le réseau LVQ
	Reseau_LVQ(vector<double> &MLP_Constructer,deque<vector<vector<double>>> &Poids_Init);
	Reseau_LVQ();

	//Réécris le réseau avec un autre 
	void Overwrite_Reseau_LVQ(vector<double> &, vector<double> &);

	//Remet le pourcentage de réussite à 0
	void Reset_Pourcentage(){Pourcentage_Reussite = 0.0;}

	//Retourne le pourcentage de réussite 
	double Get_Pourcentage(){return Pourcentage_Reussite;}

	//Fonction principale de calcule pour la distance
	void Trouver_Distance(vector<double> &, int );

	//Fonction principale pour mettre à jour les poids
	void Update_Poid(vector<double> &);

	//Dégrade le taux d'apprentissage
	void Alpha_Decay();

	//Compare si la classes obtenue est la même que la classe désirer
	bool Compare_Valeur(){return Classe_Obtenue==Classe_Desirer;}

	//Retourne le vecteur contenant les représentants de toutes les classes
	vector<Neurone> Get_All_Class() { return All_Class; }

	//Retourne Enable_Alpha_Decay_Only_Correct
	bool Get_Enable_Alpha_Decay_Only_Correct() { return Enable_Alpha_Decay_Only_Correct; }
private:
	//Vecteur contenant toutes les classes
	//Ici un Neurone est une classe pour identifier une entré
	vector<Neurone> All_Class;

	//Classe obtenue après les calcules de distance
	int Classe_Obtenue;

	//Classe que l'entrée appartient 
	int Classe_Desirer;

	//Représentant de la classe obtenue
	int Numero_Representant;

	//Pourcentage de réussite du réseau LVQ
	double Pourcentage_Reussite;

	//Dégrade le taux d'apprentissage de la valeur 
	double Alpha_Decay_Rate;

	//Active la dégradation du alpha seulement s'il obtient la bonne réponse
	//Déactive la dégradation en fin d'époque
	bool Enable_Alpha_Decay_Only_Correct;
};


double Train(Reseau_LVQ &, vector<double> &config, deque<vector<vector<double>>> &dequeTrain, deque<vector<vector<double>>> &dequeVC, deque<vector<vector<double>>> &dequeTest);
double VC(Reseau_LVQ &, vector<double> &config, deque<vector<vector<double>>> &dequeVC);
double Test(Reseau_LVQ &, vector<double> &config, deque<vector<vector<double>>> &dequeTest);
