#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Echo with multiple arguments" {
    run "./dsh" <<EOF                
echo Hello World!
EOF

    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    expected_output="Hello World!dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "List directory using ls" {

    mkdir -p test_ls_dir
    touch test_ls_dir/file1 test_ls_dir/file2 test_ls_dir/file3

    run "./dsh" <<EOF                
ls test_ls_dir
EOF

    expected_output="file1file2file3dsh2>dsh2>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    rm -rf test_ls_dir

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Find text using grep" {
    echo -e "Hello\nWorld\nDSH" > test_grep.txt

    run "./dsh" <<EOF                
grep World test_grep.txt
EOF

    expected_output="Worlddsh2>dsh2>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    rm -f test_grep.txt

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Display file content using cat" {
    echo "Hello this is a test" > test_cat.txt

    run "./dsh" <<EOF                
cat test_cat.txt
EOF

    expected_output="Hellothisisatestdsh2>dsh2>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    rm -f test_cat.txt

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Echo command with special characters" {
    run "./dsh" <<EOF
echo "Hello\\ World"
EOF

    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    expected_output="Hello\ Worlddsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}