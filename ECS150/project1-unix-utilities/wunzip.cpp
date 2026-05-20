#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "wunzip: file1 [file2 ...]" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            cout << "wunzip: cannot open file" << endl;
            return 1;
        }

        int count;
        char character;
        while (read(fd, &count, sizeof(count)) > 0 && read(fd, &character, sizeof(character)) > 0) {
            string output(count, character);
            write(STDOUT_FILENO, output.data(), output.size());
        }

        close(fd);
    }

    return 0;
}
