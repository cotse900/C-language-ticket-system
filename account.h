// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: account.h
//
#ifndef ACCOUNT_H_
#define ACCOUNT_H_

typedef struct Demographic{
    int birthYear;
    double income;
    char country[31];
} Demographic;

typedef struct UserLogin{
    char customerName[31];
    char loginName[11];
    char password[9];
} UserLogin;

typedef struct Account{
    int accountNumber;
    char type;
    struct UserLogin login;
    struct Demographic demo;
} Account;

int loadAccounts(struct Account accounts[], int accountMaxSize);
//
void getAccount(struct Account* account);
void getUserLogin(struct UserLogin* login);
void getDemographic(struct Demographic* demographic);

void updateAccount(struct Account* account);
void updateUserLogin(struct UserLogin* login);
void updateDemographic(struct Demographic* demo);

void displayAllAccountSummaryRecords(const Account* account, int size);
void displayAllAccountDetailRecords(const Account* account, int size);
void viewArchivedAccountStatistics();
int savingAccountChanges(struct Account* accounts, int sizeAccount);
//
#endif // !ACCOUNT_H_