#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//command line arguments
#define O 10
#define E 20
#define R 40
#define OE 1
#define OR 2
#define RE 3
#define ORE 4
#define NONE -1


#define LUNGIME 1024

int processArguments(int argc , char ** arguments) ;
int processLink(char * link , char ** output);
void createDirectories(int size , char ** names) ;
void getServerName(char ** bitsAndPieces , char * output , int size );
void getPath(char ** bitsAndPieces, char * output , int size);
void getFileName(char ** bitsAndPieces , char * filename , int size );
void pullOutPage(char * server , char * path , char * filename);
int main(int argc , char ** argv){
	

	char ** output = (char **) malloc (340 * sizeof(char *));

	int summ = processArguments(argc , argv);	

	int numberOfPaths = 0 ;
	numberOfPaths = processLink(argv[1] , output);

	int i = 0 ; 
	for ( i = 0 ; i < numberOfPaths ; i ++){
		printf("output[%d] is %s \n", i , output[i]);
	}


	createDirectories (numberOfPaths , output);


	
	switch(summ){
		case O :{
		}
		case E :{
		}
		case R :{
		}
		case OE :{
		}
		case OR:{
		}
		case RE :{
		}
		case ORE :{
		}
		case NONE :{
		}
	
	}

	char * serverAddress = (char *) malloc (340 * sizeof(char));
	char * path = (char *) malloc (340 * sizeof(char));
	char * filename = (char *) malloc(340 * sizeof(char));

	getServerName(output , serverAddress , numberOfPaths);
	getPath (output , path , numberOfPaths );
	getFileName(output , filename , numberOfPaths);

	pullOutPage (serverAddress , path , filename);
	
	return 0; 
}


int processArguments(int argc , char ** arguments){
	int summ = 0 ; 

	int i = 0 ;
	for(i = 0 ; i < argc ; i++){	
		if (strstr(arguments[i] , "-o")){
			summ += O ;
		}

		if(strstr (arguments[i], "-e")){
			summ += E ;
		}

		if(strstr(arguments[i], "-r")){
			summ += R ;
		}
	}


	if(summ >= 10 && summ < 20 ){  //the integer fits between O argument values 
		return O;
	}else if( summ >= 20 && summ < 30 ){
		return E ;
	}else if( summ >= 30 && summ < 40){
		return OE ;
	}else if(summ >=40 && summ < 50){
		return R ;
	}else if(summ >= 50 && summ < 60){
		return OR ;
	}else if(summ >= 60 && summ < 70){
		return RE ;
	}else if(summ >= 70){
		return ORE ;
	}

	return NONE ;
}

int processLink(char *link , char ** output){

	int counter = 0 ;  //counting the number of elements int the output vector

	printf("Link is %s \n" , link) ;
	
	int i = 0; 
	for(i = 0 ; i < 240 ; i++){	
		output[i] = (char *) malloc (50 * sizeof(char *));
	}

	//find the http declaration 
	char * http = strstr(link , "http://");
	http = http+strlen("http://");
	printf("Http is %s \n" , http);
	
	char * token = strtok(http , "/");
	strcpy(output[0] , token );
	counter ++ ; //added the name of the website to the output 

	i = 1;
	while((token = strtok(NULL , "/")) != NULL){
		strcpy(output[i++] , token );
		counter ++ ;
		printf("token is %s \n", token);
	}
	return counter; 
}

void createDirectories(int size , char ** names){

	int i = 0; 
	if(size == 1 ){
		char * path = (char *) malloc (60*sizeof (char) );
		sprintf(path , "./");
		strcat (path ,names[i]);
		// printf("path is %s \n" , path );
		mkdir(names[0	] , 0777);
		chdir(path);
	}

	for(i = 0 ; i < size -1;i ++ ){
		char * path = (char *) malloc (60*sizeof (char) );
		sprintf(path , "./");
		strcat (path ,names[i]);
		// printf("path is %s \n" , path );
		mkdir(names[i] , 0777);
		chdir(path);
		
	}
}


void getServerName(char ** bitsAndPieces , char * output , int size ){
	if(output == NULL)
		output = (char * ) malloc (340 * sizeof(char));
	strcpy(output , bitsAndPieces[0]);
}


void getPath(char ** bitsAndPieces, char * output , int size){
	int i= 0 ;
	if(output == NULL)
		output = (char * )malloc (100 * sizeof(char));
	sprintf(output , "/");

	for(i = 1; i < size-1 ; i++){
		strcat(output , bitsAndPieces[i]);
		if(i != size- 1)
			strcat(output , "/");
	}
}

void getFileName(char ** bitsAndPieces , char * filename , int size){
	if(filename == NULL)
		filename = (char *) malloc (340 * sizeof(char));

	strcpy(filename , bitsAndPieces[size-1]);

}


void pullOutPage(char * server , char * path , char * filename){

	FILE * file = fopen (filename ,"w+");
	char * buff = calloc (0, LUNGIME);
	struct sockaddr_in address ;
	
	int fd = socket(AF_INET , SOCK_STREAM , 0);
	printf("socked description is %d\n", fd);
	struct hostent * host = gethostbyname(server);

	printf("Path is %s \n",path );

	address.sin_family = AF_INET ;
	address.sin_port = htons(80);
	memcpy(&address.sin_addr.s_addr, host->h_addr_list[0] , strlen (host->h_addr_list[0]));
	
	connect(fd ,(struct sockaddr *) &address , sizeof(address));
	
	sprintf(buff , "GET %s HTTP/1.1\nHost: %s\n\n" ,path ,server);
	send(fd , buff , strlen(buff) , 0) ;

	memset ( buff , 0 ,LUNGIME);

	int n = 0 ;
	int endOfHeader = 0 ;

	while((n = recv (fd , buff , LUNGIME , 0)) > 0 ){
		printf("%s", buff);


		char * startOfDelimiter = strstr(buff , "\r\n\r\n"); 

		if(startOfDelimiter != NULL){
			endOfHeader = 1;
			startOfDelimiter = startOfDelimiter + strlen("\r\n\r\n");
			fprintf(file , "%s" , startOfDelimiter );
			memset (buff , 0 , LUNGIME);
		}

		if(endOfHeader == 1 ){
			printf("%s", buff);
			fprintf(file , "%s", buff);

			if(strstr(buff ,"<html>") != NULL || strstr(buff,"<HTML>") != NULL){
				printf("End of file \n");
				return ;
			}
		}

		memset (buff ,0 , LUNGIME);
	}

	fclose (file);

}