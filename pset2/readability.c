#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    
    // Prompt user for string
    string text = get_string("Please input your text:  ");

    // Counting
    int letters = 0;
    int words = 0;
    int sentences = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (letters == 1)
        {
            words = 1;
        }

        if (isalpha(text[i]))
        {
            letters += 1;
        }

        if (isblank(text[i]))
        {
            words += 1;
        }

        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences += 1;
        }

    }
    // printf("%i letter(s)\n", letters);
    // printf("%i word(s)\n", words);
    // printf("%i sentence(s)\n", sentences);

    // Coleman-Liau Calculation
    // L: average number of letters per 100 words
    float L = 100 * (float) letters / (float) words;

    // S: average number of sentences per 100 words
    float S = 100 * (float) sentences / (float) words;

    // Coleman-Liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;
    // printf ("index = %f\n", index);
    
    // Grade return
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", round(index));
    }
}