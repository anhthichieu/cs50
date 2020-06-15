// https://cs50.harvard.edu/x/2020/psets/1/credit/

#include <stdio.h>
#include <cs50.h>


void spaces(int space);
void hashes(int hash);

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    for (int i = 0; i < n; i++)
    {
        spaces(n - i - 1);
        hashes(i + 1);
        spaces(2);
        hashes(i + 1);
        printf("\n");
    }
}


void spaces(int space)
{
    for (int i = 0; i < space; i++)
    {
        printf(" ");
    }
}

void hashes(int hash)
{
    for (int i = 0; i < hash; i++)
    {
        printf("#");
    }
}