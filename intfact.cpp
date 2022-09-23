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
	mpz_t accumulator1;
	mpz_init(accumulator1);
	mpz_set_si(accumulator1, 0);
	mpz_t accumulator2;
	mpz_init(accumulator2);
	mpz_set_si(accumulator2, 0);
	mpz_t rz;
	mpz_init(rz);
	while (1) {
		mod1->run_micro_step();
		mod2->run_micro_step();
		char* snippet1 = mod1->getSnippet();
		char* snippet2 = mod2->getSnippet();
		unsigned int sz1 = strlen(snippet1); 
		unsigned int sz2 = strlen(snippet2);
		if ((sz1 % 100) == (sz2 % 100)) {
			mpz_mod_ui(rz, accumulator1, 100);
			int polarity1 = determine_polarity(mpz_get_ui(rz));
			mpz_mod_ui(rz, accumulator2, 100);
			int polarity2 = determine_polarity(mpz_get_ui(rz));
			if (mpz_cmp_si(accumulator1, 0) > 0) {
				_factor1_ += boost::lexical_cast<std::string>(polarity1);
			} 
			if (mpz_cmp_si(accumulator2, 0) > 0) {
				_factor2_ += boost::lexical_cast<std::string>(polarity2);
			}
			gmp_printf("%Zd\n", accumulator1);
			gmp_printf("%Zd\n", accumulator2);
			mpz_set_si(accumulator1, 0);
			mpz_set_si(accumulator2, 0);
			mpz_add_ui(accumulator1, accumulator1, sz1);
			mpz_add_ui(accumulator2, accumulator2, sz2);
		} else {
			mpz_add_ui(accumulator1, accumulator1, sz1);
			mpz_add_ui(accumulator2, accumulator2, sz2);
		}
	}
	std::string factor1_in_decimal = _int_(_factor1_);
	std::string factor2_in_decimal = _int_(_factor2_);
	cout << num << "\t=\t" << factor1_in_decimal << "\tX\t" << factor2_in_decimal << endl;
	mpz_clear(accumulator1);
	mpz_clear(accumulator2);
	free(num);
	return 0;
}
