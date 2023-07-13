// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: accountTicketingUI.h
//
#ifndef ACCOUNT_TICKETINGUI_H_
#define ACCOUNT_TICKETINGUI_H_

#include "account.h"
#include "ticket.h"

struct AccountTicketingData
{
 struct Account* accounts; // array of accounts
 const int ACCOUNT_MAX_SIZE; // maximum elements for account array
 struct Ticket* tickets; // array of tickets
 const int TICKET_MAX_SIZE; // maximum elements for ticket array
};

void displayAccountSummaryHeader(void);
void displayAccountDetailHeader(void);
void displayAccountSummaryRecord(const struct Account* account);
void displayAccountDetailRecord(const struct Account* account);
void applicationStartup(struct AccountTicketingData* data);


int menuLogin(struct AccountTicketingData* data);
int findAccountIndexByAcctNum(int acctNum, const struct Account* account, int numberOfAccounts, int promptAcctNum);
void menuAgent(struct AccountTicketingData* accTicketingData, const struct Account* loggedInUser);
void pauseExecution(void);

void menuCustomer(struct AccountTicketingData* accTicketingData, const struct Account* loggedInUser);
int isBadAttempt(const struct AccountTicketingData* data, int* index);

void displayTicketHeader(void);
void displayOneTicketRecord(const Ticket* ticket);
void applicationStartup(struct AccountTicketingData* acctTData);
void displayAllTicketsForAccount(const struct AccountTicketingData* data, int accountNum);
void displayMessage(const Ticket* ticket);
void displayAgentMessageHeader();
void archiveCloseTicketsForAccount(struct AccountTicketingData* acctAndTicket, Account* relatedTicket);
void closingTicketsForAccount(struct AccountTicketingData* acctAndTicket, Account* relatedTicket);
void archiveRemovedAccount(struct AccountTicketingData* acctAndTicket, Account* relatedAccount);

#endif // !ACCOUNT_TICKETINGUI_H_
