#define _GNU_SOURCE
#define M_param 3
#define maxmem 5000
#define Max_taskset 5000
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


int commentflag=0; //1 - Show comments; 0 - No comments
struct node
{
	float WCET;
	float deadline;
	float period;
	struct node *previous;
};

//PRINTING LINKEDLIST
void printLL (struct node* temp)
{int i=1;
	printf("#####################################################\n<---------------Linked List Generated--------------->\n#####################################################\n");
	while(temp->previous != NULL)
	{
		printf("%d %f %f %f\n",i, temp->WCET,temp->deadline,temp->period);
		temp = temp->previous;
		i++;
	}
	printf("-----------------------------------------------------\n");
}

void printLL2 (struct node* temp)
{
	printf("Printing\n");
	while(temp->previous != NULL)
	{
		printf("%f %f %f\n", temp->WCET,temp->deadline,temp->period);
		temp = temp->previous;
	}
	printf("Completed\n");
}

//CREATES A TASK --------------A linked list that represents task.
struct node * TaskFunc ( char *metadata, struct node* current)
{
	int i=0;
	char *string, *found;
	struct node *new = (struct node *)malloc(sizeof(struct node));


	string = strdup(metadata);
    while( (found = strsep(&string," ")) != NULL )
    {
       	switch(i)
      	{
      		case 0: new->WCET = atof(found);
      		break;
      		case 1: new->deadline = atof(found);
      		break;
      		case 2: new->period = atof(found);
      		break;
      	}i++;
    }
    new->previous = current;
    printf("%f %f %f\n", new->WCET, new->deadline, new->period);
return new;
}
//##############################################################################################################################################
//EDF TESTING FUNCTION
int EDFLoad(float TotalComp, struct node* front)
{
	//printLL(front);
	///code for Loading Test
	if(commentflag==1){printf("Density>1\n<---------------Loading Test--------------->\n");}
	float l0=0;
	float l1=TotalComp;
	float cdeadline=0;
	float cperiod=0;
	float Lfactor=0;
	int countloop = 0;
	float H=0;
	int flag=0;

	struct node *current;
	struct node *tracker;
	struct node *tracker2;
	while(l0!=l1)
	{
		l0=l1;
		l1=0;
		current = front;
		while(current->previous!= NULL)
		{
			l1 += ceil(l0/(current->period))*(current->WCET);
			current = current->previous;
		}
	}
	if(commentflag==1){printf("Sync Busy Period: %f\n", l1);}

	//DEADLINES AND TASKS UTILIZATION;
	//Deadlines Tester
	while(cdeadline < l1)
	{
		tracker = front;
		if(flag==1)
			{break;}

		//Finding each deadiline
		while(tracker->previous!=NULL)
		{
			
			cperiod = (countloop*(tracker->period));
			cdeadline = (countloop*cperiod) + tracker->deadline;
			if(cdeadline>=l1){ tracker = tracker->previous; continue;}
			//CALCULATING LOADING STUFF
			H=0;
			tracker2 = front;
			while(tracker2->previous!=NULL)
			{
				H += (tracker2->WCET)*((floor(((cdeadline - tracker2->deadline)/tracker2->period))+1));
				tracker2 = tracker2->previous;
			}
			if ((H > cdeadline))
				{
					if(commentflag==1){printf("EDF Loading Test: Not Schedulable\n");} 
					flag=1;
					
				}
			Lfactor = H/cdeadline;
			if (commentflag==1)
				{
					printf("Deadline: %f Demand: %f Loading Factor: %f\n", cdeadline, H, Lfactor);
				}
			tracker = tracker->previous;			
			//printLL(tracker);
		}
		countloop++;
	}
	if ((flag==0)&&(commentflag==1))
		{
			printf("EDF Loading Test: Schedulable\n");
		}
return flag;
}

int EDFSchedule(struct node *head, float UEDF)
{
	int SFlag=0;
	//dgp Flag = 0 -- when normal EDF, 1 -- when loading factor to be used
	//printLL(head);
	int dgp=0;
	int flag;
	float DensityU=0;
	float Wsum=0;
	float denom = 0;
	struct node *current = head;
	while(current->previous!=NULL)
	{
		Wsum = Wsum + current->WCET;
		denom = ((current->period)<(current->deadline))?current->period: current->deadline;
		DensityU += (current->WCET)/(denom);
		if((current->deadline)<(current->period))	
		{dgp=1;}
		current= current->previous;
	}
	if(commentflag==1){printf("#####################################################\n<----------------------EDF TEST--------------------->\n#####################################################\n");}
	if(commentflag==1){printf("UEDF: %f Total Density Utilization: %f\n", UEDF, DensityU);}
	switch(dgp)
	{
		case 0: //(UEDF<=1)?():(if(commentflag==1){printf("1.Not Schedulable\n");});
				if(UEDF<=1)
				{
					if(commentflag==1){printf("Schedulable\n");} SFlag=1;
				}
				else
				{
					if(commentflag==1){printf("Not Schedulable\n");} SFlag=0;
				}
		break;
		case 1: //Logic for density 
				if(commentflag==1){printf("Deadline < Period\n");}
				if (DensityU>1)
					{
						//printf("REACHED HERE %f\n", Wsum);
						flag = EDFLoad(Wsum, head);
						SFlag = (flag==1)?0:1;
						if((commentflag==1)&&(SFlag==1)){printf("Schedulable\n");}
						if((commentflag==1)&&(SFlag==0)){printf("Not Schedulable\n");}
					}
				else
					{
						if(commentflag==1){printf("Schedulable\n");} SFlag=1;
					}
		break;		
	}
return SFlag; 
}

//EDF - SORTING ACCORTING TO DEADLINE 
	struct node * EDFsort (struct node *head, float taskno)
	{
		int k,l;
		struct node *prevnode = NULL;
		struct node *currentnode = head;
		struct node *nextnode = (head->previous);
		struct node *tempnode;
		struct node *temp1;
		for(k=0; k<taskno; k++)
		{
			prevnode = NULL;
			currentnode = head;
			nextnode = (head->previous);
			for(l=0; l<taskno-1; l++)
			{
				tempnode = nextnode->previous;
	
				if ((currentnode->deadline)>(nextnode->deadline))
				{
					if(prevnode == NULL)
						{head = nextnode;}
					temp1 = nextnode;
					nextnode = currentnode;
					currentnode = temp1;
					if(prevnode!=NULL)
					{prevnode->previous = currentnode;}
					currentnode->previous = nextnode;
					nextnode->previous = tempnode;
				}
				prevnode = currentnode;
				currentnode = nextnode;
				nextnode = tempnode;
			}
		}
	return head;
	}
//####################################################################################################################

//RM - SORTING ACCORTING TO PERIOD 
	struct node * RMsort (struct node *head, float taskno)
	{
		int k,l;
		struct node *prevnode = NULL;
		struct node *currentnode = head;
		struct node *nextnode = (head->previous);
		struct node *tempnode;
		struct node *temp1;
		for(k=0; k<taskno; k++)
		{
			prevnode = NULL;
			currentnode = head;
			nextnode = (head->previous);
			for(l=0; l<taskno-1; l++)
			{
				tempnode = nextnode->previous;
	
				if ((currentnode->period)>(nextnode->period))
				{
					if(prevnode == NULL)
						{head = nextnode;}
					temp1 = nextnode;
					nextnode = currentnode;
					currentnode = temp1;
					if(prevnode!=NULL)
					{prevnode->previous = currentnode;}
					currentnode->previous = nextnode;
					nextnode->previous = tempnode;
				}
				prevnode = currentnode;
				currentnode = nextnode;
				nextnode = tempnode;
			}
		}
	return head;
	}

//RM TEST SCHEDULE CASE
//head-> pointer, TotalComp ->sum of execution time, taskno -> total tasks in taskset
int RMRTest(struct node *head, float TotalComp, float taskno, float lowex, float lowdeadline)
{
	if(commentflag==1) {printf("<---------------RT TEST FUNCTION entered--------------->\n");}
	int SFlag=0;
	struct node *current = head;
	float Valone=0;
	float Valtwo=TotalComp;
	int i=1;
//	int temp;
	while(Valone!=Valtwo)	
	{
		current = head;
		Valone = Valtwo;
		Valtwo = 0;
		i=1;
		for(;i<taskno;i++)
		{
			Valtwo += ceil(Valone/(current->period))*(current->WCET);
			//if(commentflag==1){printf("%f %f %f %f\n", Valone, Valtwo, current->period, current->WCET);}
			current = current->previous;
		}
		Valtwo += lowex;
		/*if(commentflag==1)
		{
			printf("A0: %f A1: %f\n",Valone, Valtwo);
			//scanf("%d",&temp);
		}
		*/
	}
	if(commentflag==1)
	{
		printf("Time Demand: %f Total Deadline: %f\n",Valtwo, lowdeadline);
	}

	if(Valtwo<=lowdeadline)
	{
		if(commentflag==1){printf("Schedulable\n");} 
		SFlag=1;
	}
	else
	{
		if(commentflag==1){printf("Not Schedulable\n");} 
		SFlag=0;
	}
	if(commentflag==1){printf("--------------------------------------------------------\n");}
	return SFlag;
}



int RMSchedule(struct node *head, float UEDF, float taskno)
{
	if(commentflag==1){printf("#####################################################\n<-----------------------RM TEST--------------------->\n#####################################################\n");}
	int SFlag=0;
	int RT=1;
	float Uval = 0;
	// float prevmin=0;
	float currentmin=0;
	float upbound=0;
	float Wsum=0;
	float lowex=0;
	float lowdeadline=0;
	struct node *current = head;
	int c =0;   
	while(current->previous!=NULL)
	{
		c++;
		Wsum += current->WCET;
		lowex = current->WCET;
		lowdeadline = current->deadline;
		upbound = (c)*(pow(2,(1.0000000/c))-1);
		currentmin = (current->deadline>=current->period)?current->period:current->deadline;
		Uval += current->WCET/currentmin;
		if(commentflag==1){printf("Taskno: %f UEDF: %f Utilization: %f Upbound: %f\n",taskno, UEDF, Uval, upbound);}
		if(Uval > upbound)
		{
			if(commentflag==1){printf("Failure at Task %d --- RT Test required\n",c);}
			RT = RMRTest(head, Wsum, c, lowex, lowdeadline);
		}
		if(RT==0){SFlag=0; break;}
		SFlag=1;
		current = current->previous;
	}

return SFlag;
}

//###########################################################################################################################################################3

//DM - SORTING ACCORTING TO DEADLINE 
	struct node * DMsort (struct node *head, float taskno)
	{
		int k,l;
		struct node *prevnode = NULL;
		struct node *currentnode = head;
		struct node *nextnode = (head->previous);
		struct node *tempnode;
		struct node *temp1;
		for(k=0; k<taskno; k++)
		{
			prevnode = NULL;
			currentnode = head;
			nextnode = (head->previous);
			for(l=0; l<taskno-1; l++)
			{
				tempnode = nextnode->previous;
	
				if ((min(currentnode->period,currentnode->deadline))>(min(nextnode->period, nextnode->deadline)))
				{
					if(prevnode == NULL)
						{head = nextnode;}
					temp1 = nextnode;
					nextnode = currentnode;
					currentnode = temp1;
					if(prevnode!=NULL)
					{prevnode->previous = currentnode;}
					currentnode->previous = nextnode;
					nextnode->previous = tempnode;
				}
				prevnode = currentnode;
				currentnode = nextnode;
				nextnode = tempnode;
			}
		}
	return head;
	}

//DM TEST SCHEDULE CASE
int DMRTest(struct node *head, float TotalComp, float taskno, float maxex, float maxdeadline)
{
	int SFlag=0;
	float l0=0;
	float l1=TotalComp;
	struct node *current;
	int i=0;

	if(commentflag==1){printf("L0: %f L1: %f\n", l0,l1);}
	while(l0!=l1)
	{
		l0=l1;
		l1=maxex;
		current = head;
		for(i=0;i<taskno-1;i++)
		{
			l1 += ceil(l0/(current->period))*(current->WCET);
			current = current->previous;
		}
		//printf("L0: %f L1: %f\n", l0,l1);
	}
	if (l1<= maxdeadline)
		{
			if(commentflag==1){printf("Response Time: %f Deadline: %f\nSchedulable\n", l1, maxdeadline);} 
			SFlag=1;
		}
	else
		{
			if(commentflag==1){printf("Response Time: %f Deadline: %f\nNot Schedulable\n", l1, maxdeadline);}
			 SFlag=0;
		}

return SFlag;



}

//Taskes the Taskset LL and Total Utilization as inputs.
//dg Flag = 0 -- when normal 1 --- when Ordering needs to be checked.
int DMSchedule(struct node *head, float UEDF, float taskno)
{
	if(commentflag==1){printf("#####################################################\n<-----------------------DM TEST--------------------->\n#####################################################\n");}
	int SFlag=0;
	int RT=1;
	float Uval = 0;
	// float prevmin=0;
	float currentmin=0;
	float upbound=0;
	float Wsum=0;
	float lowex=0;
	float lowdeadline=0;
	struct node *current = head;
	int c =0;   
	while(current->previous!=NULL)
	{
		c++;
		Wsum += current->WCET;
		lowex = current->WCET;
		lowdeadline = current->deadline;
		upbound = (c)*(pow(2,(1.0000000/c))-1);
		currentmin = (current->deadline>=current->period)?current->period:current->deadline;
		Uval += current->WCET/currentmin;
		if(commentflag==1){printf("Taskno: %f UEDF: %f Utilization: %f Upbound: %f\n",taskno, UEDF, Uval, upbound);}
		if(Uval > upbound)
		{
			if(commentflag==1){printf("Failure at Task %d --- RT Test required\n",c);}
			RT = RMRTest(head, Wsum, c, lowex, lowdeadline);
		}
		if(RT==0){SFlag=0; break;}
		SFlag=1;
		current = current->previous;
	}
return SFlag;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//READING FROM THE FILE
////////////////////////////////////////////////////////////////////////////////////////////

void file_scheduling()
{
	char str[100];
	float no_of_tasksets = 0;
	float taskno = 0; 
	int i,j;
	float UEDF=0;

	//FILE OPEN
	FILE *in;
	in = fopen("Specification.txt","r+");
	if (in == NULL)
	{printf("FILE NOT FOUND");}

	if( fgets (str, 600, in)!=NULL );
	no_of_tasksets = atof(str);
	printf("TOTAL TASKSET COUNT: %f\n",no_of_tasksets);
	
	for(i=0; i<no_of_tasksets;i++)
	{
		printf("##########################################################\nTASKSET STARTED\n");
		UEDF=0;
		if( fgets (str, 600, in)!=NULL );
		taskno = atof(str);
		printf("%f\n",taskno);
		struct node *head = (struct node *)malloc(sizeof(struct node));
		struct node *current = (struct node *)malloc(sizeof(struct node));
		current->previous = NULL;
		for(j=0; j<taskno; j++)
		{
			if(fgets(str, 600, in)!=NULL);
			current = TaskFunc(str, current);

			// Utilization Calculation
			UEDF =UEDF+(current->WCET)/(current->period);
		}
		head = current;
		//printf("Total Utilization: %f\n", UEDF);


	//EEEEEDDDDDDFFFFF
		if(commentflag==0){printf("#####################################################\n<----------------------EDF TEST--------------------->\n#####################################################\n");}
		head = EDFsort(head, taskno);
		//printLL2(head);
		EDFSchedule(head, UEDF);
	

	//RRRRMMMMMM
		if(commentflag==0){printf("#####################################################\n<-----------------------RM TEST--------------------->\n#####################################################\n");}
		head = RMsort(head, taskno);
		RMSchedule(head, UEDF, taskno);

	//DDDDMMMMMM
		if(commentflag==0){printf("#####################################################\n<-----------------------DM TEST--------------------->\n#####################################################\n");}
		head = DMsort(head, taskno);
		DMSchedule(head, UEDF, taskno);

	}
	
}

/////////////////////////////////////////////////////////////////////////////////
//TASK SET GENERATOR
/////////////////////////////////////////////////////////////////////////////////

void UUnifast(float taskno, float Ud, struct node *start)
{
	int j=0;
	float Usum = Ud;
	float Usumnext;
	float Utask = 0;
	struct node *current = start;
	for (j=0; j<taskno-1; j++)
	{
		Usumnext=Usum*pow((rand()/(RAND_MAX*1.0)),(1.0/(taskno-j)));
		Utask = Usum - Usumnext;
		current->WCET = Utask * current->period;
		//printf("Uni: %f %f %f\n", Usum, Usumnext, Utask);
		current = current->previous;
		Usum = Usumnext;
	}
	Utask = Usum;
	current->WCET = Utask * current->period;
	//printf("Uni: %f %f %f\n", current->WCET, Utask, current->period);

}

void taskset_gen(int maxtask, int Dlocation)
{
	float UEDF=0;
	float Ucurrent=0.05;
	int tcount = 0;
	int EDFcount = 0;
	int RMcount = 0;
	int DMcount = 0;
	int i=0;
	float DLbound = 0;
	float DUbound = 0;
	
	struct node *head;
	struct node *current = NULL;


	//OUTPUT FILE CREATED
	char *Filename = malloc(sizeof(char)*maxmem);
	FILE *OFile;
	sprintf(Filename, "%s %d %d", "TASKSET ",maxtask, Dlocation);
	OFile = fopen(Filename, "w");
	if (OFile == NULL)
	{printf("FILE NOT CREATED\n");}


	
	printf("GENERATE TASK SETS: %d", maxtask);
	if(Dlocation==0)
	{
		printf("  DEADLINE RANGE: [C,T]\n");
	}
	else
	{
		printf("  DEADLINE RANGE: [C+(T-C)/2,T]\n");
	}
	for(;Ucurrent<1;Ucurrent=Ucurrent+0.1)
	{
		printf("Current U: %f %d\n", Ucurrent, maxtask);
		tcount=1;

		// TASKSET GENERATOR FOR CURRENT UTILIZATION STEP
		while(tcount<=Max_taskset)
		{
			//SET UP PERIODS OF TASKS INTO LINKEDLIST
			for(i=0; i<maxtask; i++)
			{
				struct node *new = (struct node *)malloc(sizeof(struct node));
				if (i<=(maxtask/3))
				{new->period = rand()/(RAND_MAX*1.0)*(100-10)+10;}
				else if (i>maxtask/3.0 && i<(2.0/3.0)*maxtask)
				{new->period = rand()/(RAND_MAX*1.0)*(1000-100)+100;}
				else
				{new->period = rand()/(RAND_MAX*1.0)*(10000-1000)+1000;}
				new->previous = current;
				current = new;
			}
			head = current;
			if(commentflag==1){printf("\n");}
			
			//USE UNIFAST TO SETUP UTILIZATION VALUES OF TASKS INTO LINKEDLIST
			
			UUnifast(maxtask, Ucurrent, head);
					

			//SETUP DEADLINE VALUES OF TASKS INTO LINKEDLIST
			current = head;
			for(i=0; i<maxtask; i++)
			{
				DUbound = current->period;
				DLbound = (current->WCET)+(Dlocation*(((current->period)-(current->WCET))/2));
				current->deadline = rand()/(RAND_MAX*1.0)*(DUbound-DLbound)+DLbound;
				current = current->previous;
			}

			//printf("Deadline Completed\n");			
			//printLL(head);			

			current = head;
			UEDF=0;
			while(current->previous!=NULL)
			{

				// Utilization Calculation
				UEDF =UEDF+(current->WCET)/(current->period);
				current = current->previous;

			}
//##########SYNTHETIC TASKSETS COMPLETED
			//DO ANALYSIS TRACKING ///////////////////////////////////////////////////
			
int tempe, tempr, tempd;
int input;
		//EEEEEDDDDDDFFFFF
		if(commentflag==1){printLL(head);}
		head = EDFsort(head, maxtask);
		tempe = EDFSchedule(head, UEDF);
		EDFcount += tempe;
		if (commentflag==1){printf("EDF Success count: %d %d\n-----------------------------------------------------\n", EDFcount, tcount);}
		//		if((commentflag==1)&&(tempe==0)){scanf("%d", &input);}

		//RRRRMMMMMM
		
		head = RMsort(head, maxtask);
		tempr = RMSchedule(head, UEDF, maxtask);
		RMcount += tempr;
		if (commentflag==1){printf("RM Success count: %d %d\n--------------------------------------------------------\n", RMcount, tcount);}
		if((tempr==1)&&(tempe==0)){scanf("%d", &input);}
		

		//DDDDMMMMMM
		
		head = DMsort(head, maxtask);
		tempd = DMSchedule(head, UEDF, maxtask);
		DMcount += tempd;
		if (commentflag==1){printf("DM Success count: %d %d\n--------------------------------------------------------\n", DMcount, tcount);}
//		if((commentflag==1)&&(tempd==0)){scanf("%d", &input);}

//########################################################################################################################################################
			tcount++;
		}
		//STORE THE %TASKS SUCCESS INTO FILE
		fprintf(OFile, "%f\t%f\t%f\t%f\n", Ucurrent, (1.0*(EDFcount)/Max_taskset),(1.0*(DMcount)/Max_taskset),(1.0*(RMcount)/Max_taskset));
		
		EDFcount=0;
		RMcount=0;
		DMcount=0;
	}
	//CLOSE FILE
	fclose(OFile);
}



int main()
{
	int choice=0;
	printf("Show Comments? (0/1): ");
	scanf("%d", &commentflag);

	while(choice==0)
	{
		printf("1. From File\n2. Synthetic Taskset\nEnter your Choice: ");
		scanf("%d",&choice);
		if(choice==0){printf("Invalid Choice\n");}
		else{break;}
	}
	switch(choice)
	{
		case 1:
			file_scheduling();
		break;
		case 2: 
			taskset_gen(10,0);
			taskset_gen(25,0);
			taskset_gen(10,1);
			taskset_gen(25,1);
		break;
	}
	return 0;
}


