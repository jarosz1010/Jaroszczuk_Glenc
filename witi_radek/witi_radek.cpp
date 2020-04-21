// Witi.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//


#include "pch.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;

int koszt1, koszt2, koszt3;
int cz2;
struct problem {
	int p;
	int w;
	int d;
	int lp;

};
std::vector < problem > tab;

bool cmp_r(const problem& a, const problem& b) //wykorzystywane do next_permutation
{

	return a.lp < b.lp;
}

ostream& operator<< (ostream& wyjscie, const problem& pom) {
	return wyjscie << "Numer:" << pom.lp << " p:" << pom.p << " w:" << pom.w << " d:" << pom.d << endl;
}

bool d_sort(const problem& a, const problem& b)
{
	// smallest comes first
	return a.d < b.d;
}
/////////////////////////////////////////////////////////////////
void Wczytaj(string Nazwapliku) {
	string linia;
	fstream plik;
	int liczba;

	plik.open(Nazwapliku, ios::in);
	if (plik.good() == true)
	{
		plik >> liczba;

		string pomoc;
		plik >> pomoc;
		for (int i = 0; i < liczba; i++)
		{
			problem tmp;
			plik >> tmp.p >> tmp.w >> tmp.d;
			tmp.lp = i;
			tab.push_back(tmp);

		}
		plik.close();
	}
}
////////////////////////////////////////////////////////////////////////

int funkcja_celu(vector<problem> tabela) {
	int koszt = 0;
	int czas = 0;
	for (int i = 0; i < tabela.size(); i++) {
		czas = czas + tabela[i].p;
		if (czas > tabela[i].d) koszt = koszt + ((czas - tabela[i].d) * tabela[i].w);
	}
	return koszt;
}




void permutuj(vector<problem>& tabela, int poz = 0) {

	if (poz == tabela.size()) {
		cz2 = funkcja_celu(tabela);

		koszt3 = min(koszt3, cz2);

	}
	else {
		for (int i = poz; i < tabela.size(); i++) {
			swap(tabela[i], tabela[poz]);
			permutuj(tabela, poz + 1);
			swap(tabela[i], tabela[poz]);
		}
	}

}

void permutuj2(vector<problem>& tabela)
{
	do {
		cz2 = funkcja_celu(tabela);
		koszt3 = min(koszt3, cz2);

	} while (next_permutation(tabela.begin(), tabela.end(), cmp_r));
}






/////////////////////////////////////////////

int kara(problem wyraz, int koniec)//wylicza wartosc kary dla pracy i jej czasu zakonczenia
{
	int kara = (koniec - wyraz.d) * wyraz.w;
	if (kara > 0)
		return kara;
	else
		return 0;
}


int witi(const vector<problem> &tabela)
{

	int numer_kombinacji = (1 << tabela.size()); //ilośc kombinacji dla n elementów (2^n)

	int* kombinacje = new int[numer_kombinacji]; //tablica kar dla kolejnych kombinacji


	kombinacje[0] = 0;


	for (int i = 1; i < numer_kombinacji; i++)
	{
		int koniec = 0;
		kombinacje[i] = 1000000; // duza wartosc do poprawnego dzialania funkcji mimimum

		for (int j = 0; j < tabela.size(); j++)
			if (i & (1 << j)) //koniunkcja - zadanie j(przesunieta jedynka o j miejsc) nalezy do kombinacji i
				koniec += tabela[j].p; //dodaj je - cmax?
		//cout << "koniec " << koniec << endl;
		for (int j = 0; j < tabela.size(); j++) //liczenie kary
			if (i & (1 << j)) //koniunkcja - zadanie j nalezy do kombinacji i
			{

				int tmp = kombinacje[i & (~(1 << j))] + kara(tabela[j], koniec); //kobminacja bez j + zadanie j jako ostatnie

				if (kombinacje[i] > tmp) //wybor minimum
				{
					kombinacje[i] = tmp;
				}

			}

	}
	return kombinacje[numer_kombinacji - 1];
}





/////////////////////////////////////////
int main()
{
	char wybor;

	Wczytaj("data20.txt");

	cout << "Wybierz algorytm" << endl;//menu, bo zmieniamy tablice globalnie
	cout << "-----------------" << endl;
	cout << "1. 1234 " << endl;
	cout << "2. sortD" << endl;
	cout << "3. Przeglad - rekurencyjnie" << endl;
	cout << "4. Przeglad - next_permutation" << endl;
	cout << "5. PD - iteracyjnie" << endl;
	cout << "6. PD - rekurencyjnie" << endl;



	cin >> wybor;

	switch (wybor)
	{
	case '1':
	{	koszt1 = funkcja_celu(tab);
	cout << "1234:" << koszt1 << endl; }
	break;

	case '2':
	{//koszt1 = funkcja_celu(tab);//???????????????????????????????????????
		auto start2 = std::chrono::system_clock::now();
		sort(tab.begin(), tab.end(), d_sort);
		koszt2 = funkcja_celu(tab);
		auto end2 = std::chrono::system_clock::now();
		std::chrono::duration<double> cz2 = end2 - start2;
		cout << "Czas wykonywania sortD " << cz2.count() << endl;
		cout << "sortD:" << koszt2 << endl;
	}
	break;

	case '3':
	{auto start3 = std::chrono::system_clock::now();
	koszt3 = funkcja_celu(tab);
	permutuj(tab, 0);
	auto end3 = std::chrono::system_clock::now();
	std::chrono::duration<double> cz3 = end3 - start3;
	cout << "Czas wykonywania opt: " << cz3.count() << endl;
	cout << "opt:" << koszt3 << endl;
	}
	break;

	case '4':
	{	auto start4 = std::chrono::system_clock::now();
	koszt3 = funkcja_celu(tab);
	permutuj2(tab);
	auto end4 = std::chrono::system_clock::now();
	std::chrono::duration<double> cz4 = end4 - start4;
	cout << "Czas wykonywania opt: " << cz4.count() << endl;
	cout << "opt:" << koszt3 << endl;
	}	break;

	case '5':
	{auto start5 = std::chrono::system_clock::now();

	koszt3 = witi(tab);
	auto end5 = std::chrono::system_clock::now();
	std::chrono::duration<double> cz5 = end5 - start5;
	cout << "Czas wykonywania opt: " << cz5.count() << endl;
	cout << "opt:" << koszt3 << endl;
	}	break;

	case '6':
		cout << "jeszcze nie ma" << endl;
		break;
	default: cout << "Nie ma takiej opcji w menu!";
	}







}

