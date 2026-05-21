#!/bin/bash

echo "========================================"
echo "C++ Text Game Autograder"
echo "========================================"
echo

pass_count=0
fail_count=0

pass() {
  echo "✅ PASS: $1"
  pass_count=$((pass_count + 1))
  echo
}

fail() {
  echo "❌ FAIL: $1"
  fail_count=$((fail_count + 1))
  echo
}

# ----------------------------------------
# Test 1: Required files
# ----------------------------------------
echo "Checking required files..."

if [ ! -f main.cpp ]; then
  fail "main.cpp is missing"
else
  if [ -f README.md ] || [ -f README.txt ]; then
    pass "Required files exist"
  else
    fail "README.md or README.txt is missing"
  fi
fi

# ----------------------------------------
# Test 2: Compilation
# ----------------------------------------
echo "Compiling program..."

g++ -std=c++17 -Wall -Wextra -pedantic *.cpp -o game > compile_output.txt 2>&1
compile_status=$?

if [ $compile_status -ne 0 ]; then
  echo "Compiler output:"
  cat compile_output.txt
  fail "Program did not compile"
else
  pass "Program compiled successfully"
fi

# Only continue if compile succeeded
if [ $compile_status -eq 0 ]; then

  # ----------------------------------------
  # Test 3: Startup smoke test
  # ----------------------------------------
  echo "Running startup smoke test..."

  timeout 3s ./game > startup_output.txt 2>&1
  startup_status=$?

  if [ $startup_status -eq 0 ] || [ $startup_status -eq 124 ]; then
    pass "Program starts without crashing"
  else
    echo "Program output:"
    cat startup_output.txt
    fail "Program crashed or exited unexpectedly at startup"
  fi

  # ----------------------------------------
  # Test 4: Accepts stdin input
  # ----------------------------------------
  echo "Testing command input..."

  cat <<EOF > test_input.txt
look
help
quit
EOF

  timeout 5s ./game < test_input.txt > input_output.txt 2>&1
  input_status=$?

  if [ $input_status -eq 0 ] || [ $input_status -eq 124 ]; then
    if [ -s input_output.txt ]; then
      pass "Program accepts stdin input and produces output"
    else
      fail "Program ran but produced no output"
    fi
  else
    echo "Program output:"
    cat input_output.txt
    fail "Program failed while processing sample input"
  fi

  # ----------------------------------------
  # Test 5: Invalid input robustness
  # ----------------------------------------
  echo "Testing invalid input handling..."

  cat <<EOF > invalid_input.txt
asdfgh
????
quit
EOF

  timeout 5s ./game < invalid_input.txt > invalid_output.txt 2>&1
  invalid_status=$?

  if [ $invalid_status -eq 0 ] || [ $invalid_status -eq 124 ]; then
    pass "Program does not crash on invalid input"
  else
    echo "Program output:"
    cat invalid_output.txt
    fail "Program crashed on invalid input"
  fi

  # ----------------------------------------
  # Test 6: README content
  # ----------------------------------------
  echo "Checking README contents..."

  README_FILE=""
  if [ -f README.md ]; then
    README_FILE="README.md"
  elif [ -f README.txt ]; then
    README_FILE="README.txt"
  fi

  if [ -n "$README_FILE" ]; then
    grep -qi "compile" "$README_FILE"
    has_compile=$?

    grep -qi "run" "$README_FILE"
    has_run=$?

    if [ $has_compile -eq 0 ] && [ $has_run -eq 0 ]; then
      pass "README includes compile and run instructions"
    else
      fail "README is missing compile and/or run instructions"
    fi
  else
    fail "README file not found for content check"
  fi

  # ----------------------------------------
  # Test 7: Basic code structure
  # ----------------------------------------
  echo "Checking basic code structure..."

  structure_ok=1

  grep -Eq 'int[[:space:]]+main[[:space:]]*\(' main.cpp
  if [ $? -ne 0 ]; then
    echo "Could not find main()"
    structure_ok=0
  fi

  grep -Eq '\b(for|while)\b' main.cpp
  if [ $? -ne 0 ]; then
    echo "Could not find a loop"
    structure_ok=0
  fi

  grep -Eq '\bif\b' main.cpp
  if [ $? -ne 0 ]; then
    echo "Could not find an if statement"
    structure_ok=0
  fi

  FUNCTION_COUNT=$(grep -Ec '^[a-zA-Z_][a-zA-Z0-9_:<>[:space:]\*&,-]*[[:space:]]+[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*\([^;]*\)[[:space:]]*\{' main.cpp)

  echo "Detected function count: $FUNCTION_COUNT"

  if [ "$FUNCTION_COUNT" -lt 3 ]; then
    echo "Expected at least 3 function definitions"
    structure_ok=0
  fi

  if [ $structure_ok -eq 1 ]; then
    pass "Basic code structure looks valid"
  else
    fail "Code structure requirements not met"
  fi
fi

# ----------------------------------------
# Summary
# ----------------------------------------
echo "========================================"
echo "Autograder Summary"
echo "========================================"
echo "Passed: $pass_count"
echo "Failed: $fail_count"
echo

if [ $fail_count -eq 0 ]; then
  echo "🎉 All checks passed."
  exit 0
else
  echo "Some checks failed."
  exit 1
fi
