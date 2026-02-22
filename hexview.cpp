#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std; 

void block(ifstream& f, unsigned long st, unsigned long k, unsigned long en) {
    char b[16];
    f.seekg(static_cast<std::streamoff>(st));
    f.read(b, sizeof(b));
    streamsize b_read = f.gcount();

    printf("%010lX: ", static_cast<unsigned long>(k));
    for (int i = 0; i < 8; ++i) {
        if (i < b_read and i+st < en) {
            printf("%02X ", static_cast<unsigned char>(b[i]));
        }
        else {
            printf("   ");
        }
    }
    printf("| ");

    for (int i = 8; i < 16; ++i) {
        if (i < b_read and i + st < en) {
            printf("%02X ", static_cast<unsigned char>(b[i]));
        }
        else {
            printf("   ");
        }
    }
    printf(" ");

    for (int i = 0; i < b_read and i + st < en; ++i) {
        if (static_cast<unsigned char>(b[i]) < 32) {
            printf(".");
        }
        else {
            printf("%c", b[i]);
        }
    }
    if (!f.eof() and st + 16 < en) {
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    char file[256];
    size_t l = 0;
    unsigned long st = 0;

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == 'n' && argv[i][2] == '\0') {
            if (i + 1 >= argc) {
                cerr << "error: missing argument for -n\n";
                return 1;
            }
            l = std::strtoul(argv[++i], nullptr, 10);
        }
        else if (argv[i][0] == '-' && argv[i][1] == 's' && argv[i][2] == '\0') {
            if (i + 1 >= argc) {
                cerr << "error: missing argument for -s\n";
                return 1;
            }
            st = std::strtoul(argv[++i], nullptr, 10);
        }
        else {
            int j = 0;
            while (argv[i][j] != '\0') {
                file[j] = argv[i][j];
                j++;
            }
            file[j] = '\0';
        }
    }
    if (l == 0) {
        ifstream temp(file, std::ios::binary | std::ios::ate);
        if (!temp) {
            cerr << "error: cannot open file " << file << "\n";
            return 1;
        }
        l = static_cast<size_t>(temp.tellg());
        temp.close();
    }
    l += st;

    ifstream f(file, std::ios::binary);
    unsigned long k = 0;
    while (!f.eof() && st < l) {
        block(f, st, k, l);
        st += 16;
        k += 16;
    }

    return 0;
}
