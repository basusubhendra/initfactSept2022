#include "common.hpp"
#include "adapter.hpp"
#include "modulator.hpp"

int main(int argc, char* argv[]) {
	char* num = strdup(argv[1]);
	Adapter* adp = new Adapter(num);
	adp->convert();
	Modulator* mod1 = new Modulator(num, adp->getBinary(), 0);
	Modulator* mod2 = new Modulator(num, adp->getBinary(), 1);
	std::string _factor1_ = "";
	std::string _factor2_ = "";
	unsigned long long int accumulator1 = 0;
	unsigned long long int accumulator2 = 0;
	while (1) {
		mod1->run_micro_step();
		mod2->run_micro_step();
		std::string snippet1 = mod1->getSnippet();
		std::string snippet2 = mod2->getSnippet();
		unsigned long long snippet1_size = snippet1.size();
		unsigned long long snippet2_size = snippet2.size();
		if (snippet1_size > 0 && snippet1_size == snippet2_size) {
			short int polarity1 = determine_polarity(accumulator1);
			short int polarity2 = determine_polarity(accumulator2);
			if (polarity1 == 0) {
				_factor1_ += "0";
			} else if (polarity1 == 1) {
				_factor1_ += "1";
			}
			if (polarity2 == 0) {
				_factor2_ += "0";
			} else if (polarity2 == 1) {
				_factor2_ += "1";
			}
			accumulator1 = 0;
			accumulator2 = 0;
			cout << _factor1_ << endl;
			cout << _factor2_ << endl;
			system("a=1;read a");
		} else {
			accumulator1 += snippet1_size;
			accumulator2 += snippet2_size;
		}
		//if product of _factor1_ and _factor2_ is num
		//then exit
	}
	std::string factor1_in_decimal = _int_(_factor1_);
	std::string factor2_in_decimal = _int_(_factor2_);
	cout << num << "\t=\t" << factor1_in_decimal << "\tX\t" << factor2_in_decimal << endl;
	free(num);
	return 0;
}
