#!/bin/sh

run_case() {
    echo "\n--- Testing: $* ---"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./generator "$@"
}

run_case 0-1 0-2 1-2
run_case 0-1 0-2 0-3 1-2 1-3 2-3
run_case 0-1 0-3 0-4 1-2 1-3 1-4 1-5 2-4 2-5 3-4 4-5
run_case 0-
run_case 0-1-
run_case -11-
run_case 0-a
run_case 0-1 b-2
