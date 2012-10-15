#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cmath>

typedef std::vector<std::string> string_vector;
std::ostream& operator << (std::ostream& stream,  string_vector &a);
void lcsdiff(char* f1, char* f2);
void read_sentence(char* filename, string_vector& sv);
