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

    Prueba: TACTAGAGCATT
	--------------------------------------------------------------------
*/

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct stringPosition {
    int start;
    int end;
};
struct codonPosition {
    int start;
    int id;
};
struct codonInfo {
    int pos{};
    string codon;
    string protein;
};

int nThreads, startPosition = -1, endPosition = -1;
string adnInput, arnTranscription;
vector<codonInfo> codons;
pthread_mutex_t sharedPositionMutex;

void separator();

void *makeARNtranscription(void *args);

void *makeARNtranslation(void *args);

void *createCodons(void *args);

string askDNASequence();

int findStartPosition();

int findEndPosition();

int main() {
    adnInput = askDNASequence();
    // DISTRIBUTES WORKLOAD
    nThreads = ceil(sqrt(adnInput.length()));
    pthread_t threads[nThreads];
    stringPosition positions[nThreads];
    codonPosition positions_codons[nThreads];
    // INITIALIZES THE arnTranscription VARIABLE
    for (char c: adnInput) { arnTranscription += "x"; };

    // PICKS POSITIONS FOR THE THREADS
    for (int i = 0; i < nThreads; i++) {
        positions[i].start = i * nThreads;
        positions[i].end = (i + 1) * nThreads;
        // LOADS THE LAST THREAD THE REMAINING CHARS
        if (i == nThreads - 1) {
            positions[i].end = adnInput.length();
        }
    }
    cout << "ADN input: " << adnInput << endl;

    for (int i = 0; i < nThreads; i++) {
        pthread_create(&threads[i], nullptr, &makeARNtranscription, (void *) &positions[i]);
    }
    for (auto thread: threads) {
        pthread_join(thread, nullptr);
    }

    separator();
    cout << "ARN transcription: " << arnTranscription << endl;
    separator();

    // FINDS THE STARTING CODON (AUG) POSITION AND THE STOP CODON FOR THEM TO BE SPLITTED
    startPosition = findStartPosition();
    endPosition = findEndPosition();

    // CREATES nThreads FOR THE CODONS
    int nThreadsC = ceil((endPosition - startPosition) / 3);
    pthread_t threadsC[nThreadsC];
    // ASSIGNS THE POSITIONS OF THE CODONS FOR MANIPULATION IN THREADS
    for (int i = 0; i < nThreadsC; i++) {
        positions_codons[i].start = i * 3 + startPosition;
        positions_codons[i].id = i;
    }
    // SPLITS THE CODONS IN PARALLEL
    for (int i = 0; i < nThreadsC; i++) {
        pthread_create(&threadsC[i], nullptr, &createCodons, (void *) &positions_codons[i]);
    }
    for (auto thread: threadsC) {
        pthread_join(thread, nullptr);
    }

    // PRINTS THE SPLIT CODONS IN ORDER
    cout << "ARN ha sido separado en los codones: ";
    for (int i = 0; i < codons.size(); i++) {
        for (auto &codon: codons) {
            if (codon.pos == i) {
                cout << codon.codon << " ";
            }
        }
    }
    cout << endl;
    separator();
    // STARTS THE TRANSLATION IN PARALLEL
    pthread_mutex_init(&sharedPositionMutex, nullptr);
    for (int i = 0; i < nThreadsC; i++) {
        pthread_create(&threadsC[i], nullptr, &makeARNtranslation, (void *) &codons.at(i));
    }
    for (auto thread: threadsC) {
        pthread_join(thread, nullptr);
    }
    cout << "Las proteinas producidas por el ADN son: " << endl;
    for (int i = 0; i < codons.size(); i++) {
        for (auto &codon: codons) {
            if (codon.pos == i) {
                cout << codon.protein << endl;
            }
        }
    }
    pthread_mutex_destroy(&sharedPositionMutex);
    return 0;
}

void separator() {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

string askDNASequence() {
    separator();
    string temp;
    cout << "Ingrese la cadena de ADN que desea traducir:" << endl;
    cin >> temp;
    separator();
    return temp;
}

void *makeARNtranscription(void *args) {
    auto *positions = (struct stringPosition *) args;
    int start = positions->start;
    int end = positions->end;
    for (int i = start; i < end; i++) {
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
    return nullptr;
}

string makeTranslation(string codon) {
    switch (codon[0]) {
        case 'U':
            switch (codon[1]) {
                case 'U':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Phe";
                    } else if (codon[2] == 'A' or codon[2] == 'G') {
                        return "Leu";
                    }
                    break;
                case 'C':
                    return "Ser";
                case 'A':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Tyr";
                    } else if (codon[2] == 'A' or codon[2] == 'G') {
                        return "STOP";
                    }
                    break;
                case 'G':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Cys";
                    } else if (codon[2] == 'A') {
                        return "STOP";
                    } else if (codon[2] == 'G') {
                        return "Try";
                    }
                    break;
            }
            break;
        case 'C':
            switch (codon[1]) {
                case 'U':
                    return "Leu";
                case 'C':
                    return "Pro";
                case 'A':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Gis";
                    } else if (codon[2] == 'A' or codon[2] == 'G') {
                        return "Gln";
                    }
                    break;
                case 'G':
                    return "Arg";
            }
            break;
        case 'A':
            switch (codon[1]) {
                case 'U':
                    if (codon[2] == 'U' or codon[2] == 'C' or codon[2] == 'A') {
                        return "Iso";
                    } else if (codon[2] == 'G') {
                        return "Met";
                    }
                    break;
                case 'C':
                    return "Thr";
                case 'A':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Asn";
                    } else if (codon[2] == 'A' or codon[2] == 'G') {
                        return "Lys";
                    }
                    break;
                case 'G':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Ser";
                    } else if (codon[2] == 'A' or codon[2] == 'G') {
                        return "Arg";
                    }
                    break;
            }
            break;
        case 'G':
            switch (codon[1]) {
                case 'U':
                    return "Val";
                case 'C':
                    return "Ala";
                case 'A':
                    if (codon[2] == 'U' or codon[2] == 'C') {
                        return "Asp";
                    } else if (codon[2] == 'A' or codon[2] == 'G') {
                        return "Glu";
                    }
                    break;
                case 'G':
                    return "Gly";
            }
            break;

    }
    return "";
}

void *makeARNtranslation(void *args) {
    auto *localCodon = (struct codonInfo *) args;
    pthread_mutex_lock(&sharedPositionMutex);
    // HACER AQUI LA TRADUCCION
    localCodon->protein = makeTranslation(localCodon->codon);
    //
    pthread_mutex_unlock(&sharedPositionMutex);
    pthread_exit(nullptr);
}


void *createCodons(void *args) {
    auto *positions = (struct codonPosition *) args;
    int start = positions->start;
    int i = positions->id;
    string temp = arnTranscription.substr(start, 3);
    codonInfo codon;
    codon.codon = temp;
    codon.pos = i;
    codons.push_back(codon);
}

// THIS PART MUST BE SEQUENTIAL BECAUSE IF THE STRING IS SPLIT, THERE'S A CHANCE THAT THE AUG CODON GETS CUT
/**
 *
 * @return -1 if AUG isn't found
 */
int findStartPosition() {
    string temp;
    for (int i = 0; i < arnTranscription.length() - 2; i++) {
        temp = arnTranscription[i];
        temp += arnTranscription[i + 1];
        temp += arnTranscription[i + 2];
        if (temp == "AUG") {
            return i;
        }
    }
    //cout << "res"<<res<<endl;
    return -1;
}

/**
 * Finds the end codon of the chain
 * @return -1 if a STOP codon isn't found
 */
int findEndPosition() {
    string temp;
    for (int i = startPosition; i < arnTranscription.length(); i += 3) {
        temp = arnTranscription[i];
        temp += arnTranscription[i + 1];
        temp += arnTranscription[i + 2];
        cout << "temp: " << temp << endl;
        if (temp == "UAA" or temp == "UAG" or temp == "UGA") {
            return i + 2;
        }
    }
    return -1;
}