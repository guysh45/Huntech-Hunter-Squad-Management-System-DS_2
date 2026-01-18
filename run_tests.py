import os
import argparse
import subprocess
import glob
import sys
import time
import re

# Compilation flags for C++ code
# -std=c++14: Use C++14 standard
# -DNDEBUG: Disable assertions for performance
# -Wall: Enable all compiler warnings
# -g: Include debug symbols (required for Valgrind to report line numbers in leak reports)
COMPILATION_FLAGS = ["-std=c++14", "-DNDEBUG", "-Wall", "-g"]

# Timeout tiers based on test size (operations) with Valgrind's 20x-50x slowdown
# Sized for average laptops (slower CPUs, less RAM, possible HDD) - not just high-end machines
# Test sizes: 0-4: 100-40K ops, 5-9: 60K-100K ops, 10-19: 100K-800K ops, 
#             20-27: 800K-2.5M ops, 28-29: 5M-10M ops
TIMEOUT_SMALL = 180      # 3 minutes for tests 0-4 (100-40K ops)
TIMEOUT_MEDIUM = 600     # 10 minutes for tests 5-9 (60K-100K ops)
TIMEOUT_LARGE = 1800     # 30 minutes for tests 10-19 (100K-800K ops)
TIMEOUT_VERY_LARGE = 4800  # 80 minutes for tests 20-27 (800K-2.5M ops)
TIMEOUT_EXTREME = 14400   # 240 minutes (4 hours) for tests 28-29 (5M-10M ops)


def is_valgrind_available():
    """Check if Valgrind is available on the system."""
    try:
        result = subprocess.run(
            ["valgrind", "--version"],
            capture_output=True,
            timeout=2
        )
        return result.returncode == 0
    except (FileNotFoundError, subprocess.TimeoutExpired):
        return False


def detect_memory_leaks(exe_file, input_file, result_file, timeout):
    """
    Run executable under Valgrind to detect memory leaks.
    
    Args:
        exe_file: Path to the compiled executable
        input_file: Path to test input file
        result_file: Path to write test output
        timeout: Maximum time to wait for execution (seconds)
    
    Returns:
        Tuple (success, None, None, leak_status) where:
        - success: True if program executed, False on error/timeout
        - leak_status: "LEAK" if leaks detected, "OK" if no leaks, or error message
    """
    try:
        with open(input_file, "r") as stdin, open(result_file, "w") as stdout:
            # Run Valgrind with leak detection enabled
            # --leak-check=full: Detect all types of leaks (definitely lost, indirectly lost, etc.)
            # --error-exitcode=1: Exit with code 1 if leaks found (we parse output instead)
            # --quiet: Suppress normal Valgrind output messages
            # --log-fd=2: Send Valgrind output to stderr for parsing
            process = subprocess.Popen(
                ["valgrind", "--leak-check=full", "--error-exitcode=1", 
                 "--quiet", "--log-fd=2", exe_file],
                stdin=stdin,
                stdout=stdout,
                stderr=subprocess.PIPE
            )
            
            try:
                # Wait for process to complete with timeout
                stdout_data, stderr_data = process.communicate(timeout=timeout)
                
                # Parse Valgrind's stderr output to detect leaks
                stderr_str = stderr_data.decode('utf-8', errors='ignore')
                
                # Search for "definitely lost" (confirmed leaks) and "indirectly lost" (leaked via other objects)
                # Valgrind reports these in format: "definitely lost: 1,234 bytes"
                definitely_lost = re.search(r'definitely lost: ([\d,]+) bytes', stderr_str)
                indirectly_lost = re.search(r'indirectly lost: ([\d,]+) bytes', stderr_str)
                
                def parse_bytes(s):
                    """Extract byte count from regex match, handling comma separators."""
                    if s:
                        return int(s.group(1).replace(',', ''))
                    return 0
                
                # Sum both types of leaks to get total leaked memory
                total_lost = parse_bytes(definitely_lost) + parse_bytes(indirectly_lost)
                
                # If any memory was leaked, report it
                if total_lost > 0:
                    return True, None, None, "LEAK"
                
                # No leaks detected - check if program executed successfully
                if process.returncode == 0:
                    return True, None, None, "OK"
                else:
                    return False, None, None, "Runtime error"
                    
            except subprocess.TimeoutExpired:
                # Process exceeded timeout - kill it and report timeout
                process.kill()
                process.wait()
                return False, None, None, "Timeout"
                
    except Exception as e:
        return False, None, None, str(e)


def run_test(exe_file, test_id, tests_dir):
    """
    Run a single test with memory leak detection and output validation.
    
    Args:
        exe_file: Path to the compiled executable
        test_id: Test number (0-29)
        tests_dir: Directory containing test files
    
    Returns:
        True if test passed (correct output and no leaks), False otherwise
    """
    input_file = os.path.join(tests_dir, f"test{test_id}.in")
    expected_output_file = os.path.join(tests_dir, f"test{test_id}.out")
    result_file = os.path.join(tests_dir, f"test{test_id}.res")

    # Validate that required test files exist
    if not os.path.isfile(input_file):
        print(f"Input file for test {test_id} not found")
        return False

    if not os.path.isfile(expected_output_file):
        print(f"Expected output file for test {test_id} not found")
        return False

    # Select timeout based on test size (operations grow exponentially)
    # Test sizes analyzed: 0-4: 100-40K, 5-9: 60K-100K, 10-19: 100K-800K, 
    #                     20-27: 800K-2.5M, 28-29: 5M-10M operations
    if test_id <= 4:
        timeout = TIMEOUT_SMALL
    elif test_id <= 9:
        timeout = TIMEOUT_MEDIUM
    elif test_id <= 19:
        timeout = TIMEOUT_LARGE
    elif test_id <= 27:
        timeout = TIMEOUT_VERY_LARGE
    else:  # test_id 28-29
        timeout = TIMEOUT_EXTREME

    # Run test with Valgrind to detect memory leaks
    memory_leak_detected = False
    memory_info = ""
    try:
        success, _, _, leak_status = detect_memory_leaks(
            exe_file, input_file, result_file, timeout
        )
        
        # Handle execution errors (runtime error, timeout, etc.)
        if not success:
            if leak_status == "Runtime error":
                print(f"Test {test_id} Failed: Runtime error")
            elif leak_status == "Timeout":
                print(f"Test {test_id} Failed: Timeout")
            else:
                print(f"Test {test_id} Failed: {leak_status}")
            return False
        
        # Determine memory leak status message
        if leak_status == "LEAK":
            memory_leak_detected = True
            memory_info = "Memory leak detected"
        else:
            memory_info = "No memory leaks detected"
            
    except Exception as e:
        print(f"Test {test_id} Failed: Runtime error ({e})")
        return False

    # Compare actual output with expected output
    with open(result_file, "r") as res, open(expected_output_file, "r") as expected:
        output_correct = res.read().strip() == expected.read().strip()
    
    # Report test result: test fails if either output is wrong OR memory leaks detected
    if memory_leak_detected:
        # Memory leak causes test to fail regardless of output correctness
        if output_correct:
            print(f"Test {test_id} FAILED: Output correct BUT {memory_info}")
        else:
            print(f"Test {test_id} FAILED: Wrong output AND {memory_info}")
        return False
    elif output_correct:
        # Both output correct and no leaks - test passes
        print(f"Test {test_id} PASSED: {memory_info}")
        return True
    else:
        # Output incorrect but no leaks - test fails
        print(f"Test {test_id} FAILED: Wrong output. {memory_info}")
        return False


def main():
    """
    Main function: compile code, check for Valgrind, and run all tests.
    """
    # Parse command-line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests_dir", default="./tests/", help="Directory containing test files")
    parser.add_argument("--code_dir", default="./", help="Directory containing source code")
    parser.add_argument("--compiler_path", default="g++", help="Path to C++ compiler")
    parser.add_argument("--clean", action="store_true", help="Remove all .res files and exit")
    parser.add_argument("--abort_on_fail", action="store_true", help="Stop after first test failure")
    parser.add_argument("-t", "--tests", type=int, nargs="*", default=None, help="Specific test numbers to run (default: all)")
    args = parser.parse_args()

    # Find all C++ source files
    source_files = glob.glob(os.path.join(args.code_dir, "*.cpp"))
    if not source_files:
        print("No .cpp files found")
        return -1

    # Compile all source files into executable
    exe_file = os.path.abspath(os.path.join(args.code_dir, "main.exe"))
    compile_cmd = [args.compiler_path] + COMPILATION_FLAGS + ["-o", exe_file] + source_files

    try:
        subprocess.run(compile_cmd, check=True)
    except subprocess.CalledProcessError:
        print("Compilation failed")
        return -1

    # Clean mode: remove all result files and exit
    if args.clean:
        for f in os.listdir(args.tests_dir):
            if f.endswith(".res"):
                os.remove(os.path.join(args.tests_dir, f))
        return 0

    # Valgrind is required for memory leak detection - check if available
    if not is_valgrind_available():
        print("Error: Valgrind is not available. Please install Valgrind to run memory leak detection.")
        print("On Ubuntu/Debian: sudo apt-get install valgrind")
        print("On macOS: brew install valgrind")
        return -1

    # Determine which tests to run
    if args.tests is None:
        # Run all tests found in tests directory
        tests = sorted(
            int(f.split("test")[1].split(".in")[0])
            for f in os.listdir(args.tests_dir)
            if f.endswith(".in")
        )
    else:
        # Run only specified tests
        tests = args.tests

    # Run each test
    for test_id in tests:
        ok = run_test(exe_file, test_id, args.tests_dir)
        # If abort_on_fail is set, stop after first failure
        if not ok and args.abort_on_fail:
            return 0

    return 0


if __name__ == "__main__":
    main()
