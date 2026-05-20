# UC Davis ECS Coursework

Curated programming assignments from UC Davis ECS courses.

This repository keeps source code and small supporting data files while leaving out lecture PDFs, handouts, IDE settings, compiled binaries, debug bundles, coverage files, and large generated test outputs.

## Contents

- `ECS050`: assembly/C lab work.
- `ECS32A/HW`: introductory Python assignments.
- `ECS36B`: C++ assignments.
- `ECS36C`: C/C++ data structures coursework and practice exercises.
- `ECS130`: LU decomposition and triangular solve implementation in Python.
- `ECS140/hw1`: Go exercises with tests.
- `ECS140/hw4`: Go concurrency/debugging exercises with tests.
- `ECS150/project1-unix-utilities`: C++ implementations of `wcat`, `wgrep`, `wzip`, and `wunzip`.
- `ECS150/project2-shell`: a small Unix shell implementation.
- `ECS150/project3-web-server`: concurrent C++ web server project.

## Notes

The code is preserved as coursework/portfolio material. If this repository is made public, first confirm that the relevant course policies permit publishing completed assignment solutions.

## Build And Test

```sh
# Go assignment
cd ECS140/hw1
go test ./...

cd ../hw4
go test ./...

# ECS150 Unix utilities
cd ../../ECS150/project1-unix-utilities
make

# ECS150 shell
cd ../project2-shell
c++ -std=c++17 -Wall -Wextra -pedantic wish.cpp -o wish
```
