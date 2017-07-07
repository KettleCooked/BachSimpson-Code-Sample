/*This program is a Bach Simpson code sample.
It take in two commands, appendlog and dumplog.
Appendlog takes the following arguments:
[-a]             AUX flag. Entering this set the AUX flag high.
-f "filename"    Enter file name without quotations.
-t "message"     Enter record message without quotations.
Dumplog takes the following arguments:
-f "filename"    Enter file name without quotations.

The DST, daylight savings flag, is set automatically depending
on the local timezone.
The record message can be a maximum of 30 bytes, or 30 characters.
The checksum feature is incomplete and only there to complete the
program structure.

Made by: Mohamed Ktaily
Last revision: July 6, 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void prgmLoop(void);				//Main program loop.
char *readInput(void);				//Take in user input
char **parseInput(char*);			//Parse user input into usable commands
bool verifyAppendComs(char**);		//Verify user commands entered properly
bool verifyDumpComs(char**);		//Verify user commands entered properly
void appendLog(char**);				//Execute the appendlog command
void dumpLog(char**);				//Execute the dumplog command
char checksumFunc(char, char);		//Performs checksum computation
void allocError(char*);				//Checks for memory allocation errors


int main(void)
{
	//Output program intro
	printf("     -----Bach Simpson coding sample-----\n");
	printf("     ------------------------------------\n\n\n");

	//Infinite program loop until user exits
	prgmLoop();

	return 0;
}

void prgmLoop()
{
	char *userInput;	//Holds user input char by char
	char **commands;	//Holds user commands after input was parsed

	//Loop the program flow - 1. Get user input 2. Parse input 3. Verify input 4. Execute on input 5. Repeat
	do
	{
		userInput = readInput();
		commands = parseInput(userInput);

		if (strcmp(commands[0], "appendlog") == 0)
		{
			if (verifyAppendComs(commands))
			{
				appendLog(commands);
			}
		}
		else if (strcmp(commands[0], "dumplog") == 0)
		{
			if (verifyDumpComs(commands))
			{
				dumpLog(commands);
			}
		}
		else
		{
			puts("Not a valid command.");
		}

	} while (1);
}

char *readInput(void)
{
	int bufSize = 64; 		//Valid user input should be less than the arbitrarily chosen 64*1 byte, since record holds max 40 bytes
	int bufPos = 0;			//Holds buffer position
	char *userInput = malloc(sizeof(char) * bufSize);	//Dynamic array holding user input. Dynamic just in case user enters long input
	int charFetched;		//Holds character entered by user

	allocError(userInput);	//Check if memory allocated successfully

	//Loop until finished reading in user input
	for(;;)
	{
		charFetched = getchar();

		//If we hit End Of File, save it as a null character and exit loop
		if (charFetched == EOF || charFetched == '\n') {
			userInput[bufPos] = '\0';
			return userInput;
		}
		else
		{
			userInput[bufPos] = charFetched;
		}
		bufPos++;

		//If user input exceeded the buffer, reallocate into larger buffer
		if (bufPos >= bufSize)
		{
			bufSize += 64;
			userInput = realloc(userInput, bufSize);
			allocError(userInput);
		}
	}
}

char **parseInput(char *userInput)
{
	int bufSize = 64;		//Valid user input should be less than the arbitrarily chosen 64*1 byte, since record holds max 40 bytes
	int bufPos = 0;			//Holds buffer position
	char **tokens = malloc(sizeof(char*) * bufSize);	//Dynamic array holding user commands. Dynamic just in case user enters too many commands
	char *token;			//Holds each command seperately

	allocError(tokens);		//Check if memory allocated successfully

	token = strtok(userInput, " ");	//Parse user input with 'space' as the only delimiter

	//Loop until all user input parsed
	while (token != NULL)
	{
		tokens[bufPos] = token;
		bufPos++;

		//If user input exceeded the buffer, reallocate into larger buffer
		if (bufPos >= bufSize) {
			bufSize += 64;
			tokens = realloc(tokens, sizeof(char*) * bufSize);
			allocError(tokens);
		}

		token = strtok(NULL, " ");
	}

	tokens[bufPos] = NULL; //End last token with a NULL value
	return tokens;
}

bool verifyAppendComs(char **commands)
{
	//Booleans to make sure a command isn't entered and executed more than once, or if it was not entered at all
	bool flagCom = false, fCom = false, tCom = false;

	//Variables of the aux, -f, and -t command locations in user input
	int fPosition = 0;
	int aPosition = 0;
	int tPosition = 0;

	//Size of text message user wants to enter
	int messageSize = 0;

	/*Loop through command line, check if some common errors are made before executing the commands.
	Common errors include entering same comand twice, incorrectly entering command, not entering enough
	arguments to properly execute command, entering text message larger than max 30 bytes that log records
	holds.*/
	for (int position = 1; commands[position] != NULL; position++)
	{
		if (strcmp(commands[position], "[-a]") == 0)
		{
			aPosition = position;
			if (flagCom == false)
			{
				flagCom = true;
			}
			else
			{
				puts("Don't enter same command twice!!");
				return false;
			}
		}

		if (strcmp(commands[position], "-f") == 0)
		{
			fPosition = position;
			if (commands[position + 1] != NULL)
			{
				if (strcmp(commands[position + 1], "[-a]") == 0 || strcmp(commands[position + 1], "-t") == 0)
				{
					puts("Don't follow the -f command with another command!!");
					return false;
				}
				if (fCom == false)
				{
					fCom = true;
				}
				else
				{
					puts("Don't enter same command twice!!");
					return false;
				}
			}
		}

		if (strcmp(commands[position], "-t") == 0)
		{
			tPosition = position;
			if (tPosition <= fPosition || tPosition <= aPosition)
			{
				puts("Please enter the -t command after you have entered the filename or the optional Aux flag command.");
				return false;
			}

			if (commands[position + 1] != NULL)
			{
				if (strcmp(commands[position + 1], "[-a]") == 0 || strcmp(commands[position + 1], "-f") == 0)
				{
					puts("Don't follow the -t command with another command!!");
					return false;
				}

				if (tCom == false)
				{
					tCom = true;
				}
				else
				{
					puts("Don't enter same command twice!!");
					return false;
				}

				for (int x = 1; commands[position + x] != NULL; x++)
				{
					for (int y = 0; commands[position + x][y] != NULL; y++)
					{
						messageSize++;
					}
					if (commands[position + x + 1] != NULL)
					{
						messageSize++;	//Accounting for spaces between words
					}
				}

				if (messageSize > 30)
				{
					puts("Text message is greater than 30 bytes. Records takes max 30 byte messages.");
					return false;
				}
			}
			else
			{
				puts("Nothing entered after -t command!");
				return false;
			}
		}
	}

	if (fCom == false)
	{
		puts("Filename not specified!");
		return false;
	}

	if (messageSize > 30)
	{
		puts("Text message is more than 30 bytes, record structure requires 30 bytes or less for message.");
		return false;
	}

	//Return true if all the above is OK
	return true;
}

bool verifyDumpComs(char **commands)
{
	//Booleans to make sure a command isn't entered and executed more than once
	bool fCom = false;

	//Loop through command line, check if some common errors are made before executing the commands
	for (int position = 1; commands[position] != NULL; position++)
	{
		if (strcmp(commands[position], "-f") == 0)
		{
			if (fCom == false)
			{
				fCom = true;
			}
			else
			{
				puts("Don't enter same command twice!!");
				return false;
			}
		}
	}

	if (fCom == false)
	{
		puts("Filename not specified!");
		return false;
	}

	//Return true if above is OK
	return true;
}

void appendLog(char **commands)
{
	FILE* logfile = NULL;			//File pointer
	unsigned char spareByte = 0x00;	//Holds a byte of 0 for use in log file creation
	char *filename;					//Holds filename entered by user

	//Variables holding info on number of records in file
	unsigned char recordsHighByte = 0;
	unsigned char recordsLowByte = 0;
	unsigned short records = 0;

	bool auxFlag = false;			//Aux flag set by user
	bool daylightFlag = false;		//DST flag set by program
	unsigned char flagByte = 0;		//Byte holding the aux and DSL bits in a byte
	char message[30] = {'\0'};		//Array set to 30 bytes for records message
	char messageChar;				//Individual characters from record message
	int messageLen = 0;				//Holds actual message length to fill the rest of 30 bytes with spaces
	unsigned char checksum = 0x00;	//Holds checksum value for record being processed

	//Check user commands and arguments and execute accordingly
	for (int position = 1; commands[position] != NULL; position++)
	{
		if (strcmp(commands[position], "[-a]") == 0)
		{
			auxFlag = true;		//Set AUX flag bit high
		}

		if (strcmp(commands[position], "-f") == 0)
		{
			filename = commands[position + 1];
			printf("filename is: %s\n", filename);
		}

		if (strcmp(commands[position], "-t") == 0)
		{
			//Loop message length until <= 30 bytes, and if so fill the rest with 'spaces' until reached 30 bytes
			for (int x = 1; commands[position + x] != NULL && x <= 30; x++)
			{
				for (int y = 0; commands[position + x][y] != NULL && y <= 29; y++)
				{
					message[messageLen] = commands[position + x][y];
					messageLen++;
				}
				if (commands[position + x + 1] != NULL)
				{
					message[messageLen] = ' ';
					messageLen++;
				}
			}
		}
	}

	//Open file for reading and writing, create it if it doesn't exist
	logfile = fopen(filename, "r+");
	if (logfile == NULL)
	{
		puts("Creating file...");
		logfile = fopen(filename, "w+");
	}

	//Check if file opened or created succesffuly
	if (logfile == NULL)
	{
		puts("Unable to open or create file!");
		return;
	}

	puts("Adding new record...\n");
	
	fseek(logfile, 0, SEEK_SET);	//Move to beginning of file

	//Write number of records to file
	int ch = 0;
	int lines = 0;

	//Check how many records are on file*********************************************************
	if (fgetc(logfile) != EOF)
	{
		fseek(logfile, 0, SEEK_SET);

		recordsHighByte = fgetc(logfile);
		recordsLowByte = fgetc(logfile);
		records = recordsLowByte | (recordsHighByte << 8);
	}

	//Increment records since adding this new one
	records++;

	//Update number of records on file, followed by two spare bytes if its a new file
	recordsHighByte = (records >> 8) & 0xff;
	recordsLowByte = records & 0xff;

	fseek(logfile, 0, SEEK_SET);
	fprintf(logfile, "%c%c", recordsHighByte, recordsLowByte);
	if (records <= 1)
	{
		fprintf(logfile, "%c%c", spareByte, spareByte);
	}

	//Before appending new record to file, overwrite old checksum unless its the first record in a new file
	if (records > 1)
	{
		fseek(logfile, -1, SEEK_END);
	}
	else
	{
		fseek(logfile, 0, SEEK_END);
	}

	//Add sequence number to file
	fprintf(logfile, "%c%c", recordsHighByte, recordsLowByte);

	checksum = checksumFunc(recordsHighByte, checksum);		//Update record checksum

	//Set flagByte to hold positive aux flag bit, stay 0 otherwise
	if (auxFlag == true)
	{
		flagByte = 0b10000000;
	}
	else
	{
		flagByte = 0b00000000;
	}

	//Check if we are in daylight savings time when this record was created, and update flagByte accordingly
	struct tm *str_timeNow;			
	time_t timeNow;
	time(&timeNow);
	str_timeNow = localtime(&timeNow);
	if (str_timeNow->tm_isdst == 1)
	{
		flagByte = flagByte | 0b01000000;
		puts("Setting DST flag bit to true accoring to current timezone.\n");
	}
	else if (str_timeNow->tm_isdst == -1)
	{
		puts("Error getting daylight savings flag, but setting it to 0.\n");
	}
	
	//Add flagByte to file
	fprintf(logfile, "%c", flagByte);

	checksum = checksumFunc(flagByte, checksum);	//Update record checksum

	//Add the timestamp to the record in seconds, 4 bytes total
	struct tm str_timeRef;			//Record requires time since Jan 1 2000, this acts as reference
	time_t timeRef;

	str_timeRef.tm_year = 2000 -1900;
	str_timeRef.tm_mon = 0;
	str_timeRef.tm_mday = 1;
	str_timeRef.tm_hour = 0;
	str_timeRef.tm_min = 0;
	str_timeRef.tm_sec = 0;
	str_timeRef.tm_isdst = 0;

	timeRef = mktime(&str_timeRef);

	unsigned int timeDif = 0;
	timeDif = difftime(timeNow, timeRef);

	unsigned char timeByte1 = 0;
	unsigned char timeByte2 = 0;
	unsigned char timeByte3 = 0;
	unsigned char timeByte4 = 0;

	timeByte1 = (timeDif >> 24) & 0xff;
	timeByte2 = (timeDif >> 16) & 0xff;
	timeByte3 = (timeDif >> 8) & 0xff;
	timeByte4 = timeDif & 0xff;

	fprintf(logfile, "%c%c%c%c", timeByte1, timeByte2, timeByte3, timeByte4);

	//Update record checksum
	checksum = checksumFunc(timeByte1, checksum);
	checksum = checksumFunc(timeByte2, checksum);
	checksum = checksumFunc(timeByte3, checksum);
	checksum = checksumFunc(timeByte4, checksum);

	//Add 2 spare bytes to record
	fprintf(logfile, "%c%c", spareByte, spareByte);

	//Add message to file, and check how many spaces needed to reach full 30 bytes space on record
	for (int x = 0; x <= messageLen; x++)
	{
		fprintf(logfile, "%c", message[x]);
		checksum = checksumFunc(message[x] & 0xff, checksum);	//Update checksum
	}
	int numOfSpaces = 0;
	numOfSpaces = 30 - messageLen;

	//Add spaces needed to record
	for (int x = 1; x < numOfSpaces; x++)
	{
		fprintf(logfile, " ");
		checksum = checksumFunc(" ", checksum);		//Update checksum
	}

	//Add Checksum to file, which is ones compliment of the sum of all previous bytes in this particular record sequence
	fprintf(logfile, "%c", checksum);

	int filesize = 0;
	fseek(logfile, 0, SEEK_END);
	filesize = ftell(logfile);

	printf("File is %d bytes.\n", filesize);

	//Add entire log checksum
	fseek(logfile, 0, SEEK_SET);
	unsigned char logchecksum = 0x00;

	for (int x = 0; x <= filesize; x++)
	{
		logchecksum = checksumFunc(fgetc(logfile), logchecksum);
	}
	fprintf(logfile, "%c", logchecksum);

	printf("Log file %s now has %d records.\n", filename, records);

	fclose(logfile);
}

void dumpLog(char **commands)
{
	FILE* logfile = NULL;	//File pointer initialization
	char *filename;			//Holds filename

	//Variables holding info on number of records in file
	unsigned char recordsHighByte = 0;
	unsigned char recordsLowByte = 0;
	unsigned short records = 0;

	//Variables holding info on record sequence number
	unsigned char sequenceHighByte = 0;
	unsigned char sequenceLowByte = 0;
	unsigned short sequence = 0;

	//Variables holding AUX and DST flag info in record
	bool auxFlag = false;
	bool daylightFlag = false;
	unsigned char flagByte = 0;			//Byte holding the aux and DSL bits

	//Variables holding info on record timestamp
	unsigned char timeByte1 = 0;
	unsigned char timeByte2 = 0;
	unsigned char timeByte3 = 0;
	unsigned char timeByte4 = 0;
	unsigned int timeStamp = 0;

	char message[30] = { '\0' };		//Holds 30 byte message on record
	unsigned char checksum = 0x00;		//Checksum value for each record
	unsigned char logchecksum = 0x00;	//Checksum value for entire log file

	//Get filename from user input
	for (int position = 1; commands[position] != NULL; position++)
	{
		if (strcmp(commands[position], "-f") == 0)
		{
			filename = commands[position + 1];
			break;
		}
	}

	//Open file for reading only and check if opened successfuly
	logfile = fopen(filename, "r");
	if (logfile == NULL)
	{
		puts("Unable to open file!");
		return;
	}

	//Check how many records are on file
	fseek(logfile, 0, SEEK_SET);		//Go to beginning of file

	recordsHighByte = fgetc(logfile);
	recordsLowByte = fgetc(logfile);
	records = recordsLowByte | (recordsHighByte << 8);

	//fseek(logfile, 2, SEEK_CUR);		//Skip the two spare bytes

	//Print number of records on file
	printf("\n\n******Number of records on %s: %u ******\n\n",filename, records);

	//Read and print log file checksum value
	fseek(logfile, -1, SEEK_END);
	logchecksum = fgetc(logfile);
	printf("-Log checksum: %d\n\n\n", logchecksum);

	fseek(logfile, 4, SEEK_SET);

	//Print out record info for all records on file
	for (int x = 1; x <= records; x++)
	{
		//Get record sequence
		sequenceHighByte = fgetc(logfile);
		sequenceLowByte = fgetc(logfile);
		sequence = sequenceLowByte | (sequenceHighByte << 8);

		//Get flag byte with Aux and DST bits
		flagByte = fgetc(logfile);
		if ((flagByte >> 7) == 0b00000001)
		{
			auxFlag = true;
		}
		else
		{
			auxFlag = false;
		}
		if (((flagByte >> 6) & 0b00000001) == 0b00000001)
		{
			daylightFlag = true;
		}
		else
		{
			daylightFlag = false;
		}

		//Get record timestamp info
		timeByte1 = fgetc(logfile);
		timeByte2 = fgetc(logfile);
		timeByte3 = fgetc(logfile);
		timeByte4 = fgetc(logfile);
		timeStamp = (timeByte1 << 24) | (timeByte2 << 16) | (timeByte3 << 8) | (timeByte4);

		fseek(logfile, 2, SEEK_CUR);	//Skip 2 spare bytes

		//Read 30 byte message
		for (int x = 0; x <= 29; x++)
		{
			message[x] = fgetc(logfile);
		}

		//Read sequence checksum
		checksum = fgetc(logfile);

		//Print out all info in organized manner
		printf("   ---Log #%u: ---\n\n", sequence);
		if (auxFlag == true)
		{
			printf("AUX = ON | ");
		}
		else
		{
			printf("AUX = OFF | ");
		}

		if (daylightFlag == true)
		{
			printf("DST = ON\n\n");
		}
		else
		{
			printf("DST = OFF\n\n");
		}

		printf("Timestamp: %d\n\n", timeStamp);

		printf("Message: ");
		for (int x = 0; x <= 29; x++)
		{
			printf("%c", message[x]);
		}
		printf("\n");

		printf("Checksum: %d\n\n\n", checksum);
	}

	//Close file
	fclose(logfile);
}

void allocError(char *location)
{
	if (!location)
	{
		puts("Memory allocation error!");
		exit(EXIT_FAILURE);
	}
}

char checksumFunc(char a, char b)
{
	a += b;

	return a;
}