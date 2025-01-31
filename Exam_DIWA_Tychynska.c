/* Final Exam. 2324-CT609 Fundamentals of Programming 
 * Date: 13 Jan 2024
 * Author: Yevheniia Tychynska
 * Student ID: 23105729
 * Digital Identity Wallet App (DIWA) */

#define _CRT_SECURE_NO_WARNINGS 
#define MAX_LINE_LENGTH 300

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

int selection = 0; // global variable
int numberOfDocs = 0; // global variable
int numberOfTransactions = 0; // global variable
int unfulfilled = 0; // global variable

// struct represents dates
struct Date {
	int day;
	int month;
	int year;
};

// struct represents user's documents
struct Document {
	char* DocumentID;
	char* docType;
	struct Date issueDate;
	struct Date expiryDate;
	char* countryCode;
	char* field_1;
	char* field_2;
	char* field_3;
};

// struct represents user's transactions
struct Transaction {
	time_t timeStamp;
	char* transactionType;
	char* entityWalletID;
	char* entityName;
	char* documentType;
	char* messageData;
};

// functions' declarations
void main_menu();
void display_home(char* docFileName, char* transFileName);
struct Document* load_document_data(char* fileName);
struct Transaction* load_transaction_data(char* fileName);
time_t timestampToTimet(char* token);
void timetToTimestamp(time_t time, char* timeString, size_t bufferSize);
void view_documents(char* fileName);
int count_recent_requests(struct Transaction* transactions);
void share_document(char* docFileName, char* transFileName);
struct Transaction* current_requests(struct Transaction* transactions);
void view_past_transactions(char* transFileName);
int validate_date(char* date);
void privacy_dashboard(char* docFileName, char* transFileName);
void delete_request(struct Transaction* transactions, struct Transaction* requests, char* filename, int option);
void exit_DIWA(char* docFileName, char* transFileName);
void view_current_requests(char* transFileName);
void free_document_array(struct Document* documents);
void free_transaction_array(struct Transaction* transactions, int sizeOfArray);
void swap_structs(struct Transaction* toSwap_1, struct Transaction* toSwap_2);
void bubbleSort(struct Transaction* sharedTrans, int numberOfShared);

int main(void) {
		
	char transFile[] = "WalletIDTransactions-2.txt";
	char docFile[] = "WalletIDDocuments-3.txt";
	
	display_home(docFile, transFile);
	// functions work depending on user's choise in main menu
	while (selection >= 0 && selection <= 6) {
		switch (selection) {
		case 0:
			main_menu(); 
			break;
		case 1:
			view_documents(docFile);
			break;
		case 2:
			view_current_requests(transFile);
			break;
		case 3:
			share_document(docFile, transFile);
			break;
		case 4:
			privacy_dashboard(docFile, transFile);
			break;
		case 5:
			view_past_transactions(transFile);
			break;
		case 6:
			exit_DIWA(docFile, transFile);
			break;
		}
	}
	return 0;
} // end of main

// function displays main menu and takes option from user
void main_menu() {
	printf("\n*****MAIN MENU*****\n\n");
	printf("1. View Documents\n2. View Current Requests\n3. Share Document\n4. Privacy Dashboard\n5. View Past Transactions\n6. Exit\n");
	do {
		printf("\nEnter Selection: ");

		if (scanf("%d", &selection) != 1) {
			printf("Invalid input. Your option must be a number from 1 to 6");
			while (getchar() != '\n')
				;
		}
		else if (selection < 1 || selection > 6) {
			printf("This option does not exist. Please choose an option from the list above.");
		}
	} while (selection < 1 || selection > 6);
} // end main_menu()

// function displays home page
void display_home(char* docFileName, char* transFileName) {
	struct Document* documents = load_document_data(docFileName);
	struct Transaction* transactions = load_transaction_data(transFileName);
	printf("Digital Identity Wallet (Pilot Prototype)\n\n\n");
	time_t rawTime = time(NULL);
	struct tm *date = localtime(&rawTime);
	char currentDate[50];
	strftime(currentDate, 50, "%A %d %B %Y", date);
	printf("CURRENT DATE: %s\n", currentDate);
	printf("\nSTATUS: %d outstanding requests to share data recieved in the last 24 hours\n", count_recent_requests(transactions));
	printf("\n%s\n", documents[0].field_3);
	printf("\n%s\n", documents[0].field_1);
	printf("\n%s\n", documents[0].field_2);
	free_document_array(documents);
	free_transaction_array(transactions, numberOfTransactions);
} // end display_home()

// function loads document data
struct Document* load_document_data(char* docFileName) {
	FILE* userDocs;
	char line[MAX_LINE_LENGTH] = "";
	int numberOfLines = 0;
	int length = 0;
	int longestLine = 0;
	char c;
	char* token;
	// struct of eIdentity
	struct Document eIdentity;
		eIdentity.DocumentID = "111111";
		eIdentity.docType = "eIdentity";
		eIdentity.issueDate.day = 01;
		eIdentity.issueDate.month = 01;
		eIdentity.issueDate.year = 2023;
		eIdentity.expiryDate.day = 01;
		eIdentity.expiryDate.month = 01;
		eIdentity.expiryDate.year = 2200;
		eIdentity.countryCode = "WWW";
		eIdentity.field_1 = "NAME: John Smyth";
		eIdentity.field_2 = "DATE OF BIRTH: 01 April 1984";
		eIdentity.field_3 = "MY WALLET ID: 0x23bd14810c04739b16Ee5a835114C13A8A2C6166";
	// open file
	userDocs = fopen(docFileName, "r");
	if (userDocs == NULL) {
		printf("Error in getting the documents\n");
		return NULL;
	}
	//count lines in file, don't exclude firts line to get number of docs+1 (place for eIdentity)
	while ((c = fgetc(userDocs)) != EOF) {
		if (c == '\n') {
			numberOfLines++;
		}
	}
	rewind(userDocs);
	numberOfDocs = numberOfLines;
	// allocate memory for array of structs
	struct Document* documents = malloc(numberOfLines * sizeof(struct Document)); 
	if (documents == NULL) {
		printf("Memory allocation failed");
		fclose(userDocs);
		exit(EXIT_FAILURE); 
	}
	// put eIdentity document into an array of documents
	documents[0].DocumentID = strdup(eIdentity.DocumentID); 
	documents[0].docType = strdup(eIdentity.docType);
	documents[0].countryCode = strdup(eIdentity.countryCode);
	documents[0].issueDate.day = eIdentity.issueDate.day;
	documents[0].issueDate.month = eIdentity.issueDate.month;
	documents[0].issueDate.year = eIdentity.issueDate.year;
	documents[0].expiryDate.day = eIdentity.expiryDate.day;
	documents[0].expiryDate.month = eIdentity.expiryDate.month;
	documents[0].expiryDate.year = eIdentity.expiryDate.year;
	documents[0].field_1 = strdup(eIdentity.field_1);
	documents[0].field_2 = strdup(eIdentity.field_2);
	documents[0].field_3 = strdup(eIdentity.field_3);

	// read document line by line, allocate memory, save data into an array of structs
	fgets(line, sizeof(line), userDocs); // skip the first line, it is header
	for (int i = 1; i < numberOfLines; i++) {
		// check if line reading successful 
		if (fgets(line, sizeof(line), userDocs) == NULL) {
			printf("Error reading line %d\n", i + 2);
			break;  // Exit the loop on error
		}
		size_t length = strlen(line); // count characters in a line
		if (line[length - 1] == '\n') {
			line[length - 1] = '\0'; // replace new line with end of string character 
		}
		// take strings between commas and assign them to the members of structs 
		token = strtok(line, ",");
		if (token != NULL) {
			documents[i].DocumentID = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			documents[i].docType = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			(void)sscanf(token, "%d/%d/%d", &documents[i].issueDate.day, &documents[i].issueDate.month, &documents[i].issueDate.year);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			(void)sscanf(token, "%d/%d/%d", &documents[i].expiryDate.day, &documents[i].expiryDate.month, &documents[i].expiryDate.year);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			documents[i].countryCode = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			documents[i].field_1 = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			documents[i].field_2 = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			documents[i].field_3 = strdup(token);
		}
	} // end for loop
	fclose(userDocs);
	return documents;
} // end load documents

// function loads transaction data
struct Transaction* load_transaction_data(char* fileName) {
	FILE* userTransactions;
	char line[MAX_LINE_LENGTH] = "";
	int numberOfLines = 0;
	char* token;
	char c;

	// open file
	userTransactions = fopen(fileName, "r");
	if (userTransactions == NULL) {
		printf("Error in accessing transactions file");
		return NULL;
	}

	// count lines in a file excluding first line
	fgets(line, sizeof(line), userTransactions); // skip the first line
	while ((c = fgetc(userTransactions)) != EOF) {
		if (c == '\n') {
			numberOfLines++;
		}
	}
	rewind(userTransactions);
	numberOfTransactions = numberOfLines;

	// allocate memory for an array of structs
	struct Transaction* transactions = malloc(numberOfLines * sizeof(struct Transaction));
	if (transactions == NULL) {
		printf("Memory allocation is failed");
		fclose(userTransactions);
		exit(EXIT_FAILURE);
	}

	// read data, allocate memory for strings and store data into array of structs
	fgets(line, sizeof(line), userTransactions); // skip the first line
	for (int i = 0; i < numberOfLines; i++) {
		if (fgets(line, sizeof(line), userTransactions) == NULL) {
			printf("Error reading line %d\n", i + 2);
			break;  // Exit the loop on error
		}
		size_t length = strlen(line); // count characters in a timestamp
		if (line[length - 1] == '\n') {
			line[length - 1] = '\0'; // replace new line with end of string character 
		}
		token = strtok(line, ",");
		if (token != NULL) {
			transactions[i].timeStamp = timestampToTimet(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			transactions[i].transactionType = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			transactions[i].entityWalletID = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			transactions[i].entityName = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			transactions[i].documentType = strdup(token);
		}
		token = strtok(NULL, ",");
		if (token != NULL) {
			transactions[i].messageData = strdup(token);
		}
	} // end for loop

	fclose(userTransactions);
	return transactions; 
} // end load transactions

// function converts timestamps to time_t
time_t timestampToTimet(char* token) {
	int day, month, year, hour, minute, second;
	time_t time;
	struct tm timeStamp;
	(void)sscanf(token, "%d/%d/%d-%d:%d:%d", &day, &month, &year, &hour, &minute, &second);
	timeStamp.tm_mday = day;
	timeStamp.tm_mon = month - 1;
	timeStamp.tm_year = year - 1900;
	timeStamp.tm_hour = hour;
	timeStamp.tm_min = minute;
	timeStamp.tm_sec = second;
	time = mktime(&timeStamp);
	return time;
} // end timestampToTimet()

// function converts time_t into readable timestamp 
void timetToTimestamp(time_t time, char* timeString, size_t bufferSize) {
	struct tm* localTime = localtime(&time);
	if (localTime == NULL) {
		printf("Something wrong with getting time");
		return;
	}
	if (strftime(timeString, bufferSize, "%d/%m/%Y-%H:%M:%S", localTime) == 0) {
		printf("Cannot format time");
		return;
	}
} // end timetToTimestamp()

// function displays user's documents
void view_documents(char* fileName) {
	int option;
	struct Document* documents = load_document_data(fileName);
	printf("\n%-15s %-30s %-12s %-12s %-14s %-25s %-30s %-45s\n\n",
		"Document ID", "Document Type", "Issue Date", "Expiry Date", "Country Code", 
		"Field 1", "Field 2", "Field 3");
	for (int i = 0; i < numberOfDocs; i++) {
		printf("%-15s %-30s %02d/%02d/%d   %02d/%02d/%d   %-14s %-25s %-30s %-45s\n",
			documents[i].DocumentID, documents[i].docType, 
			documents[i].issueDate.day, documents[i].issueDate.month, documents[i].issueDate.year,
			documents[i].expiryDate.day, documents[i].expiryDate.month, documents[i].expiryDate.year,
			documents[i].countryCode,
			documents[i].field_1, documents[i].field_2, documents[i].field_3);
	}
	free_document_array(documents);
	do {
		printf("\nPress 0 to call Main Menu or 1 to Exit DIWA: ");
		if (scanf("%d", &option) != 1) {
			printf("Invalid input\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		if (option != 0 && option != 1) {
			printf("This option does not exist\n");
		}
	} while (option != 0 && option != 1);
	if (option == 0) {
		selection = 0;
	}
	else {
		selection = 6;
	}
} // end view_documents()

// function counts and returns all request with type "R" for the last 24 hours
int count_recent_requests(struct Transaction* transactions) {
	int countR = 0;
	for (int i = 0; i < numberOfTransactions; i++) {
		if (strcmp(transactions[i].transactionType, "R") == 0) {
			if (difftime(time(NULL), transactions[i].timeStamp) < 24 * 60 * 60) {
				countR++;
			}
		}
	}
	return countR;
} // end count_recent_requests()

// function displays unfulfilled requests
void view_current_requests(char* transFileName) {
	char timeString[21];
	char yesNo;
	int option;
	int match = 0;
	int index = 0;
	struct Transaction* transactions = load_transaction_data(transFileName);
	struct Transaction* unfulfilled_requests = current_requests(transactions);
	
	// print unfulfilled requests
	printf("\n***** Current unfulfilled requests *****\n"); 
	printf("----------------------------------------\n"); 
	printf("%-5s %-20s %-25s %-20s %-20s %-20s %-35s\n\n",
		"No.", "Date and Time", "Type of Transaction", "Entity WALLET ID", "Entity Name", "Document Type", "Message");
	for (int i = 0; i < unfulfilled; i++) {
		timetToTimestamp(unfulfilled_requests[i].timeStamp, timeString, sizeof(timeString));
		printf("%-5d %-20s %-25s %-20s %-20s %-20s %-35s\n",
			i+1, timeString, unfulfilled_requests[i].transactionType, unfulfilled_requests[i].entityWalletID,
			unfulfilled_requests[i].entityName, unfulfilled_requests[i].documentType, unfulfilled_requests[i].messageData);
	} // end for
	 
	printf("\nDo you want to delete any request? (Y/N): ");
	(void)scanf(" %c", &yesNo);
	while (getchar() != '\n');
	// check user input 
	while (1) {
		if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
			printf("Invalid answer. Please enter Y or y or N or n: ");
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
		}
		else {
			break;
		}
	}
	if (yesNo == 'Y' || yesNo == 'y') {
		printf("Which request from the list would you like to delete?\nChoose number from the list above or enter 0 to return to Main Menu: ");
		do { // check input
			if (scanf("%d", &option) != 1) {
				printf("Invalid input. Your option must be a number from 0 to %d: ", unfulfilled);
				while (getchar() != '\n')
					;
			}
			else if (option < 0 || option > unfulfilled) {
				printf("This option does not exist. Please choose an option from 0 to %d: ", unfulfilled);
			}
		} while (option < 0 || option > unfulfilled);
		// go to main menu
		if (option == 0) {
			free_transaction_array(transactions, numberOfTransactions);
			free_transaction_array(unfulfilled_requests, unfulfilled);
			selection = 0;
			return;
		}
		else { // delete chosen request
			delete_request(transactions, unfulfilled_requests, transFileName, option);
			free_transaction_array(unfulfilled_requests, unfulfilled);
			free_transaction_array(transactions, numberOfTransactions);
		}
	}
	if (yesNo == 'N' || yesNo == 'n') {
		free_transaction_array(unfulfilled_requests, unfulfilled); 
		free_transaction_array(transactions, numberOfTransactions);
		printf("\nReturning to Main Menu\n"); 
		selection = 0;
	}
	selection = 0;
} // end view_current_requests()

// function displays list of documents and asks user what document to share, 
// shows outstanding requests for the document and asks what information to share
void share_document(char* docFileName, char* transFileName) {
	struct Document* documents = load_document_data(docFileName);
	struct Transaction* transactions = load_transaction_data(transFileName);
	int option;
	int docTypeIndex;
	int docRequest = 0;
	char timeString[21];
	char yesNo;
	time_t mostRecentRequest = 0;
	int mostRecentReqIndex = 0;
	char dateString[11] = "";
	char inputData[MAX_LINE_LENGTH] = "";
	printf("\n***** Share Document *****\n\n");
	printf("Current documents available:\n\n");
	for (int i = 0; i < numberOfDocs; i++) {
		printf("%d. %s\n", i + 1, documents[i].docType);
	}
	// check user input
	do {
		printf("\n\nEnter option to share or put 0 to call Main Menu: ");
		if (scanf("%d", &option) != 1) {
			printf("Invalid input. Your option must be a number from 1 to %d", numberOfDocs);
			while (getchar() != '\n')
				;
		}
		else if (option < 0 || option > numberOfDocs) {
			printf("This option does not exist. Please choose an option from the list above.");
		}
	} while (option < 0 || option > numberOfDocs);

	if (option == 0) {
		free_document_array(documents);
		free_transaction_array(transactions, numberOfTransactions);
		selection = 0;
		return;
	}
	docTypeIndex = option - 1; //save number of chosen document
	struct Transaction* outstanding = current_requests(transactions);
	// check whether requests for a document are exist
	for (int i = 0; i < unfulfilled; i++) {
		if (strcmp(outstanding[i].documentType, documents[docTypeIndex].docType) == 0) {
			docRequest++;
		}
	}
	if (docRequest == 0) {
		printf("\nNo unanswered requests for %s are found\n", documents[docTypeIndex].docType);
		printf("\nReturning to Main Menu\n");
		free_document_array(documents);
		free_transaction_array(transactions, numberOfTransactions);
		free_transaction_array(outstanding, unfulfilled);
		selection = 0;
		return;
	}
	// print unfullfilled requests for chosen documents
	else {
		printf("\n%d requests for %s are found:\n", docRequest, documents[docTypeIndex].docType);
		printf("\n%-25s %-25s %-25s %-50s\n\n", "Date and Time", "Entity Wallet ID", "Entity Name", "Message");
		for (int i = 0; i < unfulfilled; i++) {
			if (strcmp(outstanding[i].documentType, documents[docTypeIndex].docType) == 0) {
				timetToTimestamp(outstanding[i].timeStamp, timeString, sizeof(timeString));
				printf("%-25s %-25s %-25s %-50s\n",
					timeString, outstanding[i].entityWalletID,
					outstanding[i].entityName, outstanding[i].messageData);
				// save the most recent request time and index for further use
				if (mostRecentRequest <= outstanding[i].timeStamp) {
					mostRecentRequest = outstanding[i].timeStamp;
					mostRecentReqIndex = i;
				}
			}
		}
	}
	// prompt user to choose next step
	do {
		printf("\n\n1) Respond to the most recent request\n2) Enter entity which you want to share the document with manually\nOr put 0 to call Main Menu\nYour choise: ");
		if (scanf("%d", &option) != 1) {
			printf("Invalid input. Your option must be a number from 0 to 2");
			while (getchar() != '\n')
				;
		}
		else if (option < 0 || option > 2) {
			printf("This option does not exist. Please choose an option 0, 1 or 2");
		}
	} while (option < 0 || option > 2);
	// go to main menu if 0
	if (option == 0) {
		free_document_array(documents);
		free_transaction_array(transactions, numberOfTransactions);
		free_transaction_array(outstanding, unfulfilled); 
		selection = 0;
		return;
	}
	else {
		// reallocate array of transaction data to prepare to add new transaction
		transactions = realloc(transactions, (numberOfTransactions+1) * sizeof(struct Transaction));
		if (transactions == NULL) {
			printf("\nMemory reallocation failed\n");
			exit(EXIT_FAILURE);
		}
		numberOfTransactions++; // increment number of transactions after reallocation
		transactions[numberOfTransactions - 1].messageData = malloc(MAX_LINE_LENGTH * sizeof(char)); // allocate memory for message
		if (transactions[numberOfTransactions - 1].messageData == NULL) {
			printf("Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		// ask user what information to share as a respond to the most recent transaction and write data into array
		if (option == 1) {
			transactions[numberOfTransactions - 1].transactionType = strdup("S");
			transactions[numberOfTransactions - 1].entityWalletID = strdup(outstanding[mostRecentReqIndex].entityWalletID);
			transactions[numberOfTransactions - 1].entityName = strdup(outstanding[mostRecentReqIndex].entityName);
			transactions[numberOfTransactions - 1].documentType = strdup(documents[docTypeIndex].docType);

			printf("\n\nEntity: %s\n", outstanding[mostRecentReqIndex].entityName);
			printf("\nWallet ID: %s\n", outstanding[mostRecentReqIndex].entityWalletID);
			printf("\nChoose which personal information to share\n");
			printf("------------------------------------------\n");
			printf("Your Document ID: %s\n", documents[docTypeIndex].DocumentID);
			printf("Your Document Type: %s\n", documents[docTypeIndex].docType);
			printf("------------------------------------------\n");
			printf("Issue Date: %02d/%02d/%d   (Y/N)? ",
				documents[docTypeIndex].issueDate.day, documents[docTypeIndex].issueDate.month,
				documents[docTypeIndex].issueDate.year);

			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcpy(transactions[numberOfTransactions - 1].messageData, "Issue date: ");
				snprintf(dateString, sizeof(dateString), "%02d/%02d/%d", documents[docTypeIndex].issueDate.day,
					documents[docTypeIndex].issueDate.month, documents[docTypeIndex].issueDate.year);
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, dateString), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcpy(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("Expiry Date: %02d/%02d/%d  (Y/N)? ",
				documents[docTypeIndex].expiryDate.day, documents[docTypeIndex].expiryDate.month,
				documents[docTypeIndex].expiryDate.year);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				sprintf(dateString, "%02d/%02d/%d", documents[docTypeIndex].expiryDate.day,
					documents[docTypeIndex].expiryDate.month, documents[docTypeIndex].expiryDate.year);
				strcat(strcat(strcat(transactions[numberOfTransactions - 1].messageData, "Expiry date: "), dateString), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("Country Code: %-10s (Y/N)? ", documents[docTypeIndex].countryCode);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(strcat(transactions[numberOfTransactions - 1].messageData, "Country code: "), documents[docTypeIndex].countryCode), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("%-24s (Y/N)? ", documents[docTypeIndex].field_1);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, documents[docTypeIndex].field_1), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("%-24s (Y/N)? ", documents[docTypeIndex].field_2);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, documents[docTypeIndex].field_2), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("%-24s (Y/N)? ", documents[docTypeIndex].field_3);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, documents[docTypeIndex].field_3), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
		} // end if option 1

		if (option == 2) {
			transactions[numberOfTransactions - 1].documentType = strdup(documents[docTypeIndex].docType);
			transactions[numberOfTransactions - 1].transactionType = strdup("S");

			printf("Which entity do you want to share your %s with? ", documents[docTypeIndex].docType);
			(void)scanf("%s", inputData);
			transactions[numberOfTransactions - 1].entityName = strdup(inputData);
			printf("Enter Entity Wallet ID: ");
			(void)scanf("%s", inputData);
			transactions[numberOfTransactions - 1].entityWalletID = strdup(inputData);
			printf("Choose which personal information to share\n");
			printf("------------------------------------------\n");
			printf("Your Document ID: %s\n", documents[docTypeIndex].DocumentID);
			printf("Your Document Type: %s\n", documents[docTypeIndex].docType);
			printf("------------------------------------------\n");
			printf("Issue Date: %02d/%02d/%d   (Y/N)? ", documents[docTypeIndex].issueDate.day,
				documents[docTypeIndex].issueDate.month, documents[docTypeIndex].issueDate.year);

			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}

			if (yesNo == 'Y' || yesNo == 'y') {
				strcpy(transactions[numberOfTransactions - 1].messageData, "Issue date: ");
				snprintf(dateString, sizeof(dateString), "%02d/%02d/%d", documents[docTypeIndex].issueDate.day, documents[docTypeIndex].issueDate.month, documents[docTypeIndex].issueDate.year);
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, dateString), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcpy(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("Expiry Date: %02d/%02d/%d  (Y/N)? ",
				documents[docTypeIndex].expiryDate.day, documents[docTypeIndex].expiryDate.month, documents[docTypeIndex].expiryDate.year);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				sprintf(dateString, "%02d/%02d/%d", documents[docTypeIndex].expiryDate.day, documents[docTypeIndex].expiryDate.month, documents[docTypeIndex].expiryDate.year);
				strcat(strcat(strcat(transactions[numberOfTransactions - 1].messageData, "Expiry date: "), dateString), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("Country Code: %-10s (Y/N)? ", documents[docTypeIndex].countryCode);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(strcat(transactions[numberOfTransactions - 1].messageData, "Country code: "), documents[docTypeIndex].countryCode), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("%-24s (Y/N)? ", documents[docTypeIndex].field_1);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, documents[docTypeIndex].field_1), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("%-24s (Y/N)? ", documents[docTypeIndex].field_2);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, documents[docTypeIndex].field_2), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
			printf("%-24s (Y/N)? ", documents[docTypeIndex].field_3);
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
			// check user input 
			while (1) {
				if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
					printf("Invalid answer. Please enter Y or y or N or n: ");
					(void)scanf(" %c", &yesNo);
					while (getchar() != '\n');
				}
				else {
					break;
				}
			}
			if (yesNo == 'Y' || yesNo == 'y') {
				strcat(strcat(transactions[numberOfTransactions - 1].messageData, documents[docTypeIndex].field_3), ";");
			}
			else if (yesNo == 'N' || yesNo == 'n') {
				strcat(transactions[numberOfTransactions - 1].messageData, "");
			}
		} // end if option 2

		time(&transactions[numberOfTransactions - 1].timeStamp); // save time of transaction

		//write into the file
		FILE* fileToAppend;
		fileToAppend = fopen(transFileName, "a");
		if (fileToAppend == NULL) {
			printf("Error opening the file\n");
			return;
		}
		// convert raw time into readable timestamp
		timetToTimestamp(transactions[numberOfTransactions - 1].timeStamp, timeString, sizeof(timeString));
		fprintf(fileToAppend, "%s,%s,%s,%s,%s,%s\n", timeString, transactions[numberOfTransactions - 1].transactionType,
				transactions[numberOfTransactions - 1].entityWalletID, transactions[numberOfTransactions - 1].entityName,
				transactions[numberOfTransactions - 1].documentType, transactions[numberOfTransactions - 1].messageData);
		// close file and free memory
		fclose(fileToAppend);
		free_transaction_array(outstanding, unfulfilled);
		free_document_array(documents);
		free_transaction_array(transactions, numberOfTransactions);  
		
		printf("\nData is shared succesfully\n");
		printf("\nReturning to the Main Menu\n\n");
		selection = 0;
	}
} // end of share_document()

// function creates an array of requests that do nor have matching S transactions
struct Transaction* current_requests(struct Transaction* transactions) {
	int j = 0;
	int boolean = 0; // mark R transactions that do not have matching S transactions
	struct Transaction* current_requests = malloc(numberOfTransactions * sizeof(struct Transaction));
	if (current_requests == NULL) {
		printf("Memory allocation for current requests is unsuccessful\n");
		exit(EXIT_FAILURE); 
	}
	for (int i = 0; i < numberOfTransactions; i++) {
		if (strcmp(transactions[i].transactionType, "R") == 0) {
			boolean = 0;
			for (int x = 0; x < numberOfTransactions; x++) {
				if (strcmp(transactions[x].entityWalletID, transactions[i].entityWalletID) == 0 &&
					strcmp(transactions[x].documentType, transactions[i].documentType) == 0 &&
					strcmp(transactions[x].transactionType, "S") == 0) {
					boolean = 1;
					break;
				}
			}
			if (boolean == 0) {
				current_requests[j].documentType = strdup(transactions[i].documentType);
				current_requests[j].transactionType = strdup(transactions[i].transactionType);
				current_requests[j].entityName = strdup(transactions[i].entityName);
				current_requests[j].entityWalletID = strdup(transactions[i].entityWalletID);
				current_requests[j].messageData = strdup(transactions[i].messageData);
				current_requests[j].timeStamp = transactions[i].timeStamp;
				j++;
			}
		}
	}
	unfulfilled = j; // save number of unfulfilled transactions for further use
	// reallocate memory depending on number of transactions
	if (j < numberOfTransactions) {
		current_requests = realloc(current_requests, j * sizeof(struct Transaction));
	}
	return current_requests;
} // end of current_requests()

// function displayes all transactions fo the given period
void view_past_transactions(char* transFileName) {
	struct Transaction* transactions = load_transaction_data(transFileName);
	char startDateInput[11];
	char endDateInput[11];
	int day, month, year;
	struct tm Date;
	char timeStampString[21] = "";
	time_t startDate = 0;
	time_t endDate = 0;
	int transactionFound = 0;

	// check user input for start date
	do {
		printf("Enter the start date from which you want to view payments.\nDate format is dd/mm/yyyy:\n");
		(void)scanf("%s", startDateInput);
		if (!validate_date(startDateInput)) {
			printf("Invalid date format\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
	} while (!validate_date(startDateInput));

	// convert input into time_t
	(void)sscanf(startDateInput, "%d/%d/%d", &day, &month, &year);
	Date.tm_mday = day;
	Date.tm_mon = month - 1;
	Date.tm_year = year - 1900;
	Date.tm_hour = 0;
	Date.tm_min = 0;
	Date.tm_sec = 0;
	startDate = mktime(&Date);

	// check user input for end date and turn string into time_t
	do {
		printf("Enter the end date till which you want to view payments.\nDate format is dd/mm/yyyy:\n");
		(void)scanf("%s", endDateInput);
		if (!validate_date(endDateInput)) {
			printf("Invalid date format\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
	} while (!validate_date(endDateInput));

	// convert input into time_t
	(void)sscanf(endDateInput, "%d/%d/%d", &day, &month, &year);
	Date.tm_mday = day;
	Date.tm_mon = month - 1;
	Date.tm_year = year - 1900;
	Date.tm_hour = 23;
	Date.tm_min = 59;
	Date.tm_sec = 60;
	endDate = mktime(&Date);
	
	// check whether transactions in the period exist
	for (int i = 0; i < numberOfTransactions; i++) {
		if (transactions[i].timeStamp >= startDate && transactions[i].timeStamp <= endDate) {
			transactionFound++;
		}
	}
	if (transactionFound == 0) {
		printf("You do not have any transactions for the given period\n");
		free_transaction_array(transactions, numberOfTransactions);
		selection = 0;
		return;
	}
	else {
		printf("The following transactions are found between %s and %s\n\n", startDateInput, endDateInput);
		printf("%-25s %-20s %-25s %-35s %-40s %-45s\n\n",
			"Date and Time", "Transaction Type", "Entity Wallet ID", "Entity Name", "Document Type", "Message");
		// find transactions between user's dates
		for (int i = 0; i < numberOfTransactions; i++) {
			if (transactions[i].timeStamp >= startDate && transactions[i].timeStamp <= endDate) {
				timetToTimestamp(transactions[i].timeStamp, timeStampString, sizeof(timeStampString));
				printf("%-25s %-20s %-25s %-35s %-40s %-45s\n",
					timeStampString, transactions[i].transactionType, transactions[i].entityWalletID,
					transactions[i].entityName, transactions[i].documentType, transactions[i].messageData);
			}
		}
	}
	free_transaction_array(transactions, numberOfTransactions);
	printf("\nReturning to Main Menu\n");
	selection = 0;
} // end of view_past_transactions()

// function validates whether user's input corresponds to required format
int validate_date(char* date) {
	int dateLength = strlen(date);
	// check length
	if (dateLength != 10) {
		return 0;
	}
	// check date separators 
	if (date[2] != '/' || date[5] != '/') {
		return 0;
	}
	// check that every digit between delimeters are numbers
	for (int j = 0; j < dateLength; j++) {
		if (j != 2 && j != 5) {
			if (date[j] < '0' || date[j] > '9') {
				return 0;
			}
		}
	}
	return 1; // data is in valid format
} // end of validate_date()

// function closes all the files and leaves the program
void exit_DIWA(char* docFileName, char* transFileName) {
	FILE* transactionFile = fopen(transFileName, "r");
	// check whether transaction file is open
	if (transactionFile != NULL) {
		fclose(transactionFile);
		printf("\nClosing transactions file...\n");
	}
	// check whether document file is open
	FILE* documentsFile = fopen(docFileName, "r");
	if (documentsFile != NULL) {
		fclose(documentsFile);
		printf("Closing documents file...\n");
	}
	printf("Exiting DIWA...\n"); 
	printf("Bye!\n");
	exit(0);
} // end of exit_DIWA()

// function searches and deletes shared transactions
void privacy_dashboard(char* docFileName, char* transFileName) {
	struct Document* documents = load_document_data(docFileName); 
	struct Transaction* transactions = load_transaction_data(transFileName); 
	int option;
	int docTypeID;
	int docRequest = 0;
	char timeString[21];
	char yesNo;
	time_t mostRecentRequest = 0;
	char dateString[11] = "";
	time_t timestampPrint = 0;
	printf("\n***** Privacy Dashboard *****\n");
	printf("\nCurrent documents available:\n\n");
	for (int i = 0; i < numberOfDocs; i++) {
		printf("%d. %s\n", i + 1, documents[i].docType);
	}
	// check user input
	do {
		printf("\n\nChoose document number to view shared transactions for it or put 0 to return to Main Menu: ");
		if (scanf("%d", &option) != 1) {
			printf("\nInvalid input. Your option must be a number from 1 to %d", numberOfDocs);
			while (getchar() != '\n')
				;
			option = -1;
		}
		else if (option < 0 || option > numberOfDocs) {
			printf("\nThis option does not exist. Please choose an option from the list above.");
		}
	} while (option < 0 || option > numberOfDocs);
	// return to main menu on 0
	if (option == 0) {
		free_document_array(documents);
		free_transaction_array(transactions, numberOfTransactions);
		selection = 0;
		return;
	}
	//save index of document chosen by user
	docTypeID = option - 1;
	// allocate memory for a copy of transactions array
	struct Transaction* sharedTransactions = malloc(numberOfTransactions*sizeof(struct Transaction));
	if (sharedTransactions == NULL) {
		printf("\nMemory allocation for shared transactions is unsuccessful\n");
		free_document_array(documents); 
		free_transaction_array(transactions, numberOfTransactions); 
		exit(EXIT_FAILURE);
	}
	int numberOfSharedTrans = 0;
	// copy only shared requests
	for (int i = 0; i < numberOfTransactions; i++) {
		if (strcmp(transactions[i].documentType, documents[docTypeID].docType) == 0 &&
			strcmp(transactions[i].transactionType, "S") == 0) {
			sharedTransactions[numberOfSharedTrans].documentType = strdup(transactions[i].documentType);
			sharedTransactions[numberOfSharedTrans].entityName = strdup(transactions[i].entityName);
			sharedTransactions[numberOfSharedTrans].entityWalletID = strdup(transactions[i].entityWalletID);
			sharedTransactions[numberOfSharedTrans].messageData = strdup(transactions[i].messageData);
			sharedTransactions[numberOfSharedTrans].transactionType = strdup(transactions[i].transactionType);
			sharedTransactions[numberOfSharedTrans].timeStamp = transactions[i].timeStamp;
			numberOfSharedTrans++;
		}
	}
	if (numberOfSharedTrans == 0) {
		printf("\nYou do not have any shared transactions for %s\n", documents[docTypeID].docType);
		printf("\nReturning to Main Menu\n");
		free(sharedTransactions); 
		free_document_array(documents); 
		free_transaction_array(transactions, numberOfTransactions);
		selection = 0;
		return;
	}
	// reallocate memory for shared transactions
	if (numberOfSharedTrans < numberOfTransactions) {
		sharedTransactions = realloc(sharedTransactions, numberOfSharedTrans * sizeof(struct Transaction));
		if (sharedTransactions == NULL) {
			printf("\nMemory reallocation for shared transactions failed\nReturning to Main Menu\n");
			free(sharedTransactions);
			free_document_array(documents);
			free_transaction_array(transactions, numberOfTransactions);
			exit(EXIT_FAILURE);
		}
	}
	// sort transactions by entity name
	if (numberOfSharedTrans > 1) {
		bubbleSort(sharedTransactions, numberOfSharedTrans);
	}
	// display transactions
	printf("\n%-5s %-25s %-30s %-20s %-30s\n", 
		"No.", "Date and Time", "EntityName", "Entity Wallet ID", "Document Type");
	for (int i = 0; i < numberOfSharedTrans; i++) {
		timetToTimestamp(sharedTransactions[i].timeStamp, timeString, sizeof(timeString));
		printf("%-5d %-25s %-30s %-20s %-30s\n", (i + 1), timeString,
			sharedTransactions[i].entityName, sharedTransactions[i].entityWalletID, sharedTransactions[i].documentType);
	}
	// ask user to choose transaction for deletion and check input
	do {
		printf("\nEnter a number of record for deletion or put 0 to call Main Menu: ");
		if (scanf("%d", &option) != 1) {
			printf("\nInvalid input. Your option must be a number from 0 to %d\n", numberOfSharedTrans);
			while (getchar() != '\n')
				;
			option = -1;
		}
		else if (option < 0 || option > numberOfSharedTrans) {
			printf("\nThis option does not exist\n");
		}
	} while (option < 0 || option > numberOfSharedTrans);
	// return to main memu on 0
	if (option == 0) {
		free_document_array(documents);
		free_transaction_array(transactions, numberOfTransactions);
		free_transaction_array(sharedTransactions, numberOfSharedTrans);
		selection = 0;
		return;
	}
	// display record for deletion according to user's input
	timetToTimestamp(sharedTransactions[option-1].timeStamp, timeString, sizeof(timeString)); 
	printf("\n%-5d %-25s %-30s %-20s %-30s\n", option, timeString,
		sharedTransactions[option - 1].entityName, sharedTransactions[option - 1].entityWalletID, 
		sharedTransactions[option - 1].documentType);
	printf("\nAre you sure you wish to revoke your shared %s data from the entity %s? (Y/N) ",
		documents[docTypeID].docType, sharedTransactions[option - 1].entityName);
	(void)scanf(" %c", &yesNo);
	while (getchar() != '\n');
	// check user input 
	while (1) {
		if (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n') {
			printf("Invalid answer. Please enter Y or y or N or n: ");
			(void)scanf(" %c", &yesNo);
			while (getchar() != '\n');
		}
		else {
			break;
		}
	}
	if (yesNo == 'Y' || yesNo == 'y') {
		for (int i = 0; i < numberOfTransactions; i++) {
			if (strcmp(sharedTransactions[option - 1].entityWalletID, transactions[i].entityWalletID) == 0 &&
				strcmp(sharedTransactions[option - 1].documentType, transactions[i].documentType) == 0) {
				if (strcmp(transactions[i].transactionType, "R") == 0) {
					timestampPrint = transactions[i].timeStamp; // save time for further displaying to user
				}
				strcpy(transactions[i].transactionType, "D"); // mark transactions for delition with type D
			}
		}
		// open original file for reading
		FILE* transactionFile = fopen(transFileName, "r");
		if (transactionFile == NULL) {
			printf("Error opening the file\n");
			return;
		}
		// create temporary file for writing
		FILE* tempFile = fopen("temp.txt", "w");
		if (tempFile == NULL) {
			printf("Error creating temporary file\n");
			fclose(transactionFile); 
			return;
		}
		int currentLine = 0;
		char buffer[1000];
		// copy line by line exept lines murked with transaction type D
		while (fgets(buffer, sizeof(buffer), transactionFile) != NULL) {
			if (currentLine == 0 || (currentLine > 0 && strcmp(transactions[currentLine - 1].transactionType, "D") != 0)) {
				fputs(buffer, tempFile);
			}
			currentLine++;
		}
		fclose(transactionFile);
		fclose(tempFile);
		remove(transFileName);         
		(void)rename("temp.txt", transFileName); 
		timetToTimestamp(timestampPrint, timeString, sizeof(timeString)); 
		printf("\nShare transaction deleted. Matching request on %s deleted.\n", timeString);
	}
	else if (yesNo == 'N' || yesNo == 'n') {
		printf("\nNo records are deleted\nReturning to Main Menu\n\n");
	}
	// free memory and return to main menu
	free_transaction_array(sharedTransactions, numberOfSharedTrans);
	free_document_array(documents);
	free_transaction_array(transactions, numberOfTransactions);
	printf("\nClosing Privacy Dashboard. Returning to Main Menu\n");
	selection = 0;
} // end of privacy_dashboard()

// function deletes request of user choice 
void delete_request(struct Transaction* transactions, struct Transaction* requests, char* filename, int option) {
	for (int i = 0; i < numberOfTransactions; i++) {
		if (requests[option-1].timeStamp == transactions[i].timeStamp && 
			(strcmp(requests[option-1].entityWalletID, transactions[i].entityWalletID) == 0) &&
			(strcmp(requests[option-1].documentType, transactions[i].documentType) == 0)) {
			FILE* transactionFile = fopen(filename, "r");
			if (transactionFile == NULL) {
				printf("Error opening the file\n");
				return;
			}
			FILE* tempFile = fopen("temp.txt", "w");
			if (tempFile == NULL) {
				printf("Error creating temporary file\n");
				fclose(transactionFile);
				return;
			}
			int currentLine = 0;
			char buffer[1000];
			while (fgets(buffer, sizeof(buffer), transactionFile) != NULL) {
				if (currentLine == 0 || currentLine != i + 1) {
					fputs(buffer, tempFile);
				}
				currentLine++;
			}
			fclose(transactionFile);
			fclose(tempFile);
			remove(filename);          
			(void)rename("temp.txt", filename); 
			printf("\nRequest No. %d succesfully deleted\n\n", option);
		}
	}
} // end of delete_request()

// function frees memory allocated for an array of transactions structs and all strings inside
void free_transaction_array(struct Transaction* transArray, int sizeOfArray) {
	for (int i = 0; i < sizeOfArray; i++) {
		free(transArray[i].documentType);
		free(transArray[i].transactionType);
		free(transArray[i].entityWalletID);
		free(transArray[i].entityName);
		free(transArray[i].messageData);
	}
	free(transArray);
} // end of free_transaction_array()

// function frees memory allocated for an array of documents structs and all strings inside 
void free_document_array(struct Document* documents) {
	for (int i = 0; i < numberOfDocs; i++) {
		free(documents[i].DocumentID); 
		free(documents[i].docType); 
		free(documents[i].countryCode); 
		free(documents[i].field_1); 
		free(documents[i].field_2); 
		free(documents[i].field_3); 
	}
	free(documents);
} 

// function sorts shared transactions by entity name
void bubbleSort(struct Transaction* sharedTrans, int numberOfShared) {
	for (int pass = 1; pass < numberOfShared; pass++) {
		for (int i = 0; i < numberOfShared - 1; i++) {
			if (strcmp(sharedTrans[i].entityName, sharedTrans[i + 1].entityName) > 0) {
				swap_structs(&sharedTrans[i], &sharedTrans[i + 1]);
			}
		} 
	} 
} // end bubbleSort

// function helps to avoid memory allocation for temporary struct while sorting
void swap_structs(struct Transaction* toSwap_1, struct Transaction* toSwap_2) {
	struct Transaction temporary = *toSwap_1;
	*toSwap_1 = *toSwap_2;
	*toSwap_2 = temporary;
} // end of swap_structs