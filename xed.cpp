#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

void printHex(ifstream &file) {
    unsigned char buffer[16]; // Read 16 bytes at a time
    size_t address = 0;

    while (file.read(reinterpret_cast<char*>(buffer), sizeof(buffer)) || file.gcount()) {
        size_t bytesRead = file.gcount();

        // Print memory address
        cout << hex << setw(7) << setfill('0') << address << ": ";

        // Print hex values in groups of two
        for (size_t i = 0; i < bytesRead; i++) {
            cout << setw(2) << setfill('0') << static_cast<int>(buffer[i]) << " ";
            if (i == 7) cout << " "; // Extra space after the first 8 bytes
        }

        // Padding for incomplete rows
        if (bytesRead < 16) {
            for (size_t i = bytesRead; i < 16; i++) {
                cout << "   "; // Maintain spacing
                if (i == 7) cout << " ";
            }
        }

        // Print ASCII characters
        cout << " ";
        for (size_t i = 0; i < bytesRead; i++) {
            cout << (isprint(buffer[i]) ? static_cast<char>(buffer[i]) : '.');
        }

        cout << endl;
        address += 16;
    }
}

void printBinary(ifstream &file) {
    unsigned char buffer[6]; // Read 6 bytes at a time
    size_t address = 0;

    while (file.read(reinterpret_cast<char*>(buffer), sizeof(buffer)) || file.gcount()) {
        size_t bytesRead = file.gcount();

        // Print memory address
        cout << hex << setw(7) << setfill('0') << address << ": ";

        // Print binary representation
        for (size_t i = 0; i < bytesRead; i++) {
            for (int j = 7; j >= 0; j--) {
                cout << ((buffer[i] >> j) & 1);
            }
            cout << " ";
        }

        // Padding for incomplete rows
        if (bytesRead < 6) {
            for (size_t i = bytesRead; i < 6; i++) {
                cout << "         "; // Maintain alignment
            }
        }

        // Print human-readable characters
        cout << " ";
        for (size_t i = 0; i < bytesRead; i++) {
            cout << (isprint(buffer[i]) ? static_cast<char>(buffer[i]) : '.');
        }

        cout << endl;
        address += 6;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        cerr << "Usage: xed [-b] <filename>" << endl;
        return 1;
    }

    bool binaryMode = false;
    string filename;

    if (argc == 3 && string(argv[1]) == "-b") {
        binaryMode = true;
        filename = argv[2];
    } else {
        filename = argv[1];
    }

    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    if (binaryMode) {
        printBinary(file);
    } else {
        printHex(file);
    }

    return 0;
}
