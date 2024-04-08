#!/usr/bin/env bash

# Array of test suite names
test_suites=(
  "call_cd"
  "call_env"
  "remove_quo"
  "expand_complex"
  "expand_simple"
)

# ANSI color codes
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

# Function to run tests for a given suite
run_tests() {
  suite_name=$1

  echo -e "### ${MAGENTA}$suite_name${NC} ###"
  echo
  ./utest_minishell --filter "$suite_name*"
  echo
}

# Build the test executable
export LD_LIBRARY_PATH=:./criterion/lib
make
make test
echo 

# Iterate over the test suites and run tests for each suite
for suite in "${test_suites[@]}"
do
  run_tests "$suite"
done
