#include 	"unp.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<stdbool.h>
#include 	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<netinet/tcp.h>
#include 	<time.h>
#include 	<unistd.h>
#include 	<sys/ipc.h>
#include 	<sys/shm.h>
#include 	<sys/socket.h>
#include	<sys/msg.h>
#define RED "\x1b[91m" // 紅色
#define BLACK "\x1b[30m" // 黑色
#define RESET "\x1b[0m"


char a[52][10] = {
    "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??",
        "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??",
    "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", 
    "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??", "??",
    };

// void setTimeout(int socketFD, int seconds) {
//     struct timeval timeout;
//     timeout.tv_sec = seconds;
//     timeout.tv_usec = 0;

//     if (setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
//         perror("Error setting receive timeout");
//     }
//     if (setsockopt(socketFD, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
//         perror("Error setting send timeout");
//     }
//}
int alive = 1;
void modifyAccount(char fname[50], int loginFD){
    char username[MAXLINE], password[MAXLINE];
    char fileUserName[MAXLINE], filePassword[MAXLINE];
    float balance;
    //setTimeout(loginFD, 70);
    FILE *logFile;
    //printf("fname: %s\n",fname);
    char msg1[MAXLINE], msg2[MAXLINE], msg3[MAXLINE];
    snprintf(msg1, sizeof(msg1), "Enter the verify name you want: \n");
    snprintf(msg2, sizeof(msg2), "Enter the password you want: \n");
    snprintf(msg3, sizeof(msg3), "Enter the money you want to top up: \n");

    writen(loginFD, msg1, strlen(msg1));
    alive = readline(loginFD, username, MAXLINE);
	if(!alive){
		printf("Account Input error\n");
		return;
	}
	alive = 1;
    for(int i = 0; i < strlen(username); i++){
    	if(username[i] == '\n'){
    		username[i] = '\0';
    		break;
    	}
    }
    printf("username: %s\n", username);
    
    writen(loginFD, msg2, strlen(msg2));
    alive = readline(loginFD, password, MAXLINE); 
	if(!alive){
		printf("Password Input error\n");
		return;
	}
	alive = 1;
    for(int i = 0; i < strlen(password); i++){
    	if(password[i] == '\n'){
    		password[i] = '\0';
    		break;
    	}
    }
    printf("password: %s\n", password);
    
    logFile = fopen(fname, "r");
    if(logFile == NULL){
        perror("Error opening log file");
        return;
    }
    fscanf(logFile, "%s %s %f", fileUserName, filePassword, &balance);
    writen(loginFD, msg3, strlen(msg3));
    printf("fileUserName: %s\nfilePassword: %s\nbalance: %f\n", fileUserName, filePassword, balance);
    char getMoney[MAXLINE];
    float topUp = 0.0;
    int checkSuccess;
    readline(loginFD, getMoney, MAXLINE);
    printf("getMoney: %s\n", getMoney);
    checkSuccess = sscanf(getMoney, "%f\n", &topUp);
    if(checkSuccess){ 
        balance += topUp;
        printf("current balance: %f\n", balance);
    }
    else {
        writen(loginFD, "modify failed :(\n", strlen("modify failed :(\n"));
        fclose(logFile);
        return;
    }
    fclose(logFile);

    logFile = fopen(fname, "w");
    fprintf(logFile, "%s %s %.2f", username, password, balance);
    fclose(logFile);
    writen(loginFD, "Modify finish, please relogin!\n", strlen("Modify finish, please relogin!\n"));
    //writen(loginFD, "User modified successfully.\n", strlen("User modified successfully.\n"));
    printf("modify finish\n");
	return;
}

int registerUser(char fname[50], int loginFD) {
    char username[MAXLINE], password[MAXLINE];
    //setTimeout(loginFD, 70);
    FILE *logFile;
    printf("Register new user\n");
    char msg1[MAXLINE], msg2[MAXLINE];
    snprintf(msg1, sizeof(msg1), "Enter verify name: \n");
    snprintf(msg2, sizeof(msg2), "Enter password: \n");
    
    writen(loginFD, msg1, strlen(msg1));
    alive = readline(loginFD, username, MAXLINE);
	if(!alive){
		printf("Account Input error\n");
		return -3;
	}
	alive = 1;
    for(int i = 0; i < strlen(username); i++){
    	if(username[i] == '\n'){
    		username[i] = '\0';
    		break;
    	}
    }
    printf("username: %s\n", username);
    writen(loginFD, msg2, strlen(msg2));
	
    alive = readline(loginFD, password, MAXLINE); 
	if(!alive){
		printf("Password Input error\n");
		return -3;
	}
	alive = 1;
    for(int i = 0; i < strlen(password); i++){
    	if(password[i] == '\n'){
    		password[i] = '\0';
    		break;
    	}
    }
    printf("password: %s\n", password);
    char openFile[MAXLINE];
    snprintf(openFile, MAXLINE , "%sAccount.log", fname);
    logFile = fopen(openFile, "w");
    if (logFile == NULL) {
        perror("Error opening log file");
        return 0;
    }
    fprintf(logFile, "%s %s %.2f", username, password, 100.0);
    fclose(logFile);
    writen(loginFD, "User registered successfully.\n", strlen("User registered successfully.\n"));
	return 1;
}

int loginUser(char fname[50], int loginFD) {
    char username[50], password[50], fileUsername[50], filePassword[50];
    //setTimeout(loginFD, 70);
    char openFile[MAXLINE];
    float balance;
    FILE *logFile;

    printf("Login\n");
    char msg1[MAXLINE], msg2[MAXLINE];
    snprintf(msg1, sizeof(msg1), "Enter verify name: \n");
    snprintf(msg2, sizeof(msg2), "Enter password: \n");
    
    writen(loginFD, msg1, strlen(msg1));
    alive = readline(loginFD, username, MAXLINE);
	if(!alive){
		printf("Account Input error\n");
		return -3;
	}
    for(int i = 0; i < strlen(username); i++){
    	if(username[i] == '\n'){
    		username[i] = '\0';
    		break;
    	}
    }
    //printf("%s\n", username);
    writen(loginFD, msg2, strlen(msg2));
    alive = readline(loginFD, password, MAXLINE); 
	if(!alive){
		printf("Password Input error\n");
		return -3;
	}
    for(int i = 0; i < strlen(password); i++){
    	if(password[i] == '\n'){
    		password[i] = '\0';
    		break;
    	}
    }
	//printf("%s\n", password);
    snprintf(openFile, sizeof(openFile) , "%sAccount.log", fname);
    //printf("%s", openFile);
    logFile = fopen(openFile, "r");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        return 0;
    }
    char sendline[MAXLINE];
    //int value;
    //value = fscanf(logFile, "%s %s %f", fileUsername, filePassword, &balance);
    //printf("%s %s %f", fileUsername, filePassword, balance);
    //printf("value: %d\n", value);
    if (fscanf(logFile, "%s %s %f", fileUsername, filePassword, &balance)) {

        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            
            snprintf(sendline, sizeof(sendline),"Account balance for %s: %.2f\n", username, balance);
            writen(loginFD, sendline, strlen(sendline));
            writen(loginFD, "Login Successful\n", strlen("Login Successful\n"));
            fclose(logFile);
            return 1;
        }
        else{
			writen(loginFD, "error, please press ctrl+c to relogin!\n", strlen("error, please press ctrl+c to relogin!\n"));
			return 0;
		}
    }
    //printf("Invalid username or password.\n");
    fclose(logFile);
	printf("somethings wrong...\n");
	return 0;
}

void peopleNum(int playerFd[4], int num){
    char line[MAXLINE];
    int pNum = 0;
    for(int j=0;j<4;j++)
	{
		if(playerFd[j]!=-1)
		{	
			pNum++;
		}
	}
    if(pNum == 1) strcpy(line, "one\n");
    else if(pNum == 2) strcpy(line, "two\n");
    else if(pNum == 3) strcpy(line, "three\n");
    else if(pNum == 4) strcpy(line, "four\n");
    //else strcpy(line, "error\n");
    for(int i = 0; i < 4; i++){
    	if(playerFd[i]!=-1)
            writen(playerFd[i], line, strlen(line));
    }
}
void
sig_chld(int signo)
{
        pid_t   pid;
        int             stat;

        while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
                ;
        return;
}
void signalHandler(int signal) {
    printf("new round!");
}
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void play(int sockfd1, int sockfd2, int sockfd3, int sockfd4, char*FIRST_ID, char*SECOND_ID, char*THIRD_ID, char*FOURTH_ID, int Msgid,int Msgid2,int Msgid3,int g)
{
    //char send[MAXLINE], recv[MAXLINE];
    //int*data = (int*)shmat(shmid, (void*)0, 0);
    int socketfd[4]={0};
    socketfd[0]=sockfd1;
    socketfd[1]=sockfd2;
    socketfd[2]=sockfd3;
    socketfd[3]=sockfd4;
    fd_set	rset;
    int maxfdp1;
    char*game_over="Game over!\n";
    int end=0;
    int card[52]={0};
    int i=0;
    char*keepalive="keepalive\n";
    int withdraw=-1;
    //keepalive1
    for(int j=0;j<4;j++)
    {
    	char ack[MAXLINE];
	writen(socketfd[j],keepalive,strlen(keepalive));
	FD_ZERO(&rset);
        FD_SET(socketfd[j], &rset);
        maxfdp1 = socketfd[j];
        maxfdp1++;

        struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
	if(c==0)
	{
		end=1;
		break;
	}
	else
			readline(socketfd[j],ack,MAXLINE);
    }
    if(end==1) {
    	for(int j=0;j<4;j++) {
			writen(socketfd[j],game_over,strlen(game_over));
		}
		return;
    }
    else {
    	for(int j=0;j<4;j++){
			writen(socketfd[j],keepalive,strlen(keepalive));
		}
    }
    while(i<52){
        int x = rand()%52;
        int repeat=0;
        for(int j=0;j<i;j++){
            if(card[j]==x)
                repeat=1;
        }
        if(!repeat){
            card[i]=x;
            i++;
        }	
    }

    for(i=0;i<52;i++){
        printf("%d ",card[i]);
    }
    printf("\nplay!\n");
    int	player_card[4][13];

    for(i=0;i<4;i++){
        for(int j=0;j<13;j++){
            player_card[i][j]=card[i*13+j];
        }
        insertionSort(player_card[i], 13);
    }

    for(i=0;i<52;i++){
        char send[MAXLINE];
        snprintf(send, sizeof(send), "%d\n", player_card[i/13][i%13]);
        if(i<13)
            writen(sockfd1,send,strlen(send));
        else if(i<26)
            writen(sockfd2,send,strlen(send));
        else if(i<39)
            writen(sockfd3,send,strlen(send));
        else if(i<52)
            writen(sockfd4,send,strlen(send));
        }
        char*game_message1="You get all the cards! Please ready to call the king.\n";
        for(int j=0;j<4;j++)
        {
            writen(socketfd[j], game_message1, strlen(game_message1));
            printf("sent:%s",game_message1);
        }
	
	//keepalive2
	for(int j=0;j<4;j++)
	{
	    	char ack[MAXLINE];
		writen(socketfd[j],keepalive,strlen(keepalive));
		FD_ZERO(&rset);
		FD_SET(socketfd[j], &rset);
		maxfdp1 = socketfd[j];
		maxfdp1++;

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
		if(c==0)
		{
			end=1;
			break;
		}
		else
			readline(socketfd[j],ack,MAXLINE);
	}
	if(end==1)
	{
	    	for(int j=0;j<4;j++)
		{
			writen(socketfd[j],game_over,strlen(game_over));
		}
		return;
	}
	else
	{
	    	for(int j=0;j<4;j++)
		{
			writen(socketfd[j],keepalive,strlen(keepalive));
		}
	}
        int pass=0;
        char*game_message2="It is your turn! Call king or pass(0).\n";
        int turn=-1;
        char king[10];
        int k=-1;
        int win_standard=0;
        int final_call=-1; 
	int current_king=0;
        while(1)//determine king  
        {
        	
        	    int n=0;	
		    char recv[MAXLINE];
		    if(pass==3)
		    	break;
		    
		    turn++;
		    turn=turn%4;

		    printf("It is #%d player's turn!\n", turn+1);
		    writen(socketfd[turn],game_message2,strlen(game_message2));
		    
		    char current_message[50];
		    snprintf(current_message, sizeof(current_message), "%d %d\n", current_king, win_standard);
		    writen(socketfd[turn],current_message,strlen(current_message));
		    FD_ZERO(&rset);
            	    FD_SET(socketfd[turn], &rset);
                    maxfdp1 = socketfd[turn];
                    maxfdp1++;

           	struct timeval timeout;
		    timeout.tv_sec = 60;
		    timeout.tv_usec = 0;
		    int c = select(maxfdp1, &rset, NULL, NULL, &timeout);
		    //printf("c:%d\n",c);
		    if(c==1)
		    	n=readline(socketfd[turn], recv, MAXLINE);
		    else{
		    	for(int j=0;j<4;j++) {
					if(j!=turn)
					 {	writen(socketfd[j],game_over,strlen(game_over));//timeout gameover
					 	printf("Player%d left the game!\n",j+1);
						withdraw=turn;
					 	
					 	char endmessage[50];
						snprintf(endmessage, sizeof(endmessage), "Player%d left the game!\n", withdraw+1);
		    				writen(socketfd[j],endmessage,strlen(endmessage));
					 }
			 	}
				end=1;
				break;
		    }
		    
		    if(n==0) {
		    	for(int j=0;j<4;j++) {
			 	if(j!=turn)
				{   	writen(socketfd[j],game_over,strlen(game_over));//normal gameover
					printf("Player%d left the game!\n",j+1);
					withdraw=turn;
				    	char endmessage[50];
					snprintf(endmessage, sizeof(endmessage), "Player%d left the game!\n", withdraw+1);
		    			writen(socketfd[j],endmessage,strlen(endmessage));
		    		}
			 	}
				end=1;
				break;
		    }	   
		    if(strcmp(recv,"0\n")==0)//pass
		    {	    //keepalive
			for(int j=0;j<4;j++)
			{
			    	char ack[MAXLINE];
			    	writen(socketfd[j],keepalive,strlen(keepalive));
			    	//printf("w=%d",w);
				FD_ZERO(&rset);
				FD_SET(socketfd[j], &rset);
				maxfdp1 = socketfd[j];
				maxfdp1++;

				struct timeval timeout;
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;
				int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
				printf("c=%d",c);
				if(c==0)
				{
					printf("Player%d left the game!\n",j+1);
					withdraw=j;
					end=1;
					break;
				}
				else
				{	int r=readline(socketfd[j],ack,MAXLINE);
					printf("r=%d\n",r);
					printf("recv:%s\n",ack);
					if(r==0)
					{
						printf("Player%d left the game!\n",j+1);
						withdraw=j;
						end=1;
						break;
					}
				}
			}
			if(end==1)
			{
				for(int k=0;k<4;k++)
				{
					if(k!=withdraw)
					{	writen(socketfd[k],game_over,strlen(game_over));
						char endmessage[50];
						snprintf(endmessage, sizeof(endmessage), "Player%d left the game!\n", withdraw+1);
		    				writen(socketfd[k],endmessage,strlen(endmessage));
						
					}
				}
				break;
			}
			else
			{
				for(int j=0;j<4;j++)
				{
					writen(socketfd[j],keepalive,strlen(keepalive));
				}
			}
		    	    
			    char pass_message[50];
			    snprintf(pass_message, sizeof(pass_message), "#%d player pass.\n", turn+1);
			    for(int j=0;j<4;j++)
			    {
				    if(j!=turn)
				    	writen(socketfd[j],pass_message,strlen(pass_message));
			    }
			    pass++;
		    }
		    else
		    {	    
		    	    sscanf(recv, "%s %d\n", king, &win_standard);
		    	    
		    	    if(strcmp(king,"spades")==0)
				current_king=4;
			    if(strcmp(king,"hearts")==0)
				current_king=3;
			    if(strcmp(king,"diamonds")==0)
				current_king=2;
			    if(strcmp(king,"clubs")==0)
				current_king=1;
			//keepalive
			for(int j=0;j<4;j++)
			{
			    	char ack[MAXLINE];
			    	writen(socketfd[j],keepalive,strlen(keepalive));
			    	//printf("w=%d",w);
				FD_ZERO(&rset);
				FD_SET(socketfd[j], &rset);
				maxfdp1 = socketfd[j];
				maxfdp1++;

				struct timeval timeout;
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;
				int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
				printf("c=%d",c);
				if(c==0)
				{
					printf("Player%d left the game!\n",j+1);
					withdraw=j;
					end=1;
					break;
				}
				else
				{	int r=readline(socketfd[j],ack,MAXLINE);
					printf("r=%d\n",r);
					printf("recv:%s\n",ack);
					if(r==0)
					{
						printf("Player%d left the game!\n",j+1);
						withdraw=j;
						end=1;
						break;
					}
				}
			}
			if(end==1)
			{
				for(int k=0;k<4;k++)
				{
					if(k!=withdraw)
					{	writen(socketfd[k],game_over,strlen(game_over));
						char endmessage[50];
						snprintf(endmessage, sizeof(endmessage), "Player%d left the game!\n", withdraw+1);
		    				writen(socketfd[k],endmessage,strlen(endmessage));
						
					}
				}
				break;
			}
			else
			{
				for(int j=0;j<4;j++)
				{
					writen(socketfd[j],keepalive,strlen(keepalive));
				}
			}
			
		            char add_message[50];
		            snprintf(add_message, sizeof(add_message), "#%d player ask %s %d.\n", turn+1, king, win_standard);
		            for(int j=0;j<4;j++)
		            {
		                if(j!=turn)
		                    writen(socketfd[j],add_message,strlen(add_message));
		            }
		            final_call=turn;
		            pass=0;
		     }
    }
    if(end==1)
    {
    	printf("withdraw:%d",withdraw);
    	    if((withdraw%2)==1)
	    {    int m=99;
		msgsnd(Msgid, &m, sizeof(m), 0);
	    }
	    else if((withdraw%2)==0)
	    {    int m=98;
		msgsnd(Msgid, &m, sizeof(m), 0);
	    }
    	sleep(10);	
    	return;
    }	
    
    	
	    char game_message3[50];
		snprintf(game_message3, sizeof(game_message3), "King decided.\n");
	    char king_message[50];
		snprintf(king_message, sizeof(king_message), "King is %s\n",king);
	    for(int j=0;j<4;j++)
	    {
		    writen(socketfd[j],game_message3,strlen(game_message3));
		    writen(socketfd[j],king_message,strlen(king_message));
	    }
	    if(strcmp(king,"spades")==0)
		k=0;
	    if(strcmp(king,"hearts")==0)
		k=1;
	    if(strcmp(king,"diamonds")==0)
		k=2;
	    if(strcmp(king,"clubs")==0)
		k=3;
        char game_message4[50];
        snprintf(game_message4, sizeof(game_message4), "You need to win %d rounds.\n", win_standard+6);
        char game_message5[50];
        snprintf(game_message5, sizeof(game_message5), "You need to win %d rounds.\n", 8-win_standard);
        int win_standard1=0;
        int win_standard2=0;
        if((final_call%2)==0)
        {
            win_standard1=win_standard+6;
            win_standard2=8-win_standard;
            writen(socketfd[0],game_message4,strlen(game_message4));
            writen(socketfd[2],game_message4,strlen(game_message4));
            writen(socketfd[1],game_message5,strlen(game_message5));
            writen(socketfd[3],game_message5,strlen(game_message5));
        }
        else
        {
            win_standard1=8-win_standard;
            win_standard2=win_standard+6;
            writen(socketfd[1],game_message4,strlen(game_message4));
            writen(socketfd[3],game_message4,strlen(game_message4));
            writen(socketfd[0],game_message5,strlen(game_message5));
            writen(socketfd[2],game_message5,strlen(game_message5));
        }
	
	
        turn=final_call;
        turn=turn%4;
        int round_card[13][4];
        int team1_point=0;
        int team2_point=0;
        int winner=0;
        sleep(6);
    for(i=0;i<13;i++)//play game
    {
    	int n=0;
    	//keepalive
	for(int j=0;j<4;j++)
	{
		if(end==1)
			break;
	    char ack[MAXLINE];
		writen(socketfd[j],keepalive,strlen(keepalive));
		FD_ZERO(&rset);
		FD_SET(socketfd[j], &rset);
		maxfdp1 = socketfd[j];
		maxfdp1++;

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
		if(c==0)
		{
			printf("Player%d left the game!\n",j+1);
			withdraw=j;
			end=1;
			break;
		}
		else
		{	int r=readline(socketfd[j],ack,MAXLINE);
			printf("r=%d\n",r);
			printf("recv:%s\n",ack);
			if(r==0)
			{
				printf("Player%d left the game!\n",j+1);
				withdraw=j;
				end=1;
				break;
			}
		}
	}
	if(end==1)
	{
		for(int k=0;k<4;k++)
		{
			if(k!=withdraw)
			{	writen(socketfd[k],game_over,strlen(game_over));
				char endmessage[50];
				snprintf(endmessage, sizeof(endmessage), "Player%d left the game!\n", withdraw+1);
		    		writen(socketfd[k],endmessage,strlen(endmessage));
				
			}
		}
		break;
		
	}
	else
	{
	    	for(int j=0;j<4;j++)
		{
			writen(socketfd[j],keepalive,strlen(keepalive));
		}
	}
    	
        
        //msgsnd(Msgid, &i, sizeof(i), 0);
        int current=-1;
        if(team1_point==win_standard1)
        {
            winner=1;
            break;
        }
        if(team2_point==win_standard2)
        {
            winner=2;
            break;
        }
        
        int m=i+1;
        msgsnd(Msgid, &m, sizeof(m), 0);
            char game_message6[50];
            snprintf(game_message6, sizeof(game_message6), "Now is round%d.\n", i+1);
            char game_message7[50];
            snprintf(game_message7, sizeof(game_message7), "It is your turn! Please send a card\n");
            for(int j=0;j<4;j++)
            {
                writen(socketfd[j],game_message6,strlen(game_message6));
            }
            for(int j=0;j<4;j++)
            {
				if(end==1)
					break;
                char recv[MAXLINE];
                int tmp;
                turn++;
                turn=turn%4;
                printf("It is #%d player's turn!\n", turn+1);
                writen(socketfd[turn],game_message7,strlen(game_message7));
                
                char current_message[50];
            	snprintf(current_message, sizeof(current_message), "%d\n", current);
                writen(socketfd[turn],current_message,strlen(current_message));
                //timeout
                n=readline(socketfd[turn], recv, MAXLINE);
                if(n==0)//normal end
                {
					for(int h=0;h<4;h++)
					{
						if(h!=turn)
						{	
							writen(socketfd[h],game_over,strlen(game_over));
							char endmessage[50];
							snprintf(endmessage,sizeof(endmessage),"Player%d left the game!\n", turn+1);
							writen(socketfd[h],endmessage,strlen(endmessage));
						}
					}
					withdraw=turn;
					end=1;
					break;
                }
				if(end==1)
					break;
                sscanf(recv, "%d\n", &tmp);
                if(j==0)
                {
                    current=player_card[turn][tmp-1]/13;
                }
                round_card[i][turn]=player_card[turn][tmp-1];// #d player's card
                char game_message8[50];
                snprintf(game_message8, sizeof(game_message8), "Player #%d post %d\n", turn+1, round_card[i][turn]);
                for(int l=0;l<4;l++)
                {
					//keepalive
					for(int j=0;j<4;j++)
					{
						char ack[MAXLINE];
						writen(socketfd[j],keepalive,strlen(keepalive));
						FD_ZERO(&rset);
						FD_SET(socketfd[j], &rset);
						maxfdp1 = socketfd[j];
						maxfdp1++;

						struct timeval timeout;
						timeout.tv_sec = 1;
						timeout.tv_usec = 0;
						int c=select(maxfdp1, &rset, NULL, NULL, &timeout);
						if(c==0)
						{
							printf("Player%d left the game!\n",j+1);
							withdraw=j;
							end=1;
							break;
						}
						else
						{	int r=readline(socketfd[j],ack,MAXLINE);
							printf("r=%d\n",r);
							printf("recv:%s\n",ack);
							if(r==0)
							{
								printf("Player%d left the game!\n",j+1);
								withdraw=j;
								end=1;
								break;
							}
						}
					}
					if(end==1)
					{
						for(int k=0;k<4;k++)
						{
							if(k!=withdraw)
							{	
								writen(socketfd[k],game_over,strlen(game_over));
								char endmessage[50];
								snprintf(endmessage, sizeof(endmessage), "Player%d left the game!\n", withdraw+1);
								writen(socketfd[k],endmessage,strlen(endmessage));
							}
						}
						break;
					}
					else
					{
						for(int j=0;j<4;j++)
						{
							writen(socketfd[j],keepalive,strlen(keepalive));
						}
					}
					if(l!=turn)
						writen(socketfd[l],game_message8,strlen(game_message8));
				}
                for(int k=tmp-1;k<12;k++)
                {
                    player_card[turn][k]=player_card[turn][k+1];
                }
            }
            if(end==1)
            	break;
            int win=-1;
            int max=0;
            int priority[4]={0};
            int num_king=0;
            for(int j=0;j<4;j++)
            {
                printf("king is %d. Current is %d.\n", k, current);
                if((round_card[i][j]/13)==k)
                {
                    priority[j]=2;
                    num_king++;
                }
                else if((round_card[i][j]/13)==current)
                {
                    priority[j]=1;
                }
            }

            if(num_king==0)//no king
            {
                for(int j=0;j<4;j++)
                {
                    printf("#%d player post %s   (%d)  %d\n", j+1, a[round_card[i][j]], round_card[i][j], priority[j]);
                    if(priority[j]==1 && round_card[i][j]>max)
                    {
                        max=round_card[i][j];
                        win=j;
                    }
                }

            }
            else
            {
                for(int j=0;j<4;j++)
                {
                    printf("#%d player post %s   (%d)  %d\n", j+1, a[round_card[i][j]], round_card[i][j], priority[j]);
                    if(priority[j]==2 && round_card[i][j]>max)
                    {
                        max=round_card[i][j];
                        win=j;
                    }
                }

            }



            char*game_message9="This round ended.\n";
            for(int j=0;j<4;j++)
            {
                writen(socketfd[j],game_message9,strlen(game_message9));
            }

            char game_message10[100];
            if(win%2==0)
            {	
                team1_point++;
                snprintf(game_message10, sizeof(game_message10), "Player #%d win this round! Team1 now have %d points.\n", win+1, team1_point);
                msgsnd(Msgid2, &team1_point, sizeof(team1_point), 0);
            }
            else
            {
                team2_point++;
                snprintf(game_message10, sizeof(game_message10), "Player #%d win this round! Team2 now have %d points.\n", win+1, team2_point);
                msgsnd(Msgid3, &team2_point, sizeof(team2_point), 0);
            }

        for(int j=0;j<4;j++)
            {
                writen(socketfd[j],game_message10,strlen(game_message10));
            }
            printf("%s\n",game_message10);
            turn=win+3;
    }//gameover
    if(end==1)
    {
    	printf("withdraw:%d",withdraw);
    	    if((withdraw%2)==1)
	    {    int m=99;
		msgsnd(Msgid, &m, sizeof(m), 0);
	    }
	    else if((withdraw%2)==0)
	    {    int m=98;
		msgsnd(Msgid, &m, sizeof(m), 0);
	    }
    	sleep(10);	
    	return;
    }	
    if(winner==1)
    {    int m=99;
        msgsnd(Msgid, &m, sizeof(m), 0);
    }
    else if(winner==2)
    {    int m=98;
        msgsnd(Msgid, &m, sizeof(m), 0);
    }
    char game_message11[50];
        snprintf(game_message11, sizeof(game_message11), "Team1 gets %d points.\n", team1_point);
        char game_message12[50];
        snprintf(game_message12, sizeof(game_message12), "Team2 gets %d points.\n", team2_point);

        for(int j=0;j<4;j++)
        {
            writen(socketfd[j],game_over,strlen(game_over));
            writen(socketfd[j],game_message11,strlen(game_message11));
            writen(socketfd[j],game_message12,strlen(game_message12));
        }
    char game_message13[50]="You win!\n";
    char game_message14[50]="You lose!\n";
        if(winner==1)
        {
            writen(socketfd[0],game_message13,strlen(game_message13));
            writen(socketfd[2],game_message13,strlen(game_message13));
            writen(socketfd[1],game_message14,strlen(game_message14));
            writen(socketfd[3],game_message14,strlen(game_message14));

        }
    else if(winner==2) {
        writen(socketfd[0], game_message14, strlen(game_message14));
        writen(socketfd[2], game_message14, strlen(game_message14));
        writen(socketfd[1], game_message13, strlen(game_message13));
        writen(socketfd[3], game_message13, strlen(game_message13));
    }
    sleep(10);
}

int main(int argc, char **argv) {
	int	num=100;//100
	int	load=96;//96
    int	num_connections=0;
    int num_player=0;
    int	num_audience=0;
    int	n, listenfd,maxfdp1;
    int connfd[num];
    int	playerfd[4];
    int	audiencefd[load];
    int	game[25];

    pid_t		childpid;
    socklen_t		clilen;
    struct sockaddr_in	servaddr, cliaddr[num];
    FILE* fp;
    char*message1="Enter 1 to play card, enter 2 to be an audience, enter 3 to be an modify Account\n";
    char*message2="There are four players! Ready to play game.\n";
    char ID[num][MAXLINE];
    char player_ID[4][MAXLINE];
    char audience_ID[load][MAXLINE];
	int audienceBet[100][100][3] = {0};
    fd_set	rset;

    srand( time(NULL) );
    
    int msgid[25];
    int msgid2[25];
    int msgid3[25];
    
    int round_value[25];
    int team1_score[25];
    int team2_score[25];
    
    for(int i=0;i<25;i++)
    {
    	key_t key1 = ftok("/tmp", i);
    	key_t key2 = ftok("/tmp", 25+i);
    	key_t key3 = ftok("/tmp", 50+i);
    	msgid[i] = msgget(key1, 0666 | IPC_CREAT);
    	msgid2[i] = msgget(key2, 0666 | IPC_CREAT);
    	msgid3[i] = msgget(key3, 0666 | IPC_CREAT);
    	team1_score[i]=-1;
    	team2_score[i]=-1;
    	round_value[i]=-1;
    	game[i]=0;
    }
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

        Signal(SIGCHLD, sig_chld);      /* must call waitpid() */

        if ((fp = fopen("finalserv23.log", "a")) == NULL) {
           printf("log file open error!\n");
           exit(0);
        };

        for(int i=0;i<num;i++)
        {
            connfd[i]=-1;
        }
        for(int i=0;i<4;i++)
        {
            playerfd[i]=-1;
        }
        for(int i=0;i<load;i++)
        {
            audiencefd[i]=-1;
        }

        
        for (;;)
        {	

		for(int i=0;i<25;i++)
		{ 	
		        if (msgrcv(msgid2[i], &team1_score[i], sizeof(team1_score[i]), 0, IPC_NOWAIT) != -1) {
		            
		             printf("Number of audience:%d\n",num_audience);
		             char team1_message[50];							
				    snprintf(team1_message, sizeof(team1_message), "Team1 gets %d points.\n", team1_score[i]);
				    for(int a=0;a<load;a++)
				    {	
					if(audiencefd[a]!=-1)
					{	writen(audiencefd[a], team1_message, strlen(team1_message));
					    printf("sent:%s",team1_message);
					}
				    }
                	
                }
                 	if (msgrcv(msgid3[i], &team2_score[i], sizeof(team2_score[i]), 0, IPC_NOWAIT) != -1) {
		            
		             printf("Number of audience:%d\n",num_audience);
		             char team2_message[50];							
				    snprintf(team2_message, sizeof(team2_message), "Team2 gets %d points.\n", team2_score[i]);
				    for(int a=0;a<load;a++)
				    {	
					if(audiencefd[a]!=-1)
					{	writen(audiencefd[a], team2_message, strlen(team2_message));
					    printf("sent:%s",team2_message);
					}
				    }
                 	}
                 	
		        if (msgrcv(msgid[i], &round_value[i], sizeof(round_value[i]), 0, IPC_NOWAIT) != -1) {
		            
		            printf("Number of audience:%d\n",num_audience);
		            char round_message[50];
		            char winner_message[50];
		            if((round_value[i]%100)==99)
		            {	
						snprintf(round_message, sizeof(round_message), "Game%d end.\n", i+1);
				    	snprintf(winner_message, sizeof(winner_message), "Team1 wins!\n");
						for(int a=0;a<load;a++)
						{	
							if(audiencefd[a]!=-1)
							{	

								writen(audiencefd[a], round_message, strlen(round_message));
								printf("sent:%s",round_message);

								char AudiName[MAXLINE], fileUsername[MAXLINE], filePassword[MAXLINE];
								float balance;
								strcpy(AudiName, audience_ID[a]);
								printf("AudiName: %s\n", AudiName);
								FILE *logFile;
								char openFile[MAXLINE];
								snprintf(openFile, MAXLINE , "%sAccount.log", AudiName);
								
								logFile = fopen(openFile, "r");
								if(logFile == NULL){
									perror("Error opening log file");
									break;
								}
								fscanf(logFile, "%s %s %f", fileUsername, filePassword, &balance);
								fclose(logFile);
								balance += audienceBet[a][i+1][1];
								logFile = fopen(openFile, "w");
								fprintf(logFile, "%s %s %.2f", fileUsername, filePassword, balance);
								fclose(logFile);
								snprintf(winner_message, sizeof(winner_message), "Team1 wins!, you remain: %f\n", balance);
								writen(audiencefd[a], winner_message, strlen(winner_message));
								audienceBet[a][i+1][1] = 0;
							}
						}
						num_connections=num_connections-4;
						num_player=num_player-4;
						printf("num_connections:%d\n",num_connections);
						printf("num_player:%d\n",num_player);
						game[i]=0;
						round_value[i]=-1;
						for(int a=0;a<25;a++)
						{
							if(game[a]!=0)
								printf("%d\n",a);
						}
		            }
		            else if((round_value[i]%100)==98)
		            {
		            	    							
						snprintf(round_message, sizeof(round_message), "Game%d end.\n", i+1);
						for(int a=0;a<load;a++)
						{	
							if(audiencefd[a]!=-1)
							{	
								writen(audiencefd[a], round_message, strlen(round_message));
								printf("sent:%s",round_message);

								char AudiName[MAXLINE], fileUsername[MAXLINE], filePassword[MAXLINE];
								float balance;
								strcpy(AudiName, audience_ID[a]);
								printf("AudiName: %s\n", AudiName);
								FILE *logFile;
								char openFile[MAXLINE];
								snprintf(openFile, MAXLINE , "%sAccount.log", AudiName);
								
								logFile = fopen(openFile, "r");
								if(logFile == NULL){
									perror("Error opening log file");
									break;
								}
								fscanf(logFile, "%s %s %f", fileUsername, filePassword, &balance);
								fclose(logFile);
								balance += audienceBet[a][i+1][2];
								logFile = fopen(openFile, "w");
								fprintf(logFile, "%s %s %.2f", fileUsername, filePassword, balance);
								fclose(logFile);
								snprintf(winner_message, sizeof(winner_message), "Team2 wins!, you remain: %f\n", balance);
								writen(audiencefd[a], winner_message, strlen(winner_message));
								audienceBet[a][i+1][2] = 0;
							}
						}
						num_connections=num_connections-4;
						num_player=num_player-4;
						printf("num_connections:%d\n",num_connections);
						printf("num_player:%d\n",num_player);
						game[i]=0;
						round_value[i]=-1;
						for(int a=0;a<25;a++)
						{
							if(game[a]!=0)
								printf("%d\n",a);
						}
		            }
		            else
		            {					
				    snprintf(round_message, sizeof(round_message), "Now game%d is round%d\n", i+1, round_value[i]);
				    for(int a=0;a<load;a++)
				    {	
					if(audiencefd[a]!=-1)
					{	writen(audiencefd[a], round_message, strlen(round_message));
					    printf("sent:%s",round_message);
					}
				    }
				    for(int a=0;a<25;a++)
				    {
				    	if(game[a]!=0)
				    		printf("%d\n",a);
				    	}
			    }

		        }
		        
		}
            FD_ZERO(&rset);
            FD_SET(listenfd, &rset);
            maxfdp1 = listenfd;
			for(int i=0;i<load;i++)
			{
				if(audiencefd[i]!=-1)
				{
				FD_SET(audiencefd[i], &rset);
				if(audiencefd[i]>listenfd)
					maxfdp1 = audiencefd[i];
				}
			}
            maxfdp1++;

            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            select(maxfdp1, &rset, NULL, NULL, &timeout);
			for(int i=0;i<load;i++)
            {
            	if(audiencefd[i]!=-1)
            	{	
            		char recvline[MAXLINE];
            		int input = 0;
					int team = 0;
            		float bet = 0.0;
            		if(FD_ISSET(audiencefd[i], &rset))
            		{	
            					int r=readline(audiencefd[i], recvline, MAXLINE);
						if(r<=0){
							
							num_connections--;
                        				num_audience--;
                        				audiencefd[i]=-1;
							memset(audience_ID[i], 0, MAXLINE);
							printf("num_connections:%d\n",num_connections);
							printf("num_audience:%d\n",num_audience);
							break;
						}
						sscanf(recvline, "game%d team%d %f\n", &input, &team, &bet);
            			//check game
            			//check bet 
						char AudiName[MAXLINE], fileUsername[MAXLINE], filePassword[MAXLINE];
						float balance;
						strcpy(AudiName, audience_ID[i]);
						printf("audience name: %s\n", AudiName);
						FILE *logFile;
						char openFile[MAXLINE];
						snprintf(openFile, MAXLINE , "%sAccount.log", AudiName);
						logFile = fopen(openFile, "r");
						if(logFile == NULL){
							perror("Error opening log file");
							break;
						}
						fscanf(logFile, "%s %s %f", fileUsername, filePassword, &balance);
						fclose(logFile);
						if(bet > balance){
							writen(audiencefd[i], "error bet!\n", strlen("error bet!\n"));
						}
						else{
							audienceBet[i][input][team] += bet;
							audienceBet[i][input][team%2+1] -= bet;
							char successMsg[MAXLINE]; 
							snprintf(successMsg, MAXLINE, "bet successfully!\n");
							writen(audiencefd[i],successMsg, strlen(successMsg));
							logFile = fopen(openFile, "w");
							if(logFile == NULL){
								perror("Error opening log file");
								break;
							}
							fprintf(logFile, "%s %s %.2f", fileUsername, filePassword, balance);
							fclose(logFile);
						}
            		}
            	}
            
            }
            if(FD_ISSET(listenfd, &rset) && num_connections<num)//new connection
            {
                printf("new connection!\n");
                printf("num_connections:%d\n",num_connections);
                int i=0;
                bool j=0;
                while(i<=num)
            	{
                if(j==1)
                    break;
                //startHere
                if(connfd[i]==-1)
                {
                    bzero(&cliaddr[i], sizeof(cliaddr[i]));
                    char recvline[MAXLINE];
                    connfd[i] = accept(listenfd, (SA *) &cliaddr[i], &clilen);
                    printf("%d connect ok\n",i);

                    if ((n = read(connfd[i], ID[i], MAXLINE)) <= 0) {
                            printf("error, please relogin!\n");
                            fprintf(fp, "recv error\n");
                            break;
                     }
                    
                    int state = -1;
                    char filename[50];
                    snprintf(filename, sizeof(filename), "%sAccount.log", ID[i]);
                    FILE* fp2;
                    if((fp2 = fopen(filename, "r"))==NULL) {
                        printf("New customer\n");
                        state = registerUser(ID[i], connfd[i]);
                    }
                    
                    else{
                    	state = loginUser(ID[i], connfd[i]);
                    } 
					if(state == -3) break;

                    //printf("recv ID : %d %d %s\n",i ,connfd[i], ID[i]);
                    if(state) writen(connfd[i], message1, strlen(message1));
                    printf("sent:%s",message1);
                    
                    fd_set	rrset;
                    FD_ZERO(&rrset);
					FD_SET(connfd[i], &rset);
					maxfdp1 = connfd[i];
					maxfdp1++;

					struct timeval Timeout;
					Timeout.tv_sec = 60;
					Timeout.tv_usec = 0;
					int c=select(maxfdp1, &rset, NULL, NULL, &Timeout);
		    if(c==0)
		    {
		    	printf("not recv role!\n");
		    	break;
		    }
                    int m=readline(connfd[i], recvline, MAXLINE);
                    printf("recv:%s",recvline);	
                    //num_connections++;
                    if(m==0)
                    {
                    	printf("connection fails.\n");
                    	break;
                    }
                    if(strcmp(recvline,"1\n")==0)
                    {
                    	num_connections++;
                        num_player++;
                        

                        // char message3[100];							
                        // snprintf(message3, sizeof(message3), "Please wait other players.\n");
                        // writen(connfd[i], message3, strlen(message3));
                        //keepalive
			    for(int k=0;k<4;k++)
				{
					
					if(playerfd[k]!=-1)
					{
						char*keepalive="keepalive\n";
						char ack[MAXLINE];
						fd_set	trset;
						int maxfdp;
						writen(playerfd[k],keepalive,strlen(keepalive));
						//printf("w=%d",w);
						FD_ZERO(&trset);
						FD_SET(playerfd[k], &trset);
						maxfdp = playerfd[k];
						maxfdp++;

						struct timeval timeout;
						timeout.tv_sec = 1;
						timeout.tv_usec = 0;
						int c=select(maxfdp, &trset, NULL, NULL, &timeout);
						printf("c=%d",c);
						if(c==0)
						{
							playerfd[k]=-1;	
							num_player--;
							num_connections--;
							
						}
						else
						{
							int r=readline(playerfd[k],ack,MAXLINE);
							printf("r=%d\n",r);
							printf("recv:%s\n",ack);
							if(r<=0)
							{
								playerfd[k]=-1;	
								num_player--;
								num_connections--;
							}
						}
					}
				}


						char m3[100];							
                        snprintf(m3, sizeof(m3), "player message\n");
                        writen(connfd[i], m3, strlen(m3));
						int which = 0;
						for(int it = 0; it < 4; it++){
							if(playerfd[it] == -1) {
								which = it;
								break;
							}
						}
						for(int k=0;k<4;k++)
                        {
                            if(playerfd[k]==-1)
                            {
                                playerfd[k]=connfd[i];
                                strncpy(player_ID[k],ID[i],n);
                                printf("%s is player%d\n ",player_ID[k], num_player);
                                break;
                            }
                        }
						char message3[100];							
                        snprintf(message3, sizeof(message3), "You are the #%d player. Please wait other players.\n", which);
                        writen(connfd[i], message3, strlen(message3));
                        peopleNum(playerfd, num_player);

			int g=0;
                        if((num_player%4)==0 && num_player!=0)//start new game
                        {
                             for(int k=0;k<25;k++)	
                            {
                                if(game[k]==0)
                                {
                                	g=k;
                                	printf("game%d ready to start!\n",g+1);
                                	game[k]=1;
                                	break;
                                }
                            }
                        		
                            for(int k=0;k<4;k++)	
                            {
                                writen(playerfd[k], message2, strlen(message2));
                            }
                            if((childpid=fork())==0)
                            {
                                close(listenfd);
                                play(playerfd[0],playerfd[1],playerfd[2],playerfd[3],player_ID[0],player_ID[1],player_ID[2],player_ID[3], msgid[g], msgid2[g],msgid3[g],g);
                                exit(0);
                            }
                            for(int k=0;k<4;k++)	
                            {
                                playerfd[k]=-1;
                                memset(player_ID[k], 0, MAXLINE);
                                //*player_ID[k]=NULL;
                            }
                            for(int k=0;k<4;k++)
                            {
                                close(playerfd[k]);
                            }

                        }

                    }
                    else if(strcmp(recvline,"2\n")==0){
                    	num_connections++;
                        num_audience++;
                        for(int k=0;k<load;k++)
                        {
                            if(audiencefd[k]==-1)
                            {	
                            	audiencefd[k]=connfd[i];
                                strncpy(audience_ID[k],ID[i],n);
                                printf("%s is #%d audience.\n", audience_ID[k], k+1);
                                int s=0;
                                for(int l=0;l<25;l++)
                                {
                                    if(game[l]>0 && round_value[l]!=-1)
                                    {
                                        char message4[50];							
                                        snprintf(message4, sizeof(message4), "now game%d is round%d\n", l+1, round_value[l]);
                                        writen(audiencefd[k],message4,strlen(message4));
                                        printf("sent:%s",message4);
                                        s++;
                                    }
                                    if(game[l]==0 && round_value[l]!=-1)
                                    {
                                    	round_value[l]=-1;
                                    }
		                        }
                                if(s==0)
                                {
                                    char*message5="No games begin.\n";						
                                    writen(audiencefd[k],message5,strlen(message5));
                                    printf("sent:%s",message5);
                                }
                                
                                break;
                            }
                        }

                    }
                    else if(strcmp(recvline, "3\n") == 0){
                        //int error = 0;
                        int modifier;
                        modifier = connfd[i];
                        printf("modifing\n");
                        printf("filename: %s\n", filename);
                        FILE* fp2;
                        if((fp2 = fopen(filename, "r"))==NULL) {
                            printf("New customer\n");
                            state = registerUser(ID[i], connfd[i]);
                        }
                        else{
                            modifyAccount(filename, modifier);
                        }
                            
                    }
                    j=1;
                }
                else
                    i++;
            }
        }
    }
    exit(0);
}


