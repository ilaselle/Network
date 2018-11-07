#include "network.h"
#include "random.h"

void Network::resize(const size_t& n) {
	values.resize(n);
	RNG.normal(values);
}

bool Network::add_link(const size_t& a, const size_t& b) {
	if ((a >= values.size()) or  (b >= values.size()) or (a==b)) {
		return false;
	}
	for (auto node_linked : neighbors(a)) {
		if (node_linked == b) {
			return false;
		}
	}
	links.insert(std::pair<size_t, size_t>(a,b));
	links.insert(std::pair<size_t, size_t>(b,a));
	return true;
}

size_t Network::random_connect(const double& mean) { 
	links.clear();
	std::vector<size_t> shuffle_values;
	for (size_t i(0); i < values.size(); ++i) {
	shuffle_values.push_back(i);
	}
	
	size_t nb_links(0);
	
	for (size_t j(0); j < values.size(); ++j) {
		
		RNG.shuffle(shuffle_values);
	
		size_t deg(RNG.poisson(mean));
		
		if (deg >= values.size()) {
			deg = (values.size()-1);
		}
		
/*!
  Dans la prochaine boucle for, on va créer pour chaque noeuds *n* du resau, *deg(n)* links avec d'autres noeuds, choisis au hasard.
  Dans la fonction add_link, on s'assure que les deux noeuds passés en paramètre ne sont pas les mêmes et que les deux ne sont pas encore liés.
  Neanmoins, si add_link return false, alors on doit choisir un autre noeud avec lequel faire un lien (vu que chaque noeud *n* à la fin doit avoir *deg(n)* connections).
  On va compter le lien entre (a,b) et (b,a) une seule fois.
  
  Avec ce procédé, le programme NetSim s'execute raisonablement, par contre ne passe pas tous les tests...
  Cela est parce que dans le test, on compte chaque lien deux fois, ce qui n'a pas de sens. 
  
  * Par contre, si on remplace cette prochaine partie de code par une boucle plus simple (mais fausse pour ce qu'on voudrait calculer, vu que le nombre de liens 
  * deviendrait beaucoup trop grand), comme celle-ci dessous, tous les tests passent bien.
  *
		size_t x(0);
		while (x<deg) {
			add_link(values[j], shuffle_values[x]);
			++nb_links;
			++x;
		} 
  * 
 */
		for (size_t i(0); i < deg; ++i) {
			if (add_link(values[j], shuffle_values[i])) {
				++nb_links;
			} else {
				size_t i(0);
				while(!add_link(values[j], values[i]) and i < values.size())
				{
					++i;
				}
				if(i < values.size()) ++nb_links;
			} 
		}
	}
	return nb_links;	  
}

size_t Network::set_values(const std::vector<double>& new_values) {
	size_t old_size(values.size());
	size_t new_size(new_values.size());
	
	if (new_size <= old_size) {
		for (size_t i(0); i < new_size; i++) {
			values[i] = new_values[i];
		}
		return new_size;
	} else {
		for (size_t i(0); i < old_size; i++) {
			values[i] = new_values[i];
		}
		return old_size;
	}
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	size_t nb_degrees(0);
	if ((links.find(_n)->first == _n) or (links.find(_n)->second == _n)) {
		++nb_degrees;
	}
	return nb_degrees;
}

double Network::value(const size_t &_n) const {
	return values[_n];
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> tmp = values;
	std::sort (tmp.begin(), tmp.end());
	std::reverse (tmp.begin(), tmp.end());
	return tmp;
}

std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> neighbors;
	for (auto i : links) {
		if(i.first == n) {
			neighbors.push_back(i.second);
		}
	}
	return neighbors;
}
