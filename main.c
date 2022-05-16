#include "election.h"
int main()
{
    while (1)
    {
        int ch;
        printf("\n1.Admin Panel\n2.Student Panel\n3.Exit\n");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            adminPanel();
            break;
        case 2:
            studentPanel();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid Option");
            break;
        }
    }
    return 0;
}