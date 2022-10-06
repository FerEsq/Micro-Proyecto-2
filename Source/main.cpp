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
#include <cmath>
using namespace std;

struct stringPosition{
    int start;
    int end;
};

int nThreads;
string adnInput, arnTranscription, aminoacids;

void separator();
string askDNASequence();
void* makeARNtranscription(void *args);

int main() {
    adnInput = askDNASequence();
    // DISTRIBUTES WORKLOAD
    nThreads = ceil(sqrt(adnInput.length()));
    pthread_t threads[nThreads];
    stringPosition positions[nThreads];
    // INITIALIZES THE arnTranscription VARIABLE
    for(char c : adnInput){ arnTranscription += "x"; };

    // PICKS POSITIONS FOR THE THREADS
    for(int i = 0; i < nThreads; i++){
        positions[i].start = i*nThreads;
        positions[i].end = (i+1)*nThreads;
        // LOADS THE LAST THREAD THE REMAINING CHARS
        if(i == nThreads-1){
            positions[i].end = adnInput.length();
        }
    }
    cout << adnInput << endl;
    for(int i = 0; i < nThreads; i++){
        pthread_create(&threads[i], nullptr, &makeARNtranscription, (void*) &positions[i]);
    }
    for(auto thread: threads){
        pthread_join(thread, nullptr);
    }
    cout << arnTranscription << endl;
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

void* makeARNtranscription(void *args) {
    auto *positions = (struct stringPosition*) args;
    int start = positions->start;
    int end = positions->end;
    for (int i = start; i < end; i++){
        switch (toupper(adnInput[i])) {
            case 'G':
                arnTranscription[i] = 'C';
                break;
            case 'C':
                arnTranscription[i] = 'G';
                break;
            case 'T':
                arnTranscription[i] = 'A';
                break;
            case 'A':
                arnTranscription[i] = 'U';
                break;
        }
    }
}