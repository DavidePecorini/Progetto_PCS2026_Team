#pragma once
#include "unidirected_graph.hpp"
#include "LifoFifo.hpp"
#include <set>
#include <queue>
#include <limits>
#include <optional>


template<typename T> 

unidirected_graph<T> graph_visit(const unidirected_graph<T>& G, const T& nodo_sorgente, auto& contenitore)
{
	unidirected_graph<T> grafo_ris;
	
	// Usiamo un set al posto di un vettore di booleani poichè non abbiamo garanzie sul tipo di T e 
	// quindi non è detto che l'accesso nodi_visitati[i] = true sia lecito
	std::set<T> nodi_visitati;
	
	// Controllo che il nodo fornito sia presente nel grafo altrimenti restituisco grafo vuoto
	auto nodi = G.all_nodes();
	if (!nodi.count(nodo_sorgente)) {
		return grafo_ris;
	}
	// Aggiungo il nodo sorgente al contenitore e anche al set dei nodi già visitati
	contenitore.put(nodo_sorgente);
	nodi_visitati.insert(nodo_sorgente);
	
	while(!contenitore.empty()) {
		T nodo = contenitore.get();
		
		// Il metodo neighbours restituisce direttamente i nodi vicini di nodo
		for (const T& nodo_vicino : G.neighbours(nodo)) {
			
			// Controllo che il nodo vicino non sia già stato visitato
			if (!nodi_visitati.count(nodo_vicino)) {
				
				// Aggiungo il nodo al set dei nodi già visitati
				nodi_visitati.insert(nodo_vicino);
				
				// Aggiungo l'arco all'albero risultante dalla visita (e quindi aggiorno tutte
				// le strutture associate
				grafo_ris.add_edge(unidirected_edge<T> (nodo, nodo_vicino));
				
				// Aggiungo il nodo vicino nel contenitore
				// In base al contenitore il nodo verrà messo all'inizio o alla fine
				contenitore.put(nodo_vicino);
			}
		}
	}
	return grafo_ris;
}

template<typename T> 

// Creo una funzione ausiliaria poichè al fine di fare la chiamata ricorsiva ho bisogno
// di passare come parametri anche il set di nodi_visitati e il grafo risultante
void recursive_dfs_struct(const unidirected_graph<T>& G, 
						  const T& nodo, 
						  std::set<T>& nodi_visitati,
						  unidirected_graph<T>& grafo_ris)
{
	
	nodi_visitati.insert(nodo);
	
	for (const T& nodo_vicino : G.neighbours(nodo)) {
		
		// Controllo che il nodo non sia già stato visitato
		if (!nodi_visitati.count(nodo_vicino)) {
			
			// Aggiungo l'arco all'albero risultante dalla visita (e quindi aggiorno tutte
			// le strutture associate
			grafo_ris.add_edge(unidirected_edge<T>(nodo, nodo_vicino));
						
			// Ora faccio la vera e propria chiamata ricorsiva sul nodo vicino
			// Il metodo ricorsivo è per forza di tipo DFS poichè usando lo stack implicito 
			// che è LIFO per natura: l'ultimo elemento è per forza il primo ad essere tolto
			
			recursive_dfs_struct(G, nodo_vicino, nodi_visitati, grafo_ris);
			
			// In questo caso non è presente nodi_visitati.insert(nodo_vicino); poichè l'inserimento
			// di nodo vicino allo stack è la prima cosa fatta dalla chiamata ricorsiva
		}
	}
}


template<typename T> 

unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& G, const T& nodo_sorgente) 
{
	unidirected_graph<T> grafo_ris;
	
	std::set<T> nodi_visitati;
	
	auto nodi = G.all_nodes();
	if (!nodi.count(nodo_sorgente)) {
		return grafo_ris;
	}
	
	recursive_dfs_struct(G, nodo_sorgente, nodi_visitati, grafo_ris);
	
	return grafo_ris;
}
	
template<typename T> 

// Nel nostro caso l'algoritmo di Djikstra restituisce una coppia di mappe: la prima di forma nodo - cammino minimo per raggiungerlo dal nodo sorgente;
// la seconda restituisce per ogni nodo qual è il nodo immediatamente precedente nel cammino minimo (se ce n'è uno)
std::pair<std::map<T, int>, std::map<T, std::optional<T>>> djikstra(const unidirected_graph<T>& G, const T& nodo_sorgente) 
{
	// Inizializzo la mia coppia di mappe
	std::map<T, int> distanze;
	std::map<T,std::optional<T>> predecessori;
	
	// Controllo che il nodo sorgente sia parte del grafo, in caso contrario restituisco le mappe vuote
	auto nodi = G.all_nodes();
	if (!nodi.count(nodo_sorgente)) {
		return {distanze, predecessori};
	}
	
	// Setto per ogni nodo il cammino minimo a lunghezza massima e il predecessore a nullopt
	for (const T& nodo : G.all_nodes()) {
		distanze[nodo] = std::numeric_limits<int>::max();
		predecessori[nodo] = std::nullopt; //non metto -1 come nello pseudocodice perchè se il nodo non è di tipo int non funziona
	}
	// Setto ora cammino minimo e predecessore del nodo sorgente come nello pseudocodice
	predecessori[nodo_sorgente] = nodo_sorgente;
	distanze[nodo_sorgente] = 0;
	
	// Inizializzo la mia struttura priority queue
	
	// Creo un alias per quello che sarà l'elemento contenuto nella priority queue
	// L'elemento della pq sarà una coppia distanza accumulata nodo
	using Coppia = std::pair<int, T>;
	
	// uso std::greater il modo che il minore va in cima (processo per prima la distanza minima)
	std::priority_queue<Coppia, std::vector<Coppia>, std::greater<Coppia>> pq;
	
	// Inserisco il mio nodo_sorgente a distanza 0 nella pq
	pq.push({0, nodo_sorgente});
	
	while (!pq.empty()) {
		// estraggo l'elemento in cima alla pq (quello con distanza corrente minore, ovvero il più vicino al nodo_sorgente) 
		// e lo rimuovo per evitare di processarlo di nuovo
		auto [distanza_corrente, nodo_corrente] = pq.top();
		pq.pop();
		
		// scorro i vicini di nodo_corrente e recupero gli archi che li collegano con relativo peso
		for (const T& nodo_vicino : G.neighbours(nodo_corrente)) {
			// Creo l'arco tra nodo_vicino e nodo_corrente
			unidirected_edge<T> arco_corrente(nodo_corrente, nodo_vicino);
			// Trovo la posizione di arco_corrente nel mio set di archi del grafo con un iteratore
			auto it = G.all_edges().find(arco_corrente);
			// lo dereferenzio e estraggo dall'arco nel set il peso 
			const auto& arco_trovato = *it;
			int peso = arco_trovato.peso();
			
			// Controllo se la distanza memorizzata per arrivare a nodo_vicino è maggiore di quella trovata passando da nodo_corrente
			// se passare da nodo_corrente conviene aggiorno le mappe e passo questa nuova combinazione alla pq
			if (distanze[nodo_vicino] > distanze[nodo_corrente] + peso) {
				distanze[nodo_vicino] = distanze[nodo_corrente] + peso;
				predecessori[nodo_vicino] = nodo_corrente;
				pq.push({distanze[nodo_vicino], nodo_vicino});
			}
		}
	}
	return {distanze, predecessori};
}

// Aggiungo una funzione che crea l'albero di visita a partire dalla mappa predecessori ottenuta da Djikstra
template <typename T> 

unidirected_graph<T> albero_predecessori(const std::map<T, std::optional<T>>& predecessori, const T& nodo_sorgente) 
{
	unidirected_graph<T> grafo_ris;
	
	for (const auto& [nodo, pred] : predecessori) {
		// Salto il nodo_sorgente e i nodi che non hanno predecessori
		// has_value() è una funzione booleana che restituisce true se dentro optional è presente un valore
		if (nodo != nodo_sorgente && pred.has_value()) {
			
			// pred.value() restituisce il T dentro l'optional 
			grafo_ris.add_edge(unidirected_edge<T> (pred.value(), nodo));
		}
	}
	
	return grafo_ris;
}
