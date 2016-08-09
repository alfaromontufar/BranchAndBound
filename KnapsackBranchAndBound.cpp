//============================================================================
// Name        : KnapsackBranchAndBound.cpp
// Author      : Carlos A. Alfaro
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

string IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

class solution {
public:
	int items;
	int value;
	int weight;
	int * taken;
	bool isOptimum;

	solution(int n){
		isOptimum = false;
		value = 0;
		weight = 0;
		items = n;
		taken = new int[items];
		for(int i = 0; i < items; i++)
			taken[i] = 0;
	}

	void print(){
		if(value >= 0){
			cout << endl << "Value: " << value << endl;
			for(int i = 0; i < items; i++ )
				cout << taken[i] << " ";
		}

	}

	void lightprint(){
		for(int i = 0; i < items; i++ )
				cout << taken[i] << " ";
	}
};


class knapsack {
public:

	int items;
	int capacity;
	int * values;
	int * weights;

	knapsack(string File){

		File = "Escenarios\\" + File;

		ifstream fin(File.c_str()); ///////////////////////////////////////////////// ARCHIVO ENTRADA

		if (fin.is_open()) {

			cout << "Cargando archivo ... ";

			string cadena;

			///////////////////////////////////////////////////// Encabezados /////////////////////////////////////////////////////

			getline(fin, cadena);

			int inicio = 0, final = cadena.find(" ", inicio);

			istringstream( cadena.substr(inicio, final) ) >> items;

			inicio = final + 1;
			final = cadena.length();

			istringstream( cadena.substr(inicio, final - inicio) ) >> capacity;

			values = new int[items];
			weights = new int[items];

			for(int i = 0; i < items; i++){
				getline(fin, cadena);

				inicio = 0;
				final = cadena.find(" ", inicio);

				istringstream( cadena.substr(inicio, final) ) >> values[i];

				inicio = final + 1;
				final = cadena.length();

				istringstream( cadena.substr(inicio, final - inicio) ) >> weights[i];
			}

			fin.close();

			cout << "termino de cargar.";

		} else
			cout << "No se pudo abrir el archivo: " << File;
	}

	void print(){
		cout << endl << "Items: " << items;
		cout << endl << "Capacity: " << capacity;

		cout << endl << endl << "i value weight";

		for(int i = 0; i < items; i++)
			cout << endl << i << " - " << values[i] << " " << weights[i];
	}

	solution greedy(){

		solution S(items);

		for(int i = 0; i < items; i++)
			if(S.weight + weights[i] <= capacity){
				S.weight += weights[i];
				S.value += values[i];
				S.taken[i] = 1;
			} else
				S.taken[i] = 0;

		return S;
	}

	solution BandB(){

		solution S(items);

		// Depth-first-search

		cout << endl << endl << "* Depth-First-Search" << endl;

		int leaves = 0;
		int TotalWeight = 0;

		int level = 0;
		bool isGoingDown = true;
		bool isGoingLeft = true;

		string nada;

		 int steps = 0;

		while(level != -2){

			cout << endl << level << " " << (isGoingDown? "bajando " : "subiendo ") << (isGoingLeft? "izquierda " : "derecha ");
			S.lightprint();

			// It is going down
			if(isGoingDown == true){
				// It is going down and left
				if(isGoingLeft == true){

					if(level >= items - 1 ){

						S.taken[level] = 1;

						// Es una hoja
						steps++;
						isGoingDown = false;
						isGoingLeft = false;
						level--;
						// Imprime

						//if(TotalWeight + weights[level] <= capacity){

							TotalWeight += weights[level];
							cout << endl;
							S.lightprint();
							//cout << " peso total: "<< TotalWeight;
						//}

					} else {

						if(level >= 0){

							S.taken[level] = 1;
							level++;

							/*
							// Bounding
							if(TotalWeight + weights[level] <= capacity){
								S.taken[level] = 1;
								level++;
								TotalWeight += weights[level];

							} else {
								isGoingDown = false;
								isGoingLeft = false;
								level = level - 1;
							}
							*/

						} else {
							level++;
						}



					}
				// It is going down and right
				} else {

					if(level >= items - 1 ){

						S.taken[level] = 0;

						// Es una hoja
						steps++;
						isGoingDown = false;
						isGoingLeft = true;
						level--;
						// Imprime
						//if(TotalWeight <= capacity){
							cout << endl;
							S.lightprint();
							//cout << " peso total: "<< TotalWeight;
						//}

					} else {

						isGoingLeft = true;
						S.taken[level] = 0;
						level++;

					}

				}
			// It is going up
			} else {
				// It is going up and left
				if(isGoingLeft == true){

					if(level >= 0)
						if(S.taken[level] == 1){
							isGoingLeft = false;
							level--;
						} else
							level--;
					else{
						level--;

					}
				// It is going up and right
				} else {

					isGoingDown = true;
					level++;

				}

			}
		}

		cout << endl << "Total de hojas " << steps;

		return S;
	}
};


void solve(int n, int i){

	string cadena = "ks_" + IntToString(n) + "_" + IntToString(i);
	knapsack Problem(cadena);


	cout << endl << endl << "* Problem values" << endl;
	Problem.print();

	//solution Output = Problem.greedy();
	solution Output = Problem.BandB();

	cout << endl << endl << "* Solution" << endl;
	Output.print();

}


int main() {

	solve(4,0);

	return 0;
}
