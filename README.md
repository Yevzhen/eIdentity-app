# eIdentity-app
C-based prototype application to securely host eIdentity and share personal documents with entities

## Files required to run the program
WalletIDTransactions-2.txt <br>
WalletIDDocuments-3.txt


## Program Introduction
This is a Digital Identity Wallet Application Prototype that allows to host eIdentity and to share personal documents with entities. The application’s users should be able to see incoming requests, share documents and view shared transaction history. Thus, main menu should contain the following options to choose: <br>
⦁	View documents (to view available documents for sharing). <br>
⦁	View current requests (to view incoming unfulfilled requests).<br>
⦁	Share document (to share document with entities).<br>
⦁	Privacy dashboard (to revoke shared data).<br>
⦁	View past transactions (to view all transactions between two dates entered by user).<br>
⦁	Exit (to leave the application).<br>

## Pseudocode

The pseudocode for my program is presented below. <br>
**Top level tasks** <br>
Display home page<br>
Display main menu<br>
Display documents<br>
Display outstanding requests<br>
Share document<br>
Revoke shared transaction<br>
Display transactions between given dates<br>
Exit app<br>

**Functions refinements** <br> 

*Home page:* <br>
  display header “Digital Identity Wallet”<br>
  display date<br>
  display status: number of requests for the last 24 hours - use count_recent_requests() function<br>
  display user’s details (wallet ID, name, date of birth)<br>
  display main menu - use main_menu() function<br> <br>
*Main menu:* <br>
	display list of actions available to the user<br>
	ask user to choose desired action<br>
	validate user’s input <br>
	switch function according to user’s choice <br> <br>
*Display documents:* <br>
	load documents - use load_document_data() function <br>
  display header and list of available documents <br>
  ask user to choose calling main menu or exiting app <br> <br>
*Display outstanding requests:* <br>
	load transactions - use load_transaction_data() function <br>
  load unfulfilled requests - use current_requests() function <br>
  display unfulfilled requests with header <br>
  ask user whether they want to delete any request<br>
  validate user’s input<br>
  &nbsp;&nbsp;&nbsp;&nbsp; if yes – delete request - use delete_request() function <br>
  &nbsp;&nbsp;&nbsp;&nbsp; if no – call main menu <br> <br>
*Share document:* <br>
	display documents - use load_documents_data() function <br>
  ask user to choose which document they want to share <br>
  load transactions and outstanding transactions - use load_transactions_data() function; use current_requests() function <br>
  display outstanding requests for the given document <br>
  ask user whether they want to (i) respond to the most recent request or (ii) enter entity which they want to share the document with manually <br>
    &nbsp;&nbsp;&nbsp;&nbsp; if (i) { <br>
	    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; display data for the most recent request <br>
	    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ask to choose data for sharing <br>
	    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; write chosen data into new transaction struct in array of transactions <br>
    &nbsp;&nbsp;&nbsp;&nbsp;} <br>
    &nbsp;&nbsp;&nbsp;&nbsp; if (ii) { <br>
	    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ask user to enter entity manually <br>
	    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ask to choose data for sharing <br>
	    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; write chosen data into new transaction struct in array of transactions <br>
    &nbsp;&nbsp;&nbsp;&nbsp; } <br>
	write new data into the transactions file <br> <br>
*Revoke shared transaction:* <br>
	display documents - use load_documents_data() function <br>
  ask user to choose document to view shared transactions for it <br>
  display shared transactions: <br>
    &nbsp;&nbsp;&nbsp;&nbsp;⦁	use load_transactions_data() function <br>
    &nbsp;&nbsp;&nbsp;&nbsp;⦁	copy shared transactions to another array <br>
    &nbsp;&nbsp;&nbsp;&nbsp;⦁	sort shared transactions by entity name (use bubbleSort() function) <br>
  ask user whether they want to delete any shared transaction  <br>
   &nbsp;&nbsp;&nbsp;&nbsp;⦁ if no – call main menu  <br>
   &nbsp;&nbsp;&nbsp;&nbsp;⦁	if yes {  <br>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   find matching request  <br>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  delete chosen shared transaction and matching request from file  <br>
   &nbsp;&nbsp;&nbsp;&nbsp;}  <br>  <br>
*Display past transactions:*  <br>
	load transactions - use load_transactions_data() function  <br>
  ask user to enter start date and validate input  <br>
	ask user to enter end date and validate input  <br>
	display all transactions between the two dates  <br>  <br>
*Exit:*  <br>
	check whether files are closed  <br>
	print message about exiting  <br>  <br>

**Required functions that are not in the main menu**  <br>
*Load document data:*  <br>
	create struct that contains hardcoded data for eIdentity document	 <br>
  read documents file counting lines, rewind <br>
	dynamically allocate memory for array of structs <br>
	read documents file writing all the data into structs <br>
	close file <br>
	return a pointer on the array of structs <br>  <br>
*Load transactions data*  <br>
	read transactions file counting lines (exclude first line), rewind  <br>
	dynamically allocate memory for array of structs  <br>
	read transactions file writing all the data into structs  <br>
	close file  <br>
	return a pointer on the array of structs  <br> <br>
*Count recent requests*  <br>
	loop through transactions array  <br>
	find all the transactions with transaction type “R”  <br>
	fetch time of found transaction  <br>
	compare time with current time  <br>
	if difference is less then 24*60*60, increment counter  <br>
  return counter  <br>  <br>
*Delete request:*  <br>
	loop through array of transactions  <br>
	find transactions with specified date, entity wallet ID and document type  <br>
	delete the found request from transactions file  <br>  <br>

