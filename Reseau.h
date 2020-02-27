/*
Fichier : Reseau.h

Auteur : William Gauvin
		 David Barri�re
		 Charles Masson

Description: Contient la d�claration de la classe r�seau

*/

#pragma once

//Include relatif au r�seau
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


/****************** CLASSE DU R�SEAU LVQ ********************************/
/*
Classe du r�seau LVQ, elle contient toutes les classes diff�rentes
pour identifier � quelle classe apartient une entr�. 

*/
class Reseau_LVQ {
public:
	//Constructeur pour le r�seau LVQ
	Reseau_LVQ(vector<double> &MLP_Constructer,deque<vector<vector<double>>> &Poids_Init);
	Reseau_LVQ();

	//R��cris le r�seau avec un autre 
	void Overwrite_Reseau_LVQ(vector<double> &, vector<double> &);

	//Remet le pourcentage de r�ussite � 0
	void Reset_Pourcentage(){Pourcentage_Reussite = 0.0;}

	//Retourne le pourcentage de r�ussite 
	double Get_Pourcentage(){return Pourcentage_Reussite;}

	//Fonction principale de calcule pour la distance
	void Trouver_Distance(vector<double> &, int );

	//Fonction principale pour mettre � jour les poids
	void Update_Poid(vector<double> &);

	//D�grade le taux d'apprentissage
	void Alpha_Decay();

	//Compare si la classes obtenue est la m�me que la classe d�sirer
	bool Compare_Valeur(){return Classe_Obtenue==Classe_Desirer;}

	//Retourne le vecteur contenant les repr�sentants de toutes les classes
	vector<Neurone> Get_All_Class() { return All_Class; }

	//Retourne Enable_Alpha_Decay_Only_Correct
	bool Get_Enable_Alpha_Decay_Only_Correct() { return Enable_Alpha_Decay_Only_Correct; }
private:
	//Vecteur contenant toutes les classes
	//Ici un Neurone est une classe pour identifier une entr�
	vector<Neurone> All_Class;

	//Classe obtenue apr�s les calcules de distance
	int Classe_Obtenue;

	//Classe que l'entr�e appartient 
	int Classe_Desirer;

	//Repr�sentant de la classe obtenue
	int Numero_Representant;

	//Pourcentage de r�ussite du r�seau LVQ
	double Pourcentage_Reussite;

	//D�grade le taux d'apprentissage de la valeur 
	double Alpha_Decay_Rate;

	//Active la d�gradation du alpha seulement s'il obtient la bonne r�ponse
	//D�active la d�gradation en fin d'�poque
	bool Enable_Alpha_Decay_Only_Correct;
};


double Train(Reseau_LVQ &, vector<double> &config, deque<vector<vector<double>>> &dequeTrain, deque<vector<vector<double>>> &dequeVC, deque<vector<vector<double>>> &dequeTest);
double VC(Reseau_LVQ &, vector<double> &config, deque<vector<vector<double>>> &dequeVC);
double Test(Reseau_LVQ &, vector<double> &config, deque<vector<vector<double>>> &dequeTest);
