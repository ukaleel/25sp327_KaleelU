#include <stdio.h>
#include <stdlib.h>
#include <errno.h>




void print_error_msg_temp(){
    fprintf(stderr, "Usage: ./future_value <monthly-investment> <annual-interest-rate> <number-of-years>\n\n");
    fprintf(stderr, "monthly-investment: numeric value in range [1, 1000] representing the amount invested each month.\n");
    fprintf(stderr, "annual-interest-rate: decimal value in range [1, 15] representing the annual interest rate.\n");
    fprintf(stderr, "number-of-years: integer value in range [1, 50] representing the duration of the investment in years.\n");
    


}

void display_results(int monthlyInvestment, double annualInterestRate, int numYears, double futureValue, double results[][2]){
    printf("Monthly Investment: %.2f\n", (double)monthlyInvestment);
    printf("Yearly Interest Rate: %.2f%%\n", annualInterestRate);
    printf("Years: %d\n\n", numYears);
    printf("Future value after %d years is $%.0f.\n\n", numYears, futureValue);
    printf("Year Accum Interest        Value\n");
    printf("==== ============== ===========\n");

    for (int i=0; i<numYears; ++i){
        printf("%4d %14.2f %12.2f\n", i+1, results[i][0], results[i][1]);
    }


}

void calculate_future_vals_and_interest(double monthlyInvestment, double annualInterestRate, int numYears){
    double monthlyInvestmentRate = annualInterestRate / 12 / 100;
    int months = numYears * 12;
    double futureValue;
    double resultsStorage[numYears][2];

    for (int i=0; i < months; ++i){
        futureValue += monthlyInvestment;
        double monthlyInterestAccrual = futureValue * monthlyInvestmentRate;
        futureValue += monthlyInterestAccrual;


        if ((i+1)%12==0){//if its the end of the year
            resultsStorage[i/12][0] = futureValue - (monthlyInvestment *(i+1));
            resultsStorage[i/12][1] = futureValue;
        }
    }

    display_results(monthlyInvestment, annualInterestRate, numYears, futureValue, resultsStorage);





}

long convert_to_long(const char *strval, const char *inputName){
    char *endptr;
    errno = 0;
    long lval = strtol(strval, &endptr, 10);
    if (errno != 0 || *endptr != '\0'){
        fprintf(stderr,"Error: Invalid input for %s\n", inputName);
        print_error_msg_temp();
        exit(EXIT_FAILURE);
    }
    return lval;
}

double convert_to_double(const char *strval, const char *inputName){
    char *endptr;
    errno = 0;
    double dval = strtod(strval, &endptr);
    if (errno != 0 || *endptr != '\0'){
        fprintf(stderr,"Error: Invalid input for %s\n", inputName);
        print_error_msg_temp();
        exit(EXIT_FAILURE);
    }
    return dval;
}


void input_validation_algebraic(double monthlyInvestment, double annualInterestRate, int numYears){
    if (monthlyInvestment < 1 || monthlyInvestment > 1000){
        fprintf(stderr, "Error: Invalid input for monthly investment.\n");
        print_error_msg_temp();
        exit(EXIT_FAILURE);
    }
    if (annualInterestRate < 1 || annualInterestRate > 15){
        fprintf(stderr, "Error: Invalid input for annual interest rate.\n");
        print_error_msg_temp();
        exit(EXIT_FAILURE);
    }
    if (numYears < 1 || numYears > 50){
        fprintf(stderr, "Error: Invalid input for number of years.\n");
        print_error_msg_temp();
        exit(EXIT_FAILURE);
    }


}


int main(int argc, char *argv[]){
    if (argc != 4){
        print_error_msg_temp();
        return EXIT_FAILURE;
    }
    
    double monthlyInvestment = convert_to_double(argv[1], "monthly investment. ");
    double annualInterestRate = convert_to_double(argv[2], "annual interest rate.");
    long numYears = convert_to_long(argv[3], "number of years. ");
    input_validation_algebraic(monthlyInvestment, annualInterestRate, numYears);
    calculate_future_vals_and_interest(monthlyInvestment, annualInterestRate, numYears);
    return EXIT_SUCCESS;
}




// exit(EXIT_FAILURE);

// fprintf(stderr, "error message"); for errors