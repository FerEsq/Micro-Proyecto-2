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

    Prueba: ATGATCTCGTAA
	--------------------------------------------------------------------
*/

#include <iostream>
#include <cmath>
using namespace std;

struct stringPosition{
    int start;
    int end;
};

int nThreads, startPosition, nCodons = -1;
string adnInput, arnTranscription, aminoacids;

void separator();
void* makeARNtranscription(void *args);
void* countCodons(void *args);
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
    cout << "ADN input: " << adnInput << endl;
    for(int i = 0; i < nThreads; i++){
        pthread_create(&threads[i], nullptr, &makeARNtranscription, (void*) &positions[i]);
    }
    for(auto thread: threads){
        pthread_join(thread, nullptr);
    }
    separator();
    cout << "ARN transcription: " << arnTranscription << endl;
//     FIND THE START POSITION IN THE TRANSCRIPTION (AUG)
    startPosition = findStartPosition();
//     SEPARATES THE ARN TRANSCRIPT INTO CODONS
    int sharedPos = startPosition;


//    for(auto thread : threads){
//        pthread_create(&thread, nullptr, )
//    }

//     1. Contar Codones (void function) que suma 1 cada 3 chars de arnTranscription desde start position hasta que sea >= que el arnTranscription.length()  == /3 y redondear?)
//     2. Crear un array con esa cantidad de espacios y coloocar las 3 letras en cada posicion (void function) llevar un contador de posicion compartido
//     3. Traducir cada espacio del array hasta reccibir la señal de stop o que haya traducido tod0 (void function) llevar un contador de posicion compartido

    return 0;
}

void separator(){
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
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

void* countCodons(void *args){
    auto *positions = (struct stringPosition*) args;
    int start = positions->start;
    int end = positions->end;
    int pos = 0;

    nCodons = (arnTranscription.length() - start)  / 3;
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