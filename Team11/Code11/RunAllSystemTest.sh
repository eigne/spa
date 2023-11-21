#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Error: Please provide the AutoTester path as an argument."
  exit 1
fi

# Get the path to the AutoTester script
AUTO_TESTER_PATH="$1"

# Get all test folder names in the ./tests folder
TEST_FOLDERS=$(find ./../Tests11 -mindepth 1 -type d -name "*" | sed 's:.*/::')
# Keep count of number of tests that failed
TOTAL_FAILED=0

# Get the path to the analysis.xsl file
ANALYSIS_XSL_PATH="./tests/analysis.xsl"

# Iterate over all test folders
for TEST_FOLDER in $TEST_FOLDERS; do
  # Run the AutoTester script
  OUTPUT=$(./$AUTO_TESTER_PATH "./../Tests11/$TEST_FOLDER/${TEST_FOLDER}_source.txt" "./../Tests11/$TEST_FOLDER/${TEST_FOLDER}_queries.txt" "./tests/${TEST_FOLDER}_output.xml" 2>&1)

  # Get the exit code of the AutoTester script
  ERR_CODE=$?

  # Check if the test failed
  if [[ -n "$OUTPUT" && ( "$OUTPUT" =~ "Missing:" || "$OUTPUT" =~ "Additional:" ) ]]; then
    echo "${TEST_FOLDER}_queries.txt failed"
    TOTAL_FAILED=$((TOTAL_FAILED+1))
  fi

  # Check if the AutoTester script exited with an error code
  if [[ $ERR_CODE -ne 0 ]]; then
    echo "${TEST_FOLDER}_source.txt either failed parsing or timed out"
    TOTAL_FAILED=$((TOTAL_FAILED+1))
  fi

done

# Print the total number of failed tests
echo "Total failed tests: $TOTAL_FAILED"
