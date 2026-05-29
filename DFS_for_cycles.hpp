#include "graph_visit.hpp"
#include <list>
#include <vector>
#include <map>
#include "unidirected_graph.hpp"
#include "unidirected_edge.hpp"


bool finding_path(const unidirected_graph<int>& tree, const int& u, const int& v, std::vector<int>& path, std::map<int,bool>& visited) {
	visited[u] = true;     // metto u a true
	path.push_back(u);     // aggiungo u al ciclo 
	if (u == v) {       // se ho raggiunto v restituisco true
		return true;
	}
	for (const auto& neighbour:tree.neighbours(u)) {     // guardo i vicini di u
		if (!visited[neighbour]) {     // se non l'ho visitato 
			if (finding_path(tree, neighbour,v, path, visited)){  // chiamo finding_path ricorsivamente
				return true;
			}
		}
	}
	path.pop_back(u);
	return false;
}

		


template typename<T>
std::vector<std::vector<int>> DFS_for_cycles(const unidirected_graph& g) {
	int starting_node = *g.all_nodes().begin();       // scelgo un nodo di partenza per la DFS
	std::vector<std::vector<int>> cycles;       // inizializzo un vettore di vettori per i cicli 
	unidirected_graph<int> T = recursive_DFS_support(g, starting_node);    // utilizzo la DFS ricorsiva per calcolare l'albero di visita
	unidirected_graph<int> C = g - T;      // coalbero
	std::set<unidirected_edge<int>> C_edges = C.all_edges();     // prendo tutti gli archi del coalbero
	std::set<int> nodes = T.all_nodes();     // prendo tutti i nodi dell'albero
	std::vector<int> path;      // inizializzo il ciclo
	std::map<int,bool> visited;      // inizializzo mappa di bool per finding path
	for (const auto& edge:C_edges) {
		for (const auto& node:nodes) {    // resetto visited
			visited[node] = false;
		}
		bool finding_path(T, edge.from(), edge.to(), path, visited);
		cycles.push_back(path);     // aggiungo il ciclo
		path.clear();     // svuoto il ciclo
	}
	return cycles;
}
		
		
		
	