/*
 * ENCABEZADO
 *
 *
 */

#include <iostream>
using namespace std;

const int CHARS_PER_THREAD = 10;

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