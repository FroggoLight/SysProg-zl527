#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Example: Multiple pipes - ls -l | grep dsh | wc -l" {
    run "./dsh" <<EOF
ls -l | grep dsh | wc -l
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="5dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Example: Commands with arguments | grep word" {
    run "./dsh" <<EOF
echo "This is a test string with the word word in it" | grep word
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="Thisisateststringwiththewordwordinitdsh3>dsh3>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Error Case: Command not found in pipe - nosuchcommand | cat" {
    run "./dsh" <<EOF
nosuchcommand | cat
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Error Case: Pipe with invalid command syntax - ls | grep [" {
    run "./dsh" <<EOF
ls | grep [
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="grep:brackets([])notbalanceddsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Error Case: Too many commands (over 8)" {
    run "./dsh" <<EOF
echo 1 | echo 2 | echo 3 | echo 4 | echo 5 | echo 6 | echo 7 | echo 8 | echo 9 
EOF
    # Check for "error: piping limited to ..." message (or your defined error message)
    if ! grep -q 'piping limited to' <<< "$output"; then
        echo "Error message 'piping limited to' not found in output:"
        echo "$output"
        false # Fail if error message is missing
    fi
    # Exit status might be OK (0) in parent shell, or your error code - adjust if needed
    [ "$status" -eq 0 ] # Or check for specific error exit code if you return one in this scenario
}

@test "Error Case: Just pipes |||" {
    run "./dsh" <<EOF
|||
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>error:parseerrorwithemptypipesdsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Example: Redundant pipes - ls || grep c" {
    run "./dsh" <<EOF
ls || grep c
EOF
    # Behavior might vary: treat as "ls | grep c", or error.  Adjust test accordingly.
    # This example assumes it treats "||" as a single pipe and runs "ls | grep c"
    if ! grep -q 'dshlib.c' <<< "$output"; then
        echo "Output does not contain 'dshlib.c':"
        echo "$output"
        false
    fi
    [ "$status" -eq 0 ] # Adjust if you expect non-zero status based on error handling
}