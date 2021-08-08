#include <iostream>
#include <vector>
#include <tuple>
#include <memory>

class WordConverter{
private:
	enum NType{ PRECISE, NON_PRECISE, NEITHER };
	static const std::vector<std::tuple<NType, std::string, int64_t>> wordtable;
	const std::size_t tablesize = wordtable.size();
	//Helper function...
	std::tuple<NType, std::string, int64_t> getNumericMatch(std::string word){
		for(size_t pos = 0; pos < tablesize; ++pos)
			if(std::get<1>(wordtable[pos]) == word)
				return wordtable[pos];
		return std::make_tuple(NType::NEITHER, "", -1);
    }
public:
	WordConverter(){}
	static WordConverter *createWordProcessor(){ return new WordConverter(); }
	int64_t inputSequenceAnalyzer(){
	    int ch;
		std::tuple<NType, std::string, int64_t> cell, prev_cell(NType::NEITHER, "", -1);
	    int64_t num_equiv = 0, token_equiv = 0, left_over = 0;
	    bool done = 0, seen_num = 0, incomplete = 0, seen_and = 0;
	    std::string junk;
	    while(!done){
		    while((ch = std::cin.get())){
			    switch(ch){
		        	case ',' : case ' ' :
					case '.' : case '\n':
						if(junk.empty())
								;
						else if(junk == "and")
								seen_and = 1;
						else if(incomplete){
							token_equiv += std::get<2>(getNumericMatch(junk));
							incomplete = 0;
							seen_num = 1;
						}else{
							cell = getNumericMatch(junk);
							if(std::get<0>(cell) == NType::NON_PRECISE){
								if(seen_num || seen_and){
									token_equiv *= std::get<2>(cell);
									if(seen_and){
										num_equiv += token_equiv;
										token_equiv = seen_and = 0;
									}
								}else token_equiv = std::get<2>(cell);
							}else{
								if(seen_and){
									token_equiv += std::get<2>(cell);
									seen_num = 0;
								}
								else if(seen_num){
/**/								num_equiv += (token_equiv + std::get<2>(cell));
									token_equiv = 0;
								}else{
									token_equiv = std::get<2>(cell);
									seen_num = 1;
								}
						    }

						}
						if(ch == ','){
							num_equiv += token_equiv;
							token_equiv = 0;
							seen_num = 0;
							left_over = 0;
						}else if(ch == '\n')
							return num_equiv + token_equiv;
						junk.clear();
						break;
					case '-':
						token_equiv += std::get<2>(getNumericMatch(junk));
						incomplete = 1;
						junk.clear();
						break;
					case 'A' ... 'Z':
						junk += tolower(ch);
						break;
					case 'a' ... 'z':
						junk += ch;
						break;
		       }
		    }
		    if(num_equiv > 0)
				done = 1;
		    else std::cout<<"Expected an expression!\n";
		}
		return num_equiv;
	}
};
const std::vector<std::tuple<WordConverter::NType, std::string, int64_t>> WordConverter::wordtable = {
	{PRECISE, "one", 1}, {PRECISE, "two", 2}, {PRECISE, "three", 3}, {PRECISE, "four", 4},
	{PRECISE, "five", 5}, {PRECISE, "six", 6}, {PRECISE, "seven", 7}, {PRECISE, "eight", 8},
	{PRECISE, "nine", 9}, {PRECISE, "ten", 10}, {PRECISE, "eleven", 11}, {PRECISE, "twelve", 12},
	{PRECISE, "thirteen", 13}, {PRECISE, "fourteen", 14}, {PRECISE, "fifteen", 15}, {PRECISE, "sixteen", 16},
	{PRECISE, "seventeen", 17}, {PRECISE, "eighteen", 18}, {PRECISE, "nineteen", 19}, {PRECISE, "twenty", 20},
	{PRECISE, "thirty", 30}, {PRECISE, "forty", 40}, {PRECISE, "fifty", 50}, {PRECISE, "sixty", 60}, {PRECISE, "seventy", 70},
	{NON_PRECISE, "eighty", 80}, {NON_PRECISE, "ninety", 90}, {NON_PRECISE, "hundred", 100}, {NON_PRECISE, "thousand", 1000}, {NON_PRECISE, "million", 1000000},
	{NON_PRECISE, "billion", 1000000000}, {NON_PRECISE, "trillion", 1000000000000}, {NON_PRECISE, "quadrillion", 1000000000000000}};
int main(){
	std::shared_ptr<WordConverter> wc(WordConverter::createWordProcessor());
	auto rt(wc->inputSequenceAnalyzer());
	std::cout<<"Number equivalent: "<<rt;
}
