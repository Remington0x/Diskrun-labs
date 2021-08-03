#ifndef TSTRING_HPP
#define TSTRING_HPP

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

struct TString {
    TString() {
        string[0] = '\0';
    }
    char string[256];
    TString& operator=(const TString& b);
};


//works with lower-cased chars
bool operator<(TString a, TString b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0') {
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
    } else {
        return false;
    }
}

bool operator>(TString a, TString b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0') {
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
    } else {
        return false;
    }
}

bool operator==(TString a, TString b) {
    int i = 0;
    while (a.string[i] != '\0' && b.string[i] != '\0') {
        if (a.string[i] != b.string[i]) {
            return false;
        }
        ++i;
    }
    //at this point one or two strings have ended
    if (a.string[i] == '\0' && b.string[i] == '\0') {
        return true;
    } else {
        return false;
    }
}

std::ostream& operator<<(std::ostream& out, TString a) {
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