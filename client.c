#include "unp.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
int aaa = 0;
#define R "\x1b[31m" // red
#define W "\x1b[37m" // while
#define RESET "\x1b[0m"
int trash = 0;
char whoWin[MAXLINE];
int myPlayerNum = 0;
void UsernamePart(int loginfd){
	time_t start_time,end_time;
    time(&start_time);
	char sendline[MAXLINE];
	while(1)
	{	
		int	maxfdp1;
		fd_set	rset;
		FD_ZERO(&rset);
		FD_SET(fileno(stdin), &rset);
		maxfdp1 = fileno(stdin);
		maxfdp1++;

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
		time(&end_time);
		if((end_time-start_time)==30)
			printf("Remaining 30 seconds! Please enter.\n");
		if((end_time-start_time)==50)
			printf("Remaining 10 seconds! Please enter.\n");
		if((end_time-start_time)>=60)
		{	
			printf("Timeout!\n");
			return; 
		}
		else if(c==1)
		{
			Fgets(sendline, MAXLINE, stdin);
			writen(loginfd, sendline, strlen(sendline));
			return;
		}
	}
}
int PasswordPart(int loginfd){
	time_t start_time,end_time;
    time(&start_time);
	char recvline[MAXLINE];
	char sendline[MAXLINE];
	while(1)//determine role
	{	
    int	maxfdp1;
    fd_set	rset;
    FD_ZERO(&rset);
    FD_SET(fileno(stdin), &rset);
    maxfdp1 = fileno(stdin);
    maxfdp1++;

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
    time(&end_time);
    if((end_time-start_time)==30)
        printf("Remaining 30 seconds! Please enter.\n");
    if((end_time-start_time)==50)
        printf("Remaining 10 seconds! Please enter.\n");
    if((end_time-start_time)>=60)
    {	
        printf("Timeout!\n");
        return 0; 
    }
    else if(c==1)
    {
        
        Fgets(sendline, MAXLINE, stdin);
        writen(loginfd, sendline, strlen(sendline));

        readline(loginfd, recvline, MAXLINE);
        printf("%s", recvline);
        if(!strcmp("User registered successfully.\n", recvline)) return 1;
        readline(loginfd, recvline, MAXLINE);
        printf("recv: %s", recvline);
        if(!strcmp("Login Successful\n", recvline)) return 1;
        else return 0;
    	}
	}
}
int MoneyPart(int loginfd){
	time_t start_time,end_time;
    time(&start_time);
	char recvline[MAXLINE];
	char sendline[MAXLINE];
	while(1)//determine role
	{	
    int	maxfdp1;
    fd_set	rset;
    FD_ZERO(&rset);
    FD_SET(fileno(stdin), &rset);
    maxfdp1 = fileno(stdin);
    maxfdp1++;

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
    time(&end_time);
    if((end_time-start_time)==30)
        printf("Remaining 30 seconds! Please enter.\n");
    if((end_time-start_time)==50)
        printf("Remaining 10 seconds! Please enter.\n");
    if((end_time-start_time)>=60)
    {	
        printf("Timeout!\n");
        return 0; 
    }
    else if(c==1)
    {
        
        Fgets(sendline, MAXLINE, stdin);
        writen(loginfd, sendline, strlen(sendline));

        readline(loginfd, recvline, MAXLINE);
        printf("%s", recvline);
        if(!strcmp("Modify finish, please relogin!\n", recvline)) return 1;
        else return 0;
    	}
	}
}

int modifyAccount(int loginfd){
	int cs;
	char recvline[MAXLINE];
	readline(loginfd, recvline, MAXLINE);
	for(int i = 0; i < strlen(recvline); i++){
			if(recvline[i] == '\n'){
				recvline[i] = '\0';
				break;
			}
		}
	printf("%s", recvline);
	fflush(stdout);
	UsernamePart(loginfd);

	readline(loginfd, recvline, MAXLINE);
	for(int i = 0; i < strlen(recvline); i++){
		if(recvline[i] == '\n'){
			recvline[i] = '\0';
			break;
		}
	}
	printf("%s", recvline);
	fflush(stdout);
	//其實是password!!!!!!!
	UsernamePart(loginfd);

	readline(loginfd, recvline, MAXLINE);
	for(int i = 0; i < strlen(recvline); i++){
		if(recvline[i] == '\n'){
			recvline[i] = '\0';
			break;
		}
	}
	printf("%s", recvline);
	fflush(stdout);
	//其實是money!!!!!!!
	cs = MoneyPart(loginfd);
	return cs;
}

int connecting(int loginfd){
	int cs;
	char recvline[MAXLINE];
	readline(loginfd, recvline, MAXLINE);
	for(int i = 0; i < strlen(recvline); i++){
			if(recvline[i] == '\n'){
				recvline[i] = '\0';
				break;
			}
		}
	printf("%s", recvline);
	fflush(stdout);
	UsernamePart(loginfd);
	readline(loginfd, recvline, MAXLINE);
	for(int i = 0; i < strlen(recvline); i++){
		if(recvline[i] == '\n'){
			recvline[i] = '\0';
			break;
		}
	}
	printf("%s", recvline);
	fflush(stdout);
	cs = PasswordPart(loginfd);
	return cs;
}
int recvNum = 0;
void waiting(char line[MAXLINE]){
    if(strcmp(line,"one\n")==0){
        printf("Please wait...\n");
        printf("■ - - - \n");
    }
    if(strcmp(line,"two\n")==0){
        printf("Please wait...\n");
        printf("■ ■ - - \n");
    }
    if(strcmp(line,"three\n")==0){
        printf("Please wait...\n");
        printf("■ ■ ■ - \n");
    }
    if(strcmp(line,"four\n")==0){
        printf("Please wait...\n");
        printf("■ ■ ■ ■ \n");
        usleep(1000);
    }
    if(strcmp(line,"error\n")==0){
        printf("Sysem ERROR!!!\n");
    }
}
char id[MAXLINE];

char a[53][10] = {
    "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??",
        "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??",
    "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", 
    "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??"
    };
char color[4][10] = {
    "?", "?", "?", "?"
};
void xchg_data(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    int	cards[13];
    int end=0;
    int role = 0;
    char *ack="ack\n";
    
    writen(sockfd, id, strlen(id));
    printf("sent: %s\n", id);//sent ID
    int cs;
    cs = connecting(sockfd);
    if(!cs) return;
	usleep(100);
    readline(sockfd, recvline, MAXLINE);
    printf("recv: %s", recvline);//determine role
    
    // set timeout
    time_t start_time,end_time;
    time(&start_time);
	
    while(1)//determine role
    {	
    	int	maxfdp1;
    	fd_set	rset;
    	FD_ZERO(&rset);
    	FD_SET(fileno(fp), &rset);
        maxfdp1 = fileno(fp);
        maxfdp1++;

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
		time(&end_time);
		if((end_time-start_time)==30)
			printf("Remaining 30 seconds! Please decide your role.\n");
		if((end_time-start_time)==50)
			printf("Remaining 10 seconds! Please decide your role.\n");
		if((end_time-start_time)>=60)
		{	
			printf("Timeout!\n");
			return; 
		}
		else if(c==1)
		{
			if(Fgets(sendline, MAXLINE, fp)==NULL)
				break;
			//printf("send:%s",sendline);
			if (strcmp(sendline,"1\n")==0) 
			{
				//clock_t end_time = clock();
				printf("I am a player!\n");
				role = 1;
				writen(sockfd, sendline, strlen(sendline));
				break;
			}
			else if (strcmp(sendline,"2\n")==0)
			{
				printf("I am an audience!\n");
				role = 2;
				writen(sockfd, sendline, strlen(sendline));
				break;
			}
			else if(strcmp(sendline, "3\n") == 0){
				printf("I am modifing!\n");
				role = 3;
				writen(sockfd, sendline, strlen(sendline));
				break;
			}
			else
				printf("Please enter 1 or 2 or 3!\n");
		}
	}
    
    if(role == 1) {
		char keepline[MAXLINE];
		char keepline1[MAXLINE];
		int b=0;
		while(1){
			if(b==1)
				break;
		    readline(sockfd, recvline, MAXLINE);
		    if(strcmp(recvline,"keepalive\n")==0)
		    {
		    	writen(sockfd,ack,strlen(ack));
		    }
			else if(strcmp(recvline,"player message\n")==0){
				
				readline(sockfd, keepline, MAXLINE);
				printf("recv: %s", keepline);//# player
				sscanf(keepline, "You are the #%d player. Please wait other players.\n", &myPlayerNum);
				myPlayerNum %= 4;
				myPlayerNum = myPlayerNum+1;
				snprintf(keepline1, sizeof(keepline1), "You are the #%d player. Please wait other players.\n", myPlayerNum);
			}
		    else{
			    aaa = system("clear");
			    printf("recv: %s", keepline1);
			    if(strcmp(recvline, "four\n") == 0 || strcmp(recvline, "error\n") == 0) {
					waiting(recvline);
					b=1;
			    }
			    else 
					waiting(recvline);
		    }
		}
		usleep(10000);
		// num of player
		//readline(sockfd, recvline, MAXLINE);
		readline(sockfd, recvline, MAXLINE);
		aaa = system("clear"); 
		//printf("recv:%s", recvline);//ready to start
		
		//keepalive1
		readline(sockfd, recvline, MAXLINE);
		//printf("%s\n",recvline);
		if(strcmp(recvline,"Game over!\n")==0)
		{
			printf("Someone left! Game over!\n");
		    	end=1;
		    	return;
		}
		writen(sockfd,ack,strlen(ack));
		readline(sockfd, recvline, MAXLINE);
		//printf("%s\n",recvline);
		if(strcmp(recvline,"Game over!\n")==0)
		{
			printf("Someone left! Game over!\n");
		    	end=1;
		    	return;
		}
		
		printf("\r  ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? \n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		int top[4] = {52, 52, 52, 52};
		printf("??");
		printf("          ");
		for(int it = 0; it < 4; it++){
		    if(top[it]<13 || top[it]>38){
		        printf(W);
		        printf("%s ", a[top[it]]);
		        printf(RESET);
		    }
		    else{
		        printf(R);
		        printf("%s ", a[top[it]]);
		        printf(RESET);
		    }
		}
		printf("         ");
		printf("??\n");

		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("??                           ??\n");
		printf("  ");
		for(int i=0;i<13;i++)//recv cards
		{			
		    readline(sockfd, recvline, MAXLINE);	
		    sscanf(recvline, "%d\n", &cards[i]);
		}
		for(int j=0;j<13;j++)
		{
		    if(cards[j]<13 || cards[j]>38)
		        printf(W "%s ", a[cards[j]]);
		    else
		        printf(R "%s ", a[cards[j]]);
		    printf(RESET);
		}
		printf("\n");
		printf(RESET);
		printf("\n");
		readline(sockfd, recvline, MAXLINE); 
		printf("recv:%s", recvline);//get all cards
		
		//keepalive2
		readline(sockfd, recvline, MAXLINE);
		//printf("%s\n",recvline);
		if(strcmp(recvline,"Game over!\n")==0)
		{
			printf("Someone left! Game over!\n");
		    	end=1;
		    	return;
		}
		writen(sockfd,ack,strlen(ack));
		readline(sockfd, recvline, MAXLINE);
		if(strcmp(recvline,"Game over!\n")==0)
		{
			printf("Someone left! Game over!\n");
		    	end=1;
		    	return;
		}
		
		while(1)//determine king
		{
		    readline(sockfd, recvline, MAXLINE);//keepalive
		    if(strcmp(recvline,"Game over!\n")==0)
		    {
			//printf("Someone left! Game over!\n");
			readline(sockfd, recvline, MAXLINE);
			printf("%s", recvline);
		    	end=1;
		    	return;
		    }
		    else if(strcmp(recvline,"keepalive\n")==0)
		    {
			    writen(sockfd,ack,strlen(ack));
			    //printf("ack\n");
			    readline(sockfd, recvline, MAXLINE);
			    //printf("%s\n", recvline);
			    if(strcmp(recvline,"Game over!\n")==0)
			    {
					//printf("Someone left! Game over!\n");
					readline(sockfd, recvline, MAXLINE);
					printf("%s", recvline);
				    	end=1;
				    	return;
			    }
			    
		    }
		    else if(strcmp(recvline,"King decided.\n")==0)
		    {
		    	printf("%s", recvline);
		        readline(sockfd, recvline, MAXLINE);
		        printf("%s", recvline);
		        break;
		    }
		    else if(strcmp(recvline,"It is your turn! Call king or pass(0).\n")==0)
		    {
		    	printf("%s", recvline);
		    	int current_king=0;
			int current_target=0;
		    	readline(sockfd, recvline, MAXLINE);
		    	sscanf(recvline, "%d %d\n", &current_king, &current_target);
		    	time(&start_time);
		    	while(1)
		    	{
		    		int	maxfdp1;
				fd_set	rset;
			    	FD_ZERO(&rset);
			    	FD_SET(fileno(fp), &rset);
				maxfdp1 = fileno(fp);
				maxfdp1++;

				struct timeval timeout;
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;
				int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
				time(&end_time);
				if((end_time-start_time)==30)
					printf("Remaining 30 seconds! Please decide king.\n");
				if((end_time-start_time)==50)
					printf("Remaining 10 seconds! Please decide king.\n");
				if((end_time-start_time)>=60)
				{	
					printf("Timeout!\n");
					return; 
				}
				else if(c==1)
				{
				    	if(Fgets(sendline, MAXLINE, fp)==NULL)
				    		return;
					char call_king[10];
					int call_k=0;
					int call_t=0;
					if(current_king==0 && strcmp(sendline,"0\n")==0)//First one
					{
						printf("Not allow call! Please try again.\n");
					}
					else if(current_king!=0 && strcmp(sendline,"0\n")==0)//not firstone
					{
						writen(sockfd, sendline, strlen(sendline));
						break;
					}
					else
					{	
						sscanf(sendline, "%s %d\n", call_king, &call_t);
						if(strcmp(call_king,"spades")==0)
							call_k=4;
						else if(strcmp(call_king,"hearts")==0)
							call_k=3;
						else if(strcmp(call_king,"diamonds")==0)
							call_k=2;
						else if(strcmp(call_king,"clubs")==0)
							call_k=1;
						else
							call_k=0;
						//printf("%d\n",call_t);
						if(call_t>current_target && call_k!=0)
						{
							writen(sockfd, sendline, strlen(sendline));
							break;
						}
						else if(call_t==current_target && call_k>current_king &&call_t!=0)
						{
							writen(sockfd, sendline, strlen(sendline));
							break;
						}
						else
						{
							printf("Not allow call! Please try again.\n");
						}
					}
				}
		        }
		    }
		    else
		    	printf("recv:%s", recvline);

		}
		if(end==1)
			return;
			
		
		readline(sockfd, recvline, MAXLINE);//get winning target
		printf("%s", recvline);
		sleep(3);
		for(int i = 3; i > 0; i--){
			aaa = system("clear");
			printf("the game will be start in %d sec\n", i);
			sleep(1);
		}
		//counting!!!!
		//char useless[MAXLINE];
		//printf("input anything to start!\n");
		//scanf("%s", useless);
		
		for(int i=0;i<13;i++)
		{
			//keepalive
			readline(sockfd, recvline, MAXLINE);
			//printf("%s\n",recvline);
			if(strcmp(recvline,"Game over!\n")==0)
			{
				printf("Someone left! Game over!\n");
			    	end=1;
			    	return;
			}
			writen(sockfd,ack,strlen(ack));
			readline(sockfd, recvline, MAXLINE);
			//printf("%s\n",recvline);
			if(strcmp(recvline,"Game over!\n")==0)
			{
				printf("Someone left! Game over!\n");
			    	end=1;
			    	return;
			}
		   
		    readline(sockfd, recvline, MAXLINE);//round
		    printf(R "\n%s", recvline);
		    printf(RESET);
		    if(strcmp(recvline,"Game over!\n")==0)
		    {
		        readline(sockfd, recvline, MAXLINE);//team 1's points
		        printf(R "\n%s", recvline);
		        printf(RESET);
		        readline(sockfd, recvline, MAXLINE);//team 2's points
		        printf(R "\n%s", recvline);
		        printf(RESET);
		        readline(sockfd, recvline, MAXLINE);//winner
		        printf(R "\n%s", recvline);
		        printf(RESET);
		        break;

		    }
		    else
		    {
		        for(int i=0;i<13;i++)
		        {
		            char round_message[50];
		            snprintf(round_message, sizeof(round_message), "Now is round%d.\n", i+1);
		            if(strcmp(recvline, round_message)==0)
		            {
		                aaa = system("clear");
						printf("%s\n", round_message);
						if(i+1 != 1) printf("%s\n", whoWin);
		                printf("\r  ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? \n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                int top[4] = {52, 52, 52, 52};
		                printf("??");
		                printf("          ");
		                for(int it = 0; it < 4; it++){
		                    if(top[it]<13 || top[it]>38){
		                        printf(W);
		                        printf("%s ", a[top[it]]);
		                        printf(RESET);
		                    }
		                    else{
		                        printf(R);
		                        printf("%s ", a[top[it]]);
		                        printf(RESET);
		                    }
		                }
		                printf("         ");
		                printf("??\n");


		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("??                           ??\n");
		                printf("  ");
		                for(int j=0;j<13-i;j++)
		                {
		                    if(cards[j]<13 || cards[j]>38)
		                        printf(W "%s ", a[cards[j]]);
		                    else
		                        printf(R "%s ", a[cards[j]]);
		                    printf(RESET);
		                }
		                printf("\n");
		            }
		        }
		        //int count=0
		        //int current=-1
				int cardState[4] = {52, 52, 52, 52};
				int myCard = 0;
		        while(1)//recv and send card
		        {

		            readline(sockfd, recvline, MAXLINE);
		            //printf("%s", recvline);
		            if(strcmp(recvline,"keepalive\n")==0)
		    	    {
				    writen(sockfd,ack,strlen(ack));
				    //printf("ack\n");
				    readline(sockfd, recvline, MAXLINE);
				    //printf("%s\n", recvline);
				    if(strcmp(recvline,"Game over!\n")==0)
				    {
						//printf("Someone left! Game over!\n");
						readline(sockfd, recvline, MAXLINE);
						printf("%s", recvline);
					    	end=1;
					    	return;
				    }
			    
		    	    }
		            else if(strcmp(recvline,"Game over!\n")==0){
		            		readline(sockfd, recvline, MAXLINE);
					printf("%s", recvline);
					end=1;
			    		return;
			    }
		            else if(strcmp(recvline,"This round ended.\n")==0)
		            {
		            	printf("%s", recvline);
		                readline(sockfd, whoWin, MAXLINE);//who win
		                printf("%s", recvline);
		                break;
		            }
		            else if(strcmp(recvline,"It is your turn! Please send a card\n")==0)
		            {//send card
		            	printf(W "%s", recvline);
		            	int current;
		            	int limit=0;
		            	readline(sockfd, recvline, MAXLINE);
		            	sscanf(recvline, "%d\n", &current);
		            	//printf("current:%d\n",current);
		            	
		            	if(current==-1)
		            		limit=0;
		            	else
		            	{
		            		for(int l=0;l<(13-i);l++)
		            		{
		            			if((cards[l]/13)==current)
		            				limit=1;
		            		}
		            	}
		            	
		            			time(&start_time);
						while(1)//send card
						{
							int	maxfdp1;
							fd_set	rset;
								FD_ZERO(&rset);
								FD_SET(fileno(fp), &rset);
							maxfdp1 = fileno(fp);
							maxfdp1++;

							struct timeval timeout;
							timeout.tv_sec = 1;
							timeout.tv_usec = 0;
							int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
							time(&end_time);
							if((end_time-start_time)==30)
								printf("Remaining 30 seconds! Please decide card.\n");
							if((end_time-start_time)==50)
								printf("Remaining 10 seconds! Please decide card.\n");
							if((end_time-start_time)>=60)
							{	
								printf("Timeout!\n");
								return; 
							}
							else if(c==1)
							{
								if(Fgets(sendline, MAXLINE, fp)==NULL)
									return;
								int	tmp;
								sscanf(sendline, "%d\n", &tmp);
								myCard = cards[tmp-1];
								if(0<tmp && tmp<=(13-i)) {
									if(limit==0) {
										for(int	j=tmp-1;j<12;j++){
											cards[j]=cards[j+1];
										}	
										printf("\n");
										writen(sockfd, sendline, strlen(sendline));
										break;	
									}
									else if((cards[tmp-1]/13)==current) {	
										for(int	j=tmp-1;j<12;j++){
											cards[j]=cards[j+1];
										}
										printf("\n");
										writen(sockfd, sendline, strlen(sendline));
										break;
									}
									else
										printf("Not allow input! Please follow first player.\n");
								}
								else {
									printf("Not allow input! Please try again.\n");
								}
							}
						}
						
						cardState[myPlayerNum-1] = myCard;
						aaa = system("clear");
						printf(RESET);
						//printf("myPlayerNum: %d\n", myPlayerNum);
						//printf("myCard: %s\n", a[myCard]);
						printf("\r  ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? \n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??");
						printf("          ");
						for(int it = 0; it < 4; it++){
							if(cardState[it]<13 || cardState[it]>38){
								printf(W "%s ", a[cardState[it]]);
								printf(RESET);
							}
							else{
								printf(R "%s ", a[cardState[it]]);
								printf(RESET);
							}
						}
						printf("         ");
						printf("??\n");


						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("  ");
						for(int j=0;j<13-i-1;j++)
						{
							if(cards[j]<13 || cards[j]>38)
								printf(W "%s ", a[cards[j]]);
							else
								printf(R "%s ", a[cards[j]]);
							printf(RESET);
						}
						printf("\n");
					}
					else {//recv card
					
						//printf("%s", recvline);
						int p,c;
						sscanf(recvline, "Player #%d post %d\n", &p, &c);
						cardState[p-1] = c;
						char recv_player[50];
						snprintf(recv_player, sizeof(recv_player), "Player #%d post ", p);
						aaa = system("clear");
						printf(RESET);
						printf("\r  ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? \n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??");
						printf("          ");
						for(int it = 0; it < 4; it++){
							if(cardState[it] < 13 || cardState[it] > 38){
								printf(W "%s ", a[cardState[it]]);
								printf(RESET);
							}
							else{
								printf(R "%s ", a[cardState[it]]);
								printf(RESET);
							}
						}
						printf("         ");
						printf("??\n");


						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("??                           ??\n");
						printf("  ");
						for(int j = 0; j < 13-i; j++) {
							if(cards[j]<13 || cards[j]>38)
								printf(W "%s ", a[cards[j]]);
							else
								printf(R "%s ", a[cards[j]]);
							printf(RESET);
						}
						printf("\n");
						printf(W "%s", recv_player);
						if(c < 13 || c > 38)
							printf(W "%s \n", a[c]);
						else
							printf(R "%s \n", a[c]);
						printf(RESET);
					}
		        }
		    }
		}
	}
	else if(role == 2){
		printf("You could enter how much you want to anted up at any time to any game!\n");
		printf("ex: game1 team1 100\n");
		fd_set readfds;
		int maxfd = sockfd;

		while(1){
			FD_ZERO(&readfds);
			FD_SET(sockfd, &readfds);
			FD_SET(STDIN_FILENO, &readfds);

			int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);

			if(ready == -1){
				perror("select error");
				exit(EXIT_FAILURE);
			}

			if(FD_ISSET(sockfd, &readfds)){
				char recvline[MAXLINE];
				readline(sockfd, recvline, MAXLINE);
				printf("%s", recvline);
			}

			if(FD_ISSET(STDIN_FILENO, &readfds)){
				char input[MAXLINE];
				int game = 0;
				int team = 0;
				float bet = 0.0;
				if(fgets(input, MAXLINE, stdin) != NULL){
					if(sscanf(input, "game%d team%d %f\n", &game, &team, &bet)){
						printf("You entered: %s", input);
						if(!game || !team || !bet){
							printf("Please input legal words!\n");
							continue;
						}
						writen(sockfd, input, strlen(input));
					}
					else {
						printf("Please input how much you want to anted up! \n");
					}
				}
			}
		}
	}
	else if(role == 3){
		int good;
		good = modifyAccount(sockfd);
		if(good) printf("press ctrl+c if not drop\n");
		else printf("modify failed :(( \n");
	}
};

int main(int argc, char **argv)
{
    int					sockfd;
    struct sockaddr_in	servaddr;

    if (argc != 3)
        err_quit("usage: tcpcli <IPaddress> <ID>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    strcpy(id, argv[2]);
	
    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    xchg_data(stdin, sockfd);		

    exit(0);
}

