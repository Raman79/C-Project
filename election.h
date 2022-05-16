#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
struct currentValidID
{
    int year;
    char branch[6];
    int totalVoters;
};
typedef struct candidate
{
    int cid;
    char cname[20];
    int votes;
} CANDIDATE;

// GLOBALS
struct currentValidID currentValidID; // stores current Valid user ID parameters
CANDIDATE candidateArray[20];         // to store information all candidates
int numberOfCandidates;               // Total number of candidates standing for election
char studentVotes[180];               // to store information of votes given by each student
int presentYear = 2022;
int numberofStudents = 180;
// ADMIN PANEL OPTION

// Extracting branch name, usn and year for example 1DT20CSE109 branch: CSE USN:109 Year:2020

int extractbranch(char inputID[11])
{
    char branch[5];
    int i;
    for (i = 5; i < 7; i++)
    {
        branch[i - 5] = inputID[i];
    }
    branch[i - 5] = '\0';
    if (strcmp(branch, currentValidID.branch) == 0) // Checking the branch codes
        return 1;
    else
        return 0;
}
int extractUSN(char inputID[11])
{
    int usn = 0;
    char numstr[3];
    int i = 7;
    for (i = 7; inputID[i] != '\0'; ++i)
    {
        numstr[i - 7] = inputID[i];
    }
    numstr[i - 7] = '\0';
    usn = atoi(numstr);
    return usn;
}

int extractYear(char inputID[11])
{
    int i, year = 0;
    char numstr[4] = {"20"};
    for (i = 3; i <= 4; i++)
    {
        numstr[i - 1] = inputID[i];
    }
    numstr[4] = '\0';
    year = atoi(numstr);
    return year;
}
void initiateNewElection()
{
    printf("\nNew Election Initiation:\n");

    printf("\nElections for which year students: ");
    scanf("%d", &currentValidID.year);
    printf("Enter branch code:");
    scanf("%s", currentValidID.branch);
    printf("Enter number of voters/students:");
    scanf("%d", &currentValidID.totalVoters);
    printf("Enter the no. of candidates:");
    scanf("%d", &numberOfCandidates);

    for (int i = 0; i < numberOfCandidates; i++)
    {
        candidateArray[i].cid = i + 1;
        printf("Enter name of candidate %d: ", i + 1);
        scanf(" %s", candidateArray[i].cname);
        candidateArray[i].votes = 0;
    }
    return;
}
void saveElectionInfoInFile()
{
    printf("Saving Election Info in File...\n");
    FILE *fp = fopen("ElectionInfo.txt", "w");
    if (fp == NULL)
    {
        printf("\nError in file creation\n");
        fclose(fp);
        return;
    }
    fprintf(
        fp, "%d\n%s\n%d\n%d",
        currentValidID.year,
        currentValidID.branch,

        currentValidID.totalVoters,
        numberOfCandidates);
    fclose(fp);
    printf("Saved Successfully : )\n");
}
void deleteIllegalVote(char userID[15])
{
    FILE *fp, *fcp;
    char filename[20];
    char line[20];

    int location = extractUSN(userID);
    sprintf(filename, "candidate%d.txt", candidateArray[studentVotes[location - 1] - 49].cid);
    candidateArray[studentVotes[location - 1] - 49].votes--;
    studentVotes[location - 1] = '0';
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("\nFile cannot be opened...Operation Failed");
        return;
    }
    printf("\nDeleting in process...\n ");
    if ((fcp = fopen("tmp.txt", "w")) == NULL)
    {
        printf("\nFile cannot be opened...Operation Failed");
        return;
    }

    while (!feof(fp))
    {
        fscanf(fp, "%s", line);
        fprintf(fcp, "%s\n", line);
    }
    fclose(fp);
    fclose(fcp);
    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("\nFile cannot be opened...Operation Failed");
        return;
    }
    int numFromFile;
    char cnameFromFile[20];
    fcp = fopen("tmp.txt", "r");
    fscanf(fcp, "%d", &numFromFile);
    fprintf(fp, "%d", numFromFile - 1);
    fscanf(fcp, "%s", cnameFromFile);
    fprintf(fp, "\n%s", cnameFromFile);
    while (!feof(fcp))
    {
        fscanf(fcp, "%d", &numFromFile);
        if (numFromFile != location)
            fprintf(fp, "\n%d", numFromFile);
    }
    fclose(fp);
    fclose(fcp);
    remove("tmp.txt");
    printf("\nVote deleted successfully\nPress any key to continue...");
    getch();
}

void createCandidateFile()
{
    printf("\nCreating Candidate File...\n");
    FILE *candidateFile;
    char filename[20];
    for (int i = 1; i <= numberOfCandidates; i++)
    {
        sprintf(filename, "candidate%d.txt", i);
        candidateFile = fopen(filename, "w");
        fprintf(candidateFile, "\n%s", candidateArray[i - 1].cname);
        fprintf(candidateFile, "\n%d", candidateArray[i - 1].votes);
        fclose(candidateFile);
    }
    printf("\n Created files Succesfully! \n");
}

void loadElectionInfoFromFile()
{
    FILE *f1, *f2, *f3;
    f1 = fopen("ElectionInfo.txt", "r");
    if (f1 == NULL)
        printf("Not Exist");
    fscanf(f1, "%d", &currentValidID.year);
    printf("\nElections for year  :%d\n", currentValidID.year);
    fseek(f1, 2, SEEK_CUR);
    fscanf(f1, "%s", currentValidID.branch);
    printf(" Branch: %s\n", currentValidID.branch);
    fseek(f1, 2, SEEK_CUR);
    fscanf(f1, "%d", &currentValidID.totalVoters);
    printf("Total Voters: %d\n", currentValidID.totalVoters);
    fseek(f1, 2, SEEK_CUR);
    fscanf(f1, "%d", &numberOfCandidates);
    printf(" Number of Candidates: %d\n", numberOfCandidates);
    fclose(f1);

    // load candidates info and student votes
    for (int i = 0; i < currentValidID.totalVoters; i++)
    {
        studentVotes[i] = '0';
    }
    for (int i = 1; i <= numberOfCandidates; i++)
    {
        int location;
        char filename[20];
        sprintf(filename, "candidate%d.txt", i);
        f2 = fopen(filename, "r+");
        candidateArray[i - 1].cid = i;
        fscanf(f2, "%d", &candidateArray[i - 1].votes);
        printf("Votes: %d\n", candidateArray[i - 1].votes);
        fscanf(f2, "%s", candidateArray[i - 1].cname);
        printf("Candidate name:  %s\n", candidateArray[i - 1].cname);
        while (!feof(f2))
        {
            fscanf(f2, "%d", &location);
            studentVotes[location - 1] = i + 48;
        }
        fclose(f2);
    }
}
int getWinner()
{
    int maxV = -1;
    int winnerCid;
    for (int i = 0; i < numberOfCandidates; i++)
    {
        if (candidateArray[i].votes > maxV)
        {
            winnerCid = candidateArray[i].cid;
            maxV = candidateArray[i].votes;
        }
        else if (candidateArray[i].votes == maxV)
        {
            return -1;
        }
    }
    return winnerCid;
}
int authenticateAdmin()
{
    char username[6], password[6];

    printf("\nEnter username: ");
    scanf("%s", username);
    if ((strcmp(username, "Admin")) != 0)
        return 0;
    else
    {
        printf("Enter Password: \n");
        int i = 0;
        for (i = 0; i < 5; i++)
        {
            password[i] = getch();
            printf("%c", '*');
        }
        password[i] = '\0';
        if ((strcmp(password, "admin")) != 0)
            return 0;
    }
    return 1;
}

void adminPanel()
{

    while (1)
    {
        if (authenticateAdmin() != 1)
        {
            printf("\nInvalid Username or Password\n");
            break;
        }

        printf("\nLog in succesfull!");
        getch();
        while (1)
        {

            char inputID[15];
            char ip;
            char banInp;
            int WinnerCid, totalVotedNow = 0;
            printf("\n1.New Election\n2.Continue Previous Election\n3.Result\nOption:");
            scanf(" %c", &ip);
            switch (ip)
            {
            case '1':
                initiateNewElection();
                saveElectionInfoInFile();
                createCandidateFile();
                break;
            case '2':
                loadElectionInfoFromFile();
                break;
            case '3':
                WinnerCid = getWinner();
                if (WinnerCid != -1)
                {
                    printf("\nWinner is %s with %d votes\n", candidateArray[WinnerCid - 1].cname, candidateArray[WinnerCid - 1].votes);
                }
                else
                {
                    printf("\nIts A TIE");
                }
                printf("\nFull Result\n");
                for (int i = 0; i < numberOfCandidates; i++)
                {
                    totalVotedNow += candidateArray[i].votes;
                    printf("%d. %s -> %d votes\n", candidateArray[i].cid, candidateArray[i].cname, candidateArray[i].votes);
                }
                printf("\nVoting Percentage: %d %%\n\n", (totalVotedNow * 100) / currentValidID.totalVoters);
                break;
            default:
                printf("Invalid!");
                return;
            }
        }
    }
}
// STUDENT PANEL OPTION
void saveVote(char userID[15], char voteInput, int number)
{
    char filename[20];
    sprintf(filename, "candidate%d.txt", voteInput - 48);
    FILE *fp = fopen(filename, "r+");
    int location = number;
    studentVotes[location - 1] = voteInput;
    candidateArray[voteInput - 49].votes++;
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", candidateArray[voteInput - 49].votes);
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "\n%d", location);
    fclose(fp);
}
int authStudent()
{
    char USN[11];
    FILE *f1;
    f1 = fopen("ElectionInfo.txt", "w");
    int year, number, branch;
    printf("\nEnter USN: \n");
    scanf("%s", USN);
    number = extractUSN(USN);
    year = extractYear(USN);
    fscanf(f1, "%d", &numberOfCandidates);
    printf("%d", number);
    printf("\n%d", year);
    branch = extractbranch(USN);
    if (number <= numberOfCandidates && year == currentValidID.year)
    {
        if (branch == 1)
            printf("\nValid number and year..\n");
        return 0;
    }
    return 1;
    fclose(f1);
}
void studentPanel()
{
    while (1)
    {
        char usn[10], vote;
        int year, number, branch;
        printf("\nEnter your USN\n");
        scanf("%s", usn);
        printf("%s", usn);
        // if (usn[10] != '\0')
        // {
        //     printf("Invalid USN");
        //     break;
        // }
        // else
        //{
        year = extractYear(usn);
        number = extractUSN(usn);
        branch = extractbranch(usn);
        if (year == currentValidID.year && number <= 180 && branch == 1)
        {

            printf("\n Valid USN!\n");
            printf("Canditate Names:");
            for (int i = 0; i < numberOfCandidates; i++)
            {
                printf("\n%d %s", i + 1, candidateArray[i].cname);
            }
            printf("\nEnter your vote according to the serial number: ");
            vote = getch();
            printf("*");
            if (vote - 48 > numberOfCandidates || vote - 48 < 1)
            {
                printf("\nInValid Vote:(...Try Again\n");
                getch();
                continue;
            }
            saveVote(usn, vote, number);
            printf("\nGreat Job :)!\n");
        }
        else
        {
            printf("Invalid USN");
            break;
        }
    }
}