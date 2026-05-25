#!/usr/bin/env bash
# GENERATED WITH AI TO PROVIDE EASE OF USE FOR PROVIDED TESTS
# --------------------------------------------------------------
# Executes every test defined in test-1.sh (the password‑generator
# test harness).  The script iterates over the known test names,
# invokes test‑1.sh with each name, and reports the result.
#
# Usage:
#   chmod +x run-all-tests.sh
#   ./run-all-tests.sh               # run every test
#   ./run-all-tests.sh <name> …      # run only the listed tests
# --------------------------------------------------------------

# Path to the test harness – adjust if the file lives elsewhere
TEST_HARNESS="./test-1.sh"

# ------------------------------------------------------------------
# All test identifiers that test‑1.sh understands (copy from the
# `case "$TEST_NAME"` block).  Keep them in an array so new tests
# can be added in one place.
# ------------------------------------------------------------------
ALL_TESTS=(
    build
    basic_cli
    default_alphabet
    flags_lu
    flags_ds
    custom_alphabet
    pw_count_length
    info_content
    file_created
    file_nonempty
    invalid_length
    invalid_quantity
    invalid_flags
    clean_target
    required_files
)

# ------------------------------------------------------------------
# Helper: print a nicely formatted line for each result.
# ------------------------------------------------------------------
report() {
    local name=$1
    local status=$2   # 0 = PASS, non‑zero = FAIL
    if [[ $status -eq 0 ]]; then
        printf "✅ %-20s PASS\n" "$name"
    else
        printf "❌ %-20s FAIL (exit %s)\n" "$name" "$status"
    fi
}

# ------------------------------------------------------------------
# If the user supplied arguments, treat them as the list of tests to
# run; otherwise run the full suite.
# ------------------------------------------------------------------
if [[ $# -gt 0 ]]; then
    SELECTED_TESTS=("$@")
else
    SELECTED_TESTS=("${ALL_TESTS[@]}")
fi

# ------------------------------------------------------------------
# Main loop – invoke the harness for each test name.
# ------------------------------------------------------------------
overall_status=0

for test in "${SELECTED_TESTS[@]}"; do
    # Guard against typos – skip unknown names but warn the user.
    if [[ ! " ${ALL_TESTS[*]} " =~ " ${test} " ]]; then
        printf "⚠️  Unknown test name: %s – skipping\n" "$test"
        continue
    fi

    # Run the harness; capture its exit code (0 = PASS).
    "$TEST_HARNESS" "$test"
    exit_code=$?

    report "$test" "$exit_code"

    # Remember if any test failed.
    if [[ $exit_code -ne 0 ]]; then
        overall_status=1
    fi
done

# ------------------------------------------------------------------
# Final result – exit 0 only when every executed test passed.
# ------------------------------------------------------------------
if [[ $overall_status -eq 0 ]]; then
    echo "All requested tests passed."
else
    echo "One or more tests failed."
fi

exit $overall_status
