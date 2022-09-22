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
		if (snippet1.size() == snippet2.size()) {
			int polarity1 = determine_polarity(accumulator1);
			int polarity2 = determine_polarity(accumulator2);
			if (accumulator1 > 0) {
				_factor1_ += boost::lexical_cast<std::string>(polarity1);
			} 
			if (accumulator2 > 0) {
				_factor2_ += boost::lexical_cast<std::string>(polarity2);
			}
			accumulator1 = accumulator2 = 0;
		} else {
			accumulator1 += snippet1.size();
			accumulator2 += snippet2.size();
		}
	}
	std::string factor1_in_decimal = _int_(_factor1_);
	std::string factor2_in_decimal = _int_(_factor2_);
	cout << num << "\t=\t" << factor1_in_decimal << "\tX\t" << factor2_in_decimal << endl;
	free(num);
	return 0;
}
