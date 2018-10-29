#include <algorithm>
#include <iostream>
#include "network.h"
#include "random.h"

size_t Network::size() const{return values.size();}

bool Network::add_link(const size_t &a,const size_t &b){
	if(a==b or a>=values.size() or b>=values.size()) return false;
	else{
		if(linkExists(a,b)
				return false; // Check if the link exists
		else{
			
			links.insert(std::pair <size_t, size_t> (a, b));
			links.insert(std::pair <size_t, size_t> (b, a));
			return true;
		}
	} 
}	

size_t Network::set_values(const std::vector<double>& val){
	values.clear();
	values=val;
	return size();
}

size_t Network::degree(const size_t & _n) const{return links.count(_n);}

double Network::value(const size_t & _n) const{return values[_n];}

void Network::resize(const size_t& size){
	std::vector<double> newVect(values);
	newVect.resize(size);
	RNG.normal(newVect);
	values=newVect;
	//Ici nous supprimons les liens sur les nodes qui sont en dehors
	//de la taille de values. Si jamais nous avons un lien de 199 à 200,
	//et que nous resizons à 100, les liens en trop subsistent...
	if(values.size()>0)
		for(auto iter(links.find(values.size()-1)); iter != links.end();)
			iter=links.erase(iter);
}

size_t Network::random_connect(const double& mean_deg){
	size_t linksDone(0);
	links.clear();
	std::mt19937 rng;
	std::uniform_int_distribution<> unif(0, values.size()-1);
	
	for (size_t i=0; i<values.size();++i){
		size_t degree(RNG.poisson(mean_deg));
		for(size_t j=1; j <= degree;++j){
			while(not add_link(i, unif(rng))) continue;
			++linksDone;
		}
	}
	return linksDone;
}

std::vector<size_t> Network::neighbors(const size_t& key) const{
	std::vector<size_t> res;
	bool finished = false;
	for (auto iter = links.begin(); iter != links.end() && !finished;++iter){
		while(iter->first == key){
			finished = true;
			res.push_back(iter->second);
			++iter;
		}
	}
	return res;
}


std::vector<double> Network::sorted_values() const{
	std::vector<double> temp = values;
	std::sort(temp.begin(), temp.end(), std::greater<double>());
	return temp;
}


// ________________________________________________________________

bool Network::linkExists(const size_t& a, const size_t& b){
	for(auto I : links){
		if((I.first==a or I.first==b) and (I.second == a or I.second==b))
			return true; // Check if the link exists
	}
	return false;
}