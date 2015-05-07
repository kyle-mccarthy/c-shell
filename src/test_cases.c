/**
 * Build:    gcc test_cases.c -o testCases -lreadline
 */
#include "../include/shell_utils.h"
#include <stdio.h>

void testCase(int number, int want_status, int status);

int main(int argc, char * argv[])
{
    printf("%s\n", "TEST CASES");
    testCase(1, -1, cd("/var/www"));
    testCase(2, 1, cd("/vagrant"));
    testCase(3, 1, pwd());
    testCase(4, 1, ls(""));
    testCase(5, 1, cd("pr4"));
    testCase(6, 1, ls(""));
    return(0);
}

void testCase(int number, int want_status, int status) {
    printf("%s %d %s\n", "TEST CASE #", number, ((status == want_status) ? "PASSED" : "FAILED"));
}