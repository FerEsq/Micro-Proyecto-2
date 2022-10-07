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

int nThreads, startPosition = -1;
string adnInput, arnTranscription, aminoacids;

void separator();
void* makeARNtranscription(void *args);
string askDNASequence();
int findStartPosition();

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
    // FIND THE START POSITION IN THE TRANSCRIPTION (AUG)
    startPosition = findStartPosition();
//     SEPARATES THE ARN TRANSCRIPT INTO CODONS
//     a partir de la posición inicial, los hilos deben sumar de 3 en 3 su posición hasta
//     1. transcribir toda la cadena e indicar que no encontro un Stop
//     2. encontrar un stop y mandar una señal

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

// THIS PART MUST BE SEQUENTIAL BECAUSE IF THE STRING IS SPLIT, THERE'S A CHANCE THAT THE AUG CODON GETS CUT
/**
 *
 * @return -1 if AUG isn't found
 */
int findStartPosition(){
    string temp;
    for(int i = 0; i < arnTranscription.length() - 2; i++){
        temp = arnTranscription[i];
        temp += arnTranscription[i+1];
        temp += arnTranscription[i+2];
        if(temp == "AUG"){
            return i;
        }
    }
    return -1;
}