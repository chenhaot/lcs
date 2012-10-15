#include "lcs.h"

#define __VERBOSE__ 2
#define __ERROR__ 3
#define __WARN__ 2
#define __INFO__  1

typedef std::vector<int> lengths;

/*
  The "members" type is used as a sparse set for LCS calculations.
  Given a sequence, xs, and members, m, then
  if m[i] is true, xs[i] is in the LCS.
*/
typedef std::vector<bool> members;


/*
  Calculate LCS row lengths given iterator ranges into two sequences.
  On completion, `lens` holds LCS lengths in the final row.
*/
template <typename it>
void lcs_lens(it xlo, it xhi, it ylo, it yhi, lengths & lens) {
  // Two rows of workspace.
  // We need the 1 for the leftmost column.
  lengths curr(1 + distance(ylo, yhi), 0);
  lengths prev(curr);
  for (it x = xlo; x != xhi; ++x)
  {
    swap(prev, curr);
    int i = 0;
    for (it y = ylo; y != yhi; ++y, ++i)
    {
      curr[i + 1] = (*x == *y)
        ? prev[i] + 1
        : std::max(curr[i], prev[i + 1]);
    }
  }       
  swap(lens, curr);
}

/*
  Recursive LCS calculation.
  See Hirschberg for the theory!
  This is a divide and conquer algorithm.
  In the recursive case, we split the xrange in two.
  Then, by calculating lengths of LCSes from the start and end
  corners of the [xlo, xhi] x [ylo, yhi] grid, we determine where
  the yrange should be split.

  xo is the origin (element 0) of the xs sequence
  xlo, xhi is the range of xs being processed
  ylo, yhi is the range of ys being processed
  Parameter xs_in_lcs holds the members of xs in the LCS.
*/
template <typename it>
void calculate_lcs(it xo, it xlo, it xhi, it yo, it ylo, it yhi,
  members & xs_in_lcs, members & ys_in_lcs) {
  unsigned const nx = distance(xlo, xhi);
  if (nx == 0) {
    // empty range. all done
    return;
  } else if (nx == 1) {
    // single item in x range.
    // If it's in the yrange, mark its position in the LCS
    // xs_in_lcs[distance(xo, xlo)] = (find(ylo, yhi, *xlo) != yhi);
    it pos = find(ylo, yhi, *xlo);
    if (pos != yhi)
    {
      xs_in_lcs[distance(xo, xlo)] = 1;
      ys_in_lcs[distance(yo, pos)] = 1;
    }
    return;
  } else {
    // split the xrange
    it xmid = xlo + nx / 2;
    // Find LCS lengths at xmid, working from both ends of the range
    lengths ll_b, ll_e;
    std::reverse_iterator<it> hix(xhi), midx(xmid), hiy(yhi), loy(ylo);

    lcs_lens(xlo, xmid, ylo, yhi, ll_b);
    lcs_lens(hix, midx, hiy, loy, ll_e);

    // Find the optimal place to split the y range
    lengths::const_reverse_iterator e = ll_e.rbegin();
    int lmax = -1;
    it y = ylo, ymid = ylo;

    for (lengths::const_iterator b = ll_b.begin();
      b != ll_b.end(); ++b, ++e) {
      if (*b + *e > lmax) {
        lmax = *b + *e;
        ymid = y;
      }
      // ll_b and ll_e contain one more value than the range [ylo, yhi) 
      // As b and e range over dereferenceable values of ll_b and ll_e,
      // y ranges over iterator positions [ylo, yhi] _including_ yhi.
      // That's fine, y is used to split [ylo, yhi), we do not
      // dereference it. However, y cannot go beyond ++yhi.
      if (y != yhi) {
        ++y;
      }  
    }
    // Split the range and recurse
    calculate_lcs(xo, xlo, xmid, yo, ylo, ymid, xs_in_lcs, ys_in_lcs);
    calculate_lcs(xo, xmid, xhi, yo, ymid, yhi, xs_in_lcs, ys_in_lcs);
    return;
  }
}


/*
  Print matched sentence to stdout.
*/
void output_matched_sentence(string_vector& s, members &m) {
  string_vector result;
  for(int i = 0; i < s.size(); i++) {
    if (m[i])
      result.push_back(s[i] + "^");
    else    
      result.push_back(s[i] + "#");
  }
  std::cout << result << std::endl;
}


/*
  Find the match and output the difference.
*/
void output_difference(string_vector& x, string_vector& y) {
  members x_in_lcs(x.size(), false);
  members y_in_lcs(y.size(), false);
  calculate_lcs(x.begin(), x.begin(), x.end(), y.begin(), y.begin(), y.end(),
    x_in_lcs, y_in_lcs);
  output_matched_sentence(x, x_in_lcs);
  output_matched_sentence(y, y_in_lcs);
}


/*
  Read sentences from two files and output the result.
*/
void lcsdiff(char* f1, char* f2) {
  std::vector<string_vector> sv1, sv2;
  if (f2) {
    read_sentence(f1, sv1);
    read_sentence(f2, sv2);
    if (sv1.size() == sv2.size()) {
      for (int i = 0; i < sv1.size(); i++) {
        output_difference(sv1[i], sv2[i]);
      }
    } else {
      std::cerr << "two files with different number of sentences" << std::endl;
    }
  } else {
    read_sentence(f1, sv1);
    if (sv1.size() % 2 == 0) {
      for (int i = 0; i < sv1.size(); i += 2) {
        output_difference(sv1[i], sv1[i + 1]);
      }
    } else {
      std::cerr << "odd number of sentences" << std::endl;
    }
  }
  return;
}

/*
  Read a sentence from the file
*/
void read_sentence(char* f, std::vector<string_vector>& sv)
{
  std::ifstream myfile (f);
  std::string word;
  std::string line;
  if (myfile.is_open()) {
    while (std::getline(myfile, line)) {
      if (__VERBOSE__ < __INFO__)
        std::cout << "INFO ORIGINAL: " << line << std::endl;
      std::istringstream iss (line);
      string_vector s;
      while (iss.good()) {
        iss >> word;
        if (word.length() == 0) {
          if (__VERBOSE__ < __ERROR__)
            std::cout << "ERROR EMPTY WORD" << std::endl;
          continue;
        }
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        s.push_back(word);
      }
      sv.push_back(s);
    }
    myfile.close();
  } else {
    if (__VERBOSE__ < __ERROR__) {
      std::cout << "ERROR " << "fail to find file " << f << std::endl;
    }
  }
}

/*
  Join a vector of string.
*/
std::string get_string(const string_vector &s) {
  std::string result = "";
  for(string_vector::const_iterator iter = s.begin(); iter != s.end(); iter++) {
    result.append((*iter) + " ");
  }
  return result;
}

/*
  Override the operator to output the string.
*/
std::ostream& operator << (std::ostream& out,  string_vector &a) {
  out << get_string(a) << " ";
  return out;
}

