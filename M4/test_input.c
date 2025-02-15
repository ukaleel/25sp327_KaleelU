#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define UNUSED(x)  (void) (x)
bool validate_double(char* input, double low, double high, double* number, char* err_msg);

void test_validate_double_when_valid(void **state) {
    UNUSED(state);

    // Arrange
    double number;
    char err_msg[100];
    
    // Act
    bool result = validate_double("5.5\n", 0.0, 10.0, &number, err_msg);

    // Assert
    assert_true(result);
    assert_double_equal(number, 5.5, 0.001);
}

void test_validate_double_when_range_error(void **state){
    UNUSED(state);

    //Arrange
    double number;
    char err_msg[100];

    //Act

    bool result = validate_double("98.0\n", 0.0, 10.0, &number, err_msg);

    //Assert
    assert_false(result);
    assert_false(result);
    assert_string_equal(err_msg, "Entry must be greater than 0.00 and less than or equal to 10.00. Please try again.");
}


void test_validate_double_when_non_numeric(void **state){
    UNUSED(state);

    //Arrange
    double number;
    char err_msg[100];

    //Act

    bool result = validate_double("nonnumeric\n", 0.0, 10.0, &number, err_msg);

    //Assert
    assert_false(result);
    assert_string_equal(err_msg, "Entry must be numeric. Please try again.");
}

void test_validate_double_when_more_entries(void **state){
    UNUSED(state);

    //Arrange
    double number;
    char err_msg[100];

    //Act

    bool result = validate_double("1.9 3.4 5.6\n", 0.0, 10.0, &number, err_msg);

    //Assert
    assert_false(result);
    assert_string_equal(err_msg, "Entry must be a single numeric value. Please try again.");

}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_validate_double_when_valid),
        cmocka_unit_test(test_validate_double_when_more_entries),
        cmocka_unit_test(test_validate_double_when_non_numeric),
        cmocka_unit_test(test_validate_double_when_range_error)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}