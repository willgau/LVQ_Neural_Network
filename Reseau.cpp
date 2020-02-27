/*
Fichier : Reseau.cpp

Auteur : William Gauvin
David Barri�re
Charles Masson

Description: Ce fichier contient les fonctions pour le
fonctionnement du r�seau LVQ.

Fonction importante: Contient la principale boucle d'apprentissage
avec la validation crois�e et la validation des
tests.

�tape d'apprentissage:
		*�tape 2: Toutes les entr�es sont tester
		*�tape 3: Trouve la distance minimale
*/

//Include relatif au r�seau
#include "Reseau.h"
#include "ReadWrite.h"
#include "Interface.h"
#include "Neurone.h"
#include <time.h>

/*
�tape 3: Trouve la distance eucl�dienne entre l'entr�e et
un repr�sentant

Entr�e: L'entr�e (Input)
		La r�ponse de l'entr�e

Description: Calcule la distance entre tous les points
de l'entr�e et tous les poids d'un repr�sentant pour
par la suite compar� chaque distance et seulement
consid�rer la plus petite distance pour par la suite
comparer la classe d'appartenance de l'entr�e et celle
du repr�sentant.

Distance eucl�dienne particularit�: Le vrai calcule
de la distance eucl�dienne est le suivant:
	sqrt(SOMME((Xi - Wi)^2))
mais la comparaison de cette distance est la m�me
si la racine carr� est pr�senten ou non donc � des fins
d'optimisation de temps de calcule du processeur la racine
carr� est n�glig� et la comparaison est r�alis� seulement
sur l'�quation suivante:
		SOMME((Xi-Wi)^2)
*/
void Reseau_LVQ::Trouver_Distance(vector<double> &inputVals, int Reponse) {

	//Variables pour identifier le repr�sentant gagnant
	double Distance = 0.0;
	double Min = DBL_MAX;
	int Indice = 0;
	int Representant_Gagnant = 0;

	//Boucle � travers toutes les classes
	for (auto i = 0; i < All_Class.size(); i++) {
		//Boucle � travers chaque repr�sentant de la classe
		for (auto j = 0; j < All_Class[i].Get_Total_of_Representant(); j++) {
			vector<double> CurrentClass = All_Class[i].Get_X_Representant((int)j);
			//Boucle � travers tous les poids du repr�sentant
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
	//Identifie les classes � fin de comparaison pour l'ajustement des poids
	Classe_Obtenue = Indice;
	Classe_Desirer = Reponse;
	Numero_Representant = Representant_Gagnant;
}

/*
�tape 4: Actualise les poids
Compare si la classe obtenue par le calcule de la distance est de la
m�me classe que celle de la valeur d'entr�e

Entr�e: Entr�e(Input)

Description: Appel la bonne fonction d'ajustement des poids en
comparant si la classe obtenue set la m�me que celle de l'entr�e
*/
void Reseau_LVQ::Update_Poid(vector<double> &Input) {

	//Compare si la classe obtenue est la m�me que celle d'entr�e
	if (Classe_Desirer == Classe_Obtenue) {

		//Ajuste les poids
		All_Class[Classe_Obtenue].Update_Poids_Pareil(Input, Numero_Representant);

		//Augmente le pourcentage de r�ussite du r�seau LVQ
		Pourcentage_Reussite++;

		//Si la fonctionnalit� est activ�, la d�gradation de alpha est faite seulement sur la classe du gagnant
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
D�gradation du alpha selon un certain taux

Description: R�duit le taux d'apprentissage selon un taux
pr�d�fini dans le fichier de configuration
*/
void Reseau_LVQ::Alpha_Decay() {

	//Ajuste alpha sur tous les neurones ("classes")
	for (auto i = 0; i < All_Class.size(); i++) {
		All_Class[i].Set_Alpha(All_Class[i].Get_Alpha()*Alpha_Decay_Rate);
	}

}
/*
Constructeur du r�seau LVQ

Entr�e: Configuration pour le r�seau
		Poids initiaux (Repr�sentant)

Description: Construit un r�seau LVQ avec les
sp�cifications donn�es par l'utilisateur avec
les poids initiaux qui sont g�n�r� par certains algorithmes

*/
Reseau_LVQ::Reseau_LVQ(vector<double> &LVQ_Constructer, deque<vector<vector<double>>> &Poids_Init) {


	//Boucle pour toutes les classes 
	for (auto i = 0; i < LVQ_Constructer[3]; i++) {
		//Cr�e le neurone ou la "classe" et le met dans le r�seau LVQ
		All_Class.push_back(Neurone(Poids_Init[i], LVQ_Constructer));
	}

	//Initialise les variables du r�seau LVQ
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
Remplace le r�seau LVQ par un autre r�seau LVQ

Entr�e: Les poids de chaques repr�sentants
		La configuration

Description: Remplace le r�seau LVQ par un r�seau
LVQ qui a �t� lu par le programme. Le r�seau lu
peut �tre un r�seau optimal pour pouvoir le tester directement
*/
void Reseau_LVQ::Overwrite_Reseau_LVQ(vector<double> &Mes_Poids, vector<double> &config) {

	//Variable pour les repr�sentants
	vector<double> Representant;
	vector<vector<double>> All_Representant;

	//Boucle � travers toutes les donn�es pour les mettre dans les bons neurones
	for (auto i = 0; i < config[3]; i++) {
		for (auto j = 0; j < config[4]; j++) {
			for (auto w = 0; w < (config[1] * config[2]); w++) {
				Representant.push_back(Mes_Poids[0]);
				Mes_Poids.erase(Mes_Poids.begin());
			}
			All_Representant.push_back(Representant);
			Representant.clear();
		}

		//Rempalce les repr�sentants pour cr�er le nouveau r�seau LVQ
		All_Class[i].Set_All_Representant(All_Representant);

		//Met le nombre total de repr�sentant
		All_Class[i].Set_Total_of_Representant();
		All_Representant.clear();
	}
	//Rempalce les nouvelles variables importantes du r�seau LVQ
	Alpha_Decay_Rate = config[9];
	Enable_Alpha_Decay_Only_Correct = config[12];
}
/*
Fonction principale d'apprentissage

Entr�e/Sortie : R�seau_LVQ
				Configuration
				Queue de donn�e

Description : Tous les calcules sont r�alis�s dans cette fonction.
Les repr�sentants K_Mean sont trouv� puis l'algorithme du LVQ est d�but�.
*/
double Train(Reseau_LVQ &reseau_LVQ, vector<double> &config, deque<vector<vector<double>>> &dequeTrain, deque<vector<vector<double>>> &dequeVC, deque<vector<vector<double>>> &dequeTest){
	
	//Initialise le random
	srand(time(0));

	//Variable locale
	int nb_Epoque = 0;
	int trainingPass = 0;
	double pourcFin = -1;
	bool END_LEARNING = false;

	//Boucle d'�poque
	while (nb_Epoque < config[0] && !END_LEARNING) {
		//Passe � travers toutes les donn�es 1 fois
		while (trainingPass < dequeTrain.size() - 1) {
			//Choisis al�atoirement une valeur d'entr�e
			int valeur = rand() % (dequeTrain.size() - 1);
			vector<vector<double>> dequeVector = dequeTrain[valeur];
			vector<double> inputVals = vector2Dto1D(dequeVector);

			//Calcule la distance eucl�dienne (Algorithme LVQ)
			reseau_LVQ.Trouver_Distance(inputVals, dequeTrain[dequeTrain.size() - 1][valeur][0]);

			//Mise � jour des poids
			reseau_LVQ.Update_Poid(inputVals);

			//Incr�mente le compteur
			trainingPass++;
		}

		//Affiche sur le graphique de l'interface
		Graphique(100, (reseau_LVQ.Get_Pourcentage() / (dequeTrain.size() - 1)));

		//Remet � 0 le compteur pour faire 1 �poque
		trainingPass = 0;

		//Remet � 0 le pourcentage
		reseau_LVQ.Reset_Pourcentage();

		//Met le pourcentage finale
		pourcFin = VC(reseau_LVQ,config, dequeVC);

		//D�gradation du taux d'apprentissage
		if (!reseau_LVQ.Get_Enable_Alpha_Decay_Only_Correct()) {
			reseau_LVQ.Alpha_Decay();
		}

		//Si le threshold est atteint
		if (config[13] < pourcFin) {	
			//Si l'activation de l'�criture est demand�
			if (config[6]) {
				Write_File(reseau_LVQ, config, to_string(nb_Epoque));
			}
			//Reseau = reseau_LVQ;
			Test(reseau_LVQ,config, dequeTest);
			END_LEARNING = true;
			return 0;
		}

		//Fait les tests avec le r�seau
		Test(reseau_LVQ, config, dequeTest);

		//Compteur d'�poque
		nb_Epoque++;
	}

	return 0;
}
/*
Fonction principale de la validation crois�e

Entr�e/Sortie : R�seau_LVQ
				Configuration
				Queue de donn�e

Description : Calcule uniquement la distance pour chaque
entr�e pour faire la validation crois�.
*/
double VC(Reseau_LVQ &reseau_LVQ, vector<double> &config, deque<vector<vector<double>>> &dequeVC)
{
	//Variable locale
	int VCPASS = 0;
	double pourc = 0;

	//Boucle de validation crois�e
	while (VCPASS < dequeVC.size() - 1) {
		//Choisis al�atoirement une donn�e
		int valeur = rand() % (dequeVC.size() - 1);
		vector<vector<double>> dequeVector = dequeVC[valeur];
		vector<double> inputVals = vector2Dto1D(dequeVector);

		//Calcule la distance seulement l'algorithme LVQ
		reseau_LVQ.Trouver_Distance(inputVals, dequeVC[dequeVC.size() - 1][valeur][0]);

		//Compare les classes pour le pourcentage de r�usssite du r�seau
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

Entr�e/Sortie : R�seau_LVQ
				Configuration
				Queue de donn�e

Description : Calcule uniquement la distance entre
chaque entr�e pour toutes les valeurs de tests
*/
double Test(Reseau_LVQ &reseau_LVQ, vector<double> &config, deque<vector<vector<double>>> &dequeTest)
{
	//Variable locale
	int TESTPASS = 0;
	double pourc = 0;
	
	//Boucle pour les tests
	while (TESTPASS < dequeTest.size() - 1) {
		//Choisis al�atoirement une entr�e
		int valeur = rand() % (dequeTest.size() - 1);
		vector<vector<double>> dequeVector = dequeTest[valeur];
		vector<double> inputVals = vector2Dto1D(dequeVector);

		//Calcule la distance eucl�dienne selon l'algorithme LVQ
		reseau_LVQ.Trouver_Distance(inputVals, dequeTest[dequeTest.size() - 1][valeur][0]);

		//Augmente le pourcentage de r�ussite
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



