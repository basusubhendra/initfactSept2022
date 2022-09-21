#ifndef _MODULATOR_
#define _MODULATOR_
#include "common.hpp"

class Modulator {
	private:
		char* num;
		unsigned long long int l;
		std::string binary_string1;
		std::string snippet;
		short int parity;
		unsigned long long int offset;
		FILE* fp;
		FILE *fe;
	        vector<unsigned long long int>* markers;
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
	this->markers = new vector<unsigned long long int>[1];
}

void Modulator::run_micro_step() {
	unsigned long long int ctr = this->offset;
	unsigned long long int idx = 0;
	vector<int> pi_pivots;
	vector<int> e_pivots;
	while (idx == 0 || ((idx % this->l != 0) && (idx > 0))) {
		char pp = 0, ee = 0;
		fscanf(this->fp, "%c",&pp);
		fscanf(this->fe, "%c",&ee);
		short int pp1 = pp - '0';
		short int ee1 = ee - '0';
		fscanf(this->fp, "%c",&pp);
		fscanf(this->fe, "%c",&ee);
		short int pp2 = pp - '0';
		short int ee2 = ee - '0';
		short int relation1 = _getStateRelation_(_deriveStateRelation_(pp1, pp2));
		short int relation2 = _getStateRelation_(_deriveStateRelation_(ee1, ee2));
		if (((relation1 == 2 && relation2 == 1 && (this->binary_string1[idx % this->l] == '0')) 
					|| (relation1 == 1 && relation2 == 2 && (this->binary_string1[idx % this->l] == '1'))) && 
				((this->parity == 0 && (pp1*10 + pp2) < (ee1*10 + ee2))
				 || (this->parity == 1 && (pp1*10 + pp2) > (ee1*10 + ee2)))) {
			markers->push_back(ctr);
			pi_pivots.push_back(pp1*10 + pp2);
			e_pivots.push_back(ee1*10+ ee2);
			++idx;
		}
		ctr += 2;
	}
	this->offset = ctr;
	std::string _factor_ = "";
	for (int i = 0; i < ctr; i+=2) {
		vector<unsigned long long int>::iterator it1 = std::find(markers->begin(), markers->end(), i);
		vector<unsigned long long int>::iterator it2 = std::find(markers->begin(), markers->end(), ctr - 2 - i);
		if (it1 != markers->end() && it2 != markers->end()) {
			unsigned long long int index = it1 - markers->begin();
			int pi_pivot = pi_pivots[index];
			int e_pivot = e_pivots[index];
			short int _pp1 = pi_pivot / 10;
			short int _pp2 = pi_pivot % 10;
			short int _ee1 = e_pivot / 10;
			short int _ee2 = e_pivot % 10;
			short int _relation1 = _getStateRelation_(_deriveStateRelation_(_pp1, _pp2));
			short int _relation2 = _getStateRelation_(_deriveStateRelation_(_ee1, _ee2));
			if (this->parity == 0 && pi_pivot < e_pivot) {
				if (_relation1 == 1 && _relation2 == 2) {
					_factor_ += "1";
				} else if (_relation1 == 2 && _relation2 == 1) {
					_factor_ += "0";
				}
			} else if (this->parity == 1 && pi_pivot > e_pivot) {
				if (_relation1 == 1 && _relation2 == 2) {
					_factor_ += "1";
				} else if (_relation1 == 2 && _relation2 == 1) {
					_factor_ += "0";
				}
			}
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
