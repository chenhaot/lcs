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
void lcsdiff(char* f1, char* f2=NULL);
void read_sentence(char* filename, std::vector<string_vector>& sv);
