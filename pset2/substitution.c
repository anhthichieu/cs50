// https://cs50.harvard.edu/x/2020/psets/2/substitution/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Remind user to use command-line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    // Validate key
    int keyLength = strlen(argv[1]);
    
    // Check key length
    if (keyLength != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }
    
    for (int i = 0; i < keyLength; i++)
    {
        argv[1][i] = tolower(argv[1][i]);
        
        // Check non-alphabetic characters
        if (!isalpha(argv[1][i]))
        {
            printf("Key contains a character(s) that is not alphabetic\n");
            return 1;
        }
        
        // Check repeated characters
        for (int j = i + 1; j < keyLength; j++) 
        {
            argv[1][j] = tolower(argv[1][j]);
            
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must not contain repeated characters\n");
                return 1;
            }
        }
    }
    // End of key validation
    
    // Prompt user for plainText
    string plainText = get_string("plaintext: ");
    
    int plainTextLength = strlen(plainText);
    int printIndex;
    
    printf("ciphertext: ");
    
    for (int i = 0; i < plainTextLength; i++)
    {
        if (isalpha(plainText[i]))
        {
            // Letter mapping & preserve case
            if (isupper(plainText[i]))
            {
                printIndex = plainText[i] - 65;
                argv[1][printIndex] = toupper(argv[1][printIndex]);
            }
            else if (islower(plainText[i]))
            {
                printIndex = plainText[i] - 97;
                argv[1][printIndex] = tolower(argv[1][printIndex]);
            }
            printf("%c", argv[1][printIndex]);
        }
        else // Print all non-alphabetic characters
        {
            printf("%c", plainText[i]);
        }
    }
    printf("\n");
    return 0;
}
