/*
Fichier: ReadWrite.h

Auteur: William Gauvin
		David Barrière
		Charles Masson

Description : Fonction de lecture et d'écriture
			  pour le réseau LVQ
*/
#pragma once
//Include utile pour le fichier
#include "Reseau.h"


//Écriture d'un réseau LVQ en fichier texte
void Write_File(Reseau_LVQ &, vector<double> &, string);

//Fonction de lecture de fichier contenant les poids d'un réseau LVQ
void Read_File(Reseau_LVQ &, vector<double> &);