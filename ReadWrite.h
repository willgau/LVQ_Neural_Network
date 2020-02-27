/*
Fichier: ReadWrite.h

Auteur: William Gauvin
		David Barri�re
		Charles Masson

Description : Fonction de lecture et d'�criture
			  pour le r�seau LVQ
*/
#pragma once
//Include utile pour le fichier
#include "Reseau.h"


//�criture d'un r�seau LVQ en fichier texte
void Write_File(Reseau_LVQ &, vector<double> &, string);

//Fonction de lecture de fichier contenant les poids d'un r�seau LVQ
void Read_File(Reseau_LVQ &, vector<double> &);