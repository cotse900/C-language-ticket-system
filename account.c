// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: account.c
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "commonHelpers.h"
#include "account.h"
#include "accountTicketingUI.h"

int loadAccounts(struct Account accounts[], int accountMaxSize)
{

	int i = 0;
	FILE* fp = fopen("accounts.txt", "r"); // read

	if (fp != NULL)
	{
		for (i = 0; i < accountMaxSize; i++)
		{

			fscanf(fp, "%d~%c~%[^~]~%[^~]~%[^~]~%d~%lf~%30[^\n]",
				&accounts[i].accountNumber,
				&accounts[i].type,
				accounts[i].login.customerName,
				accounts[i].login.loginName,
				accounts[i].login.password,
				&accounts[i].demo.birthYear,
				&accounts[i].demo.income,
				accounts[i].demo.country);
		}
		fclose(fp);
		fp = NULL;
	}
	else
	{
		printf("ERROR: Unable to open file for reading.\n");
	}
	return i;
}

void getAccount(struct Account* account)
{
	printf("New Account Data (Account#:%d)\n", account->accountNumber);
	printf("----------------------------------------\n");

	char validChars[3] = "AC";
	printf("\nEnter the account type (A=Agent | C=Customer): ");
	account->type = getCharOption(validChars);
	printf("\n");
}

void getUserLogin(struct UserLogin* login)
{
	printf("User Login Data Input\n");
	printf("----------------------------------------\n");

	int haveWhiteSpace = 1;
	while(haveWhiteSpace == 1){
		printf("Enter user login (10 chars max): ");
		getCString(login->loginName, 1, 10);
		if(!haveWhitespace(login->loginName, 10)){
			haveWhiteSpace = 0;
		}else{
			printf("ERROR: The user login must NOT contain whitespace characters.\n");
		}
	}

	printf("Enter the display name (30 chars max): ");
	getCString(login->customerName, 1, 30);

	
	int isValid = 0;
	while(isValid == 0){
		printf("Enter the password (must be 8 chars in length): ");
		getCString(login->password, 8, 8);
		if(containsTwoOfDigitsUpLowSymbol(login->password, 8)){
			isValid = 1;
		}else{
			printf("SECURITY: Password must contain 2 of each:\n");
			printf("\tDigit: 0-9\n");
			printf("\tUPPERCASE character\n");
			printf("\tlowercase character\n");
			printf("\tsymbol character:(!@#$%%^&*)\n");
		}
	}

	printf("\n");
}

void getDemographic(struct Demographic* demographic)
{
	printf("Demographic Data Input\n");
	printf("----------------------------------------\n");
	printf("Enter birth year (current age must be between 18 and 110): ");
	demographic->birthYear = getIntFromRange(currentYear() - 110, currentYear() - 18);
	printf("Enter the household Income: $");
	demographic->income = getPositiveDouble();
	printf("Enter the country (30 chars max.): ");
	getCString(demographic->country, 1, 30);
	stringToUpper(demographic->country);
	printf("\n");
}

void updateUserLogin(struct UserLogin* login){
	int menuOption = -1;
	int isValid = 0;

	while(menuOption != 0){
		printf("User Login: %s - Update Options\n", login->loginName);
		printf("----------------------------------------\n");
		printf("1) Display name (current value: %s)\n", login->customerName);
		printf("2) Password\n");
		printf("0) Done\n");
		printf("Selection: ");
		
		menuOption = getIntFromRange(0,2);
		isValid = 0; //reset
		printf("\n");
		switch(menuOption){
			case 1:
				printf("Enter the display name (30 chars max): ");
				getCString(login->customerName, 1, 30);
				printf("\n");
				break;
			case 2:
				while(isValid == 0){
					printf("Enter the password (must be 8 chars in length): ");
					getCString(login->password, 8, 8);
					if(containsTwoOfDigitsUpLowSymbol(login->password, 8)){
						isValid = 1;
					}else{
						printf("SECURITY: Password must contain 2 of each:\n");
						printf("\tDigit: 0-9\n");
						printf("\tUPPERCASE character\n");
						printf("\tlowercase character\n");
						printf("\tsymbol character:(!@#$%%^&*)\n");
					}
					printf("\n");
				}
				break;
		}
	}
}
void updateDemographic(struct Demographic* demo){
	int menuOption = -1;

	while(menuOption != 0){
		printf("Demographic Update Options\n");
		printf("----------------------------------------\n");
		printf("1) Household Income (current value: $%.2lf)\n", demo->income); //$%10.2lf
		printf("2) Country (current value: %s)\n", demo->country);
		printf("0) Done\n");
		printf("Selection: ");

		menuOption = getIntFromRange(0,2);
		putchar('\n');
		switch(menuOption){
			case 1:
				printf("Enter the household Income: $");
				demo->income = getPositiveDouble();
				putchar('\n');
				break;
			case 2:
				printf("Enter the country (30 chars max.): ");
				getCString(demo->country, 1, 30);
				stringToUpper(demo->country);
				putchar('\n');
				break;
		}
	}
}

void updateAccount(struct Account* account){

	char validChars[3] = "AC";
	int menuOption = -1;

	while(menuOption != 0){
		printf("Account: %05d - Update Options\n", account->accountNumber);
		printf("----------------------------------------\n");	
		printf("1) Update account type (current value: %c)\n", account->type);
		printf("2) Login\n");
		printf("3) Demographics\n");
		printf("0) Done\n");
		printf("Selection: ");

		menuOption = getIntFromRange(0,3);
		putchar('\n');
		switch(menuOption){
			case 1:
				printf("Enter the account type (A=Agent | C=Customer): ");
				account->type = getCharOption(validChars);
				break;
			case 2:
				updateUserLogin(&account->login);
				break;
			case 3:
				updateDemographic(&account->demo);
				break;
			case 0:
				break;
		}
	}


}

void displayAllAccountSummaryRecords(const Account* account, int size)
{

	displayAccountSummaryHeader();
	int i;
	for (i = 0; i < size; i++)
	{
		if (account[i].accountNumber != 0)
			displayAccountSummaryRecord(&account[i]);
	}
	putchar('\n');
}

void displayAllAccountDetailRecords(const Account* account, int size)
{

	displayAccountDetailHeader();
	int i;
	for (i = 0; i < size; i++)
	{
		if (account[i].accountNumber > 0)
		{
			displayAccountDetailRecord(&account[i]);
		}
	}
}


void viewArchivedAccountStatistics()
{
	struct Account accounts[15] = { {0} };
	int i = 0;
	FILE* fp = fopen("accounts_arc.txt", "r"); // read

	if (fp == NULL)

		printf("ERROR: Unable to open file for reading.\n");

	while (!feof(fp))
	{
		fscanf(fp, "%d~%c~%[^~]~%[^~]~%[^~]~%d~%lf~%30[^\n]",
			&accounts[i].accountNumber,
			&accounts[i].type,
			accounts[i].login.customerName,
			accounts[i].login.loginName,
			accounts[i].login.password,
			&accounts[i].demo.birthYear,
			&accounts[i].demo.income,
			accounts[i].demo.country);

		i++;
	}
	fclose(fp);
	printf("There are %d account(s) currently archived.\n", i - 1);
	return;
}

int savingAccountChanges(struct Account* accounts, int sizeAccount)
{
	int i = 0;
	int numberOfAccountSaved = 0;
	FILE* fp = NULL;
	fp = fopen("accounts.txt", "w"); // rewrite to textfile

	if (fp != NULL)
	{
		for (i = 0; i < sizeAccount; i++)
		{
			//printf("\n");
			if (accounts[i].accountNumber != 0)
			{

				fprintf(fp, "%d~%c~%s~%s~%s~%d~%lf~%s\n",
					accounts[i].accountNumber,
					accounts[i].type,
					accounts[i].login.customerName,
					accounts[i].login.loginName,
					accounts[i].login.password,
					accounts[i].demo.birthYear,
					accounts[i].demo.income,
					accounts[i].demo.country);
				if (accounts[i].accountNumber != 0)
				{
					numberOfAccountSaved++;
				}
			}
		}
		fclose(fp);
	}
	else
	{
		printf("ERROR: Unable to open file for reading.\n");
	}

	return numberOfAccountSaved;
}

