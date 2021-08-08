/*
    Author:      Adesina Meekness
    E-mail:      zenon8adams@gmail.com
    Created:     10-12-2019
    Last Edited: 31-01-2021
*/

#include <iostream>
#include <deque>
#include <string>
#include <cstdlib>

using stringlist = std::deque<std::string>;

static const stringlist order
{
    {
        "thousand", "million", "billion", "trillion", "quadrillion",
        "quintillion", "sextillion", "septillion", "octillion",
        "nonillion", "decillion", "undecillion", "deudecillion",
        "tredecillion", "quattourdecillion", "quindecillion",
        "sexdecillion", "septondecillion", "octodecillion",
        "novemdecillion", "vigindecillion"
    }
};

static const char *unit(int n)
{
    if( n < '1' || n > '9' )
        return "";
    static const char *transl[ ] = { "one", "two", "three", "four", "five",
                                     "six", "seven", "eight", "nine" };
    return transl[ n - '1'];
}

static const char *ten(int n)
{
    if( n < '1' || n > '9' )
        return "";
    static const char *transl[ ] = { "ten", "twenty", "thirty", "fourty", "fifty",
                                     "sixty", "seventy", "eighty", "ninety" };

    return transl[ n - '1'];
}

static const char *others(int n)
{
    if( n < 11 || n > 19 )
        return "";
    static const char *transl[ ] = { "eleven", "twelve", "thirteen", "fourteen",
		                             "fifteen", "sixteen", "seventeen", "eighteen",
		                             "nineteen" };
	return transl[ n - 11];
}

stringlist parse(std::string num)
{
    stringlist toks;
    if(num.size() == 1){
        toks.push_front(unit(num[0]));
        return toks;
    }
    for(size_t nlen = num.size()-1, idx = nlen-1, count = 1, pos = (size_t)-1; idx != (size_t)-1; --idx){
        if(num[idx] != ',' && num[idx] != ' ')
            ++count;
            std::string tmp;
        if(count == 3){
            if(num[idx+1] != '0' && num[idx+2] != '0'){
                    if(num[idx+1] == '1')
                        tmp = others(10 * (num[idx+1]-'0') + (num[idx+2]-'0'));
                    else
                        tmp = ten(num[idx+1]) + std::string(1, '-') + unit(num[idx+2]);
            }else if(num[idx+1] != '0' && num[idx+2] == '0'){

                if(num[idx+1] == '1' && num[idx+2] != '0'){

                    tmp = others(10 * (num[idx+1]-'0') + (num[idx+2]-'0'));
                   }
                else{

                    tmp = ten(num[idx+1]);
                    if(num[idx+1] != '0') std::string(1, '-') + unit(num[idx+1]);
                }
            }else
                tmp = unit(num[idx+2]);
            if(num[idx] != '0'){
                if(!tmp.empty())
                    tmp = unit(num[idx]) + std::string(" hundred and ") + tmp;
                else{
                    tmp = unit(num[idx]) + std::string(" hundred");
                    if(toks.empty())
                        tmp = "and " + tmp;
               }
            }else{
                if(toks.empty())
                    tmp = "and " + tmp;
            }
            if(pos != (size_t)-1 && !tmp.empty()) tmp += " " + order[pos];
            ++pos;
            count = 0;
            if(!tmp.empty())
                toks.push_front(tmp);
            tmp.clear();
        }else if(idx == 0){
            if(count == 2){
                if(num[idx] != '0' && num[idx+1] != '0'){
                    if(num[idx] == '1')
                        tmp = others(10 * (num[idx]-'0') + (num[idx+1]-'0'));
                    else
                        tmp = ten(num[idx]) + std::string(1, '-') + unit(num[idx+1]);
                }else if(num[idx] != '0' && num[idx+1] == '0'){
                    if(num[idx] == '1' && num[idx+1] != '0')
                        tmp = others(10 * (num[idx]-'0') + (num[idx+1]-'0'));
                    else{
                        tmp = ten(num[idx]);
                        if(num[idx+1] != '0') std::string(1, '-') + unit(num[idx+1]);
                    }
                }else
                    tmp = unit(num[idx+1]);
            }else if(count == 1){
                tmp = unit(num[idx]);
            }
            if(pos != (size_t)-1 && !tmp.empty()) tmp += " " + order[pos];
            if(!tmp.empty())
                toks.push_front(tmp);

            tmp.clear();
        }
    }
    return toks;
}

int count(std::string digits, char counted){
    int counter = 0;
    for(const auto& byte : digits)
        if(byte == counted)
            ++counter;
    return counter;
}
int main()
{
#if defined(WIN32) || defined(WINNT)
    system("color 70");
#endif
    std::string num;
    while(1){
        std::getline(std::cin, num);
        if(num.size() == 0){
            std::cout<<"Expected a number!\n";
            continue;
        }
        if(num.size()-1 - (count(num, ',') + count(num, ' ')) > (order.size()-1) * 3){
            std::cout<<"Out of range!\n";
            continue;
        }
        auto tokens(parse(num));
        int n = 0;
        for(const auto& quota : tokens){
            if(n == 0)
                std::cout<<char(toupper(quota[0]))<<quota.substr(1);
            else
                std::cout<<", "<<quota;
            ++n;
        }
        putchar('\n');
    }
    return 0;
}
