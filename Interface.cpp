/*
Laboratoire 2. ELE778

Interface.cpp

Auteurs : David Barrière, Charles Masson, William Gauvin

Description : Ce fichier est utilisé pour l'interface principale du laboratoire. Elle permet l'affichage de tous les contrôles et les fenêtres.
La plupart des fonctions d'affichages sont des fonctions tirées de l'API (WINAPI) utilisé. Elle sont donc très peu commentées.

*/

#include <windows.h>
#include <wchar.h>
#include <string>
#include <cstring>
#include <iostream>
#include "Reseau.h"
#include "ReadWrite.h"
#include "Interface.h"

//Variables pour la conservation des fenêtres ouvertes et utilisées
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LPCWSTR Edit1 = L"xx";
HWND win[15];

//Déclaration des fonctions utilisées
void AddControls(HWND);
void createButton(HWND);
void outline(HWND, int, int, int, int, COLORREF);
void setValue(vector<double>);


//Variables utilisées pour conserver les meilleurs performances.
double pourcPrecTrain = 0;
double meilleurPourcTrain = 0;
double pourcPrecVC = 0;
double meilleurPourcVC = 0;
double pourcPrecTest = 0;
double meilleurPourcTest = 0;
double tempReseauPourc = 0;
int epoque = 1;
int meilleureEpoque = 1;
string textB = "";



//Variables pour la construction du réseau initial 
deque<vector<vector<double>>> dequeTrain;
deque<vector<vector<double>>> dequeVC;
deque<vector<vector<double>>> dequeTest;
deque<vector<vector<double>>> K_mean_Representant;
vector<double> config = LoadConfig();
bool START_DONE = false;
bool BYPASS_TRAIN = false;

//Initialisation du réseau
Reseau_LVQ myNet;

/*
Entrée: HInstance, arguments, ncmdshow 

Description: La fonction utilisée pour la création de la fenêtre principale.
*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevIns, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"myWindowClass", L"Laboratoire #3: IA", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 300, 200, 1000, 570, NULL, NULL, NULL, NULL);
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
/*
Entrée: Fenetre appelée, message envoyé, paramètre W, paramètre L

Description: La fonction utilisée pour le dessin des fenêtres, la manipulation des fenêtres et contrôles et pour gérer les évènements de l'interface.
*/
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	double conf[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	LPCSTR text = "";

	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		createButton(hWnd);
		AddControls(hWnd);
		setValue(config);
		SetWindowPos(CreateWindowW(L"static", L"100", WS_VISIBLE | SS_CENTER | WS_CHILD, 240, 150, 25, 15, hWnd, NULL, NULL, NULL), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(CreateWindowW(L"static", L"erreur", WS_VISIBLE | SS_CENTER | WS_CHILD, 240, 125, 40, 15, hWnd, NULL, NULL, NULL), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(CreateWindowW(L"static", L"epoque", WS_VISIBLE | SS_CENTER | WS_CHILD, 550, 485, 50, 20, hWnd, NULL, NULL, NULL), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		win[13] = CreateWindowW(L"static", L"10", WS_VISIBLE | SS_CENTER | WS_CHILD, 750, 485, 40, 20, hWnd, NULL, NULL, NULL);
		setValue(config);
		text = "You can start now!";
		SetWindowText(win[11], text);

		break;
		//LOWORD(wp) selon le bouton qui est cliqué, on effectue l'action requise
	case WM_COMMAND:
		if (LOWORD(wp) == 1)
		{

			//Bouton Start 
			START_DONE = true;
			//Lis les données
			dequeTrain = LoadTrain(config);
			dequeVC = LoadVC(config);
			dequeTest = LoadTest(config);

			//Fait l'algorithme K_Mean
			K_mean_Representant = k_mean(dequeTrain, config);

			//Crée le réseau LVQ
			Reseau_LVQ Net(config, K_mean_Representant);

			//Lis un réseau sauvegardé
			if (config[7]) {
				Read_File(Net, config);
				BYPASS_TRAIN = true;
			}

			myNet = Net;
			setValue(config);
			text = "Net Created";
			SetWindowText(win[11], text);
		}
		if (LOWORD(wp) == 2)
		{
			if (START_DONE) {
				if (BYPASS_TRAIN){
					tempReseauPourc = Test(myNet, config, dequeTest);
				}
				else {
					text = "Please Start Training";
					SetWindowText(win[11], text);
				}
				
			}
			else {
				text = "Please Click Start";
				SetWindowText(win[11], text);
			}
			
		}
		if (LOWORD(wp) == 3)
		{
			if (START_DONE) {
				tempReseauPourc = Train(myNet,config, dequeTrain, dequeVC, dequeTest);
				BYPASS_TRAIN = true;
			}
			else {
				text = "Please Click Start";
				SetWindowText(win[11], text);
			}

		}
		if (LOWORD(wp) == 4)
		{
			if (START_DONE) {
				if (BYPASS_TRAIN) {
					tempReseauPourc = VC(myNet, config, dequeVC);
				}
				
			else {
				text = "Please Start Training";
				SetWindowText(win[11], text);
			}
			}
		else{
				text = "Please Click Start";
				SetWindowText(win[11], text);
			}
		}
	if (LOWORD(wp) == 8)
		{
			DestroyWindow(hWnd);
		}
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}

}
/*
Entrée: Fenetre parent des contrôles d'affichage du fichier de configuration

Description: La fonction utilisée pour effectuer l'affichage des contrôles pour l'affichage des valeurs de la configuration
*/
void AddControls(HWND hWnd)
{
	CreateWindowW(L"static", L"Nombre MFCC", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 10, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Nombre de Set", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 55, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Nombre de classes", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 100, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Nombre de representants", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 145, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Alpha", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 190, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Alpha Decay Rate", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 235, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Amplification Alpha", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 280, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Write Access", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 325, 100, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Threshold VC", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 370, 100, 40, hWnd, NULL, NULL, NULL);

	win[0] = CreateWindowW(L"Edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 10, 100, 40, hWnd, NULL, NULL, NULL);//NB Param MFCC
	win[1] = CreateWindowW(L"Edit", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 55, 100, 40, hWnd, NULL, NULL, NULL);//NB Set
	win[2] = CreateWindowW(L"Edit", L"2", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 100, 100, 40, hWnd, NULL, NULL, NULL);//NB classes
	win[3] = CreateWindowW(L"Edit", L"3", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 145, 100, 40, hWnd, NULL, NULL, NULL);//NB representants
	win[4] = CreateWindowW(L"Edit", L"4", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 190, 100, 40, hWnd, NULL, NULL, NULL);//Eta
	win[5] = CreateWindowW(L"Edit", L"5", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 235, 100, 40, hWnd, NULL, NULL, NULL);//Eta decay
	win[6] = CreateWindowW(L"Edit", L"6", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 280, 100, 40, hWnd, NULL, NULL, NULL);//Alpha decay rate
	win[7] = CreateWindowW(L"Edit", L"7", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 325, 100, 40, hWnd, NULL, NULL, NULL);//WriteAccess
	win[8] = CreateWindowW(L"Edit", L"8", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 370, 100, 40, hWnd, NULL, NULL, NULL);//Threshold
	win[10] = CreateWindowW(L"static", L"Erreur sur VC", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 230, 120, 740, 390, hWnd, NULL, NULL, NULL);//graphique
	win[11] = CreateWindowW(L"static", L"Etat", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 650, 10, 320, 100, hWnd, NULL, NULL, NULL);//console

	SetWindowPos(win[13], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

/*
Entrée: vecteur de configuration

Description: La fonction utilisée pour effectuer l'affichage des valeurs du fichier de configuration à l'écran
*/
void setValue(vector<double> config)
{
	LPCSTR text;
	string str;

	str = to_string((int)config[0]);
	text = str.c_str();
	SetWindowText(win[13], text);
	
	str = to_string(config[1]);
	text = str.c_str();
	SetWindowText(win[0], text);

	str = to_string(config[2]);
	text = str.c_str();
	SetWindowText(win[1], text);

	str = to_string(config[3]);
	text = str.c_str();
	SetWindowText(win[2], text);

	str = to_string(config[4]);
	text = str.c_str();
	SetWindowText(win[3], text);

	str = to_string(config[5]);
	text = str.c_str();
	SetWindowText(win[4], text);

	str = to_string(config[6]);
	text = str.c_str();
	SetWindowText(win[7], text);

	str = to_string(config[9]);
	text = str.c_str();
	SetWindowText(win[5], text);

	str = to_string(config[11]);
	text = str.c_str();
	SetWindowText(win[6], text);

	str = to_string(config[13]);
	text = str.c_str();
	SetWindowText(win[8], text);
}

/*
Entrée: Texte à afficher

Description: La fonction utilisée pour effectuer l'affichage de texte dans la fenêtre d'information.
*/
void writeToBox(string text)
{
	LPCSTR temp = text.c_str();
	SetWindowText(win[11], temp);
}

/*
Entrée: Fenetre parent des boutons

Description: La fonction utilisée pour effectuer la création des boutons à l'interface.
*/
void createButton(HWND hWnd)
{
	CreateWindow(TEXT("button"), TEXT("Start"), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 230, 10, 100, 50, hWnd, (HMENU)1, NULL, NULL);
	CreateWindow(TEXT("button"), TEXT("Train"), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 230, 60, 100, 50, hWnd, (HMENU)3, NULL, NULL);
	CreateWindow(TEXT("button"), TEXT("Generalisation"), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 330, 60, 100, 50, hWnd, (HMENU)2, NULL, NULL);
	CreateWindow(TEXT("button"), TEXT("VC"), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 330, 10, 100, 50, hWnd, (HMENU)4, NULL, NULL);
	CreateWindow(TEXT("button"), TEXT("Quit"), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 430, 30, 100, 50, hWnd, (HMENU)8, NULL, NULL);	
}

/*
Entrée: Fenetre du graphique, coordonnes de la ligne à dessiner et couleur de la ligne

Description: La fonction utilisée pour dessiner les lignes du graphique
*/
void outline(HWND hwnd, int startX, int startY, int endX, int endY, COLORREF color)
{

	HDC hdc;                        /*  get a graphics device context */
	hdc = GetDC(hwnd);

	HPEN pencolor;             /*  set pen color and style  */
	pencolor = CreatePen(PS_SOLID, 1, color);      /* width 0, = 1 pixel thick */
	SelectObject(hdc, pencolor);

	MoveToEx(hdc, startX, startY, NULL);    /* Set starting pen, drawing position */
	LineTo(hdc, endX, endY);

	ReleaseDC(hwnd, hdc);

	return;
}

/*
Entrée: Couleur du graph et erreur à afficher

Description: La fonction dessine le graphique d'apprentissage, de VC et de généralisation selon la couleur.
*/
void Graphique(double couleur, double erreurPresente)
{
	if (epoque == 1)
	{
		outline(win[10], 40, 360, 40, 20, 1);
		outline(win[10], 710, 360, 40, 360, 1);
	}

	if (couleur == 100)
	{
		outline(win[10], 40 + 670 / config[0] * (epoque - 1), 360 - 320 * pourcPrecTrain, 40 + 670 / config[0] * epoque, 360 - 320 * erreurPresente, couleur);

		meilleurPourcTrain = erreurPresente;
		textB = "Train ";
		textB += to_string(epoque);
		textB += " : ";
		textB += to_string(meilleurPourcTrain);
		textB += "\r\n";
		pourcPrecTrain = erreurPresente;
	}
	else if (couleur == 200)
	{
		outline(win[10], 40 + 670 / config[0] * (epoque - 1), 360 - 320 * pourcPrecVC, 40 + 670 / config[0] * epoque, 360 - 320 * erreurPresente, couleur);

		//meilleureEpoque = epoque - 1;
		if (meilleurPourcVC < erreurPresente)
		{
			meilleurPourcVC = erreurPresente;
			meilleureEpoque = epoque;
		}
		textB += "VC ";
		textB += to_string(epoque);
		textB += " : ";
		textB += to_string(erreurPresente);
		textB += "\r\n";

		textB += "Best VC ";
		textB += to_string(meilleureEpoque);
		textB += " : ";
		textB += to_string(meilleurPourcVC);
		textB += "\r\n";

		pourcPrecVC = erreurPresente;
		//writeToBox(textB);
		//textB = "";
		
	}
	else if (couleur == 300)
	{
		outline(win[10], 40 + 670 / config[0] * (epoque - 1), 360 - 320 * pourcPrecTest, 40 + 670 / config[0] * epoque, 360 - 320 * erreurPresente, couleur);
		meilleurPourcTest = erreurPresente;
		textB += "Test ";
		textB += to_string(epoque-1);
		textB += " : ";
		textB += to_string(erreurPresente);	
		pourcPrecTest = erreurPresente;		
		if (meilleurPourcVC > config[13])
		{
			textB += "\r\nThreshold VC atteint : " + to_string(meilleurPourcVC);			
		}
		writeToBox(textB);
		textB = "";
		epoque++;
	}
}




