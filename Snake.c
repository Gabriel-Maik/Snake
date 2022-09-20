#define _CRT_SECURE_NO_WARNINGS
#define DLUGOSC_NAZWY_PLIKU 51
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

struct elementWeza
{
	int x;
	int y;
	struct elementWeza* nastepny; // do glowy
	struct elementWeza* poprzedni; // do ogona
};

void setCursor(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void losowanieJedzenia(int** mapa, int X, int Y)
{
	int x;
	int y;
	srand(time(NULL));
	do
	{
		x = rand() % X;
		y = rand() % Y;
	} while ((mapa[y][x] == 2) || (mapa[y][x] == 3) || (mapa[y][x] == 1));
	mapa[y][x] = 3;
	setCursor(x, y); printf("o");
}

void ruchWeza(int** mapa, int X, int Y, int kierunekRuchu, int kierunekPoprzedni, int* czyGrac, int* dlugoscWeza, struct elementWeza** glowaWeza, struct elementWeza** ogonWeza,int *iloscPolNaJedzenie)
{
	//int xZmiana;
	//int yZmiana;
	//int xNastepny;
	//int yNastepny;
	int przesuniecie = 0;
	int czyJedzono = 0;
	int xOgona = (*ogonWeza)->x;
	int yOgona = (*ogonWeza)->y;
	struct elementWeza* tymczasowy = (*ogonWeza)->nastepny;

	((*ogonWeza)->nastepny)->poprzedni = NULL; // ogon na poczatek
	(*ogonWeza)->nastepny = NULL;
	(*glowaWeza)->nastepny = *ogonWeza;
	(*ogonWeza)->poprzedni = *glowaWeza;
	*glowaWeza = *ogonWeza;
	*ogonWeza = tymczasowy;
	switch (kierunekRuchu)
	{
		case 1: //góra
			(*glowaWeza)->x = (*glowaWeza)->poprzedni->x;
			if ((*glowaWeza)->poprzedni->y > 0) (*glowaWeza)->y = (*glowaWeza)->poprzedni->y - 1;
			else (*glowaWeza)->y = Y - 1;
			break;
		case 2: //prawo
			(*glowaWeza)->y = (*glowaWeza)->poprzedni->y;
			if ((*glowaWeza)->poprzedni->x < X - 1) (*glowaWeza)->x = (*glowaWeza)->poprzedni->x + 1;
			else (*glowaWeza)->x = 0;
			break;
		case 3: //dó³
			(*glowaWeza)->x = (*glowaWeza)->poprzedni->x;
			if ((*glowaWeza)->poprzedni->y < Y - 1) (*glowaWeza)->y = (*glowaWeza)->poprzedni->y + 1;
			else (*glowaWeza)->y = 0;
			break;
		case 4: //lewo
			(*glowaWeza)->y = (*glowaWeza)->poprzedni->y;
			if ((*glowaWeza)->poprzedni->x > 0) (*glowaWeza)->x = (*glowaWeza)->poprzedni->x - 1;
			else (*glowaWeza)->x = X - 1;
			break;
		default:
			printf("B³¹d krytyczny: Nieprawid³owy kierunek!"); system("pause");
			break;
	}
	if ((mapa[(*glowaWeza)->y][(*glowaWeza)->x] == 0)||(mapa[(*glowaWeza)->y][(*glowaWeza)->x] == 3))
	{
		if (mapa[(*glowaWeza)->y][(*glowaWeza)->x] == 3)
		{
			//setCursor(0, 0); printf("test"); system("pause");
			czyJedzono = 1;
			*dlugoscWeza = *dlugoscWeza + 1;
			tymczasowy = NULL;
			tymczasowy = malloc(sizeof(struct elementWeza));
			tymczasowy->poprzedni = NULL;
			tymczasowy->nastepny = *ogonWeza;
			tymczasowy->x = xOgona;
			tymczasowy->y = yOgona;
			(*ogonWeza)->poprzedni = tymczasowy;
			*ogonWeza = tymczasowy;
		}
		mapa[(*glowaWeza)->y][(*glowaWeza)->x] = 2;
		setCursor((*glowaWeza)->x, (*glowaWeza)->y);
		if((kierunekRuchu==1) || (kierunekRuchu == 3)) printf("|");
		else printf("-");
	}
	else
	{
		if ((*glowaWeza)->y <= 4) przesuniecie = Y - 4;
		setCursor(0, przesuniecie + 0); printf("Koniec Gry! ");
		setCursor(0, przesuniecie + 1); printf("Wynik: %d ", *dlugoscWeza);
		setCursor(0, przesuniecie + 2); printf("R/r - spróbuj ponownie ", *dlugoscWeza);
		setCursor(0, przesuniecie + 3); printf("Esc - wyjœcie "); setCursor(X - 1, Y - 1);
		przesuniecie = 0;
		while ((przesuniecie != 27 )&& (przesuniecie != 'r')&& (przesuniecie != 'R')) przesuniecie = _getch();
		if (przesuniecie == 27) *czyGrac = 0;
		else *czyGrac = 2;
		while (_kbhit()) _getch();
	}
	if (czyJedzono == 0)
	{
		mapa[yOgona][xOgona] = 0;// usuwana 2 na ogonie
		setCursor(xOgona, yOgona); printf(" ");
		//obsluga zakrecen

	}
	else
	{
		if (*iloscPolNaJedzenie > 0)
		{
			losowanieJedzenia(mapa, X, Y);
			*iloscPolNaJedzenie = *iloscPolNaJedzenie - 1;
		}
		else
		{
			if ((*glowaWeza)->y <= 4) przesuniecie = Y - 4;
			setCursor(0, przesuniecie + 0); printf("Wygra³eœ! ");
			setCursor(0, przesuniecie + 1); printf("Mistrzowski wynik: %d ", *dlugoscWeza);
			setCursor(0, przesuniecie + 2); printf("R/r - spróbuj ponownie ", *dlugoscWeza);
			setCursor(0, przesuniecie + 3); printf("Esc - wyjœcie "); setCursor(X - 1, Y - 1);
			przesuniecie = 0;
			while ((przesuniecie != 27) && (przesuniecie != 'r') && (przesuniecie != 'R')) przesuniecie = _getch();
			if (przesuniecie == 27) *czyGrac = 0;
			else *czyGrac = 2;
			while (_kbhit()) _getch();
		}
		
	}
	// eksperymentalna g³owa wê¿a
	setCursor((*glowaWeza)->x, (*glowaWeza)->y);
	switch (kierunekRuchu)
	{
	case 1://mozliwosc zmiany w roznych kierunkach
		printf("^");
		break;
	case 2:
		printf(">");
		break;
	case 3:
		printf("v");
		break;
	case 4:
		printf("<");
		break;
	default:
		setCursor(0, 0);	printf("B³¹d krytyczny!");	system("pause");
		*czyGrac = 0;
	}
	// obsluga zakrecen
	przesuniecie = ((kierunekRuchu - kierunekPoprzedni + 4) % 4);
	setCursor((*glowaWeza)->poprzedni->x, (*glowaWeza)->poprzedni->y);
	if (przesuniecie == 1) // skrecono w prawo
	{
		if ((kierunekRuchu == 1) || (kierunekRuchu == 3))
		{
			printf("\\");
		}
		else
		{
			printf("/");
		}
	}
	else if (przesuniecie == 3) // skrecono w lewo
	{
		if ((kierunekRuchu == 1) || (kierunekRuchu == 3))
		{
			printf("/");
		}
		else
		{
			printf("\\");
		}
	}
	else if (przesuniecie == 0) // nie skrecono
	{
		if ((kierunekRuchu == 1) || (kierunekRuchu == 3))
		{
			printf("|");
		}
		else
		{
			printf("-");
		}
	}
	setCursor(X - 1, Y - 1);
}

void wczytywanieNapisu(char *napis, int dlugosc)
{
	int znak, i;
	int czyKontynuowac = 1;
	printf("\nProszê podaæ nazwê pliku: ");
	i = 0;
	while (czyKontynuowac)
	{
		/* //zwykle wczytywanie
		scanf("%100s", napis);
		i = 0;
		while ((znak = getchar()) != '\n' && znak != EOF)
		{
			i++;
		}
		if (i > 0)
		{
			printf("\nProszê podaæ krótsze polecenie! Maksymalna dozwolona dlugosc to 100 znakow.\n\n");
		}
		else czyKontynuowac = 0;*/
		// wczytywanie z getchar
		znak = getchar();
		if ((znak != EOF) && (znak != '\n')) napis[i] = znak;
		else
		{
			czyKontynuowac = 0;
			napis[i] = 0;
		}
		i = i + 1;
		if (i >= dlugosc)
		{
			printf("\nProszê podaæ krótsze polecenie! Maksymalna dozwolona dlugosc to %d znakow.\n\n", dlugosc);
			while ((znak = getchar()) != '\n' && znak != EOF)
				i = 0;
		}
	}
}

void wypelnienieTablicyProstokotnejZerami(int** tablica, int iloscWierszy, int iloscKolumn)
{
	int i, j;
	for (i = 0; i < iloscWierszy; i++)
	{
		for (j = 0; j < iloscKolumn; j++)
		{
			tablica[i][j] = 0;
		}
	}
}

void alokowanieTablicyDwuwymiarowej(int ***tablica, int iloscWierszy, int iloscKolumn)
{
	int i;
	*tablica = malloc(iloscWierszy * sizeof(*(*tablica)));
	if (*tablica != NULL)
	{
		for (i = 0; i < iloscWierszy; i++)
		{
			(*tablica)[i] = malloc(iloscKolumn * sizeof(*tablica));

			if ((*tablica)[i] == NULL)
			{
				for (i; i >= 0; i--)
				{
					free(*tablica[i]);
				}
				free(*tablica);
				break;
			}
		}
	}
	else
	{
		free(*tablica);
	}
}

int menu(int iloscOpcji, char *opcje[])
{
	int i;
	int znak;
	int zatwierdzenie = 1;
	int wskazanaOpcja = 1;

	while (zatwierdzenie)
	{
		system("cls");
		printf("%s\n", opcje[iloscOpcji - 1]);
		for (i = 0; i < iloscOpcji - 1; i++)
		{
			if (wskazanaOpcja == (i + 1)) printf("---> "); else printf("     "); printf("%s\n", opcje[i]);
		}
		znak = _getch();
		switch (znak)
		{
		case 224:
			if (_kbhit())
			{
				znak = _getch();
				switch (znak)
				{
				case 72:	//strza³ka w góre
					if (wskazanaOpcja > 1) wskazanaOpcja = wskazanaOpcja - 1;
					else wskazanaOpcja = (iloscOpcji - 1);
					break;
				case 80:	//strza³ka w dol
					if (wskazanaOpcja < iloscOpcji - 1) wskazanaOpcja = wskazanaOpcja + 1;
					else wskazanaOpcja = 1;
					break;
				}
				break;
			}
			break;
		case 13:
			zatwierdzenie = 0;
			break;
		case 32:
			zatwierdzenie = 0;
			break;
		case 83:	//S
			if (wskazanaOpcja < iloscOpcji - 1) wskazanaOpcja = wskazanaOpcja + 1;
			else wskazanaOpcja = 1;
			break;
		case 87:	//W
			if (wskazanaOpcja > 1) wskazanaOpcja = wskazanaOpcja - 1;
			else wskazanaOpcja = (iloscOpcji - 1);
			break;
		case 115:	//s
			if (wskazanaOpcja < iloscOpcji - 1) wskazanaOpcja = wskazanaOpcja + 1;
			else wskazanaOpcja = 1;
			break;
		case 119:	//w
			if (wskazanaOpcja > 1) wskazanaOpcja = wskazanaOpcja - 1;
			else wskazanaOpcja = (iloscOpcji - 1);
			break;
		}
	}
	printf("\n");
	while (_kbhit()) znak = _getch();
	return wskazanaOpcja;
}

void koniecLinii(FILE *plik)
{
	int znak;
	int powtorzenie = 1;
	while (powtorzenie)
	{
		znak = getc(plik);
		if (znak == 10 || znak == EOF) powtorzenie = 0;
	}
}

void przewiniecieOdstepu(FILE *plik)
{
	int powtorzenie = 1;
	int znak;
	do
	{
		znak = getc(plik);
		if ((znak != 32) && (znak != 9))
		{
			fseek(plik, -1, SEEK_CUR);
			powtorzenie = 0;
		}
	} while (powtorzenie);
}

void obslugaKomentarzy(FILE *plik)
{
	int powtorzenie = 1;
	int znak;

	do
	{
		znak = getc(plik);
		if (znak == 32 || znak == 9)
		{
			przewiniecieOdstepu(plik);
		}
		else if (znak == 35)
		{
			koniecLinii(plik);
		}
		else if (znak == 10)
		{
			;
		}
		else
		{
			fseek(plik, -1, SEEK_CUR);
			powtorzenie = 0;
		}
	} while (powtorzenie);
}

void ustawWielkoscKonsoli(int X, int Y)
{
	//**************************************************************
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE); // uchwyt do okna konsoli
	SMALL_RECT windowSize = { 0, 0, X - 1, Y - 1 }; // strunktura przechowujaca koordynaty lewego gornego rogu i prawego dolnego, konsoli
	COORD dwSize = { X, Y }; // struktura przechowuje wielkosc bufora
	HWND uchwytKonsoli = GetConsoleWindow(); // zmienna przechowuje uchwyt do okna konsoli

	//**************************************************************

	//ustawia maxymalny rozmiar konsoli
	//MoveWindow(uchwytKonsoli, 0, 0, 5000, 5000, TRUE);
	
	//zmiana wielkosci okna konsoli
	SetConsoleWindowInfo(wHnd, 1, &windowSize);

	//zmiana wielkosci bufora konsoli
	SetConsoleScreenBufferSize(wHnd, dwSize);
}

int wczytywaniePoziomu(int*** mapa, int* X, int* Y, int* x, int* y, int czyGrac, int* wybor, char *nazwaPliku)
{
	int i, j;
	int czyZakonczonoPowodzeniem = 1;
	int czyPauza = 0;
	FILE *plik = NULL;
	//char nazwaPliku[51];
	char *menuWyboruPoziomu[6];
	char *labirynt = "_labirynt";
	menuWyboruPoziomu[0] = "Pole";
	menuWyboruPoziomu[1] = "Piaskownica";
	menuWyboruPoziomu[2] = "Miejska D¿ungla";
	menuWyboruPoziomu[3] = "Autostrada (robi³ £ysy)";
	menuWyboruPoziomu[4] = "Poziom niestandardowy";
	menuWyboruPoziomu[5] = "\n     Wybierz poziom\n";
	if (*wybor == 0) *wybor = menu(6, &menuWyboruPoziomu[0]);
	switch (*wybor)
	{
	case 1: //Pole
		plik = fopen("_pole.pgm", "r"); system("COLOR 2F");
		break;
	case 2: //Piaskownica
		plik = fopen("_piaskownica.pgm", "r"); system("COLOR E0");
		break;
	case 3: //Miejska D¿ungla
		plik = fopen("_miejskaDzungla.pgm", "r"); system("COLOR 80");
		break;
	case 4: //Autostrada
		plik = fopen("_autostrada.pgm", "r");
		break;
	case 5: //Poziom niestandardowy
		if (czyGrac == 1)
		{
			wczytywanieNapisu(nazwaPliku, DLUGOSC_NAZWY_PLIKU - 1);
			printf("\nNazwa wczytana pomyœlnie!");
			czyPauza = 1;
		}
		plik = fopen(nazwaPliku, "r");
		break;
	default:
		printf("B³¹d krytyczny!");
		break;
	}
	if (plik == NULL)
	{
		printf("\n\nNie odnaleziono pliku: %s\n\n", nazwaPliku);
		czyZakonczonoPowodzeniem = 0;
	}
	else
	{
		//printf("\n\nPlik otworzony pomyœlnie!\n\nRozpoczêto wczytywanie.");
		//printf("\nPlik otworzony pomyœlnie!\nRozpoczêto wczytywanie. Mo¿e to zaj¹æ parê sekund.");
		obslugaKomentarzy(plik); fscanf(plik, "P2");
		obslugaKomentarzy(plik); fscanf(plik, "%d", X);
		obslugaKomentarzy(plik); fscanf(plik, "%d", Y); //printf("test %d %d", *X, *Y);
		alokowanieTablicyDwuwymiarowej(mapa, *Y, *X);
		obslugaKomentarzy(plik); fscanf(plik, "%d", &i); //printf("Glebia: %d", strukturaObrazu->glebia);
		if (mapa == NULL)
		{
			printf("B³¹d krytyczny!\nAlokacja dynamicznej tablicy dwuwymiarowej zakoñczy³a siê niepowodzeniem!\n");
			czyZakonczonoPowodzeniem = 0;
		}
		else if ((*X>106)||(*Y>28))
		{
			printf("\n\nWymiary poziomu wykraczaj¹ poza dozwolone 106x28!\n\n");
			czyPauza = 1;
			czyZakonczonoPowodzeniem = 0;
			fclose(plik);
		}
		else
		{
			wypelnienieTablicyProstokotnejZerami(*mapa, *Y, *X);
			for (i = 0; i < *Y; i++)
			{
				//printf("\n"); //wypisanie do testowania
				for (j = 0; j < *X; j++)
				{
					obslugaKomentarzy(plik);
					fscanf(plik, "%d", &((*mapa)[i][j]));
					if ((*mapa)[i][j] == 2)
					{
						*x = j;
						*y = i;
					}
					//printf("%d ", (*mapa)[i][j]); //wypisanie do testowania
				}
			}
			if(czyGrac != 2)
			{ 
				setCursor(X - 1, Y - 1);
				printf("\n\nWczytano pomyœlnie!\n\n");
			}
			else czyPauza = 0;
			fclose(plik); 
		}
	}
	if(czyPauza) system("pause");
	return czyZakonczonoPowodzeniem;
}

void wyswietlaniePoziomu(int** mapa, int X, int Y)
{
	int i, j;
	setCursor(0, 0);
	for (i = 0; i < Y; i++)
	{
		for (j = 0; j < X; j++)
		{
			if (mapa[i][j] == 1)
			{
				setCursor(j, i);
				printf("#");
			}
		}
	}
	setCursor(X - 1, Y - 1);
}

void usuwanieWeza(struct elementWeza** ogonWeza, struct elementWeza** glowaWeza)
{
	struct elementWeza* tymczasowy;
	while ((*ogonWeza)->nastepny != NULL)
	{
		tymczasowy = (*ogonWeza)->nastepny;
		free(*ogonWeza);
		*ogonWeza = tymczasowy;
	}
	free(*ogonWeza);
	*ogonWeza = NULL;
	*glowaWeza = NULL;
}

int wyznaczanieIlosciPolNaJedzenie(int** mapa, int iloscWierszy, int iloscKolumn)
{
	int i, j, iloscPolNaJedzenie;
	iloscPolNaJedzenie = 0;
	for (i = 0; i < iloscWierszy; i++)
	{
		for (j = 0; j < iloscKolumn; j++)
		{
			if (mapa[i][j] == 0) iloscPolNaJedzenie++;
		}
	}
	return iloscPolNaJedzenie;
}

int main()
{
	int kierunek = 2; // 1-góra, 2-prawo, 3-dó³, 4-lewo
	int kierunekPoprzedni = 2;
	int c1 = 0;
	int c2 = 0;
	int i = 0;
	int **mapa;
	int X = 36;
	int Y = 20;
	int x = 0;
	int y = 0;
	int wybor = 0;
	int czyKontynuowac = 1;
	int czyGrac = 1;
	int czyWczytano = 0;
	int iloscPolNaJedzenie;
	int dlugoscWeza;
	DWORD czasKlatkiX = 75;
	DWORD czasKlatkiY = 150;
	char nazwaPliku[DLUGOSC_NAZWY_PLIKU];
	char *menuGlowne[4];
	char *menuOpcji[4];
	struct elementWeza* glowaWeza = NULL;
	struct elementWeza* ogonWeza = NULL;
	menuGlowne[0] = "Graj";
	menuGlowne[1] = "Opcje";
	menuGlowne[2] = "WyjdŸ";
	menuGlowne[3] = "__                    __\n\\ \\        __        / /    __        _______\n \\ \\      /  \\      / /    /  \\      /____  /\n  \\ \\    / /\\ \\    / /    / /\\ \\      ___/ /__\n   \\ \\  / /  \\ \\  / /    / /__\\ \\    /__  ___/\n    \\ \\/ /    \\ \\/ /    / ______ \\     / /____\n     \\__/      \\__/    /_/      \\_\\_  /______/\n                                  \\_\\";
	menuOpcji[0] = "£atwy";
	menuOpcji[1] = "Œredni";
	menuOpcji[2] = "Trudny";
	menuOpcji[3] = "\n      Wybierz poziom trudnoœci\n";

	setlocale(LC_ALL, "polish_poland");
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0); // uwa¿aæ na to!
	while (czyKontynuowac)
	{
		ustawWielkoscKonsoli(75, 25); ustawWielkoscKonsoli(75, 25); system("COLOR 07");
		switch (menu(4, &menuGlowne[0]))
		{
		case 1: // Graj
			// custom level - poziom niestandardowy
			czyGrac = 1;
			do
			{
				kierunek = 2;
				// wczytywanie
				czyWczytano = wczytywaniePoziomu(&mapa, &X, &Y, &x, &y, czyGrac, &wybor, &nazwaPliku[0]);
				czyGrac = 1;
				if (czyWczytano)
				{
					system("cls");
					setCursor(0, 0);
					ustawWielkoscKonsoli(X, Y);
					ustawWielkoscKonsoli(X, Y); // empirycznie sprawdzone, ze nie ma bledu, gdy dwa razy jest wywolana

					glowaWeza = malloc(sizeof(struct elementWeza));
					ogonWeza = malloc(sizeof(struct elementWeza));
					glowaWeza->nastepny = NULL;	glowaWeza->poprzedni = ogonWeza;
					ogonWeza->poprzedni = NULL;	ogonWeza->nastepny = glowaWeza;
					glowaWeza->x = x; glowaWeza->y = y; // ustawienie dla kierunku==2 czyli prawo !!!
					setCursor(x, y); printf(">");
					ogonWeza->y = y;
					if (x > 0) ogonWeza->x = x - 1;
					else ogonWeza->x = X - 1;
					mapa[ogonWeza->y][ogonWeza->x] = 2;
					setCursor(ogonWeza->x, y); printf("-");
					losowanieJedzenia(mapa, X, Y);
					iloscPolNaJedzenie = wyznaczanieIlosciPolNaJedzenie(mapa, Y, X);
					wyswietlaniePoziomu(mapa, X, Y);
					dlugoscWeza = 2;
					//setCursor(0, 0);
					_getch(); while (_kbhit()) _getch();
				}
				else czyGrac = 0;
				while (czyGrac == 1)
				{
					if ((kierunek == 1) || (kierunek == 3)) Sleep(czasKlatkiY);
					else Sleep(czasKlatkiX);
					kierunekPoprzedni = kierunek;
					if (_kbhit())
					{
						while (_kbhit())
						{
							c1 = _getch();
							c2 = _getch();
							//printf("\nkod klawisza: %d, %d (%c, %c)", c1, c2, c1, c2);
						}
						//printf("\nOstatni znak to: %d, %d (%c, %c)", c1, c2, c1, c2);
						switch (c1)
						{
						case 27:
							czyGrac = 0;
							break;
						case 80:
						case 112:
							_getch(); while (_kbhit()) _getch();
							break;
						case 224:
							switch (c2)
							{
							case 72: // góra
								if (kierunek != 3) kierunek = 1;
								break;
							case 77: // prawo
								if (kierunek != 4) kierunek = 2;
								break;
							case 80: // dó³
								if (kierunek != 1) kierunek = 3;
								break;
							case 75: // lewo
								if (kierunek != 2) kierunek = 4;
								break;
							default:
								break;
							}
							break;
						default:
							break;
						}
					}
					ruchWeza(mapa, X, Y, kierunek, kierunekPoprzedni, &czyGrac, &dlugoscWeza, &glowaWeza, &ogonWeza, &iloscPolNaJedzenie);
				}
				if (czyWczytano) // zwalnianie pamieci
				{
					for (i = Y - 1; i >= 0; i--)
					{
						free(mapa[i]);
					}
					free(mapa);
					usuwanieWeza(&ogonWeza, &glowaWeza);
				}
			} while (czyGrac == 2);
			wybor = 0;
			break;
		case 2: // Opcje
			switch (menu(4, &menuOpcji[0]))
			{
			case 1:
				czasKlatkiX = (DWORD)200;
				czasKlatkiY = (DWORD)400;
				break;
			case 2:
				czasKlatkiX = (DWORD)100;
				czasKlatkiY = (DWORD)200;
				break;
			case 3:
				czasKlatkiX = (DWORD)75;
				czasKlatkiY = (DWORD)150;
				break;
			default:
				printf("B³¹d krytyczny");
				break;
			}
			printf("\nZatwierdzono zmiany!\n\n");
			system("pause");
			break;
		case 3: // Wyjdz
			printf("\n\nGabriel Maik\t2019\n\nSpecjalne podziêkowania dla £ysego za stworzenie poziomu Autostrada\n\n"); 
			czyKontynuowac = 0;
			system("pause");
			break;
		}
	}
	return 0;
}
//struct i struktury danych, dynamicznie alokowanie, pliki(zabezpieczenie), menu