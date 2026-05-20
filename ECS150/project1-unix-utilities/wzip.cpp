#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void emitRun(int count, char character) {
    write(STDOUT_FILENO, &count, sizeof(count));
    write(STDOUT_FILENO, &character, sizeof(character));
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "wzip: file1 [file2 ...]" << endl;
        return 1;
    }

    char currentChar = 0;
    int count = 0;

    for (int i = 1; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            cout << "wzip: cannot open file" << endl;
            return 1;
        }

        char buffer[8192];
        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
            for (ssize_t j = 0; j < bytesRead; ++j) {
                if (buffer[j] == currentChar) {
                    count++;
                } else {
                    if (count > 0) {
                        emitRun(count, currentChar);
                    }
                    currentChar = buffer[j];
                    count = 1;
                }
            }
        }

        close(fd);
    }

    if (count > 0) {
        emitRun(count, currentChar);
    }

    return 0;
}
