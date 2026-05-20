# UC Davis Coursework

Curated UC Davis computer science coursework.

This repository keeps source code and small supporting data files while leaving out lecture PDFs, handouts, IDE settings, compiled binaries, debug bundles, coverage files, local databases, and large generated test outputs.

## Contents

- `ECS050`: assembly/C lab work.
- `ECS32A`: introductory Python assignments under `ECS32A/HW1` through `ECS32A/HW7`.
- `ECS36B`: C++ assignments.
- `ECS36C`: C/C++ data structures coursework and practice exercises.
- `ECS130`: numerical linear algebra homework source and small support data.
- `ECS140/hw1`: Go exercises with tests.
- `ECS140/hw2`: Common Lisp exercises with tests.
- `ECS140/hw3`: Prolog exercises with tests.
- `ECS140/hw4`: Go concurrency/debugging exercises with tests.
- `ECS150/project1-unix-utilities`: C++ implementations of `wcat`, `wgrep`, `wzip`, and `wunzip`.
- `ECS150/project2-shell`: a small Unix shell implementation.
- `ECS150/project3-web-server`: concurrent C++ web server project.
- `ECS150/project4-distributed-file-system`: source-only distributed file system submission files.
- `ECS154A/circuits`: Logisim circuit assignments.
- `ECS164/password-generator`: password generator project source.

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

# ECS164 backend
cd ../../ECS164/password-generator/backend
python -m pip install -r requirements.txt
python identify.py
```
