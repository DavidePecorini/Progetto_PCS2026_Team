#pragma once
#include <vector> 
#include <set>
#include <map>
#include <iostream>
#include <algorithm>

// Creo una struttura per poter salvare le caratteristiche del tipo componente
// La scelta di struct sopra class è dovuta alla possibilità di accedere facilmente ai valori di ogni caratteristica
struct component
{
	char tipo = ' ';
	int id = 0;
	double valore = 0.0;
	int nodo1 = -1;
	int nodo2 = -1;
};

template<typename T> 
class unidirected_edge
{
	private:
		T nodo1;
		T nodo2;
		char _tipo;
		double _peso;
		
	public:
		// Costruttore di default
		unidirected_edge()
			: nodo1(T{}), nodo2(T{}), _tipo(""), _peso(1)
		{}
		
		// Aggiungo ad ogni arco l'attributo peso (settato di default a valore 1.0 se non specificato)
		unidirected_edge(const T& n1, const T& n2, char t, double p = 1.0)
			: _peso(p), _tipo(t)
		{
			if (t == 'V' && n1 > n2) {
				_peso = - _peso;
			}
			if (n1 < n2) {
				nodo1 = n1;
				nodo2 = n2;
			} else {
				nodo1 = n2;
				nodo2 = n1;
			}
		}
		
		// Definisco i meteodi from(),to() e peso()
		T from() const {return nodo1;}
		T to() const {return nodo2;}
		int peso() const {return _peso;}
		char tipo() const {return _tipo;}
		
		// Definisco il confronto tra archi
		bool operator<(const unidirected_edge<T>& other) const
		{
			if (nodo1 != other.nodo1)
			{
				// Un arco è minore di un altro se il nodo di partenza è minore
				return nodo1 < other.nodo1;
			} 
			// Se i nodi di partenza hanno stesso valore si confrontano quelli di arrivo
			return nodo2 < other.nodo2;
		}
		
		// Definisco l'uguaglianza tra archi
		bool operator==(const unidirected_edge<T>& other) const
		{
			// Se nodi di partenza e arrivo sono uguali gli archi sono uguali
			return nodo1 == other.nodo1 && nodo2 == other.nodo2;
		}
		
};

// Definisco l'operatore di stream per la classe unidirected_edge, l'arco sarà printato come (a,b)
template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& arco)
		{
			os << "(" << arco.from() << "," << arco.to() << ") : peso = " << arco.peso() << ", tipo = " << arco.tipo() << std::endl; 
			return os;
		}

template<typename T>
class unidirected_graph
{
	// La classe grafo contiene diverse strutture atte alla chiamata dei diversi metodi
	private: 
		std::map<T, std::set<T>> lista_adiacenza; //per add_edge()
		std::set<T> nodi; //per all_nodes()
		std::set<unidirected_edge<T>> archi; // per all_edges(), edge_at() e edge_number()
	
	public:
		// Costruttore di default
		unidirected_graph()
			: lista_adiacenza(), nodi(), archi()
		{}
		
		// Costruttore di copia
		unidirected_graph(const unidirected_graph& other)
			: lista_adiacenza(other.lista_adiacenza),
			  nodi(other.nodi),
			  archi(other.archi)
		{}
		
		// Uso la lista di adiacenza per trovare i nodi vicini
		const std::set<T>& neighbours(const T& nodo) const
		{
			return lista_adiacenza.at(nodo);
		}
		
		// Definisco l'operazione di aggiunta di un arco, che deve aggiornare tutte le mie strutture
		void add_edge(const unidirected_edge<T>& arco) 
		{
			// Il metodo insert, oltre ad aggiungere un elemento al set,  restituisce una coppia 
			// (iteratore, booleano), che dipendeno dalla presenza o meno dell'arco, il booleano dà
			// true se l'arco è stato inserito e l'iteratore dà l'indirizzo della cella di memoria
			auto controllo = archi.insert(arco);
			
			// Controllo se l'arco è stato inserito
			if (controllo.second) {
				
				// Aggiorno i valori delle altre strutture
				lista_adiacenza[arco.from()].insert(arco.to());
				lista_adiacenza[arco.to()].insert(arco.from());
				nodi.insert(arco.from());
				nodi.insert(arco.to());
				
			}
			// Se l'insert su archi non è passata, le altre operazioni non vengono neanche 
			// eseguite
		}
		
		// Per il metodo all_edges() semplicemente restituisco il riferimento al mio set archi (cambiamento rispetto al codice precedente)
		const std::set<unidirected_edge<T>>& all_edges() const
		{
			return archi;
		}
		
		// Allo stesso modo restituisco il riferimento al set nodi in all_nodes() (cambiamento rispetto al codice precedente)
		const std::set<T>& all_nodes() const
		{
			return nodi;
		}
		
		// Sia per edge_number che per edge_at uso la proprietà di set di essere implementato come
		// un albero rosso-nero, che quindi ad ogni inserimento mantiene l'ordinamento secondo 
		// l'operatore <, che nel nostro caso è stato definito anche per unidirected_edge
		
		int edge_number(const unidirected_edge<T>& arco) const
		{
			// Inizializzo un iteratore che punta all'arco di cui voglio restituire la posizione
			auto indirizzo_arco = archi.find(arco);
			
			// Se l'arco non viene trovato returno un indice impossibile ovvero -1
			if (indirizzo_arco == archi.end()) {
				std::cerr << "ERRORE: l'arco " << arco << " non è presente nel grafo\n";
				return -1;
			}
			
			// Faccio scorrere l'iteratore sugli elementi del set fino a che non trovo l'arco,
			// di cui so con certezza la presenza, e aggiorno un indice finchè non lo trovo
			int indice = 0;
			for (auto it = archi.begin(); it != indirizzo_arco; it++) {
				indice++;
			}
			return indice;
		}
		
		// Passo l'indice come size_t al posto di int perchè va confrontato con archi.size()
		unidirected_edge<T> edge_at(size_t indice) const
		{
			// Se l'indice è maggiore del numero di archi returno un arco vuoto
			if (indice >= archi.size()) {
				std::cerr << "ERRORE indice " << indice << ": l'indice fornito è maggiore del numero di archi\n";
				return unidirected_edge<T> ();
			}
			// Inizializzo un iteratore sul set
			auto indirizzo_arco = archi.begin();
			
			// Aggiorno l'iteratore all'elemento successivo per tante volte quante il valore dell'indice
			for (size_t i = 0; i < indice; i++) {
				++indirizzo_arco;
			}
			// Returno ciò a cui punto l'iteratore così aggiornato, ovvero l'arco desiderato
			return *indirizzo_arco;
		}

		// Ridefinisco l'operatore - per calcolare la differenza di grafi
		unidirected_graph operator-(const unidirected_graph& other) const
		{
			// Inizializzo il grafo risultante
			unidirected_graph ris;
			
			// Per ogni arco presente nel minuendo controllo se è presente nel sottraendo,
			// se non lo è allora lo aggiungo al grafo risultante
			for (const unidirected_edge<T>& arco : archi) 
			{
				if (!other.archi.contains(arco)) 
				{
					ris.add_edge(arco);
				}
			}
			return ris;
		}
};