/*
Fichier: ReadWrite.cpp

Auteur: William Gauvin
David Barrière
Charles Masson

Description : Fonction de lecture et d'écriture
			  pour le réseau LVQ
*/

//Include utile pour le fichier
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <deque>
#include <direct.h>
#include "ReadWrite.h"
/*
Écris les données traitées dans un nouveau fichier

Entrée/Sortie : Réseau LVQ
				Configuration
				Epoque

Description :
Écris le réseau LVQ dans un fichier texte
*/
void Write_File(Reseau_LVQ &reseau_LVQ, vector<double> &config, string epoque) {
	
	//Nom du dossier et du fichier
	string Folder_Name = "../Debug/ReseauSave";
	string File_Name = Folder_Name + "/Reseau" + epoque + ".txt";

	//Créer le dossier
	_mkdir(Folder_Name.c_str());

	//Créer le fichier et l'ouvre en mode écriture
	ofstream myfile(File_Name);

	//Écrit la configuration choisis 
	for (auto k = 0; k < config.size(); k++) {
		myfile << config[k] << ' ';
	}

	//Extrait tous les neurones
	vector<Neurone> neurone = reseau_LVQ.Get_All_Class();

	//Boucle à travers tous les neurones 
	for (auto i = 0; i < neurone.size(); i++) {
		vector<vector<double>> All_Representant = neurone[i].Get_All_Representant();
		//Boucle à travers tous les représentants pour la classe du neurone
		for (auto j = 0; j < All_Representant.size(); j++) {
			vector<double> Representant = All_Representant[j];
			//Boucle à travesr tous les poids d'un représentant
			for (auto w = 0; w < Representant.size(); w++) {
				//Écris le poids dans le fichier
				myfile << Representant[w] << ' ';
			}
		}
	}

	//Ferme le fichier
	myfile.close();
}

/*
Lis les données d'un réseau LVQ précédent et 
remplace les représentants par les nouveaux qui
sont lus

Entrée/Sortie : Réseau LVQ
				Configuration

Description :
Lis un fichier d'un réseau préalablement sauvegardé
et écris les poids dans un réseau LVQ pour le tester directement
avec la validation croisé ou les tests. Il est possible aussi
de continuer l'apprentissage à partir de ce nouveau réseau LVQ.
*/
void Read_File(Reseau_LVQ &reseau_LVQ, vector<double> &config) {

	//Nom du fichier à lire
	string File_Name = "../Debug/ReseauSave/Reseau" + to_string((int)config[8]) + ".txt";
	ifstream myfile(File_Name);
	string line;

	//Variable utile pour lire la configuration en gardant les valeurs imporantes
	double EnableWriteAcess = config[6];
	int Config_Size = config.size();
	bool CONFIG_DONE = false;
	int Indice_Config = 0;
	
	//Vecteur contenant tous les poids de tous les representants
	vector<double> Poids;

	//Ouvre le fichier
	if (myfile.is_open()) {

		//Boucle de lecture du fichier 
		while (getline(myfile, line, ' ')) {

			//Lis la configuration ou les données du réseau LVQ
			switch (CONFIG_DONE) {
			case false: {	
					//Lis la nouvlle configuration
					config[Indice_Config] = stod(line);
					Indice_Config++;
					//Indique quand la lecture de la configuration est complété
					if (Indice_Config == Config_Size) {
						CONFIG_DONE = true;
					}
				}break;

			//Lis tous les poids du réseau LVQ sauvegardé			
			case true: { Poids.push_back(stod(line));} break;
			}
		}

		//Réécris le droit d'écriture s'il était activé précédament 
		config[6] = EnableWriteAcess;

		//Forme le réseau LVQ avec les nouveaux poids pour les représentants
		reseau_LVQ.Overwrite_Reseau_LVQ(Poids, config);
	}
}