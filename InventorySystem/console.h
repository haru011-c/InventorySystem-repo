// console.h - centers everything printed with cout inside the terminal window
#pragma once

// Create one of these at the start of main(). While it exists, every line
// written to cout gets a left margin so the program sits in the middle of the
// terminal (the width is re-checked on every line, so resizing works too).
class CenterOutput {
    struct Impl;
    Impl* impl;
public:
    explicit CenterOutput(int contentWidth = 89);   // 89 = width of the widest table
    ~CenterOutput();
    CenterOutput(const CenterOutput&) = delete;
    CenterOutput& operator=(const CenterOutput&) = delete;
};

// Tell the centering layer the cursor is at the start of a new line
// (called after the user presses ENTER, which cout cannot see).
void markNewLine();
