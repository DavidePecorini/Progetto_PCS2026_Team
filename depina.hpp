#include "graph_visit.hpp"
#include "struct_class.hpp"
#include <algorithm>
#include <vector>
#include <map>

//definisco il prodotto scalare booleano
int prodotto_scalare(const std::vector<bool>& vec1, const std::vector<bool>& vec2){
    //faccio un test sulla dimensione
    if(vec1.size() != vec2.size()){
        throw std::invalid_argument("I vettori devono avere la stessa dimensione");
    }
    int somma = 0;
    for(size_t t=0;t < vec1.size();t++){
        if(vec1[t] == vec2[t] && vec1[t] == 1){
            somma++;
        }
    }
    if(somma%2 == 0){
        return 0;
    }
    else {
        return 1;
    }
}
//definisco l'operazione di differenza tra vettori booleani
std::vector<bool> differenza_booleana(const std::vector<bool>& vec1, const std::vector<bool>& vec2){
     if(vec1.size() != vec2.size()){
        throw std::invalid_argument("I vettori devono avere la stessa dimensione");
    }
    std::vector<bool> risultato; //inizializzo il vettore da riempire
    for(size_t t=0;t < vec1.size();t++){
        if(vec1[t]==vec2[t]){
            risultato.push_back(0);
        }
        else{
            risultato.push_back(1);
        }
    }
    return risultato;
}

template<typename T>
std::vector<bool> trova_ciclominimo(const unidirected_graph<T>& g, const std::vector<bool>& S)
{
	// Creo il grafo sdoppiato 
	
	unidirected_graph<T> G_primo;
	
	int n = g.all_nodes().size();
	int m = g.all_edges().size();
	
	for (const auto& arco : g.all_edges()) 
	{
		T u = arco.from();
		T v = arco.to();
		
		// Controllo che l'arco sia attivo, valuto il valore di S_i per l'indice dell'arco in g
		// Al posto dell'etichettatura +/- al fine di raddoppiare la dimensione del grafo, uso una mappa che crea i doppioni come 
		// il nodo originale sommato alla dimensione di g
		if (S[g.edge_number(arco)]) 
		{
			// Si noti che nel nostro modello vale u = u+ e v+n+1 = v-
			// La scelta di n+1 al posto di n è dovuta al fatto di dover evitare la comparsa di un nodo 0 quando viene fatta l'operazione %n
			// Notiamo che questa soluzione funziona solo in caso di nodi numerati consecutivamente
			G_primo.add_edge(unidirected_edge<T>(u,v+n+1));
			G_primo.add_edge(unidirected_edge<T>(u+n+1,v));
		}
		else
		{
			G_primo.add_edge(unidirected_edge<T>(u,v));
			G_primo.add_edge(unidirected_edge<T>(u+n+1,v+n+1));
		}
	}
	// Creo il vettore di booleani che restituisce true in posizione edge_number(arco) se l'arco è parte del cammino minimo
	std::vector<bool> C_best(m,true);
	
	for (const auto& nodo : g.all_nodes()) 
	{
		auto [distanze, predecessori] = djikstra(G_primo, nodo);
		auto g_opt = grafo_cammino_minimo(predecessori, distanze, nodo, nodo+n+1);
		
		if (!g_opt.has_value()) continue;
		
		unidirected_graph<T> g_minimo = g_opt.value();
		
		std::vector<bool> C(m,false);
		T u;
		T v;
		for (const auto& arco : g_minimo.all_edges()) 
		{
			u = (arco.from())%(n+1);
			v = (arco.to())%(n+1);
			
			size_t indice = g.edge_number(unidirected_edge<T>(u,v));
			C[indice] = !C[indice];
		}
		if (std::count(C.begin(), C.end(), true) < std::count(C_best.begin(), C_best.end(), true)) 
		{
			C_best = C;
		}
	}
	return C_best;
}
			
			
			
		


template<typename T>

std::vector<std::vector<bool>> de_pina(const unidirected_graph<T>& g){//si noti come l'output sarà un vettore contenenti tutti quelli che nel PDF vengono chiamati vettori d'incidenza
	// Scelgo un nodo 
	T starting_node = *g.all_nodes().begin();

	unidirected_graph<T> tree = recursive_dfs(g, starting_node);
	
	unidirected_graph<T> cotree = g - tree;
	
	size_t m = g.all_edges().size();  //questo m è il numero di archi del grafo e sarà la dimensione di tutti i vettori booleani che creeremo

	std::vector<std::vector<bool>> S;  //creo la matrice dove inserirò i vettori Si, S avrà quindi dimensione k X m di elementi booleani
	for(const auto& arco_coalbero : cotree.all_edges()){
		std::vector<bool> si(m,false); //impongo la dimensione e inizializzo gli elementi falsi
		int indice = g.edge_number(arco_coalbero);
		si[indice] = true;
		S.push_back(si);
	}
	//costruzione dell'algoritmo di De Pina
	std::vector<std::vector<bool>> cicli_minimi; //questo vettore di vettori sarà il risultato dell'intera funzione
	size_t k = S.size(); //equivale al numero dei vettori del coalbero, basti vedere da che ciclo for è stato generato
	//seguo l'iter del pdf, implemento lo pseudocodice e poi implementerò la funzione che trova veramente il ciclo minimo
	for(size_t i = 0; i < k; i++){
		std::vector<bool> Ci = trova_ciclominimo(g,S[i]);
		cicli_minimi.push_back(Ci);
		for(size_t j = i+1; j<k;j++)
		{
			if(prodotto_scalare(Ci,S[j]) == 1)
			{
				S[j] = differenza_booleana(S[j],S[i]);
			}
		}
	}
	return cicli_minimi;
}
