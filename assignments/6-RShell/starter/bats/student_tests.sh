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

@test "Check built in cmd 'exit'" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>cmdloopreturned0"

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

@test "Check built in cmd 'stop-server'" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>cmdloopreturned0"

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

@test "Check if echo command works" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
echo hello
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>hellodsh4>cmdloopreturned0"

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

@test "Check if ls command works" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
ls
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>batsdshdsh.dSYMdsh_cli.cdshlib.cdshlib.hmakefilersh_cli.crsh_server.crshlib.hdsh4>cmdloopreturned0"

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

@test "Check if built in command cd works" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
cd ./bats
ls
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>DirectoryChangedSuccessfully.dsh4>assignment_tests.shstudent_tests.shdsh4>cmdloopreturned0"

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

@test "Check if command with flag works" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
ls -a
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>....debug.gitignorebatsdshdsh.dSYMdsh_cli.cdshlib.cdshlib.hmakefilersh_cli.crsh_server.crshlib.hdsh4>cmdloopreturned0"

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

@test "Check if piping command works" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
ls | grep rsh
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>rsh_cli.crsh_server.crshlib.hdsh4>cmdloopreturned0"

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

@test "Check for invalid commands" {
    ./dsh -s &
    server_pid=$!
    sleep 1
    run ./dsh -c <<EOF
asd
not-a-command
im-cooked
exit
EOF
    if ps -p $server_pid > /dev/null; then
        kill $server_pid
    fi
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>execvpfailed:Nosuchfileordirectorysocketservermode:addr:0.0.0.0:1234->Single-ThreadedModedsh4>execvpfailed:Nosuchfileordirectorysocketservermode:addr:0.0.0.0:1234->Single-ThreadedModedsh4>execvpfailed:Nosuchfileordirectorysocketservermode:addr:0.0.0.0:1234->Single-ThreadedModedsh4>cmdloopreturned0"

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