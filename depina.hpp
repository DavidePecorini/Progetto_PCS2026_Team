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
    else{
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
//costruisco la funzione che cerca il ciclo minimo e che userò dopo nel De Pina
template <typename T> 
std::vector<bool> trova_ciclominimo(const unidirected_graph<T>& g, const vector<bool>& v){
    //procedo con il lifting: creo il grafo sdoppiato
    unidirected_graph<std::pair<T, bool>> G_primo;
    for(const auto& arco : g.all_edges()){
        T u = arco.from();
        T v = arco.to();
        int indice = g.edge_number(arco);
        bool is_active; //devo verificare se per il vettore v l'arco è attivo perchè se lo è nella costruzione di G_primo creo gli archi incrociati
        if (indice != -1) {
            is_active = v[indice]; // Se l'indice è valido, prendi il valore dal vettore
        } 
        else {
            is_active = false;    // Se l'indice è -1 (errore), metti falso di default. il -1 è il risultato "di gestione d'errore" in caso io non trovi l'arco
        }
        //dai due nodi di un arco in g creo i quattro nodi che saranno in G_primo
        std::pair<T, bool> u_piu  = {u, true};
        std::pair<T, bool> u_meno = {u, false};
        std::pair<T, bool> v_piu  = {v, true};
        std::pair<T, bool> v_meno = {v, false};
        //prima di creare gli archi in G_primo mi recupero il tipo ed il peso dell'arco di g
        char t = arco.tipo();
        double p = arco.peso();
        if (is_active){
            G_primo.add_edge(u_piu,v_meno,t,p);
            G_primo.add_edge(u_meno,v_piu,t,p);
        }
        else{
            G_primo.add_edge(u_piu,v_piu,t,p);
            G_primo.add_edge(u_meno,v_meno,t,p);
        }
    }
    //qua si chiude la costruzione del grafo G_primo, non resta che cercare al suo interno i cicli minimi con il djikstra
    int peso_minimo_ciclo = std::numeric_limits<int>::max(); //lo inizializzo con il massimo intero che posso inserire
    std::vector<std::pair<T, bool>> percorso_migliore;
    
    for(const auto& nodo : g.all_nodes()) {
        std::pair<T, bool> start_node = {nodo, true};
        std::pair<T, bool> target_node = {nodo, false};
        auto [distanze, predecessori] = djikstra(G_primo, start_node); //dato che djikstra restituisce un pacchetto con due elementi questo comando me li spacchetta e salva in distanze e predecessori
        if (distanze.count(target_node) && distanze.at(target_node) < peso_minimo_ciclo) {
            peso_minimo_ciclo = distanze.at(target_node); 
            //questo if mi fa capire se sono arrivato al nodo target con meno distanza, .count() cerca la chiave
            //ora devo creare il cammino migliore annullando il precedente e ricostruendo questo qua
            percorso_migliore.clear();
            std::pair<T,bool> passo = target_node;
            while(passo!=start_node){
                percorso_migliore.push_back(passo);
                passo = predecessori.at(passo).value() //il metodo .at() mi permette di recuperare un predecessore senza, nel caso in cui passo non sia una chiave, modificare la mappa
            }
            percorso_migliore.push_back(start_node);
            std::reverse(best_path_nodes.begin(), best_path_nodes.end()); //devo ribaltarlo perchè in questo momento ho un cammino che va dal mio nodo finale a quello iniziale
        }
    }
    std::vector<bool> ciclo(m, false); //inizializzo il vettore che sarà il mio risultato
}

//adesso creo l'intera funzione che mi restituirà i cicli minimi: al suo interno c'è dunque una serie di operazioni di inizializzazione e l'algoritmo di De Pina vero e proprio
template<typename T>
std::vector<std::vector<bool>> cicli_minimi(const unidirected_graph<T>& g){//si noti come l'output sarà un vettore contenenti tutti quelli che nel PDF vengono chiamati vettori d'incidenza
    //fase di inizializzazione: devo associare ad ogni arco di g un indice che mi dia informazioni sull'ordine, in questo caso lessicografico. Mi tornano utili le funzioni edge_number e l'operatore < definito nella classe degli archi
    T starting_node = *g.all_nodes().begin();
    unidirected_graph<T> tree = recursive_DFS_support(g, starting_node);    
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
        for(int j = i+1; j<k;j++){
            if(prodotto_scalare(Ci,S[j]) == 1){
                S[j] = differenza_booleana(S[j],S[i]);
            }
        }
    }
    return cicli_minimi;
}
