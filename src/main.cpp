#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

std::string strip_punctuation(std::string s) { // TODO: don't strip accent marks like "é"
    for (int i = 0; i < s.size(); i++) {
        if (s[i] < 'A' || s[i] > 'Z' && s[i] < 'a' || s[i] > 'z') { 
            s.erase(i, 1);
            i--;
        }
    }
    return s;
}

std::string lower_case(std::string s) {
    for (int i = 0; i < s.size(); i++)
        s[i] = tolower(s[i]);
    return s;
}

// from https://rosettacode.org/wiki/Levenshtein_distance#C.2B.2B

using std::size_t;

size_t uiLevenshteinDistance(const std::string &s1, const std::string &s2)
{
    const size_t m(s1.size());
    const size_t n(s2.size());

    if( m==0 ) return n;
    if( n==0 ) return m;

    size_t *costs = new size_t[n + 1];

    for( size_t k=0; k<=n; k++ ) costs[k] = k;

    size_t i = 0;
    for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
    {
    costs[0] = i+1;
    size_t corner = i;

    size_t j = 0;
    for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
    {
        size_t upper = costs[j+1];
        if( *it1 == *it2 )
        {
            costs[j+1] = corner;
        }
        else
        {
        size_t t(upper<corner?upper:corner);
        costs[j+1] = (costs[j]<t?costs[j]:t)+1;
        }

        corner = upper;
    }
    }

    size_t result = costs[n];
    delete [] costs;

    return result;
}

int main (int argc, char** argv) {
    if (argc > 1) {
        std::vector<std::string> words;
        std::ifstream dict("/usr/share/dict/words");

        std::string line;
        while (std::getline(dict, line)) {
            line = lower_case(strip_punctuation(line));
            words.push_back(line);
        }

        // TODO: search for command line switches

        for (int i = 1; i < argc; i++) {
            std::string word_str = lower_case(strip_punctuation(argv[i]));
            auto word = std::find(words.begin(), words.end(), word_str);
            char letter = word_str[0];
            int len = word_str.size();

            if (word == words.end()) {
                std::cout << "Misspelled word: " << word_str;

                std::vector<std::string> suggestions;
                suggestions.reserve(4);

                for (int i = 0; i <= 4; i++) {
                    auto suggestion = std::find_if(words.begin(), words.end(), [word_str, letter, len, i](std::string s){
                        auto fits = s[0] == letter && (s.length() >= len - 1 && s.length() <= len + 1); // TODO: add options for same letter / length options
                        auto distance = uiLevenshteinDistance(s, word_str);

                        return (fits && (distance <= i));
                    });

                    if (suggestion != words.end()) {
                        auto dup = std::find(suggestions.begin(), suggestions.end(), *suggestion);
                        suggestions.push_back(*suggestion);
                    }

                    auto end_unique = suggestions.end();
                    for(auto iter = suggestions.begin(); iter != end_unique; ++iter ) {
                        end_unique = std::remove(iter+1, end_unique, *iter);
                    }
                    suggestions.erase(end_unique, suggestions.end());
                }

                if (!suggestions.empty()) {
                    std::cout << " ( Perhaps you meant: ";
                    for (auto s : suggestions)
                        std::cout << s << " ";
                    std::cout << ")";
                }

                std::cout << std::endl;
            }
        }
    } else
        std::cout << "usage: spellcheck [words...]" << std::endl;

    return 0;
}
