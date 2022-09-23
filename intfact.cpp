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
	unsigned long accumulator1 = 0, accumulator2 = 0;
	while (1) {
		mod1->run_micro_step();
		mod2->run_micro_step();
		char* snippet1 = mod1->getSnippet();
		char* snippet2 = mod2->getSnippet();
		unsigned int sz1 = strlen(snippet1); 
		unsigned int sz2 = strlen(snippet2);
		if ((sz1 % 100) == (sz2 % 100)) {
			int polarity1 = determine_polarity(accumulator1);
			int polarity2 = determine_polarity(accumulator2);
			if (accumulator1 > 0) {
				_factor1_ += boost::lexical_cast<std::string>(polarity1);
			} 
			if (accumulator2 > 0) {
				_factor2_ += boost::lexical_cast<std::string>(polarity2);
			}
			accumulator1 = accumulator2 = 0;
			accumulator1 = ( accumulator1 + sz1) % 100;
			accumulator2 = ( accumulator2 + sz2) % 100;
		} else {
			accumulator1 = ( accumulator1 + sz1) % 100;
			accumulator2 = ( accumulator2 + sz2) % 100;
		}
	}
	std::string factor1_in_decimal = _int_(_factor1_);
	std::string factor2_in_decimal = _int_(_factor2_);
	cout << num << "\t=\t" << factor1_in_decimal << "\tX\t" << factor2_in_decimal << endl;
	free(num);
	return 0;
}
