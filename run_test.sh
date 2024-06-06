#!/bin/bash

# Defined the directory containing all test cases and the main to run properly
BASE_DIR="TestCases"
MAIN_DIR=$(pwd)

# Original data for restoration
ORIGINAL_FOODS="F0001|Baklava|Rich middle eastern cake of thin layers of flaky pastry filled with nuts and honey|8.50
F0002|Gnocchi|Italian pasta dish made from potato and served with a rich cheese sauce|12.50
F0003|Haggis|Scottish delicacy made from tripe and oats and served with whiskey sauce|18.70
F0004|Jambalaya|Creole rish dish containing prawns, chicken, sausage and vegetables|23.00
F0005|Bibimbab|Spicy Korean rice dish with vegetables, bean paste and beef|15.80
F0006|Fondue|European dish of assorted melted cheeses. Served with bread for dipping|31.50
F0007|Kelewele|From Ghana, fried plantains seasoned with spices|11.50"
ORIGINAL_COINS="5000,10
2000,10
1000,10
500,6
200,20
100,30
50,5
20,3
10,40
5,20"

# Function to find a file in directory
find_file() {
    local dir=$1
    local pattern=$2
    find "$dir" -maxdepth 1 -type f -name "$pattern" | head -n 1
}

# Initialize a flag to track if any test fails
ALL_TESTS_PASSED=1

# Runs test cases by finding directory and then running files in there
for TEST_CASE_DIR in "$BASE_DIR"/*; do
    if [ -d "$TEST_CASE_DIR" ]; then
        echo "Running tests in $TEST_CASE_DIR"

        # Find the input, expected output, and expected coins files
        INPUT_FILE=$(find_file "$TEST_CASE_DIR" "*.input")
        EXPECTED_OUTPUT_FILE=$(find_file "$TEST_CASE_DIR" "*.expected_output")
        ACTUAL_OUTPUT_FILE=$(echo "$EXPECTED_OUTPUT_FILE" | sed 's/expected/actual/')
        EXPECTED_COINS_FILE=$(find_file "$TEST_CASE_DIR" "*.expected_coins")
        ACTUAL_COINS_FILE=$(echo "$EXPECTED_COINS_FILE" | sed 's/expected/actual/')

        # Ensure all required files are found and if not skip and move to next one
        if [[ -z $INPUT_FILE || -z $EXPECTED_OUTPUT_FILE || -z $EXPECTED_COINS_FILE ]]; then
            echo "Missing required files in $TEST_CASE_DIR. Skipping this test case."
            continue
        fi

        # Run the program with the input file and save the output to actual_output
        "$MAIN_DIR/ftt" "$MAIN_DIR/foods.dat" "$MAIN_DIR/coins.dat" < "$INPUT_FILE" > "$ACTUAL_OUTPUT_FILE"

        # Run diff to compare the different output files and see if they are the same
        diff -w "$EXPECTED_OUTPUT_FILE" "$ACTUAL_OUTPUT_FILE"
        output_diff_exit_code=$?

        # Run diff to compare the coins output files and see if they are the same
        diff -w -y "$EXPECTED_COINS_FILE" "$ACTUAL_COINS_FILE"
        coins_diff_exit_code=$?

        # Display results of the tests to see if they worked and the results
        if [ $output_diff_exit_code -eq 0 ] && [ $coins_diff_exit_code -eq 0 ]; then
            echo "Test Passed: No differences found in $TEST_CASE_DIR."
        else
            echo "Test Failed: Differences found in $TEST_CASE_DIR."

            if [ $output_diff_exit_code -ne 0 ]; then
                echo "Differences in main output in $TEST_CASE_DIR:"
                diff -w "$EXPECTED_OUTPUT_FILE" "$ACTUAL_OUTPUT_FILE"
            fi

            if [ $coins_diff_exit_code -ne 0 ]; then
                echo "Differences in coins output in $TEST_CASE_DIR:"
                diff -w -y "$EXPECTED_COINS_FILE" "$ACTUAL_COINS_FILE"
            fi

            # Set flag to indicate at least one test failed
            ALL_TESTS_PASSED=0
        fi

        # Restore original data after each test case
        echo "$ORIGINAL_FOODS" > "$MAIN_DIR/foods.dat"
        echo "$ORIGINAL_COINS" > "$MAIN_DIR/coins.dat"
    fi
done

echo "All tests completed."

# Exit with appropriate code
if [ $ALL_TESTS_PASSED -eq 0 ]; then
    exit 1
else
    exit 0
fi
