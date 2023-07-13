// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: accountTicketingUI.c
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "commonHelpers.h"
#include "account.h"

#include "accountTicketingUI.h"

void displayAccountSummaryHeader(void)
{
	printf("Acct# Acct.Type Birth\n");
	printf("----- --------- -----\n");
}

void displayAccountDetailHeader(void)
{
	printf("Acct# Acct.Type Birth Income      Country    Disp.Name       Login      Password\n");
	printf("----- --------- ----- ----------- ---------- --------------- ---------- --------\n");
}

void displayAccountSummaryRecord(const struct Account* account)
{
    printf("%05d %-9s %5d\n", account->accountNumber, account->type == 'A' ? "AGENT" : "CUSTOMER", account->demo.birthYear);
}

void displayAccountDetailRecord(const struct Account* account)
{
    char accountTypeDisplayCustomer[9] = "CUSTOMER";
    char accountTypeDisplayAgent[6] = "AGENT";

    char hiddenPassword[31];
    int i=0;
    for(i=0;i<31;i++){
        if(i%2==0){
            hiddenPassword[i] = account->login.password[i];
        }else{
            hiddenPassword[i] = '*';
        }
    }

	printf("%05d %-9s %5d $%10.2lf %-10s %-15s %-10s %8s\n", account->accountNumber, account->type == 'C' ? "CUSTOMER" : "AGENT", account->demo.birthYear, account->demo.income, account->demo.country, account->login.customerName, account->login.loginName, hiddenPassword);
}

void displayTicketHeader(void)
{
    printf("------ ------ ------------------------------ --------\n");
    printf("Ticket Status Subject                        Messages\n");
    printf("------ ------ ------------------------------ --------\n");
}

void displayOneTicketRecord(const Ticket* ticket)
{
    char ticketStatusIndicator[7];
    if (ticket->ticketStatusIndicator == 1)
    {
        strcpy(ticketStatusIndicator, "ACTIVE");
    }
    else
    {
        strcpy(ticketStatusIndicator, "CLOSED");
    }

    printf("%06d %-6s %-30s %5d\n", ticket->uniqueNumTicket, ticketStatusIndicator, ticket->subjectLine, ticket->counter_numOfMessageTicket);
}

void displayAllTicketsForAccount(const struct AccountTicketingData* data, int accountNumber)
{
    int i;
    displayTicketHeader();
    for (i = 0; i < data->TICKET_MAX_SIZE; i++)
    {
        if (data->tickets[i].customerAcctNum_Ticket == accountNumber)
        {
            displayOneTicketRecord(&data->tickets[i]);
        }
    }
    printf("------ ------ ------------------------------ --------\n");
}

void applicationStartup(struct AccountTicketingData* acctTData)
{
    menuLogin(acctTData);
    printf("==============================================\n");
    printf("Account Ticketing System - Terminated\n");
    printf("==============================================\n");
}

int menuLogin(struct AccountTicketingData* acctTData)
{
    int choice, index = -1;
    int attempts;
    char option;
    do
    {
        printf("==============================================\n");
        printf("Account Ticketing System - Login\n");
        printf("==============================================\n");
        printf("1) Login to the system\n");
        printf("0) Exit application\n");
        printf("----------------------------------------------\n");
        printf("\n");
        printf("Selection: ");
        choice = getIntFromRange(0, 1);
        putchar('\n');
        switch (choice)
        {
        case 0:
            printf("Are you sure you want to exit? ([Y]es|[N]o): ");
            option = getCharOption("yYnN");
            if (option == 'N' || option == 'n')
                choice = 1;
            else if (option == 'Y' || option == 'y')
                choice = -1;
            putchar('\n');
            break;
        case 1:
            attempts = 0;
            int result = 0;
            do
            {
                result = isBadAttempt(acctTData, &index);

                if (result == 1)
                {
                    attempts++;
                    printf("INVALID user login/password combination! [attempts remaining:%d]\n", 3 - attempts);
                    putchar('\n');
                }

            } while (result == 1 && attempts < 3);

            if (result == 1)
            {
                printf("ERROR:  Login failed!\n");
                printf("\n");
                pauseExecution();
                choice = 1;
            }
            else
            {
                printf("\n");
                if (acctTData->accounts[index].type == 'C')
                {
                    menuCustomer(acctTData, &acctTData->accounts[index]);
                    printf("\n");
                }
                else
                {
                    menuAgent(acctTData, &acctTData->accounts[index]);
                }
            }

        default:
            break;
        }
    } while (choice == 1);

    return index;
}

int findAccountIndexByAcctNum(int acctNum, const struct Account* account, int numberOfAccounts, int promptAcctNum) {

    if (promptAcctNum) {
        printf("Enter the account#: ");
        acctNum = getPositiveInteger();
    }

    int i;
    for (i = 0; i < numberOfAccounts; i++) {
        if (acctNum == account[i].accountNumber) {
            return i;
        }
    }

    return -1;
}

void menuAgent(struct AccountTicketingData* acctAndTicket, const Account* thisAgent)
{
    int choice = 0;
    int index;
    do
    {
        printf("AGENT: %s (%d)\n", thisAgent->login.customerName, thisAgent->accountNumber);
        printf("==============================================\n");
        printf("Account Ticketing System - Agent Menu\n");
        printf("==============================================\n");
        printf(" 1) Add a new account\n");
        printf(" 2) Modify an existing account\n");
        printf(" 3) Remove an account\n");
        printf(" 4) List accounts: summary view\n");
        printf(" 5) List accounts: detailed view\n");
        printf("----------------------------------------------\n");
        printf(" 6) List new tickets\n");
        printf(" 7) List active tickets\n");
        printf(" 8) List closed tickets\n");
        printf(" 9) Manage a ticket\n");
        printf("10) Archive closed tickets\n");
        printf("----------------------------------------------\n");
        printf("11) View archived account statistics\n");
        printf("12) View archived tickets statistics\n");
        printf("----------------------------------------------\n");
        printf("0) Logout\n");
        printf("\n");
        int ticketNum = 0;
        int ticketNumber = 0;
        int j;
        int i = 0;
        int fund = 0;
        int numbOfTicketsArched = 0;
        printf("Selection: ");
        choice = getIntFromRange(0, 12);
        putchar('\n');
        switch (choice)
        {
            // Add a new account
        case 1:
            // add new account
            for (j = 0; j < acctAndTicket->ACCOUNT_MAX_SIZE; j++)
            {
                if (acctAndTicket->accounts[j].accountNumber == 0)
                    break;
            }
            // j is the index that is open for a new account
            if (j < acctAndTicket->ACCOUNT_MAX_SIZE)
            {
                int biggest = 0;
                int i;
                for (i = 0; i < acctAndTicket->ACCOUNT_MAX_SIZE; i++)
                {
                    if (acctAndTicket->accounts[i].accountNumber > biggest)
                    {
                        biggest = acctAndTicket->accounts[i].accountNumber;
                    }
                }
                acctAndTicket->accounts[j].accountNumber = biggest + 1;
                printf("New Account Data (Account#:%d)\n", acctAndTicket->accounts[j].accountNumber);
                printf("----------------------------------------\n");
                printf("Enter the account type (A=Agent | C=Customer): ");
                acctAndTicket->accounts[j].type = getCharOption("AC");
                printf("\n");
                getUserLogin(&acctAndTicket->accounts[j].login);
                getDemographic(&acctAndTicket->accounts[j].demo);
                printf("*** New account added! ***\n");
                printf("\n");
            }
            else
            {
                printf("ERROR: Account listing is FULL, call ITS Support!\n");
                printf("\n");
            }
            pauseExecution();
            break;
            // Modify an existing account
        case 2:
            // modify the existing account
            index = findAccountIndexByAcctNum(0, acctAndTicket->accounts, acctAndTicket->ACCOUNT_MAX_SIZE, 1);
            putchar('\n');
            if (index < 0)
            {
                printf("ERROR:  Access Denied.\n");
                printf("\n");
                pauseExecution();
                break;
            }
            //(&accounts[index])
            updateAccount(&acctAndTicket->accounts[index]);

            break;
            // Remove an account
        case 3:
            // remove account
            index = findAccountIndexByAcctNum(0, acctAndTicket->accounts, acctAndTicket->ACCOUNT_MAX_SIZE, 1);
            if (index < 0)
            {
                putchar('\n');
                printf("ERROR:  Access Denied.\n\n");
                pauseExecution();
            }
            else if (acctAndTicket->accounts[index].accountNumber == thisAgent->accountNumber)
            {
                printf("\n");
                printf("ERROR: You can't remove your own account!\n");
                printf("\n");
                pauseExecution();
            }
            else
            {
                displayAccountDetailHeader();
                displayAccountDetailRecord(&acctAndTicket->accounts[index]);
                putchar('\n');
                printf("Are you sure you want to remove this record? ([Y]es|[N]o): ");
                if (getCharOption("YN") == 'Y')
                {

                    // put archivedTickets into tickets_arc.txt
                    //* Archiving done
                    archiveCloseTicketsForAccount(acctAndTicket, &acctAndTicket->accounts[index]);

                    //* Closing both closed & Active related tickets
                    closingTicketsForAccount(acctAndTicket, &acctAndTicket->accounts[index]);

                    // put removedAccount into accounts_arc.txt
                    //*first Archive the account data
                    archiveRemovedAccount(acctAndTicket, &acctAndTicket->accounts[index]);

                    //* Remove the Account
                    acctAndTicket->accounts[index].accountNumber = 0;
                    printf("\n");
                    printf("*** Account Removed! ***\n");
                    printf("\n");
                }
                else
                {
                    printf("\n");
                    printf("*** No changes made! ***\n");
                    printf("\n");
                }
                pauseExecution();
            }
            break;
            // List accounts: summary view
        case 4:
            // list accounts
            displayAllAccountSummaryRecords(acctAndTicket->accounts, acctAndTicket->ACCOUNT_MAX_SIZE);
            pauseExecution();
            break;
            // List accounts: detailed view
        case 5:
            // detailed view
            displayAllAccountDetailRecords(acctAndTicket->accounts, acctAndTicket->ACCOUNT_MAX_SIZE);
            printf("\n");
            pauseExecution();
            break;
            // List new tickets
        case 6:

            do
            {
                displayAgentMessageHeader();
                for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
                {
                    if (acctAndTicket->tickets[i].ticketStatusIndicator == 1 && acctAndTicket->tickets[i].counter_numOfMessageTicket == 1)
                    {
                        displayOneTicketRecordAgent(&acctAndTicket->tickets[i]);
                    }
                }
                printf("------ ----- --------------- ------ ------------------------------ --------\n");
                printf("\n");
                printf("Enter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");

                do
                {
                    ticketNum = getInteger();
                    //putchar('\n');
                    if (ticketNum < 0)
                    {
                        printf("ERROR: Value must be positive or zero: ");
                    }
                } while (ticketNum < 0);

                int foundTicket = 0;
                for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
                {
                    if (acctAndTicket->tickets[i].uniqueNumTicket == ticketNum && ticketNum != 0)
                    {
                        displayMessage(&acctAndTicket->tickets[i]);
                        //pauseExecution();
                        foundTicket = 1;
                        break;
                    }
                }

                if (foundTicket == 0 && ticketNum != 0)
                {
                    printf("\n");
                    printf("ERROR: Invalid ticket number.\n");
                    printf("\n");
                    pauseExecution();
                }

            } while (ticketNum != 0);
            printf("\n");
            break;
            // List active tickets
        case 7:
            do
            {
                displayAgentMessageHeader();
                // For showing each message
                for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
                {
                    //Just show the account that are Active and ticket number non zero
                    if (acctAndTicket->tickets[i].ticketStatusIndicator == 1 && acctAndTicket->tickets[i].uniqueNumTicket != 0)
                        displayOneTicketRecordAgent(&acctAndTicket->tickets[i]);
                }

                printf("------ ----- --------------- ------ ------------------------------ --------\n");
                printf("\n");
                printf("Enter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");

                do
                {
                    ticketNum = getInteger();
                    if (ticketNum < 0)
                    {
                        printf("ERROR: Value must be positive or zero: ");
                    }
                } while (ticketNum < 0);

                int foundTicket = 0;
                int i;
                for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
                {
                    if (acctAndTicket->tickets[i].uniqueNumTicket == ticketNum && ticketNum != 0)
                    {
                        displayMessage(&acctAndTicket->tickets[i]);
                        foundTicket = 1;
                        break;
                    }
                }

                if (foundTicket == 0 && ticketNum != 0)
                {
                    printf("\n");
                    printf("ERROR: Invalid ticket number.\n");
                    printf("\n");
                    pauseExecution();
                }

            } while (ticketNum != 0);

            printf("\n");

            break;
            // List closed tickets
        case 8:
            do
            {
                displayAgentMessageHeader();
                for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
                {
                    //Just show the account that are Closed and ticket number non zero
                    if (acctAndTicket->tickets[i].ticketStatusIndicator == 0 && acctAndTicket->tickets[i].uniqueNumTicket != 0)
                        displayOneTicketRecordAgent(&acctAndTicket->tickets[i]);
                }

                printf("------ ----- --------------- ------ ------------------------------ --------\n");
                printf("\n");
                printf("Enter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");

                do
                {
                    ticketNum = getInteger();
                    // putchar('\n');
                    if (ticketNum < 0)
                    {
                        printf("ERROR: Value must be positive or zero: ");
                    }
                } while (ticketNum < 0);

                int foundTicket = 0;
                int i;
                for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
                {
                    if (acctAndTicket->tickets[i].uniqueNumTicket == ticketNum && ticketNum != 0)
                    {
                        displayMessage(&acctAndTicket->tickets[i]);
                        //pauseExecution();
                        foundTicket = 1;
                        break;
                    }
                }

                if (foundTicket == 0 && ticketNum != 0)
                {
                    printf("\n");
                    printf("ERROR: Invalid ticket number.\n");
                    printf("\n");
                    pauseExecution();
                }

            } while (ticketNum != 0);

            printf("\n");
            break;
            // Manage a ticket
        case 9:

            printf("Enter ticket number: ");
            ticketNumber = getPositiveInteger();
            putchar('\n');
            for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == acctAndTicket->tickets[i].uniqueNumTicket)
                {
                    updateTicketAgent(&acctAndTicket->tickets[i], *thisAgent);
                    fund = 1;
                }
            }
            if (fund == 0)
            {
                printf("ERROR: Invalid ticket number - you may only modify your own ticket.\n");
            }

            printf("\n");
            break;
            // Archive closed tickets
        case 10:

            printf("Are you sure? This action cannot be reversed. ([Y]es|[N]o): ");
            choice = getCharOption("YN");
            if (choice == 'Y')
            {

                numbOfTicketsArched = archiveClosedTickets(acctAndTicket->tickets, acctAndTicket->TICKET_MAX_SIZE); // for ticket.c
                putchar('\n');
                printf("*** %d tickets archived ***\n", numbOfTicketsArched);
            }
            else
            {
            }

            printf("\n");
            pauseExecution();
            break;
            // View archived account statistics
        case 11:

            viewArchivedAccountStatistics();
            putchar('\n');
            pauseExecution();
            break;

            // View archived tickets statistics
        case 12:

            viewArchivedTicketStatus();
            putchar('\n');
            pauseExecution();
            break;

        case 0:

            printf("Saving session modifications...\n");
            printf("   %d account saved.\n", savingAccountChanges(acctAndTicket->accounts, acctAndTicket->ACCOUNT_MAX_SIZE));

            printf("   %d tickets saved.\n", savingTicketChanges(acctAndTicket->tickets, acctAndTicket->TICKET_MAX_SIZE));
            printf("### LOGGED OUT ###\n");
            printf("\n");
            break;
        default:
            break;
        }
    } while (choice != 0);
}

void pauseExecution(void)
{
	printf("<< ENTER key to Continue... >>");
	clearStandardInputBuffer();
	putchar('\n');
}

void menuCustomer(const struct AccountTicketingData* data, Account* account)
{
    int choice;
    do
    {
        printf("CUSTOMER: %s (%d)\n", account->login.customerName, account->accountNumber);
        printf("==============================================\n");
        printf("Customer Main Menu\n");
        printf("==============================================\n");
        printf("1) View your account detail\n");
        printf("2) Create a new ticket\n");
        printf("3) Modify an active ticket\n");
        printf("4) List my tickets\n");
        printf("----------------------------------------------\n");
        printf("0) Logout\n");
        printf("\n");
        printf("Selection: ");
        choice = getIntFromRange(0, 4);
        putchar('\n');
        int ticketNum = 0;
        int ticketNumber = 0;
        int j, i;
        int fund = 0;
        switch (choice)
        {
            // View your account detail
        case 1:
            displayAccountDetailHeader();
            displayAccountDetailRecord(account);
            printf("\n");
            pauseExecution();
            break;
            // Create a new ticket
        case 2:
            // add new ticket
            for (j = 0; j < data->TICKET_MAX_SIZE; j++)
            {
                if (data->tickets[j].uniqueNumTicket == 0)
                    break;
            }
            // j is the index that is open for a new ticket account
            if (j < data->TICKET_MAX_SIZE)
            {
                int biggest = 0;
                int i;
                for (i = 0; i < data->TICKET_MAX_SIZE; i++)
                {
                    if (data->tickets[i].uniqueNumTicket > biggest)
                    {
                        biggest = data->tickets[i].uniqueNumTicket;
                    }
                }
                data->tickets[j].uniqueNumTicket = biggest + 1;
                data->tickets[j].customerAcctNum_Ticket = account->accountNumber;

                printf("New Ticket (Ticket#:%06d)\n", data->tickets[j].uniqueNumTicket);
                printf("----------------------------------------\n");
                printf("Enter the ticket SUBJECT (30 chars. maximum): ");
                getCString(data->tickets[j].subjectLine, 1, 30);
                printf("\n");
                printf("Enter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
                getCString(data->tickets[j].message->messageDetails, 1, 150);
                // Number of message for ticket
                data->tickets[j].counter_numOfMessageTicket = 1;

                // Put the Stauts of ticket active for new ticket account
                data->tickets[j].ticketStatusIndicator = 1;

                // Put the account type to Customer
                data->tickets[j].message->accountTypeAuthorMessage = account->type;

                // Put the name of the related Customer
                strncpy(data->tickets[j].message->nameOfGivenUser, account->login.customerName, 31);
                printf("\n");
                printf("*** New ticket created! ***\n\n");
            }
            else
            {
                printf("ERROR: Ticket listing is FULL, call ITS Support!\n\n");
            }
            pauseExecution();
            break;
            // Modify an active ticket
        case 3:
            printf("Enter ticket number: ");
            ticketNumber = getPositiveInteger();
            putchar('\n');
            for (i = 0; i < data->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == data->tickets[i].uniqueNumTicket && data->tickets[i].customerAcctNum_Ticket == account->accountNumber)
                {

                    if (data->tickets[i].ticketStatusIndicator == 0) // Check if the ticket status is CLOSED
                    {
                        printf("ERROR: Ticket is closed - changes are not permitted.\n");
                        printf("\n");
                        fund = 1;
                        break;
                    }
                    else if (data->tickets->ticketStatusIndicator == 1) // check if the ticket status is Active
                    {
                        updateTicket(&data->tickets[i], *account);
                        fund = 1;
                        break;
                    }
                }
            }
            if (fund == 0)
            {
                printf("ERROR: Invalid ticket number - you may only modify your own ticket.\n\n");
            }

            pauseExecution();
            break;
            // List my tickets
        case 4:
            do
            {
                displayAllTicketsForAccount(data, account->accountNumber);
                printf("\n");
                printf("Enter the ticket number to view the messages or\n");
                printf("0 to return to previous menu: ");

                do
                {
                    ticketNum = getInteger();
                    while (ticketNum < 0)
                        if (ticketNum < 0 && ticketNum != 0)
                        {
                            printf("ERROR: Value must be positive or zero: ");
                            ticketNum = getInteger();
                            if (ticketNum != 0)
                            {
                                putchar('\n');
                            }
                        }

                } while (ticketNum < 0);

                int foundTicket = 0;
                int i;
                for (i = 0; i < data->TICKET_MAX_SIZE; i++)
                {
                    if (data->tickets[i].customerAcctNum_Ticket == account->accountNumber && data->tickets[i].uniqueNumTicket == ticketNum && ticketNum != 0)
                    {
                        displayMessage(&data->tickets[i]);
                        foundTicket = 1;
                        break;
                    }
                }

                if (foundTicket == 0 && ticketNum != 0)
                {
                    printf("\n");
                    printf("ERROR: Invalid ticket number - you may only access your own tickets.\n");
                    printf("\n");
                    pauseExecution();
                }

            } while (ticketNum != 0);
            printf("\n");
            break;
            // Logout
        case 0:
            printf("Saving session modifications...\n");
            printf("   %d tickets saved.\n", saveTickets(data->tickets, data->TICKET_MAX_SIZE));
            printf("### LOGGED OUT ###\n");
            break;

        default:
            break;
        }
    } while (choice != 0);
}


int isBadAttempt(const struct AccountTicketingData* data, int* index)
{
    int acctNum, badAttempt = 0;
    char aLine[100];

    printf("Enter the account#: ");
    acctNum = getInteger();

    *index = findAccountIndexByAcctNum(acctNum, data->accounts, data->ACCOUNT_MAX_SIZE, 0);

    if (*index < 0)
    {
        badAttempt = 1;
    }

    printf("User Login    : ");
    getCString(aLine, 1, 99);
    if (*index >= 0)
    {
        if (strcmp(aLine, data->accounts[*index].login.loginName))
        {
            badAttempt = 1;
        }
    }

    printf("Password      : ");
    getCString(aLine, 1, 99);
    if (*index >= 0)
    {
        if (strcmp(aLine, data->accounts[*index].login.password))
        {
            badAttempt = 1;
        }
    }
    return badAttempt;
}

void displayMessage(const Ticket* ticket)
{
    int true = 1;
    char status[7];
    if (ticket->ticketStatusIndicator == 1)
    {
        strcpy(status, "ACTIVE");
    }
    else
    {
        strcpy(status, "CLOSED");
    }
    printf("\n");
    printf("================================================================================\n");
    printf("%06d (%s) Re: %s\n", ticket->uniqueNumTicket, status, ticket->subjectLine);
    printf("================================================================================\n");
    int i;
    for (i = 0; i < ticket->counter_numOfMessageTicket; i++)
    {
        if ((i + 1) % 5 != 0)
        {
            printf("%s (%s):\n", ticket->message[i].accountTypeAuthorMessage == 'C' ? "CUSTOMER" : "AGENT", ticket->message[i].nameOfGivenUser);
            printf("   %s\n\n", ticket->message[i].messageDetails);
            true = 1;
        }
        else
        {
            printf("%s (%s):\n", ticket->message[i].accountTypeAuthorMessage == 'C' ? "CUSTOMER" : "AGENT", ticket->message[i].nameOfGivenUser);
            printf("   %s\n\n", ticket->message[i].messageDetails);
            true = 0;
            pauseExecution();
        }
    }
    if (true == 1)
    {
        pauseExecution();
    }
}

void displayAgentMessageHeader()
{
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    printf("Ticket Acct# Display Name    Status Subject                        Messages\n");
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
}

void archiveCloseTicketsForAccount(struct AccountTicketingData* acctAndTicket, Account* relatedTicket)
{
    int i;
    int j;
    //* must check if it is closed and is related to account
    for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
    {
        if (acctAndTicket->tickets[i].customerAcctNum_Ticket == relatedTicket->accountNumber)
        { // check if related ticket is closed
            if (acctAndTicket->tickets[i].ticketStatusIndicator == 0)
            {
                // archive that

                FILE* fp;
                fp = NULL;
                fp = fopen("tickets_arc.txt", "a");
                if (fp == NULL)
                    printf("ERROR: Can't open tickets_arc file.\n");

                fprintf(fp, "%d|%d|%d|%s|%d|",
                    acctAndTicket->tickets[i].uniqueNumTicket,
                    acctAndTicket->tickets[i].customerAcctNum_Ticket,
                    acctAndTicket->tickets[i].ticketStatusIndicator,
                    acctAndTicket->tickets[i].subjectLine,
                    acctAndTicket->tickets[i].counter_numOfMessageTicket);

                for (j = 0; j < acctAndTicket->tickets[i].counter_numOfMessageTicket; j++)
                {
                    fprintf(fp, "%c|%s|%s|\n",
                        acctAndTicket->tickets[i].message[j].accountTypeAuthorMessage,
                        acctAndTicket->tickets[i].message[j].nameOfGivenUser,
                        acctAndTicket->tickets[i].message[j].messageDetails);
                }

                fclose(fp);
            }
        }
    }
}

void closingTicketsForAccount(struct AccountTicketingData* acctAndTicket, Account* relatedTicket)
{
    int i;
    for (i = 0; i < acctAndTicket->TICKET_MAX_SIZE; i++)
    {
        if (acctAndTicket->tickets[i].customerAcctNum_Ticket == relatedTicket->accountNumber)
        {
            acctAndTicket->tickets[i].uniqueNumTicket = 0; // close the ticket for both Actives and Closed ones
        }
    }
}

void archiveRemovedAccount(struct AccountTicketingData* acctAndTicket, Account* relatedAccount)
{
    int i = 0;
    for (i = 0; i < acctAndTicket->ACCOUNT_MAX_SIZE; i++)
    {
        if (acctAndTicket->accounts[i].accountNumber == relatedAccount->accountNumber)
        {
            // archive that

            FILE* fp;
            fp = NULL;
            fp = fopen("accounts_arc.txt", "a");
            if (fp == NULL)
                printf("ERROR: Can't open accounts_arc.txt file.\n");

            fprintf(fp, "%d~%c~%s~%s~%s~%d~%lf~%s\n",
                acctAndTicket->accounts[i].accountNumber,
                acctAndTicket->accounts[i].type,
                acctAndTicket->accounts[i].login.customerName,
                acctAndTicket->accounts[i].login.loginName,
                acctAndTicket->accounts[i].login.password,
                acctAndTicket->accounts[i].demo.birthYear,
                acctAndTicket->accounts[i].demo.income,
                acctAndTicket->accounts[i].demo.country);

            fclose(fp);
        }
    }
}