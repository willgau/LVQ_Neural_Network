/*
Fichier : Reseau.cpp

Auteur : William Gauvin
David Barrière
Charles Masson

Description: Ce fichier contient les fonctions pour le
fonctionnement du réseau LVQ.

Fonction importante: Contient la principale boucle d'apprentissage
avec la validation croisée et la validation des
tests.

Étape d'apprentissage:
		*Étape 2: Toutes les entrées sont tester
		*Étape 3: Trouve la distance minimale
*/

//Include relatif au réseau
#include "Reseau.h"
#include "ReadWrite.h"
#include "Interface.h"
#include "Neurone.h"
#include <time.h>

/*
Étape 3: Trouve la distance euclédienne entre l'entrée et
un représentant

Entrée: L'entrée (Input)
		La réponse de l'entrée

Description: Calcule la distance entre tous les points
de l'entrée et tous les poids d'un représentant pour
par la suite comparé chaque distance et seulement
considérer la plus petite distance pour par la suite
comparer la classe d'appartenance de l'entrée et celle
du représentant.

Distance euclédienne particularité: Le vrai calcule
de la distance euclédienne est le suivant:
	sqrt(SOMME((Xi - Wi)^2))
mais la comparaison de cette distance est la même
si la racine carré est présenten ou non donc à des fins
d'optimisation de temps de calcule du processeur la racine
carré est négligé et la comparaison est réalisé seulement
sur l'équation suivante:
		SOMME((Xi-Wi)^2)
*/
void Reseau_LVQ::Trouver_Distance(vector<double> &inputVals, int Reponse) {

	//Variables pour identifier le représentant gagnant
	double Distance = 0.0;
	double Min = DBL_MAX;
	int Indice = 0;
	int Representant_Gagnant = 0;

	//Boucle à travers toutes les classes
	for (auto i = 0; i < All_Class.size(); i++) {
		//Boucle à travers chaque représentant de la classe
		for (auto j = 0; j < All_Class[i].Get_Total_of_Representant(); j++) {
			vector<double> CurrentClass = All_Class[i].Get_X_Representant((int)j);
			//Boucle à travers tous les poids du représentant
			for (auto w = 0; w < inputVals.size(); w++) {
				Distance += pow(inputVals[w] - CurrentClass[w], 2);
			}
			//Compare les distances pour identifier la plus petite
			if (Min > Distance) {
				Min = Distance;
				Indice = (int)i;
				Representant_Gagnant = (int)j;
			}
			Distance = 0.0;
		}
	}
	//Identifie les classes à fin de comparaison pour l'ajustement des poids
	Classe_Obtenue = Indice;
	Classe_Desirer = Reponse;
	Numero_Representant = Representant_Gagnant;
}

/*
Étape 4: Actualise les poids
Compare si la classe obtenue par le calcule de la distance est de la
même classe que celle de la valeur d'entrée

Entrée: Entrée(Input)

Description: Appel la bonne fonction d'ajustement des poids en
comparant si la classe obtenue set la même que celle de l'entrée
*/
void Reseau_LVQ::Update_Poid(vector<double> &Input) {

	//Compare si la classe obtenue est la même que celle d'entrée
	if (Classe_Desirer == Classe_Obtenue) {

		//Ajuste les poids
		All_Class[Classe_Obtenue].Update_Poids_Pareil(Input, Numero_Representant);

		//Augmente le pourcentage de réussite du réseau LVQ
		Pourcentage_Reussite++;

		//Si la fonctionnalité est activé, la dégradation de alpha est faite seulement sur la classe du gagnant
		if (Enable_Alpha_Decay_Only_Correct) {
			All_Class[Classe_Obtenue].Set_Alpha(All_Class[Classe_Obtenue].Get_Alpha()*Alpha_Decay_Rate);
		}
	}
	else {
		//Ajuste les poids
		All_Class[Classe_Obtenue].Update_Poids_Different(Input, Numero_Representant);
	}
}

/*
Dégradation du alpha selon un certain taux

Description: Réduit le taux d'apprentissage selon un taux
prédéfini dans le fichier de configuration
*/
void Reseau_LVQ::Alpha_Decay() {

	//Ajuste alpha sur tous les neurones ("classes")
	for (auto i = 0; i < All_Class.size(); i++) {
		All_Class[i].Set_Alpha(All_Class[i].Get_Alpha()*Alpha_Decay_Rate);
	}

}
/*
Constructeur du réseau LVQ

Entrée: Configuration pour le réseau
		Poids initiaux (Représentant)

Description: Construit un réseau LVQ avec les
spécifications données par l'utilisateur avec
les poids initiaux qui sont généré par certains algorithmes

*/
Reseau_LVQ::Reseau_LVQ(vector<double> &LVQ_Constructer, deque<vector<vector<double>>> &Poids_Init) {


	//Boucle pour toutes les classes 
	for (auto i = 0; i < LVQ_Constructer[3]; i++) {
		//Crée le neurone ou la "classe" et le met dans le réseau LVQ
		All_Class.push_back(Neurone(Poids_Init[i], LVQ_Constructer));
	}

	//Initialise les variables du réseau LVQ
	Classe_Obtenue = 0;
	Pourcentage_Reussite = 0.0;
	Numero_Representant = 0;
	Alpha_Decay_Rate = LVQ_Constructer[9];
	Enable_Alpha_Decay_Only_Correct = LVQ_Constructer[12];
}
/*
Constructeur vide pour l'interface
*/
Reseau_LVQ::Reseau_LVQ(){}

/*
Remplace le réseau LVQ par un autre réseau LVQ

Entrée: Les poids de chaques représentants
		La configuration

Description: Remplace le réseau LVQ par un réseau
LVQ qui a été lu par le programme. Le réseau lu
peut être un réseau optimal pour pouvoir le tester directement
*/
void Reseau_LVQ::Overwrite_Reseau_LVQ(vector<double> &Mes_Poids, vector<double> &config) {

	//Variable pour les représentants
	vector<double> Representant;
	vector<vector<double>> All_Representant;

	//Boucle à travers toutes les données pour les mettre dans les bons neurones
	for (auto i = 0; i < config[3]; i++) {
		for (auto j = 0; j < config[4]; j++) {
			for (auto w = 0; w < (config[1] * config[2]); w++) {
				Representant.push_back(Mes_Poids[0]);
				Mes_Poids.erase(Mes_Poids.begin());
			}
			All_Representant.push_back(Representant);
			Representant.clear();
		}

		//Rempalce les représentants pour créer le nouveau réseau LVQ
		All_Class[i].Set_All_Representant(All_Representant);

		//Met le nombre total de représentant
		All_Class[i].Set_Total_of_Representant();
		All_Representant.clear();
	}
	//Rempalce les nouvelles variables importantes du réseau LVQ
	Alpha_Decay_Rate = config[9];
	Enable_Alpha_Decay_Only_Correct = config[12];
}
/*
Fonction principale d'apprentissage

Entrée/Sortie : Réseau_LVQ
				Configuration
				Queue de donnée

Description : Tous les calcules sont réalisés dans cette fonction.
Les représentants K_Mean sont trouvé puis l'algorithme du LVQ est débuté.
*/
double Train(Reseau_LVQ &reseau_LVQ, vector<double> &config, deque<vector<vector<double>>> &dequeTrain, deque<vector<vector<double>>> &dequeVC, deque<vector<vector<double>>> &dequeTest){
	
	//Initialise le random
	srand(time(0));

	//Variable locale
	int nb_Epoque = 0;
	int trainingPass = 0;
	double pourcFin = -1;
	bool END_LEARNING = false;

	//Boucle d'époque
	while (nb_Epoque < config[0] && !END_LEARNING) {
		//Passe à travers toutes les données 1 fois
		while (trainingPass < dequeTrain.size() - 1) {
			//Choisis aléatoirement une valeur d'entrée
			int valeur = rand() % (dequeTrain.size() - 1);
			vector<vector<double>> dequeVector = dequeTrain[valeur];
			vector<double> inputVals = vector2Dto1D(dequeVector);

			//Calcule la distance euclédienne (Algorithme LVQ)
			reseau_LVQ.Trouver_Distance(inputVals, dequeTrain[dequeTrain.size() - 1][valeur][0]);

			//Mise à jour des poids
			reseau_LVQ.Update_Poid(inputVals);

			//Incrémente le compteur
			trainingPass++;
		}

		//Affiche sur le graphique de l'interface
		Graphique(100, (reseau_LVQ.Get_Pourcentage() / (dequeTrain.size() - 1)));

		//Remet à 0 le compteur pour faire 1 époque
		trainingPass = 0;

		//Remet à 0 le pourcentage
		reseau_LVQ.Reset_Pourcentage();

		//Met le pourcentage finale
		pourcFin = VC(reseau_LVQ,config, dequeVC);

		//Dégradation du taux d'apprentissage
		if (!reseau_LVQ.Get_Enable_Alpha_Decay_Only_Correct()) {
			reseau_LVQ.Alpha_Decay();
		}

		//Si le threshold est atteint
		if (config[13] < pourcFin) {	
			//Si l'activation de l'écriture est demandé
			if (config[6]) {
				Write_File(reseau_LVQ, config, to_string(nb_Epoque));
			}
			//Reseau = reseau_LVQ;
			Test(reseau_LVQ,config, dequeTest);
			END_LEARNING = true;
			return 0;
		}

		//Fait les tests avec le réseau
		Test(reseau_LVQ, config, dequeTest);

		//Compteur d'époque
		nb_Epoque++;
	}

	return 0;
}
/*
Fonction principale de la validation croisée

Entrée/Sortie : Réseau_LVQ
				Configuration
				Queue de donnée

Description : Calcule uniquement la distance pour chaque
entrée pour faire la validation croisé.
*/
double VC(Reseau_LVQ &reseau_LVQ, vector<double> &config, deque<vector<vector<double>>> &dequeVC)
{
	//Variable locale
	int VCPASS = 0;
	double pourc = 0;

	//Boucle de validation croisée
	while (VCPASS < dequeVC.size() - 1) {
		//Choisis aléatoirement une donnée
		int valeur = rand() % (dequeVC.size() - 1);
		vector<vector<double>> dequeVector = dequeVC[valeur];
		vector<double> inputVals = vector2Dto1D(dequeVector);

		//Calcule la distance seulement l'algorithme LVQ
		reseau_LVQ.Trouver_Distance(inputVals, dequeVC[dequeVC.size() - 1][valeur][0]);

		//Compare les classes pour le pourcentage de réusssite du réseau
		if (reseau_LVQ.Compare_Valeur()) {
			pourc++;
		}

		//Augmente le compteur
		VCPASS++;
	}

	//Affiche sur l'interface
	Graphique(200, (pourc / (dequeVC.size() - 1)));
	if (pourc / (dequeVC.size() - 1) > 0.95) {
		Write_File(reseau_LVQ, config, "122");
	
	}
	//Retourne le pourcentage
	return (pourc / (dequeVC.size() - 1));

}
/*
Fonction principale pour les tests

Entrée/Sortie : Réseau_LVQ
				Configuration
				Queue de donnée

Description : Calcule uniquement la distance entre
chaque entrée pour toutes les valeurs de tests
*/
double Test(Reseau_LVQ &reseau_LVQ, vector<double> &config, deque<vector<vector<double>>> &dequeTest)
{
	//Variable locale
	int TESTPASS = 0;
	double pourc = 0;
	
	//Boucle pour les tests
	while (TESTPASS < dequeTest.size() - 1) {
		//Choisis aléatoirement une entrée
		int valeur = rand() % (dequeTest.size() - 1);
		vector<vector<double>> dequeVector = dequeTest[valeur];
		vector<double> inputVals = vector2Dto1D(dequeVector);

		//Calcule la distance euclédienne selon l'algorithme LVQ
		reseau_LVQ.Trouver_Distance(inputVals, dequeTest[dequeTest.size() - 1][valeur][0]);

		//Augmente le pourcentage de réussite
		if (reseau_LVQ.Compare_Valeur()) {
			pourc++;
		}

		//Augmente le compteur
		TESTPASS++;
	}

	//Affiche sur l'interface
	Graphique(300, (pourc / (dequeTest.size() - 1)));

	return 0;
}



