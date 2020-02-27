/*
Fichier: ReadWrite.cpp

Auteur: William Gauvin
David Barri�re
Charles Masson

Description : Fonction de lecture et d'�criture
			  pour le r�seau LVQ
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
�cris les donn�es trait�es dans un nouveau fichier

Entr�e/Sortie : R�seau LVQ
				Configuration
				Epoque

Description :
�cris le r�seau LVQ dans un fichier texte
*/
void Write_File(Reseau_LVQ &reseau_LVQ, vector<double> &config, string epoque) {
	
	//Nom du dossier et du fichier
	string Folder_Name = "../Debug/ReseauSave";
	string File_Name = Folder_Name + "/Reseau" + epoque + ".txt";

	//Cr�er le dossier
	_mkdir(Folder_Name.c_str());

	//Cr�er le fichier et l'ouvre en mode �criture
	ofstream myfile(File_Name);

	//�crit la configuration choisis 
	for (auto k = 0; k < config.size(); k++) {
		myfile << config[k] << ' ';
	}

	//Extrait tous les neurones
	vector<Neurone> neurone = reseau_LVQ.Get_All_Class();

	//Boucle � travers tous les neurones 
	for (auto i = 0; i < neurone.size(); i++) {
		vector<vector<double>> All_Representant = neurone[i].Get_All_Representant();
		//Boucle � travers tous les repr�sentants pour la classe du neurone
		for (auto j = 0; j < All_Representant.size(); j++) {
			vector<double> Representant = All_Representant[j];
			//Boucle � travesr tous les poids d'un repr�sentant
			for (auto w = 0; w < Representant.size(); w++) {
				//�cris le poids dans le fichier
				myfile << Representant[w] << ' ';
			}
		}
	}

	//Ferme le fichier
	myfile.close();
}

/*
Lis les donn�es d'un r�seau LVQ pr�c�dent et 
remplace les repr�sentants par les nouveaux qui
sont lus

Entr�e/Sortie : R�seau LVQ
				Configuration

Description :
Lis un fichier d'un r�seau pr�alablement sauvegard�
et �cris les poids dans un r�seau LVQ pour le tester directement
avec la validation crois� ou les tests. Il est possible aussi
de continuer l'apprentissage � partir de ce nouveau r�seau LVQ.
*/
void Read_File(Reseau_LVQ &reseau_LVQ, vector<double> &config) {

	//Nom du fichier � lire
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

			//Lis la configuration ou les donn�es du r�seau LVQ
			switch (CONFIG_DONE) {
			case false: {	
					//Lis la nouvlle configuration
					config[Indice_Config] = stod(line);
					Indice_Config++;
					//Indique quand la lecture de la configuration est compl�t�
					if (Indice_Config == Config_Size) {
						CONFIG_DONE = true;
					}
				}break;

			//Lis tous les poids du r�seau LVQ sauvegard�			
			case true: { Poids.push_back(stod(line));} break;
			}
		}

		//R��cris le droit d'�criture s'il �tait activ� pr�c�dament 
		config[6] = EnableWriteAcess;

		//Forme le r�seau LVQ avec les nouveaux poids pour les repr�sentants
		reseau_LVQ.Overwrite_Reseau_LVQ(Poids, config);
	}
}