// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: account.h
//
#ifndef TICKET_H_
#define TICKET_H_
#define MAX_NUMBER_MESSAGE 20
typedef struct Message
{
    char accountTypeAuthorMessage;
    char nameOfGivenUser[31];
    char messageDetails[151];

} Message;

typedef struct Ticket
{
    int uniqueNumTicket;
    int customerAcctNum_Ticket;
    int ticketStatusIndicator;
    char subjectLine[31];
    int counter_numOfMessageTicket;
    Message message[MAX_NUMBER_MESSAGE];
} Ticket;

//Functions prototypes

int loadTickets(Ticket tickets[], int ticketMaxSize);

void updateTicket(Ticket* ticket, const struct Account account);

void updateTicketAgent(Ticket* ticket, const struct Account account);

void addAMessage(Ticket* ticket, const struct Account account);

void displayOneTicketRecordAgent(const Ticket* ticket);

int archiveClosedTickets(Ticket* ticket, int ticketMaxSize);

int saveTickets(Ticket* tickets, int maxArraySize);

void viewArchivedTicketStatus();

int savingTicketChanges(struct Ticket* tickets, int maxArraySize);


#endif // !TICKET_H_