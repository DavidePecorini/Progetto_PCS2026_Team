#include "graph_visit.hpp"
#include "struct_class.hpp"
#include <list>
#include <vector>
#include <map>

template< typename T> 
bool finding_path(const unidirected_graph<T>& tree, const T& u, const T& v, std::vector<T>& path,
				  std::set<T>& visited) 
{
	// Aggiungo u ai nodi visitati e al ciclo 
	visited.insert(u);    
	path.push_back(u); 
	// se ho raggiunto v restituisco true
	if (u == v) {       
		return true;
	}
	// Scorro i vicini di u
	for (const auto& neighbour : tree.neighbours(u)) {
		
		// controllo che non sia già stato visitato
		if (!visited.count(neighbour)) 
		{
			// chiamo finding_path ricorsivamente
			if (finding_path(tree, neighbour,v, path, visited))
			{
				return true;
			}
		}
	}
	path.pop_back();
	
	return false;
}



template<typename T>
std::vector<std::vector<T>> DFS_for_cycles(const unidirected_graph<T>& g) 
{
	// Scelgo il nodo di partenza per la DFS e inizializzo un vettore per i cicli
	T starting_node = *g.all_nodes().begin();
	std::vector<std::vector<T>> cycles;
	
	// utilizzo la DFS ricorsiva per calcolare l'albero di visita
	unidirected_graph<T> tree = recursive_DFS_support(g, starting_node);    
	unidirected_graph<T> cotree = g - tree;      // coalbero
	
	// inizializzo il ciclo
	std::vector<T> path;     
	
	// inizializzo mappa di bool per finding path
	std::set<T> visited;
	
	for (const auto& edge: cotree.all_edges()) {
		
		// Svuoto il set di nodi già visitati
		visited.clear();
		
		finding_path(tree, edge.from(), edge.to(), path, visited);
		
		// Aggiungo path al vettore di cicli, dopodichè lo svuoto
		cycles.push_back(path);
		path.clear();
	}
	return cycles;
}
		
		
		
	