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
	int weight;
	int * taken;
	int optimum;

	solution(int n){
		optimum = 0;
		weight = 0;
		items = n;
		taken = new int[items];
		for(int i = 0; i < items; i++)
			taken[i] = 0;
	}

	solution & operator=( solution & other ){
		if( this != &other ){
			items = other.items;
			weight = other.weight;
			optimum = other.optimum;
			//taken = new int[items];
			for(int i = 0; i < items; i++)
				taken[i] = other.taken[i];
		}
		return *this;
	}

	void print(){
		if(optimum >= 0){
			cout << endl << "Optimum value: " << optimum;
			cout << endl << "Optimum weight: " << weight << endl;
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

	bool ordered;
	int * order;
	double * ratios;
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
			ratios = new double[items];
			order = new int[items];

			for(int i = 0; i < items; i++){
				getline(fin, cadena);

				inicio = 0;
				final = cadena.find(" ", inicio);

				istringstream( cadena.substr(inicio, final) ) >> values[i];

				inicio = final + 1;
				final = cadena.length();

				istringstream( cadena.substr(inicio, final - inicio) ) >> weights[i];

				ratios[i] = ( (double) values[i] )/( (double) weights[i] );
			}

			// Getting order

			for(int i = 0; i < items; i++)
				order[i] = i;

			ordered = true;

			for(int i = 1; i < items; i++)
				if(ratios[i-1] < ratios[i])
					ordered = false;

			if(!ordered){

				// we use bubble technique

				for(int i = 0; i < items - 1; i++)
					for(int j = i + 1; j < items; j++)
						if(ratios[order[i]] < ratios[order[j]] ){
							int aux = order[i];
							order[i] = order[j];
							order[j] = aux;
						}

			}


			//Printing Ratios and Order (for Debuging)
			cout << endl << " * RATIOS " << endl;

			for(int i = 0; i < items; i++)
				cout << ratios[i] << " ";

			cout << endl << " * ORDER " << (ordered? "(esta ordenado)" : "(no esta ordenado)" ) <<endl;

			for(int i = 0; i < items; i++)
				cout << order[i] << " ";

			cout << endl;
			//Printing Ratios and Order (for Debuging)

			// Finishing order

			fin.close();

			cout << "Termino de cargar!";

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
				S.optimum += values[i];
				S.taken[i] = 1;
			} else
				S.taken[i] = 0;

		return S;
	}

	double Relaxation(solution S, int index){

		int TotalW = 0;
		double TotalV = 0;

		if(ordered){

			for(int i = 0; i <= index; i++)
				if(S.taken[i] == 1){
					if(TotalW + weights[i] < capacity){
						TotalW += weights[i];
						TotalV += values[i];
					}else{
						TotalV += ( (double) (capacity - TotalW)*values[i] )/( (double) weights[i]);
						TotalW = capacity;
						return TotalV;
					}
				}
			if(TotalW < capacity)
				for(int i = index + 1; i < items; i++)
					if(TotalW + weights[i] < capacity){
						TotalW += weights[i];
						TotalV += values[i];
					}else{
						TotalV += ( (double) (capacity - TotalW)*values[i] )/( (double) weights[i]);
						TotalW = capacity;
						return TotalV;
					}

		} else {

			for(int i = 0; i <= index; i++)
				if(S.taken[i] == 1){
					if(TotalW + weights[i] < capacity){
						TotalW += weights[i];
						TotalV += values[i];
					}else{
						TotalV += ( (double) (capacity - TotalW)*values[i] )/( (double) weights[i]);
						TotalW = capacity;
						return TotalV;
					}
				}

			if(TotalW < capacity){

				int *auxorder;

				auxorder = new int [items - index - 1];

				int auxindex = 0;
				for(int i = 0; i < items; i++){
					bool flag = true;
					for( int j = 0; j <= index; j++)
						if( order[i] == j ){
							flag = false;
							break;
						}
					if(flag){
						auxorder[auxindex++] = order[i];
					}
				}

				/*
				cout << " auxorder: ";
				for(int i = 0; i < items - index - 1; i++)
					cout << auxorder[i] << " ";
				*/

				for(int i = 0; i < items - index - 1; i++)
					if(TotalW + weights[auxorder[i]] < capacity){
						TotalW += weights[auxorder[i]];
						TotalV += values[auxorder[i]];
					}else{
						TotalV += ( (double) (capacity - TotalW)*values[auxorder[i]] )/( (double) weights[auxorder[i]]);
						TotalW = capacity;
						return TotalV;
					}
			}

		}

		return TotalV;

	}

	solution BandB(){

		solution S(items)/*Solution*/,  T(items)/*Traveling*/;

		// Depth-first-search

		cout << endl << endl << "* Depth-First-Search" << endl;

		int TotalWeight = 0;
		int TotalValue = 0;
		int BestValue = 0;
		int BestWeight = 0;

		double relaxation = 0.0;

		int level = 0;
		bool isGoingDown = true;
		bool isGoingLeft = true;

		string nada;

		int leaves = 0;

		while(level != -2){

			/*
			cout << endl << level << " " << (isGoingDown? "bajando " : "subiendo ") << (isGoingLeft? "izquierda " : "derecha ");
			T.lightprint();
			*/

			if(level > 0 && isGoingDown){
				relaxation = Relaxation(T,level-1);
				//cout << " BestValue: " << BestValue << " Relaxation: " << relaxation;
			}

			// It is going down
			if(isGoingDown == true){
				// It is going down and left
				if(isGoingLeft == true){

					if(level >= items - 1 ){ // Es una hoja

						T.taken[level] = 1;
						TotalWeight += weights[level];
						TotalValue += values[level];

						// Bounding
						if(TotalWeight <= capacity){

							if(TotalValue > BestValue){
								BestValue = TotalValue;
								BestWeight = TotalWeight;
								S = T;
							}

							// IMPRIME
							//cout << endl;
							//T.lightprint();
							//cout << " Peso total: "<< TotalWeight << " Valor total: " << TotalValue;
						}

						leaves++;
						isGoingDown = false;
						isGoingLeft = false;
						level--;

					} else {

						if(level >= 0){

							T.taken[level] = 1;
							TotalWeight += weights[level];
							TotalValue += values[level];

							// Bounding
							if(TotalWeight > capacity || ( level > 0 && relaxation < BestValue && BestValue > 0) ){
								isGoingDown = false;
								isGoingLeft = false;
								level--;
							} else
								level++;

						} else {
							level++;
						}

					}
				// It is going down and right
				} else {

					if(level >= items - 1 ){

						T.taken[level] = 0;
						TotalWeight -= weights[level];
						TotalValue -= values[level];

						if(TotalValue > BestValue){
							S = T;
							BestValue = TotalValue;
							BestWeight = TotalWeight;
						}

						// Imprime
						//cout << endl;
						//T.lightprint();
						//cout << " Peso total: "<< TotalWeight << " Valor total: " << TotalValue;

						// Es una hoja
						leaves++;
						isGoingDown = false;
						isGoingLeft = true;
						level--;

					} else {


						T.taken[level] = 0;
						TotalWeight -= weights[level];
						TotalValue -= values[level];

						// Bounding
						if(TotalWeight > capacity || ( level > 0 && relaxation < BestValue && BestValue > 0 ) ){
							isGoingDown = false;
							isGoingLeft = true;
							level--;
						} else {
							isGoingLeft = true;
							level++;
						}

					}

				}
			// It is going up
			} else {
				// It is going up and left
				if(isGoingLeft == true){

					if(level >= 0)
						if(T.taken[level] == 1){
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

		S.optimum = BestValue;
		S.weight = BestWeight;
		cout << endl << "Total de hojas " << leaves << " de  " << pow( 2.0, (double) items);

		return S;
	}

	solution BwithoutB(){ // Branch without Bounding

		solution S(items)/*Solution*/,  T(items)/*Traveling*/;

		// Depth-first-search

		cout << endl << endl << "* Depth-First-Search" << endl;

		int TotalWeight = 0;
		int TotalValue = 0;
		int BestValue = 0;
		int BestWeight = 0;


		int level = 0;
		bool isGoingDown = true;
		bool isGoingLeft = true;

		string nada;

		int leaves = 0;

		while(level != -2){

			/*
			cout << endl << level << " " << (isGoingDown? "bajando " : "subiendo ") << (isGoingLeft? "izquierda " : "derecha ");
			T.lightprint();
			cout << " BestValue: " << BestValue;
			*/

			// It is going down
			if(isGoingDown == true){
				// It is going down and left
				if(isGoingLeft == true){

					if(level >= items - 1 ){ // Es una hoja

						T.taken[level] = 1;
						TotalWeight += weights[level];
						TotalValue += values[level];

						if(TotalValue > BestValue && TotalWeight <= capacity){
							BestValue = TotalValue;
							BestWeight = TotalWeight;
							S = T;
						}

						// IMPRIME
						cout << endl;
						T.lightprint();
						cout << " Peso total: "<< TotalWeight << " Valor total: " << TotalValue;

						leaves++;
						isGoingDown = false;
						isGoingLeft = false;
						level--;

					} else {

						if(level >= 0){

							T.taken[level] = 1;
							TotalWeight += weights[level];
							TotalValue += values[level];

							level++;

						} else {
							level++;
						}

					}
				// It is going down and right
				} else {

					if(level >= items - 1 ){

						T.taken[level] = 0;
						TotalWeight -= weights[level];
						TotalValue -= values[level];

						if(TotalValue > BestValue && TotalWeight <= capacity){
							S = T;
							BestValue = TotalValue;
							BestWeight = TotalWeight;
						}

						// Imprime
						cout << endl;
						T.lightprint();
						cout << " Peso total: "<< TotalWeight << " Valor total: " << TotalValue;

						// Es una hoja
						leaves++;
						isGoingDown = false;
						isGoingLeft = true;
						level--;

					} else {


						T.taken[level] = 0;
						TotalWeight -= weights[level];
						TotalValue -= values[level];

						isGoingLeft = true;
						level++;


					}

				}
			// It is going up
			} else {
				// It is going up and left
				if(isGoingLeft == true){

					if(level >= 0)
						if(T.taken[level] == 1){
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

		S.optimum = BestValue;
		S.weight = BestWeight;
		cout << endl << "Total de hojas " << leaves << " de  " << pow( 2.0, (double) items);

		return S;
	}
};


void solve(int n, int i){

	string cadena = "ks_" + IntToString(n) + "_" + IntToString(i);
	knapsack Problem(cadena);


	cout << endl << endl << "* Problem values" << endl;
	Problem.print();

	solution Output = Problem.BandB();

	cout << endl << endl << "* Solution" << endl;
	Output.print();

}


int main() {

	solve(1,2);

	return 0;
}
