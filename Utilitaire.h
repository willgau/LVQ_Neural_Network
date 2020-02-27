/*
Fichier : Utilitaire.h

Auteur : William Gauvin
		 David Barrière
		 Charles Masson

Description: Ce fichier contient les fonctions qui aide
le réseau LVQ

Fonction importante:
		*Algorithme de K_Mean
		*Conversion d'un vecteur 2D à 1D
*/
#pragma once

//Include 
#include "Reseau.h"

//MAlgorithme K_Mean
deque<vector<vector<double>>> k_mean(deque<vector<vector<double>>> &dequeTrain, vector<double> &config);

//Convertie un vecteur 2D en 1D
vector<double> vector2Dto1D(vector<vector<double>> vecteur2D);

/*
deque<vector<vector<double>>> Get_C_Mean(deque<vector<vector<double>>> &dequeTrain, vector<double> &config);
void Normalise_Binary_Vector(vector<double> &resultVals);
void BinaryTransform(vector<double> &targetval, vector<double> &config);
void CreatedBinaryVector(vector<double> &targetval, int NbOfOutput, string BinaryPatern);
void Setup_Config(vector<double> &config, vector<unsigned> &MLP_Constructer);
int Normalise_Vector(vector<double> &resultVals);
vector<double> TransformTargetVal(int TargetVal);
*/