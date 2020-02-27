/*
Fichier : Utilitaire.cpp

Auteur : William Gauvin
		 David Barrière
		 Charles Masson

Description: Ce fichier contient les fonctions qui aide
le réseau LVQ

Fonction importante:
	*Algorithme de K_Mean
	*Conversion d'un vecteur 2D à 1D
*/
#include "Utilitaire.h"

#define SEUIL_K_MEAN 0.05

//Fonction qui représente l'algorithme K_Mean
vector<vector<double>> Calculate_K_Mean(vector<vector<double>> Prototype, int Nb_Prototype);

//Compare la distance entre deux représentants
bool Compare_Distance(vector<vector<double>> representant, vector<vector<double>> Old_representant);

/*
Transforme un vecteur 2D à un vecteur 1D

Entrée: Vecteur 2D
Sortie: Vecteur 1D

Description: Transforme un vecteur 2D à un vecteur 1D
pour la mise en forme des données du réseau
ou autres utilisations de transformation
*/
vector<double> vector2Dto1D(vector<vector<double>> vecteur2D){
	vector<double> v;
	for (int i = 0; i < vecteur2D.size(); i++){
		for (int j = 0; j < vecteur2D[i].size(); j++){
			v.push_back(vecteur2D[i][j]);
		}
	}
	return v;
}
/*
Appel de la fonction principale pour l'algorithme K_Mean

Entrée: Queue de data
		Configuration 

Description: Appel la fonction principale du calcule de K_Mean
pour calculer les représentants de chaque classe
*/
deque<vector<vector<double>>> k_mean(deque<vector<vector<double>>> &dequeTrain, vector<double> &config) {

	//Variable pour l'algorithme de k_mean
	deque<vector<vector<double>>> All_Class(10);
	deque<vector<vector<double>>> Representant;

	//Met chaque données dans leur classe approprié
	for (auto i = 0; i < dequeTrain.size() - 1; i++) {
		All_Class[dequeTrain.back()[i][0]].push_back(vector2Dto1D(dequeTrain[i]));
	}
	//Calcule les nouveaux représentants
	for (auto i = 0; i < 10; i++) {
		Representant.push_back(Calculate_K_Mean(All_Class[i], (int)config[4]));
	}
	return Representant;
}

/*
Calcule les nouveaux représentants avec l'algorithme K_Mean

Entrée: Tous les prototypes d'une classe
		Nombre de représentant pour la classe

Description: L'algorithme K_Mean est implémenté ici,
pour plus de description pour cet algorithme voir 
les notes de cours d'ELE778
*/

vector<vector<double>> Calculate_K_Mean(vector<vector<double>> Prototype, int Nb_Prototype) {

	//Variable utile pour l'algorithme
	vector<vector<double>> representant;
	vector<vector<double>> Old_representant;
	vector<double> New_rep(Prototype[0].size(), 0);
	vector<double> Clear_New_rep(Prototype[0].size(), 0);
	vector<int> Nb_Class_of_Prototype;

	//Variable pour le calcule de distance
	double distance = 0.0;
	double MIN = DBL_MAX;
	int Place_Holder = 0;
	int Divide = 0;
	int random = 0;

	//Arrêt de l'algorithme
	bool STOP = false;

	//Choisis les représentants aléatoirement
	for (auto i = 0; i < Nb_Prototype; i++) {
		random = rand() % (Prototype.size() - 1);
		representant.push_back(Prototype[random]);
	}

	//Boucle complète pour l'algorithme K_Mean
	while (!STOP) {
		//Calcule la distance pour chaque classe
		for (auto i = 0; i < Prototype.size(); i++) {
			for (auto w = 0; w < representant.size(); w++) {
				for (auto j = 0; j < Prototype[i].size(); j++) {
					distance += pow((Prototype[i][j] - representant[w][j]), 2);
				}
				if (MIN > distance) {
					MIN = distance;
					Place_Holder = (int)w;
				}
				distance = 0;
			}
			Nb_Class_of_Prototype.push_back(Place_Holder);
			MIN = DBL_MAX;
		}

		//Sauvegarde l'ancien représentant
		Old_representant = representant;
		representant.clear();

		//Calculer nouveau représentant
		for (auto i = 0; i < Nb_Prototype; i++) {
			for (auto j = 0; j < Prototype.size(); j++) {
				if (Nb_Class_of_Prototype[j] == i) {
					Divide++;
					for (auto w = 0; w < Prototype[j].size(); w++) {
						New_rep[w] += Prototype[j][w];
					}
				}
			}

			//Protège la division 
			if (Divide != 0) {
				//Calcule la moyenne
				for (auto z = 0; z < New_rep.size(); z++) {
					New_rep[z] /= Divide;
				}
			}
			//Sauvegarde le nouveau représentant
			Divide = 0;
			representant.push_back(New_rep);
			New_rep = Clear_New_rep;
		}

		//Compare les distances entre l'ancien et le nouveau représentant
		if (Compare_Distance(representant, Old_representant)) {
			STOP = true;
		}
	}
	return representant;
}
/*
Compare les distance entre les deux représentants pour l'algorithme K_Mean

Entrée: Représentant actuel
		Ancien représentant

Description: Compare la distance entre les deux vecteurs
*/
bool Compare_Distance(vector<vector<double>> representant, vector<vector<double>> Old_representant) {
	double distance = 0.0;
	double MAX = -1;
	bool Compare_Value = false;

	//Calcule la distance pour chaque classe
	for (auto i = 0; i < representant.size(); i++) {
		for (auto j = 0; j < representant[i].size(); j++) {
			distance += pow((representant[i][j] - Old_representant[i][j]), 2);
		}
		if (distance > MAX) {
			MAX = distance;
		}
		distance = 0;
	}
	//Compare avec un seuil minimale
	if (MAX < SEUIL_K_MEAN) {
		Compare_Value = true;
	}

	return Compare_Value;
}
/*
vector<double> TransformTargetVal(int TargetVal) {
	vector<double> v;

	for (int i = 0; i < 10; i++) {
		if (i == TargetVal) {
			v.push_back(1);
		}
		else {
			v.push_back(0);
		}
	}
	return v;

}

int Normalise_Vector(vector<double> &resultVals) {
	int size = resultVals.size();
	double Max = -100;
	int PlaceHolder = 0;
	vector<double> v;
	for (int i = 0; i < size; i++) {
		if (Max < resultVals[i]) {
			Max = resultVals[i];
			PlaceHolder = i;
		}
	}
	return PlaceHolder;

}
void Setup_Config(vector<double> &config, vector<unsigned> &MLP_Constructer) {

	//Nombre de neuronne dans la couche d'entrée
	MLP_Constructer.push_back((unsigned)(config[10] * config[11]));

	//Nombre de neuronne dans les couches cachées
	for (unsigned c = 0; c < config[2]; c++) {
		MLP_Constructer.push_back((unsigned)config[3]);
	}

	//Nombre de neuronne dans la couche de sortie
	MLP_Constructer.push_back((unsigned)config[1]);
}

void CreatedBinaryVector(vector<double> &targetval, int NbOfOutput, string BinaryPatern) {

	for (int i = 0; i < 4; i++) {
		if (BinaryPatern.at(i) == '0')
			targetval.push_back(0);
		else
			targetval.push_back(1);
	}
	for (int j = 0; j < NbOfOutput - 4; j++) {
		targetval.push_back(0);
	}
}

void BinaryTransform(vector<double> &targetval, vector<double> &config) {

	int Output = Normalise_Vector(targetval);
	int NbOfOutput = (int)config[1];

	targetval.clear();

	switch (Output)
	{
	case 0: CreatedBinaryVector(targetval, NbOfOutput, "0000"); break;
	case 1: CreatedBinaryVector(targetval, NbOfOutput, "0001"); break;
	case 2: CreatedBinaryVector(targetval, NbOfOutput, "0010"); break;
	case 3: CreatedBinaryVector(targetval, NbOfOutput, "0011"); break;
	case 4: CreatedBinaryVector(targetval, NbOfOutput, "0100"); break;
	case 5: CreatedBinaryVector(targetval, NbOfOutput, "0101"); break;
	case 6: CreatedBinaryVector(targetval, NbOfOutput, "0110"); break;
	case 7: CreatedBinaryVector(targetval, NbOfOutput, "0111"); break;
	case 8: CreatedBinaryVector(targetval, NbOfOutput, "1000"); break;
	case 9: CreatedBinaryVector(targetval, NbOfOutput, "1001"); break;
	default: break;
	}
}

void Normalise_Binary_Vector(vector<double> &resultVals) {
	int sum = 0;

	for (int i = 0; i < resultVals.size(); i++) {
		if (resultVals[i] > 0.2) {
			switch (i) {
			case 0: sum += 8; break;
			case 1: sum += 4; break;
			case 2: sum += 2; break;
			case 3: sum += 1; break;
			default:
				break;
			}
		}
	}
	resultVals = TransformTargetVal(sum);
}
deque<vector<vector<double>>> Get_C_Mean(deque<vector<vector<double>>> &dequeTrain, vector<double> &config) {
	vector<vector<double>> tempo;
	vector<vector<double>> c_mean;

	vector<double> mean;
	deque<vector<vector<double>>> q_c_mean;

	vector<double> nb_reponse;

	for (auto i = 0; i < config[3]; i++) {
		tempo = dequeTrain[i];
		for (auto j = 0; j < config[2]; j++) {

			for (auto w = 0; w < config[1]; w++) {
				mean.push_back(0);
			}
			c_mean.push_back(mean);
			mean.clear();
		}
		q_c_mean.push_back(c_mean);
		c_mean.clear();
		nb_reponse.push_back(0);
	}
	int reponse = 0;
	for (auto i = 0; i < dequeTrain.size() - 1; i++) {
		tempo = dequeTrain[i];
		reponse = (int)dequeTrain.back()[i][0];
		nb_reponse[reponse]++;


		for (auto j = 0; j < tempo.size(); j++) {
			for (auto w = 0; w < tempo[j].size(); w++) {
				q_c_mean[reponse][j][w] += tempo[j][w];
			}

		}

	}

	for (auto i = 0; i < q_c_mean.size(); i++) {
		tempo = dequeTrain[i];
		for (auto j = 0; j < tempo.size(); j++) {
			for (auto w = 0; w < tempo[j].size(); w++) {
				q_c_mean[i][j][w] /= nb_reponse[i];
			}

		}

	}
	return q_c_mean;
}
*/