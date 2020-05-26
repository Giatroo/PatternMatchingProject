// Includes, defines and global variables
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define debug(x) cout << #x << " = " << x << endl

// Colours define
typedef enum {
  DEFAULT = 0,
  BLACK = 30,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE
} Colour;

const int N = 1123456;
string text, pattern;
Colour text_colour[N];
Colour pattern_colour[N];

// General methods

void HitEnterClearingConsoleAndShowingMessage() {
  cin.ignore();
  fflush(stdin);
  printf("Hit enter key to continue...\n");
  getchar();
  system("clear");
}

void HitEnterToContinue() { getchar(); }

string stream_to_string(const istream &stm) {
  ostringstream oss;
  oss << stm.rdbuf();
  return oss.str();
}

// Printing methods

inline void ResetTextColour();
inline void ResetPatternColour();
inline void ColourText(int index, Colour c);
inline void ColourPattern(int index, Colour c);

inline void ColourText(int index, Colour c) { text_colour[index] = c; }

inline void ColourPattern(int index, Colour c) { pattern_colour[index] = c; }

inline void ResetTextColour() {
  for (long unsigned int i = 0; i < text.size(); i++)
    ColourText(i, DEFAULT);
}

inline void ResetPatternColour() {
  for (long unsigned int i = 0; i < pattern.size(); i++)
    ColourPattern(i, DEFAULT);
}

void PrintStrings(int text_index, int pattern_index) {
  // The strings the program will print
  // Each one will be printed in one line and the pattern must be aligned with
  // the text accordingly with the indexes
  string text_print;
  string pattern_print;

  // The indexes of the start and end of the substring of the text which we'll
  // print on the screen
  int text_start, text_end;
  text_start = max(0, text_index - 40);
  text_end = min((int)text.size(), 80);

  // The indexes of the start and end of the substring of the pattern which
  // we'll print on the screen
  int pattern_start, pattern_end;
  pattern_start = max(0, pattern_index - 40);
  pattern_end = min((int)pattern.size(), 80);

  text_print = text.substr(text_start, text_end);
  text_index -= text_start;

  // Removing line breaks of the text.
  for (long unsigned int i = 0; i <= text_print.size(); i++)
    if (text_print[i] == '\n')
      text_print[i] = ' ';

  pattern_print = pattern.substr(pattern_start, pattern_end);
  pattern_index -= pattern_start;

  // Inserting space in the pattern if it's needed for it to be aligned with the
  // text
  int num_espaces = 0;
  while (pattern_index < text_index) {
    pattern_print.insert(0, " ");
    num_espaces++;
    pattern_index++;
  }

  // Finally printing the strings
  for (long unsigned int i = 0; i < text_print.size(); i++)
    printf("\033[1;%dm%c\033[0m", text_colour[i + text_start], text_print[i]);
  printf("\n");

  for (long unsigned int i = 0; i < pattern_print.size(); i++)
    printf("\033[1;%dm%c\033[0m", pattern_colour[i - num_espaces],
           pattern_print[i]);
  printf("\n\n");
}

// String search methods declarations

int bruteforce();
int kmp();
int dfa[N][256];
int kmpmatrix();
int boyer_moore();
int rabin_karp();

// Main method

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Missing text file and pattern file!" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream text_file(argv[1]), pattern_file(argv[2]);
  ostringstream oss;

  if (!text_file.is_open()) {
    cout << "Text file not found!" << endl;
    exit(EXIT_FAILURE);
  }
  if (!pattern_file.is_open()) {
    cout << "Pattern file not found!" << endl;
    exit(EXIT_FAILURE);
  }

  text = stream_to_string(text_file);
  pattern = stream_to_string(pattern_file);

  text_file.close();
  pattern_file.close();

  // Removing possible \n from the end of the strings
  auto it = text.end();
  it--;
  if (*it == '\n')
    text.erase(it);
  it = pattern.end();
  it--;
  if (*it == '\n')
    pattern.erase(it);

  // for (int i = 0; i < text.size(); i++)
  // printf("'%c' %d\n", text[i], text[i]);
  // cout << endl;
  // for (int i = 0; i < pattern.size(); i++)
  // printf("'%c' %d\n", pattern[i], pattern[i]);

  kmpmatrix();

  return 0;
}

// Methods implementation

int bruteforce() {
  int i, j, M, N;
  M = pattern.size();
  N = text.size();

  for (i = 0, j = 0; j < M && i < N; i++, j++) {
    ColourText(i, GREEN);
    ColourPattern(j, GREEN);
    PrintStrings(i, j);

    while (text[i] != pattern[j]) {
      ColourText(i, RED);
      ColourPattern(j, RED);
      PrintStrings(i, j);
      i -= j - 1;
      j = 0;
      ResetPatternColour();
      ResetTextColour();
      ColourText(i, GREEN);
      ColourPattern(j, GREEN);
      PrintStrings(i, j);
    }
    ColourText(i, BLUE);
    ColourPattern(j, BLUE);
    PrintStrings(i, j);
  }

  if (j == M)
    return i - M;
  else
    return -1;
}

// Inicializing the dfa matrix used in the method kmpmatrix()
void initmatrix() {
  int preflen = 0;
  int c;
  long unsigned int j;
  for (c = 0; c < 256; c++)
    dfa[c][0] = 0;
  dfa[(int)pattern[0]][0] = 1;

  for (j = 1; j < pattern.size(); j++) {
    for (c = 0; c < 256; c++)
      dfa[j][c] = dfa[preflen][c];
    dfa[(int)pattern[j]][j] = j + 1;
    preflen = dfa[(int)pattern[j]][preflen];
  }
}
int kmpmatrix() {
  initmatrix();
  int i, j, M, N;
  M = pattern.size();
  N = text.size();

  for (i = 0, j = 0; j < M && i < N; i++) {
    ColourText(i, BLUE);
    ColourPattern(j, BLUE);
    PrintStrings(i, j);
    if (dfa[(int)text[i]][j] == j + 1) {
      ColourText(i, GREEN);
      ColourPattern(j, GREEN);
      PrintStrings(i, j);
    } else {
      ColourText(i, RED);
      ColourPattern(j, RED);
      PrintStrings(i, j);
      ResetTextColour();
      ResetPatternColour();
    }
    j = dfa[(int)text[i]][j];
  }

  if (j == M)
    return i - M;
  else
    return -1;
}
