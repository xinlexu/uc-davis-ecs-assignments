#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

bool containsTerm(const string& line, const string& term);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "wgrep: searchterm [file ...]" << endl;
        return 1;
    }

    string searchTerm = argv[1];
    if (argc == 2) {
        string line;
        while (getline(cin, line)) {
            if (containsTerm(line, searchTerm)) {
                cout << line << endl;
            }
        }
        return 0;
    }

    for (int i = 2; i < argc; ++i) {

        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            cout << "wgrep: cannot open file" << endl;
            return 1;
        }

        string line;
        char buffer;
        ssize_t bytesRead;

        while ((bytesRead = read(fd, &buffer, 1)) > 0) {
            if (buffer != '\n') {
                line += buffer;
            }
            else {
                if (containsTerm(line, searchTerm)) {
                    cout << line << endl;
                }
                line.clear();
            }
        }

        if (!line.empty() && containsTerm(line, searchTerm)) {
            cout << line << endl;
        }

        close(fd);
    }

    return 0;
}

bool containsTerm(const string& line, const string& term) {
    return line.find(term) != string::npos;
}
