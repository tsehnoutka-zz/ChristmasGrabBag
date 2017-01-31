/*
 ============================================================================
Name        : ChristmasGrabBag.c
 Author      : Tim Edwards
 Version     :
 Copyright   : BE NICE!
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

    #define OS_Windows 0
	#include <SendMailLinux.h>

#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

    #define OS_Windows 1
	#include <SendMailWin.h>
    #define DIV 1048576
    #define WIDTH 7

#endif

#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // for sleep ()`

#define TEST 1

#define PRINT_TO_SCREEN 1

// to send emails to participants
//#define EMAIL 1
//const int iEmailDelay 1; //  delay between sending of emails (in seconds)

//  To Print to Printer
#define PRINT_TO_PRINTER 1
const int PAGEFEED=12;



typedef int bool;
#define true 1
#define false 0


#ifdef EMAIL
	typedef struct _Person
	{
		char Name[100];
		int Spouse;
		int BuyFor;
		int Picked;
		char email[100];
	}Person;
#else
	typedef struct _Person
	{
		char Name[100];
		int Spouse;
		int BuyFor;
		int Picked;
	}Person;
#endif


#ifdef TEST
#define NumOfFriends  6

	#ifdef EMAIL
		Person Friends[NumOfFriends]= {
				{"Tim"		,1,-1,-1, "tim@tedwards.org"},
				{"Tracey"	,1,-1,-1, "tim@tedwards.org"},
				{"Erin"		,2,-1,-1, "tim@tedwards.org"},
				{"Alex"		,2,-1,-1, "tim@tedwards.org"},
				{"Belle"	,3,-1,-1, "tim@tedwards.org"},
				{"Snickers" ,3,-1,-1, "tim@tedwards.org"}
		};  //  end of person EMAIL
	#else
		Person Friends[NumOfFriends]= {
			    {"Tim"		,1,-1,-1},
			    {"Tracey"	,1,-1,-1},
			    {"Erin"		,2,-1,-1},
				{"Alex"		,2,-1,-1},
				{"Belle"	,3,-1,-1},
				{"Snickers" ,3,-1,-1}
		};  //  end of Person
	#endif
#endif



int main(int argc, char* argv[])
{
int i,x,attempt;
bool Completed = false;
char ch='n';

//prime the random pump....TWICE
int rand1 = (unsigned)time(NULL)*NumOfFriends;
srand (rand1);
int rand2 = (unsigned)time(NULL)*(unsigned)time(NULL);
srand (rand2);


printf( "rand1 = %i\nrand2 = %i\n",rand1,rand2);

/*
	Starting with the first person in the list(Friend[x]), get a random number of someone (Friend[attempt])in the list.
	Check to see if the Friend[attempt] is already picked (Friends[attempt].Picked) or if they are in the same family(Friends[attempt].Spouse == Friends[x].Spouse)
    if there are two people left and they are in the same family, reset the table and start over.
*/
while(!Completed)
	{
		for (x=0;  x<NumOfFriends; x++)  // go through all the people
		{
			while (Friends[x].BuyFor == -1)  //  Find some one for x person to Buy for
			{
				attempt = rand()%NumOfFriends;

				//  if the attempted person has not yet been picked, and the attempted person is NOT them self
				if ( (Friends[attempt].Picked == -1)  && (Friends[attempt].Spouse != Friends[x].Spouse)	)  // If the two people are in the same family || attempt == x(i.e. the same person)
				{
					Friends[x].BuyFor = attempt;
					Friends[attempt].Picked = 1;
				}

				int NumAvailable=0;  //  are there people not related that have not been picked
				int done=0;			 // has everyone been picked?
				for (i=0; i < NumOfFriends; i++) // go through everyone
				{
					// of the people that have NOT been picked, and NOT related to this person, they are available
					if ( (Friends[i].Picked == -1) && (Friends[i].Spouse != Friends[x].Spouse))
						NumAvailable++;
					if(Friends[i].Picked == 1)
						done++;
				}

				//  If no one is available to be be picked, AND we are NOT done
				if ( ( 0==NumAvailable ) && (done != NumOfFriends) ) // reset the search
				{
	        		// clear all the fields to start all over
					printf("Try again\n");
	        		for ( i = 0; i < NumOfFriends; i++)
	        		{
	        			Friends[i].BuyFor = -1;
	        			Friends[i].Picked = -1;
	        		}

	        		x = -1;  // reset the counter for the "for loop"  -  BAD!! coding technique
	        		Completed = false;
					break; // break out of While loop and restart for loop
				}
				else
				{
					int total=0;
					int y;
					for(y=0;  y<NumOfFriends; y++)  // go through all the people
					{
						if (1 == Friends[y].Picked)  // Count the number of people that have been picked
							total++;
					}
					if (total == NumOfFriends)  //  if everyone has been picked then we are done!
			        		Completed=true;
				}  // do we need to rest, or are we done?
			} //  end  Find some one for this person to Buy for while
		}  //  end for all the people

	}// end while
printf("\n*****  Calculations Complete  *****\n\n");
	for (x=NumOfFriends;  x>0; x--)
	{
		char szText[500] = "";
		sprintf(szText,"%s Has to buy for %s\n",Friends[x-1].Name,Friends[Friends[x-1].BuyFor].Name);

	    #ifdef PRINT_TO_FILE
			strcpy(zFileName,Friends[x-1].Name);
			strcat(zFileName, ".txt\0");
			txtprn=fopen(zFileName, "w");
			//fprintf(txtprn,"%s\n",  Friends[x-1].Name);
			//fprintf(txtprn,"\n");
			//fprintf(txtprn,"%s Has to buy for %s\n",Friends[x-1].Name,Friends[Friends[x-1].BuyFor].Name);
			fprintf("%s",szText);
			fclose(txtprn);
		#endif
		#ifdef PRINT_TO_PRINTER
			// Send out put to printer
			#ifdef __unix__
				FILE *printer = popen("lpr", "w");
				fprintf(printer, "%s Has to buy for %s\n%c",Friends[x-1].Name,Friends[Friends[x-1].BuyFor].Name,PAGEFEED);
				pclose(printer);
			#else
				txtprn=fopen("PRN", "w");
				fprintf(txtprn,"%s\n",  Friends[x-1].Name);
				for(y=0;y < 45;y++)
					fprintf(txtprn,"\n");
				fprintf(txtprn,"%s Has to buy for %s\n%c",Friends[x-1].Name,Friends[Friends[x-1].BuyFor].Name,PAGEFEED);
			#endif
		#endif
		#ifdef PRINT_TO_SCREEN
			printf("%s",szText);
			//if (Friends[x-1].Spouse != Friends[x-2].Spouse)
				//printf("\n");
		#endif
		#ifdef EMAIL
			int ret = sendmail(
				Friends[x-1].email, // to
				"Tsehnoutka@yahoo.com",  // from
				"Crhistmas Grab Bag",
				szText,
				"smtp.comcast.net",
				587
			);

			if (ret != 0)
				fprintf(stderr, "Failed to send mail (code: %i).\n", ret);
			else
				fprintf(stdout, "Mail successfully sent.\n");
					//send_email(Friends[x-1].email,szText);
			sleep(1); //  sleep one sec
		#endif
	} // end for

#ifdef PRINT_TO_SCREEN
	printf("Hit any key to continue...");
	scanf("%c",ch);
#endif

return 0;
}
