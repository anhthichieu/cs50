#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];
int lockPrint[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2]; // so cap: [nx(n-2)]/2

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

void printArray(int A[], int size);
void print2DArray(int A[9][9], int row, int col);
void printStructArray(pair a[], int n);
bool createsCircle(bool arr[MAX][MAX], int max_index);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            lockPrint[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // printf("Ranks array = ");
        // printArray(ranks, candidate_count);
        // printf("\n");

        // printf("Init Preferences array: \n");
        // print2DArray(preferences, candidate_count, candidate_count);

        record_preferences(ranks);

        // printf("Preferences array: \n");
        // print2DArray(preferences, candidate_count, candidate_count);

        // printf("\n");
    }

    // printf("Preferences array: \n");
    // print2DArray(preferences, candidate_count, candidate_count);

    add_pairs();

    // printf("Pairs array: \n");
    // printStructArray(pairs, pair_count);
    // printf("\n");

    sort_pairs();

    // printf("Pairs array after sorting:\n");
    // printStructArray(pairs, pair_count);
    // printf("\n");

    // printf("Init locked array: \n");
    // print2DArray(lockPrint, candidate_count, candidate_count);

    lock_pairs();

    // printf("Final locked array: \n");
    // print2DArray(lockPrint, candidate_count, candidate_count);

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int vote_check = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            vote_check++;
        }
    }

    if (vote_check != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int a, b, i, j;
    for (a = 0; a < candidate_count - 1; a++)
    {
        for (b = a + 1; b < candidate_count; b++)
        {
            i = ranks[a];
            // printf("i = %i\n", ranks[a]);
            j = ranks[b];
            // printf("j = %i\n", ranks[b]);
            preferences[i][j] += 1;
            // print2DArray(preferences, candidate_count, candidate_count);
            // printf("preferences[i][j] = %i\n", preferences[j][j]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                //   printf("preferences[%i][%i] = %i\n", i, j, preferences[i][j]);
                //   printf("preferences[%i][%i] = %i\n", j, i, preferences[j][i]);
                pairs[pair_count].winner = i;
                //   printf("pairs[%i].winner %i\n", pair_count, pairs[pair_count].winner);
                pairs[pair_count].loser = j;
                //   printf("pairs[%i].loser %i\n", pair_count, pairs[pair_count].loser);
                pair_count++;
            }
        }
    }
    // printf("\n");
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i, j, max_idx, temp_strength;
    pair temp_pair;
    int victory_strength[MAX];

    for (i = 0; i < pair_count; i++)
    {
        victory_strength[i] = preferences[pairs[i].winner][pairs[i].loser];
    }
    // printf("victory_strength array = ");
    // printArray(victory_strength, pair_count);
    // printf("\n");

    // One by one move boundary of unsorted subarray
    for (i = 0; i < pair_count - 1; i++)
    {
        // Find the maximum element in unsorted array

        max_idx = i;

        for (j = i + 1; j < pair_count; j++)
        {
            if (victory_strength[j] > victory_strength[max_idx])
                max_idx = j;
        }

        // Swap the found maximum element with the first element
        temp_strength = victory_strength[max_idx];
        victory_strength[max_idx] = victory_strength[i];
        victory_strength[i] = temp_strength;

        temp_pair = pairs[max_idx];
        pairs[max_idx] = pairs[i];
        pairs[i] = temp_pair;
    }

    // printf("victory_strength array after sorting = ");
    // printArray(victory_strength, pair_count);
    // printf("\n");
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int winnerIdx, loserIdx, lockedCheckIdx, emptyRowIdx;
    bool createsCircle = false;

    int edgeCount = 0;
    int rowEdgeCount;

    for (int i = 0; i < pair_count; i++)
    {
        winnerIdx = pairs[i].winner;
        loserIdx = pairs[i].loser;

        if (winnerIdx > loserIdx)
        {
            lockedCheckIdx = winnerIdx;
        }
        else
        {
            lockedCheckIdx = loserIdx;
        }

        edgeCount = 0;
        // printf("lockedCheckIdx = %i\n", lockedCheckIdx);

        for (int a = 0; a <= lockedCheckIdx; a++)
        {
            rowEdgeCount = 0;
            for (int b = 0; b <= lockedCheckIdx; b++)
            {
                if (locked[a][b] == true)
                {
                    rowEdgeCount++;
                }
            }

            if (rowEdgeCount == 0)
            {
                emptyRowIdx = a;
            }
            else if (rowEdgeCount == 1)
            {
                edgeCount++;
            }

            // printf("check circle lan thu %i\n", a + 1);
            // printf("emptyRowIdx = %i\n", emptyRowIdx);
            // printf("rowEdgeCount = %i\n", rowEdgeCount);
            // printf("edgeCount = %i\n", edgeCount);
            // printf("winnerIdx = %i\n", winnerIdx);

            if (edgeCount == lockedCheckIdx && winnerIdx == emptyRowIdx)
            {
                createsCircle = true;
            }
        }

        if (!createsCircle)
        {
            locked[winnerIdx][loserIdx] = true;
            lockPrint[winnerIdx][loserIdx] = 1;
        }
        // printf("Locked array: \n");
        // print2DArray(lockPrint, candidate_count, candidate_count);
        // printf("\n");
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int edgeCount;
    for (int i = 0; i <= candidate_count; i++)
    {
        edgeCount = 0;
        for (int j = 0; j <= candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                edgeCount++;
            }
        }
        // printf("edgeCount = %i\n", edgeCount);
        if (edgeCount == 0)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

// Circle check
// bool createsCircle(bool arr[MAX][MAX], int maxIndex)
// {

//     typedef struct
// {
//     bool circleCheck;
//     int remainingRowIdx;
// }
// circleOutput;

//     int edgeCount = 0;
//     int rowEdgeCount = 0;
//     circleOutput circleCheckOutput;

//     for (int i = 0; i <= maxIndex; i++)
//         {
//             for (int j = 0; j <= maxIndex; j++)
//             {
//                 if (arr[i][j] == true)
//                     {
//                         rowEdgeCount++;
//                     }
//             }

//             // printf("rowEdgeCount = %i\n", rowEdgeCount);
//             if (rowEdgeCount == 1)
//             {
//                     edgeCount++;
//             }
//             else if (rowEdgeCount == 0)
//             {
//                 circleCheckOutput.remainingRowIdx = i;
//             }

//             rowEdgeCount = 0;
//         }

//     // printf("edgeCount = %i\n", edgeCount);
//     // printf("maxIndex = %i\n", maxIndex);

//     // if (edgeCount == maxIndex)
//     //     {
//     //         return true;
//     //     }
//     // else
//     //     {
//     //         return false;
//     //     }

//     if (edgeCount == maxIndex)
//         {
//             circleCheckOutput.circleCheck = true;
//         }
//     else
//         {
//             circleCheckOutput.circleCheck = false;
//         }

//         return circleCheckOutput;
// }

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%i ", A[i]);
    printf("\n");
}

void print2DArray(int A[9][9], int row, int col)
{
    int i;
    int j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            // Print out each element
            printf("%i", A[i][j]);
        }
        // Print new line character after the row is printed in above loop
        printf("\n");
    }
    // return;
}

void printStructArray(pair a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Cap thu %i:\n", i + 1);
        printf("Winner: %i\n", a[i].winner);
        printf("Loser: %i\n", a[i].loser);
        printf("\n");
    }
}
