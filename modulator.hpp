#ifndef _MODULATOR_
#define _MODULATOR_
#include "common.hpp"

class Modulator {
	private:
		char* num;
		long long int l;
		std::string binary_string1;
		std::string snippet;
		short int parity;
		long long int offset;
		FILE* fp;
		FILE *fe;
	        vector<long long int>* markers;
		vector<int>* pi_pivots;
		vector<int>* e_pivots;
	public:
		Modulator(char*, std::string, short int);
		void run_micro_step();
		std::string getSnippet();
		~Modulator();
};

Modulator::Modulator(char* s, std::string _bin_, short int _parity_) {
	this->num = strdup(s);
	this->binary_string1 = _bin_;
	this->l = _bin_.size();
	this->parity = _parity_;
	this->fp = fopen64("./pi.txt","r");
	this->fe = fopen64("./e.txt","r");
	this->markers = new vector<long long int>[1];
	this->pi_pivots = new vector<int>();
	this->e_pivots = new vector<int>();
}

void Modulator::run_micro_step() {
	long long int ctr = this->offset;
	long long int idx = 0;
	while (idx == 0 || ((idx % this->l != 0) && (idx > 0))) {
		char pp = 0, ee = 0;
		fscanf(this->fp, "%c",&pp);
		fscanf(this->fe, "%c",&ee);
		int pp1 = pp - '0';
		int ee1 = ee - '0';
		fscanf(this->fp, "%c",&pp);
		fscanf(this->fe, "%c",&ee);
		int pp2 = pp - '0';
		int ee2 = ee - '0';
		short int relation1 = _getStateRelation_(_deriveStateRelation_(pp1, pp2));
		short int relation2 = _getStateRelation_(_deriveStateRelation_(ee1, ee2));
		if (((relation1 == 2 && relation2 == 1 && (this->binary_string1[idx % this->l] == '0')) 
					|| (relation1 == 1 && relation2 == 2 && (this->binary_string1[idx % this->l] == '1'))) && 
				((this->parity == 0 && (pp1*10 + pp2) < (ee1*10 + ee2))
				 || (this->parity == 1 && (pp1*10 + pp2) > (ee1*10 + ee2)))) {
			this->markers->push_back(ctr);
			this->pi_pivots->push_back(pp1*10 + pp2);
			this->e_pivots->push_back(ee1*10+ ee2);
			++idx;
		}
		ctr += 2;
	}
	this->offset = ctr;
	std::string _factor_ = "";
	for (int i = 0; i < ctr; i+=2) {
		vector<long long int>::iterator it1 = std::find(this->markers->begin(), this->markers->end(), i);
		vector<long long int>::iterator it2 = std::find(this->markers->begin(), this->markers->end(), ctr - 2 - i);
		if (it1 != this->markers->end() && it2 != this->markers->end()) {
			_factor_ += "X";
		}
	}
	this->snippet = _factor_;
	return;
}

std::string Modulator::getSnippet() {
	return this->snippet;
}

Modulator::~Modulator() {
	free(this->num);
	delete(markers);
	fclose(this->fp);
	fclose(this->fe);
}
#endif
