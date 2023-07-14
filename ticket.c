// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: ticket.c
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "accountTicketingUI.h"
#include "ticket.h"
#include "account.h"
#include "commonHelpers.h"
#include <string.h>

int loadTickets(Ticket tickets[], int ticketMaxSize)
{
    int i = 0;
    int j = 0;
    FILE* fp = NULL;
    fp = fopen("tickets.txt", "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d|%d|%d|%[^|]|%d|",
            &tickets[i].uniqueNumTicket,
            &tickets[i].customerAcctNum_Ticket,
            &tickets[i].ticketStatusIndicator,
            tickets[i].subjectLine,
            &tickets[i].counter_numOfMessageTicket) != EOF)
        {
            if (i == ticketMaxSize - 1)
            {
                i = ticketMaxSize;
                break;
            }

            if (tickets[i].counter_numOfMessageTicket > MAX_NUMBER_MESSAGE)
            {
                tickets[i].counter_numOfMessageTicket = MAX_NUMBER_MESSAGE;
                for (j = 0; j < tickets[i].counter_numOfMessageTicket; j++)
                {
                    fscanf(fp, "%c|%[^|]|%[^|]|",
                        &tickets[i].message[j].accountTypeAuthorMessage,
                        tickets[i].message[j].nameOfGivenUser,
                        tickets[i].message[j].messageDetails);
                }
                fscanf(fp, "%*[^\n]"); // skip to end of line
            }
            else
            {
                for (j = 0; j < tickets[i].counter_numOfMessageTicket; j++)
                {
                    fscanf(fp, "%c|%[^|]|%[^|]|",
                        &tickets[i].message[j].accountTypeAuthorMessage,
                        tickets[i].message[j].nameOfGivenUser,
                        tickets[i].message[j].messageDetails);
                }
            }

            i++;
        }
    }
    else
    {
        printf("ERROR: Unable to open file for reading.\n");
    }
    return i;
}

void updateTicket(Ticket* ticket, const struct Account account)
{
    int choice = 0;
    char option, closingMessageChoice;
    do
    {
        printf("----------------------------------------\n");
        printf("Ticket %06d - Update Options\n", ticket->uniqueNumTicket);
        printf("----------------------------------------\n");
        printf("Status  : %s\n", ticket->ticketStatusIndicator == 1 ? "ACTIVE" : "CLOSED");
        printf("Subject : %s\n", ticket->subjectLine);
        printf("----------------------------------------\n");
        printf("1) Modify the subject\n");
        printf("2) Add a message\n");
        printf("3) Close ticket\n");
        printf("0) Done\n");
        printf("Selection: ");
        choice = getIntFromRange(0, 3);
        switch (choice)
        {
        case 1:
            printf("\n");
            printf("Enter the revised ticket SUBJECT (30 chars. maximum): ");
            getCString(ticket->subjectLine, 1, 30);
            putchar('\n');
            break;
        case 2:
            printf("\n");
            if (ticket->counter_numOfMessageTicket + 1 > MAX_NUMBER_MESSAGE)
            {
                printf("ERROR: Message limit has been reached, call ITS Support!\n");
                printf("\n");
                break;
            }
            addAMessage(ticket, account);

            break;
            // Close Ticket
        case 3:
            printf("\n");
            if (ticket->ticketStatusIndicator == 1)
            {
                printf("Are you sure you CLOSE this ticket? ([Y]es|[N]o): ");
                option = getCharOption("YN");
                putchar('\n');
                if (option == 'Y')
                {
                    ticket->ticketStatusIndicator = 0; // Close the ticket
                    choice = 0;                        // to return to customer menu
                    if (ticket->counter_numOfMessageTicket == MAX_NUMBER_MESSAGE)
                    {
                        printf("*** Ticket closed! ***\n\n");
                    }

                    if (ticket->counter_numOfMessageTicket != MAX_NUMBER_MESSAGE)
                    {
                        printf("Do you want to leave a closing message? ([Y]es|[N]o): ");
                        closingMessageChoice = getCharOption("YN");
                        if (closingMessageChoice == 'Y')
                        {
                            putchar('\n');
                            addAMessage(ticket, account);
                            printf("*** Ticket closed! ***\n\n");
                        }
                        else
                            printf("\n*** Ticket closed! ***\n\n");
                    }
                }
                else
                    ticket->ticketStatusIndicator = 1; // Open the ticket
            }
            break;
        case 0:
            putchar('\n');
            break;

        default:
            break;
        }
    } while (choice != 0);
}

void addAMessage(Ticket* ticket, const struct Account account)
{
    int i; // index of empty spot
    for (i = 0; i < MAX_NUMBER_MESSAGE; i++)
    {
        if (ticket->message[i].messageDetails[0] == '\0')
        {
            break;
        }
    }
    printf("Enter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
    getCString(ticket->message[i].messageDetails, 1, 150);
    putchar('\n');

    strncpy(ticket->message[i].nameOfGivenUser, account.login.customerName, 31);

    ticket->message[i].accountTypeAuthorMessage = account.type;

    ticket->counter_numOfMessageTicket++;
}

void updateTicketAgent(Ticket* ticket, const struct Account account)
{
    int choice = 0;
    do
    {
        printf("----------------------------------------\n");
        printf("Ticket %06d - Update Options\n", ticket->uniqueNumTicket);
        printf("----------------------------------------\n");
        printf("Status  : %s\n", ticket->ticketStatusIndicator == 1 ? "ACTIVE" : "CLOSED");
        printf("Subject : %s\n", ticket->subjectLine);
        printf("Acct#   : %d\n", ticket->customerAcctNum_Ticket);
        printf("Customer: %s\n", ticket->message->nameOfGivenUser);
        printf("----------------------------------------\n");
        printf("1) Add a message\n");
        printf("2) Close ticket\n");
        printf("3) Re-open ticket\n");
        printf("0) Done\n");
        printf("Selection: ");
        choice = getIntFromRange(0, 3);
        switch (choice)
        {
            // Add a message
        case 1:

            if (ticket->counter_numOfMessageTicket + 1 > MAX_NUMBER_MESSAGE)
            {
                printf("\n");
                printf("ERROR: Message limit has been reached, call ITS Support!\n");
                printf("\n");
                break;
            }
            if (ticket->ticketStatusIndicator == 0)
            {
                printf("\n");
                printf("ERROR: Ticket is closed - new messages are not permitted.\n");
                printf("\n");
                break;
            }
            else
            {
                printf("\n");
                addAMessage(ticket, account);
            }

            break;
            // Close ticket
        case 2:

            if (ticket->ticketStatusIndicator == 0)
            {
                printf("\n");
                printf("ERROR: Ticket is already closed!\n");
                printf("\n");
            }
            else
            {
                printf("\n");
                printf("Are you sure you CLOSE this ticket? ([Y]es|[N]o): ");
                if (getCharOption("YN") == 'Y')
                {
                    putchar('\n');
                    ticket->ticketStatusIndicator = 0; // Close the ticket

                    if (ticket->counter_numOfMessageTicket == MAX_NUMBER_MESSAGE)
                    {
                        printf("*** Ticket closed! ***\n\n");
                    }

                    if (ticket->counter_numOfMessageTicket != MAX_NUMBER_MESSAGE)
                    {
                        printf("Do you want to leave a closing message? ([Y]es|[N]o): ");
                        ;
                        if (getCharOption("YN") == 'Y')
                        {
                            putchar('\n');
                            addAMessage(ticket, account);
                            printf("*** Ticket closed! ***\n\n");
                        }
                        else
                        {
                            putchar('\n');
                            printf("*** Ticket closed! ***\n\n");
                        }
                    }
                }
                else
                {
                    putchar('\n');
                    ticket->ticketStatusIndicator = 1; // Open the ticket
                }

                break;
                // Re-open ticket
        case 3:

            if (ticket->ticketStatusIndicator == 1)
            {
                printf("\n");
                printf("ERROR: Ticket is already active!\n");
                printf("\n");
            }
            else
            {
                printf("\n");
                printf("Are you sure you RE-OPEN this closed ticket? ([Y]es|[N]o): ");
                if (getCharOption("YN") == 'Y')
                {
                    putchar('\n');
                    ticket->ticketStatusIndicator = 1; // Active the ticket
                    printf("*** Ticket re-opened! ***\n");
                    printf("\n");
                }
                else
                {
                    putchar('\n');
                    ticket->ticketStatusIndicator = 0; // Stay closed
                }
                break;
        case 0:

            break;

        default:
            break;
            }
            }
        }

    } while (choice != 0);
}

int archiveClosedTickets(Ticket* tickets, int ticketMaxSize)
{
    int i = 0;
    int j = 0;
    int numbOfTicketsArched = 0;
    for (i = 0; i < ticketMaxSize; i++)
    { //* All tickets in closed state should append to tickets_arc.txt
        if (tickets[i].ticketStatusIndicator == 0 && tickets[i].uniqueNumTicket != 0)
        {
            FILE* fp;
            fp = NULL;
            fp = fopen("tickets_arc.txt", "a");
            if (fp == NULL)
                printf("ERROR: Can't open tickets_arc file.\n");

            fprintf(fp, "%d|%d|%d|%s|%d|",
                tickets[i].uniqueNumTicket,
                tickets[i].customerAcctNum_Ticket,
                tickets[i].ticketStatusIndicator,
                tickets[i].subjectLine,
                tickets[i].counter_numOfMessageTicket);

            for (j = 0; j < tickets[i].counter_numOfMessageTicket; j++)
            {
                fprintf(fp, "%c|%s|%s|\n",
                    tickets[i].message[j].accountTypeAuthorMessage,
                    tickets[i].message[j].nameOfGivenUser,
                    tickets[i].message[j].messageDetails);
            }

            fclose(fp);
            tickets[i].uniqueNumTicket = 0; //empty the closed tickets
            numbOfTicketsArched++;
        }
    }
    return numbOfTicketsArched;
}

void displayOneTicketRecordAgent(const Ticket* ticket)
{
    char status[7];
    if (ticket->ticketStatusIndicator == 1)
    {
        strcpy(status, "ACTIVE");
    }
    else
    {
        strcpy(status, "CLOSED");
    }

    printf("%06d %-5d %-15s %-6s %-30s %5d\n", ticket->uniqueNumTicket, ticket->customerAcctNum_Ticket, ticket->message->nameOfGivenUser, status, ticket->subjectLine, ticket->counter_numOfMessageTicket);
}

int saveTickets(Ticket* tickets, int maxArraySize)
{
    int i = 0;
    int j = 0;
    FILE* fp = NULL;
    fp = fopen("tickets.txt", "w"); // rewrite to textfile

    if (fp != NULL)
    {
        for (i = 0; i < maxArraySize; i++)
        {
            fprintf(fp, "%d|%d|%d|%s|%d|", tickets[i].uniqueNumTicket,
                tickets[i].customerAcctNum_Ticket,
                tickets[i].ticketStatusIndicator,
                tickets[i].subjectLine,
                tickets[i].counter_numOfMessageTicket);
            for (j = 0; j < tickets[i].counter_numOfMessageTicket; j++)
            {
                fprintf(fp, "%c|%s|%s|", tickets[i].message[j].accountTypeAuthorMessage,
                    tickets[i].message[j].nameOfGivenUser,
                    tickets[i].message[j].messageDetails);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }
    else
    {
        printf("ERROR: Unable to open file for reading.\n");
    }

    return i;
}

void viewArchivedTicketStatus()
{
    int i = 0;
    int j = 0;

    FILE* fp = NULL;
    fp = fopen("tickets_arc.txt", "r");
    struct Ticket bufferTicket;
    int uniqueIds[17];
    int numArchivedTickets = 0;
    int numArchivedMessages = 0;
    if (fp != NULL)
    {
        while (fscanf(fp, "%d|%d|%d|%[^|]|%d|",
            &bufferTicket.uniqueNumTicket,
            &bufferTicket.customerAcctNum_Ticket,
            &bufferTicket.ticketStatusIndicator,
            bufferTicket.subjectLine,
            &bufferTicket.counter_numOfMessageTicket) != EOF)
        {


            int isUnique = 1;
            for (j = 0; j < numArchivedTickets; j++)
            {
                if (bufferTicket.uniqueNumTicket == uniqueIds[j])
                {
                    isUnique = 0;
                }
            }

            if ((isUnique == 1) && (bufferTicket.uniqueNumTicket != 0))
            {
                numArchivedMessages += bufferTicket.counter_numOfMessageTicket;
                uniqueIds[numArchivedTickets] = bufferTicket.uniqueNumTicket;
                numArchivedTickets++;
            }


            if (bufferTicket.counter_numOfMessageTicket > MAX_NUMBER_MESSAGE)
            {
                bufferTicket.counter_numOfMessageTicket = MAX_NUMBER_MESSAGE;
                for (j = 0; j < bufferTicket.counter_numOfMessageTicket; j++)
                {
                    fscanf(fp, "%c|%[^|]|%[^|]|",
                        &bufferTicket.message[j].accountTypeAuthorMessage,
                        bufferTicket.message[j].nameOfGivenUser,
                        bufferTicket.message[j].messageDetails);
                }
                fscanf(fp, "%*[^\n]"); // skip to end of line
            }
            else
            {
                for (j = 0; j < bufferTicket.counter_numOfMessageTicket; j++)
                {
                    fscanf(fp, "%c|%[^|]|%[^|]|",
                        &bufferTicket.message[j].accountTypeAuthorMessage,
                        bufferTicket.message[j].nameOfGivenUser,
                        bufferTicket.message[j].messageDetails);
                }
            }

            i++;
        }
        fclose(fp);
        printf("There are %d ticket(s) and a total of %d message(s) archived.\n", numArchivedTickets, numArchivedMessages);
    }
    else
    {
        printf("ERROR: Unable to open file for reading.\n");
    }
}

int savingTicketChanges(struct Ticket* tickets, int maxArraySize)
{
    int i = 0;
    int j = 0;
    int numberOfTicketSaved = 0;
    FILE* fp = NULL;
    fp = fopen("tickets.txt", "w"); // rewrite to textfile

    if (fp != NULL)
    {
        for (i = 0; i < maxArraySize; i++)
        {
            if (tickets[i].uniqueNumTicket != 0)
            {

                fprintf(fp, "%d|%d|%d|%s|%d|", tickets[i].uniqueNumTicket,
                    tickets[i].customerAcctNum_Ticket,
                    tickets[i].ticketStatusIndicator,
                    tickets[i].subjectLine,
                    tickets[i].counter_numOfMessageTicket);
                for (j = 0; j < tickets[i].counter_numOfMessageTicket; j++)
                {
                    fprintf(fp, "%c|%s|%s|", tickets[i].message[j].accountTypeAuthorMessage,
                        tickets[i].message[j].nameOfGivenUser,
                        tickets[i].message[j].messageDetails);
                }
                fprintf(fp, "\n");
                if (tickets[i].uniqueNumTicket != 0)
                {
                    numberOfTicketSaved++;
                }
            }
        }
        fclose(fp);
    }
    else
    {
        printf("ERROR: Unable to open file for reading.\n");
    }

    return numberOfTicketSaved;
}