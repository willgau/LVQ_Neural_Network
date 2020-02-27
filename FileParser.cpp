/*
Fichier : FileParser.cpp

Auteur : William Gauvin

Fichier du laboratoire 1 pour lire les bases de données
*/
#include "FileParser.h"

using namespace std;

//Énumération pour les types de fichier
enum Choice { Train, VC, Test };

//Nombre maximal de paramètre par ligne 
#define NB_PARAMETRE 26

//Fonction privée du fichier
vector<vector<double>> get_input(string name, double parametre);
deque<vector<vector<double>>> Read_File(string nom, Choice choice, double set_number, double parametre);
vector<double> vector_String_2_Double(vector<string> svec);
vector<double> Read_Config(string nom);
vector<vector<double>> Answer(vector<string> v);

//Fonction d'appel de lecture pour les différent fichier
vector<double> LoadConfig()
{
	return Read_Config("../Debug/Config.txt");
}
deque<vector<vector<double>>> LoadTrain(vector<double> config)
{
	return Read_File("../Debug/info_train.txt", Train, config[2], config[1]);
}
deque<vector<vector<double>>> LoadTest(vector<double> config)
{
	return Read_File("../Debug/info_test.txt", Test, config[2], config[1]);
}
deque<vector<vector<double>>> LoadVC(vector<double> config)
{
	return Read_File("../Debug/info_vc.txt", VC, config[2], config[1]);
}


vector<vector<double>> get_input(string nom, double parametre) {

	//Variable locale
	vector<string> v;				//Vecteur contenant une ligne de donnée du fichier
	string line;					//Ligne lu
	ifstream myfile(nom);			//Definition pour la lecture du fichier
	vector<vector<double>> matrix;	//Vecteur 2D contenant les données au complet
	int i = 0;						//Compteur pour le NB_PARAMETRE

	if (myfile.is_open()) {
		//Lis le fichier ligne par ligne 
		while (getline(myfile, line, ' ')) {
			i++;
			if (i<(parametre + 1)) {
				v.push_back(line);
			}
			//Identifie quand le fichier change de ligne
			if (i == NB_PARAMETRE) {
				i = 0;
				//Met la ligne de donnée dans le vecteur en les transformant en double
				matrix.push_back(vector_String_2_Double(v));
				v.clear();
			}
		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return matrix;
}

deque<vector<vector<double>>> Read_File(string nom, Choice choice, double set_number, double parametre) {

	//Variable locale
	string line;							//Ligne lu
	ifstream myfile(nom);					//Definition pour la lecture du fichier
	deque<vector<vector<double>>> myq;		//deque contenant les valeurs de la base de donnée
	vector<string> filename;				//Vecteur contenant les nom de fichier 
	string s;								//Nom du fichier
	string choix;							//Choix de la base d'entraînement
	size_t Folder_Name;						//Nom du dossier
	size_t nb = 0;

	if (myfile.is_open()) {
		//Lis le fichier ligne par ligne 
		while (getline(myfile, line, '\n')) {
			//Identification de la base d'entraînement
			if (choice == Train) {
				Folder_Name = line.find("train");
				choix = "train";
				nb = 14;
			}
			else if (choice == VC) {
				Folder_Name = line.find("vc");
				choix = "vc";
				nb = 11;
			}
			else if (choice == Test) {
				Folder_Name = line.find("test");
				choix = "test";
				nb = 13;
			}
			else
				break;

			//Identification du nom du fichier
			s = "../Debug/" + line.substr(Folder_Name);
			s.insert(nb, to_string(int(set_number)));
			//to_string(set_number)
			//Met le nom du fichier dans le vecteur des noms de fichiers
			filename.push_back(s);

			//Met toute les données du fichier dans la deque
			myq.push_back(get_input(s, parametre));

		}
		//Ferme le fichier
		myfile.close();
	}

	else cout << "Unable to open file";

	myq.push_back(Answer(filename));
	//Écris la base de donnée formaté 
	//Write_File(myq,filename,choix);
	return myq;
}

/*
Convertis un string en double
Entrée : Vecteur contenant des strings

Sortie : Vecteur contenant des doubles

Description :
Convertis un vecteur contenant des strings en un
vecteur contenant des doubles.
*/
vector<double> vector_String_2_Double(vector<string> svec)
{

	//Variable locale
	vector<double> dvec;
	for (auto i = svec.begin(); i != svec.end(); ++i)
	{
		string const& element = *i;
		dvec.push_back(stod(element));
	}

	return dvec;
}

/*
Lis le fichier de configuration

Entrée : Nom du fichier

Sortie : vecteur de config

Description :
Lis le fichier de configuration
*/
vector<double> Read_Config(string nom) {

	//Variable locale
	string line;				//Ligne lu
	ifstream myfile(nom);		//Nom du fichier à ouvrir
	vector<string> config;		//Vecteur de configuration

								//Lis le fichier
	if (myfile.is_open()) {
		while (getline(myfile, line, '\n')) {
			//Ignore les commentaires
			if (line[0] != '#') {
				config.push_back(line);
			}

		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return vector_String_2_Double(config);
}
/*
Convertie la réponse en string en vecteur compréhensible
pour le réseau 

Entrée : vecteur de string

Sortie : vecteur 2D de valeur

Description :
Convertie les réponses de chaque fichier en valeur
*/
vector<vector<double>> Answer(vector<string> v) {

	vector<vector<double>> matrix;
	vector<double> myv;
	string s1, s2;

	for (int i = 0; i<v.size(); i++) {
		s1 = v[i];
		s2 = s1[s1.size() - 6];
		if (s2 == "o") {
			s2 = '0';
		}
		myv.push_back(stod(s2));
		matrix.push_back(myv);
		myv.clear();

	}

	return matrix;
}