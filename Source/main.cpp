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

struct stringPosition{
    int start;
    int end;
};

const int CHARS_PER_THREAD = 10;
string arnTranscription;
int arnPos, nThreads;
string aminoacids;

void separator();
string askDNASequence();
void* makeARNtranscription(void *args);

int main() {
    string adnInput = askDNASequence();
    nThreads = adnInput.length() / CHARS_PER_THREAD;
    pthread_t threads[nThreads];
    // INITIALIZES THE PTHREAD
    int threadPos = 0;
    stringPosition adnPos{};
    for(auto thread: threads){
        // 0 - 10 → 10 - 20 → 20 - 30
        adnPos = {CHARS_PER_THREAD*threadPos, CHARS_PER_THREAD*threadPos+1};
        pthread_create(&threads[threadPos++], nullptr, &makeARNtranscription, (void*) &adnPos);
    }
    for(auto thread: threads){
        pthread_join(thread, nullptr);
    }
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

void* makeARNtranscription(void *args){
    
}