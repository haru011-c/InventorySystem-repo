#include "utils.h"
#include "console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <cctype>
using namespace std;

string str(long v) { ostringstream o; o << v; return o.str(); }
string money(double v) { ostringstream o; o << fixed << setprecision(2) << v; return o.str(); }
string lower(string s) { for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]); return s; }
string fit(const string& s, size_t w) { return s.size() <= w ? s : s.substr(0, w - 2) + ".."; }
string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    return a == string::npos ? "" : s.substr(a, s.find_last_not_of(" \t\r\n") - a + 1);
}

string readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) throw InputClosed();
    markNewLine();                              // ENTER moved the cursor to a new line
    return trim(s);
}
int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        string s = readLine(prompt);
        char* end; errno = 0;
        long v = strtol(s.c_str(), &end, 10);
        if (!s.empty() && *end == '\0' && errno == 0 && v >= lo && v <= hi) return (int)v;
        cout << "  [!] Enter a whole number from " << lo << " to " << hi << ".\n";
    }
}
double readDouble(const string& prompt, double lo, double hi) {
    while (true) {
        string s = readLine(prompt);
        char* end; errno = 0;
        double v = strtod(s.c_str(), &end);
        if (!s.empty() && *end == '\0' && errno == 0 && v >= lo && v <= hi) return v;
        cout << "  [!] Enter an amount from " << money(lo) << " to " << money(hi) << ".\n";
    }
}
string readText(const string& prompt, size_t maxLen) {
    while (true) {
        string s = readLine(prompt);
        if (!s.empty() && s.size() <= maxLen) return s;
        cout << "  [!] Enter 1 to " << maxLen << " characters.\n";
    }
}

void clearScreen() {
    cout.flush();                       // print anything still waiting first
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    markNewLine();                      // keep the centering margin working after the clear
}