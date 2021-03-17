#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i 
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);



int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");

    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }


    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        
        printf("\n");
    }



    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // look for candidate called name
    for (int j = 0; j < candidate_count; j++)
    {
        if (strcmp(candidates[j].name, name) == 0)
        {
            preferences[voter][rank]=j; // add the candidate's index to the preference array for the voters rank
          
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // iterate thought voter preferences and add to the appropriate voter total
    for (int k = 0; k < voter_count; k++) 
    {
        if (!candidates[preferences[k][0]].eliminated) 
        {
            candidates[preferences[k][0]].votes++; 
        }
        else if (!candidates[preferences[k][1]].eliminated) 
        {
            candidates[preferences[k][1]].votes++;  
        }
        else if (!candidates[preferences[k][2]].eliminated) 
        {
            candidates[preferences[k][2]].votes++;  
        }
    }  
    
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int votes_needed_to_win;  // determiine how amy votes are needed to win
    if (voter_count % 2 > 0) 
    {
        votes_needed_to_win = (voter_count / 2) + 1;
    } 
    else 
    {
        votes_needed_to_win = (voter_count / 2) + 1;
    }

    // iterate through the candidates and find the candidate with the most votes
    for (int l = 0; l < candidate_count; l++) 
    {
        if (candidates[l].votes >= votes_needed_to_win) 
        {
            printf("%s\n", candidates[l].name); // after the winning voter is found printf then ame out
            return true;  
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min_vote = 1215752192; // set the maximun value that an integer can have.
    
    // read each candidates vote and if it is less than min_vote then assign a new value to min_vote
    for (int n = 0; n < candidate_count; n++)
    {
        if ((candidates[n].votes <= min_vote) && (!candidates[n].eliminated))
        {
            if (!candidates[n].eliminated)
            {
                min_vote = candidates[n].votes; 
            }
        }
    }
    return min_vote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int number_of_canditates_still_in_the_contest_with_min_votes = 1;
    
    // read through the candidates array to see if all the non eliminated candidatse have the same minium value.
    for (int p = 0; p < candidate_count; p++)
    {
        if (!candidates[p].eliminated)
        { 
            if (candidates[p].votes == min)
            {
                number_of_canditates_still_in_the_contest_with_min_votes++;   
            }
            else
            {
                return false;  
            }
        }
    }
    
    if (number_of_canditates_still_in_the_contest_with_min_votes > 1)
    {
        return true;   
    
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // read all the candidates vote totals. 
    for (int o = 0; o < candidate_count; o++)
    {
        if (candidates[o].votes == min) //Find the candidates that have the minimum votes. 
        {
            candidates[o].eliminated = true;  // eliminate the voter.
        
        }
    }
    return;
}
