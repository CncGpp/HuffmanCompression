#include <iostream>
#include "Huffman.h"
#include <chrono>

using namespace std;

string extractExtension(string& path);
void ManualEncodeORdecode(Huffman& h);
void encodeORdecode(int argc, char* argv[], Huffman& h);



int main(int argc, char* argv[]) {
	cout << "\t\t          _  _             __       _                      " <<endl;
	cout << "\t\t  |_|   _|__|___  _ __    /   _ __ |_) __ _  _  _  o  _ __ " << "\tGiuseppe Cianci Pio" <<endl;
	cout << "\t\t  | ||_| |  | |||(_|| |   \\__(_)||||   | (/__> _>  | (_)| |" << "\t0124001064" << endl;

	cout << endl;
	auto start = chrono::steady_clock::now();

	Huffman h(1024);

	if(argc > 1)
		encodeORdecode( argc,  argv,  h);
	else
		ManualEncodeORdecode(h);

	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << "Operazione completata, tempo trascorso: ";
	cout << chrono::duration <double, nano> (diff).count() /1000000000<< " sec" << endl;

	cout << "\n\nPremere un invio per uscire..";
	cin.ignore();
	cin.get();



	return 0;
}


void encodeORdecode(int argc, char* argv[], Huffman& h){

	for(int i = 1; i < argc; i++ ){
		string extrension = argv[i];
		extrension = extractExtension(extrension);

		if(extrension == h.getCOMPRESS_EXTRENSION()){
			cout << "Decompressione del file: " << argv[i] << " in corso..." << endl;
			h.decompress(argv[i]);
			remove(argv[i]);
			return;
		}

		if(extrension != ""){
			cout << "Compressione del file: " << argv[i] << " in corso..." << endl;
			h.compress(argv[i]);
			cout << "L'entropia del file e': " << h.getEntropy(argv[i]) <<endl;
			remove(argv[i]);
			return;
		}

		cerr << "IL FILE NON HA ESTENZIONE" << endl;
	}

}

void ManualEncodeORdecode(Huffman& h){
	cout << "EFFETTUA UNA SCELTA:" << endl;
	cout << " [0] Comprimere \n [1] Decomprimere" << endl;
	cout << "Si vuole comprimere o decomprimere un file?  ";
	char c=0;
	string path;
	cin.get(c);
	cout << "Inserisci il nome del file: ";
	cin >> path;
	switch(c){
	case '0': h.compress(path);
			  cout << "L'entropia del file e': " << h.getEntropy(path) <<endl;
		break;
	case '1': h.decompress(path);
		break;
	default:
		return;
	}

	remove(path.c_str());
}

string extractExtension(string& path){
/**Funzione che dato il path di un file, restituisce l'estensione.*/
	size_t lastdot = path.find_last_of(".");
	if (lastdot == string::npos) return "";			 //npos sta ad indicare "nessuna occorrenza"

	//DA DOPO IL PUNTO FINO ALLA FINE
	return path.substr(lastdot+1);
}






