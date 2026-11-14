#include "console.h"
#include <iostream>
#include <streambuf>
#include <string>

#ifdef _WIN32
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
#else
  #include <sys/ioctl.h>
  #include <unistd.h>
#endif
using namespace std;

static int terminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return info.srWindow.Right - info.srWindow.Left + 1;
#else
    winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) return w.ws_col;
#endif
    return 0;                                       // unknown: no margin
}

namespace {
    class CenterBuf : public streambuf {
        streambuf* dest;
        int contentWidth;
    public:
        bool atLineStart;
        CenterBuf(streambuf* d, int w) : dest(d), contentWidth(w), atLineStart(true) {}
    protected:
        int_type overflow(int_type c) override {
            if (traits_type::eq_int_type(c, traits_type::eof())) return traits_type::not_eof(c);
            char ch = traits_type::to_char_type(c);
            if (atLineStart && ch != '\n') {
                int margin = (terminalWidth() - contentWidth) / 2;
                if (margin > 0) dest->sputn(string(margin, ' ').c_str(), margin);
            }
            atLineStart = (ch == '\n');
            return dest->sputc(ch);
        }
        int sync() override { return dest->pubsync(); }
    };
    CenterBuf* active = nullptr;
}

struct CenterOutput::Impl { streambuf* original; CenterBuf* buf; };

CenterOutput::CenterOutput(int contentWidth) : impl(new Impl) {
    impl->original = cout.rdbuf();
    impl->buf = new CenterBuf(impl->original, contentWidth);
    cout.rdbuf(impl->buf);
    active = impl->buf;
}
CenterOutput::~CenterOutput() {
    cout.flush();
    cout.rdbuf(impl->original);                     // put the real buffer back before ours dies
    active = nullptr;
    delete impl->buf;
    delete impl;
}

void markNewLine() { if (active) active->atLineStart = true; }
