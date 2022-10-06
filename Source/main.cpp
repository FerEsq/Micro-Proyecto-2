/*
	------------------------------------------------------------------
	UNIVERSIDAD DEL VALLE DE GUATEMALA
	Programacion de Microprocesadores
	Ciclo 2 - 2022

	main.cpp

	Fernanda Esquivel - 21542
	Francisco Castillo - 21562
	Andres Montoya - 21552
    Diego Lemus - 21469
	--------------------------------------------------------------------
*/

#include <iostream>
using namespace std;

const int CHARS_PER_THREAD = 10;
string arnTranscription;
int arnPos;
string aminoacids;

void separator();
string askDNASequence();


int main() {
    string adnInput = askDNASequence();
    pthread_t threads[adnInput.length() / CHARS_PER_THREAD];
    return 0;
}

void separator(){
    cout << "=============================================================" << endl;
}

string askDNASequence(){
    separator();
    string temp;
    cout << "Ingrese la cadena de ADN que desea traducir:" << endl;
    cin >> temp;
    separator();
    return temp;
}