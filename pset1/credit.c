// https://cs50.harvard.edu/x/2020/psets/1/mario/more/

#include <stdio.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    // Prompt user for credit card number
    long number = get_long("Number: ");

    if (number < 1e14 || number > 1e16) 
    {
        printf("INVALID\n");
        return (0);
    }

    // Checksum calculation
    long dup;
    long sumDup = 0;
    long calc;
    long sumCalc = 0;
    long check = number;
    int digitCount = 0;
    do
    {
        // Multiply every-other-digit by 2, starting from second-to-last digit
        dup = ((check % 100) / 10) * 2;

        // Add products’ digits together
        if (dup < 10)
        {
            sumDup += dup;
        }
        else
        {
            sumDup += (dup % 10) + 1;
        }

        // Calculate sum of the digits that weren’t multiplied by 2
        calc = check % 10;
        sumCalc += calc;
  
        // Card length
        if (check >= 10)
        {
            digitCount += 2;
        }
        else
        {
            digitCount += 1;
        }
        
        check = check / 100;

    }
    while (check > 1);

    long checkSum = sumDup + sumCalc;
    // End of checkSum calculation

    // Check starting digits
    long startingDigits = number / (double) pow(10, digitCount - 2);

    // Check card type
    if (checkSum % 10 == 0)
    {
        if (digitCount == 15 && (startingDigits == 34 || startingDigits == 37))
        {
            printf("AMEX\n");
        }
        else if (digitCount == 16 && (startingDigits >= 51 && startingDigits <= 55))
        {
            printf("MASTERCARD\n");
        }
        else if ((digitCount == 16 || digitCount == 13) && (startingDigits / 10 == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
