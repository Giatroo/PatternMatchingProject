// Includes, defines and global variables
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

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
} Color;

// General methods

void HitEnterClearingConsoleAndShowingMessage() {
  cin.ignore();
  fflush(stdin);
  printf("Hit enter key to continue...\n");
  getchar();
  system("clear");
}

void HitEnterToContinue() { getchar(); }

// Printing methods

inline void ResetArrayColor(vector<Color> &color);
inline void ColorElement(vector<Color> &color, int index, Color c);

inline void ColorElement(vector<Color> &color, int index, Color c) {
  color[index] = c;
}

inline void ResetArrayColor(vector<Color> &color) {
  for (long unsigned int i = 0; i < color.size(); i++)
    ColorElement(color, i, DEFAULT);
}

void PrintStrings(string text, string pattern, int text_index,
                  int pattern_index, vector<Color> &text_color,
                  vector<Color> &pattern_color) {
  // The strings the program will print
  // Each one will be printed in one line and the pattern must be aligned with
  // the text accordingly with the indexes
  string text_print;
  string pattern_print;

  // The indexes of the start and end of the substring of the text which we'll
  // print on the screen
  int text_start, text_end;
  text_start = max(0, text_index - 40);
  text_end = min((int)text.size() - 1, 80);

  // The indexes of the start and end of the substring of the pattern which
  // we'll print on the screen
  int pattern_start, pattern_end;
  pattern_start = max(0, pattern_index - 40);
  pattern_end = min((int)pattern.size() - 1, 80);

  // ColorElement(text_color, text_index, YELLOW);
  text_print = text.substr(text_start, text_end);
  text_index -= text_start;

  // Removing line breaks of the text.
  for (long unsigned int i = 0; i <= text_print.size(); i++)
    if (text_print[i] == '\n')
      text_print[i] = ' ';

  // ColorElement(pattern_color, pattern_index, YELLOW);
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
    printf("\033[1;%dm%c\033[0m", text_color[i + text_start], text_print[i]);
  printf("\n");

  for (long unsigned int i = 0; i < pattern_print.size(); i++)
    printf("\033[1;%dm%c\033[0m", pattern_color[i - num_espaces],
           pattern_print[i]);
  printf("\n");
}

string stream_to_string(const istream &stm) {
  ostringstream oss;
  oss << stm.rdbuf();
  return oss.str();
}

// String search methods declarations

int bruteforce(string text, string pattern, vector<Color> &text_color,
               vector<Color> &pattern_color);
int kmp(string text, string pattern, vector<Color> &, vector<Color> &);
int boyer_moore(string text, string pattern, vector<Color> &, vector<Color> &);
int rabin_karp(string text, string pattern, vector<Color> &, vector<Color> &);

// Main method

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Missing text file and pattern file!" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream text_file(argv[1]), pattern_file(argv[2]);
  string text, pattern;
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

  vector<Color> text_color(text.size(), DEFAULT);
  vector<Color> pattern_color(pattern.size(), DEFAULT);

  bruteforce(text, pattern, text_color, pattern_color);
  return 0;
}

// Methods implementation

int bruteforce(string text, string pattern, vector<Color> &text_color,
               vector<Color> &pattern_color) {
  int i, j, M, N;
  M = pattern.size();
  N = text.size();

  for (i = 0, j = 0; j < M - 1 && i < N; i++, j++) {
    ColorElement(text_color, i, CYAN);
    ColorElement(pattern_color, j, CYAN);
    PrintStrings(text, pattern, i, j, text_color, pattern_color);

    if (text[i] == pattern[j]) {
      ColorElement(text_color, i-1, BLUE);
      ColorElement(pattern_color, j-1, BLUE);
      PrintStrings(text, pattern, i, j, text_color, pattern_color);
    }

    while (text[i] != pattern[j]) {
      ColorElement(text_color, i, RED);
      ColorElement(pattern_color, j, RED);
      PrintStrings(text, pattern, i, j, text_color, pattern_color);
      i -= j - 1;
      j = 0;
      ResetArrayColor(pattern_color);
      ResetArrayColor(text_color);
    }

    // cout << "j = " << j << endl;
  }

  // cout << N << " " << M << endl;
  // cout << "Fim " << i << " " << j << endl;

  if (j == M)
    return i - M;
  else
    return -1;
}
