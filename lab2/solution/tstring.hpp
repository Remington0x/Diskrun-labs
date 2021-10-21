#ifndef TSTRING_HPP
#define TSTRING_HPP

#include <cstdlib>

const int MAX_LENGTH = 256;

void toLowerCase(char* a) {
    int i = 0;
    while (a[i] != '\0') {
        if (a[i] >= 65 && a[i] <= 90) {
            a[i] += 32;
        }
        ++i;
    }
}

void stringAssignment(char* a, char* b) { // a = b
    if (a != nullptr && b != nullptr) {
        int i = 0;
        while (b[i] != '\0') {
            a[i] = b[i];
            ++i;
        }
        a[i] = b[i];
    }
}

bool stringCompare(const char* a, const char* b) {
    if (a != nullptr && b != nullptr) {
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) {
                return false;
            }
            ++i;
        }
        if (a[i] != b[i]) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

struct TString {
    char string[MAX_LENGTH];
    TString() {
        string[0] = '\0';
    }
    TString(const TString& s) {
        for (int i = 0; i < 256; ++i) {
            string[i] = s.string[i];
        }
    }
    ~TString() {}
    void nullStr() {
        for (int i = 0; i < MAX_LENGTH; ++i) {
            string[i] = '\0';
        }
    }
    TString& operator=(const TString& b);
};


//works with lower-cased chars
bool operator<(const TString& a, const TString& b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0' && i < MAX_LENGTH) {
        if (a.string[i] < b.string[i]) {
            return true;
        } else
        if (a.string[i] > b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] == '\0' && b.string[i] != '\0') {
        return true;
    }
    return false;
}

bool operator>(const TString& a, const TString& b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0' && i < MAX_LENGTH) {
        if (a.string[i] > b.string[i]) {
            return true;
        } else
        if (a.string[i] < b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] != '\0' && b.string[i] == '\0') {
        return true;
    }
    return false;
}

bool operator==(const TString& a, const TString& b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0' && i < MAX_LENGTH) {
        if (a.string[i] != b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] == '\0' && b.string[i] == '\0') {
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const TString& a) {
    int i = 0;
    while (a.string[i] != '\0') {
        out << a.string[i];
        ++i;
    }
    return out;
}

TString& TString::operator=(const TString& b) {
    int i = 0;
    while (b.string[i] != '\0') {
        string[i] = b.string[i];
        ++i;
    }
    string[i] = b.string[i];
    return *this;
}

#endif
