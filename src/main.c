/*
    Created by Ghica Ana-Maria on 28.03.2025
    Copyright (c) 2025 Ghica Ana-Maria. All rights reserved.
    Facultatea de Automatica si Calculatoare
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

// functie care prelucreaza inputul unei instructiuni de tip "for"
void convert_for_expression(char *buffer, int *flag) {
char register_var[5] = "";
char cond_jump_false[5] = "";

char *string = malloc(BUFFER_SIZE * sizeof(char));
if (!string)
{
	printf("Eroare la alocarea memoriei pentru string.\n");
	exit(1);
}

	strcpy(string, buffer);

// gasesc tipul de jump (pe cazul fals) pe care trebuie sa il fac
if (strstr(string, "<"))
	strcpy(cond_jump_false, "JGE");
if (strstr(string, "<="))
	strcpy(cond_jump_false, "JG");
// gasesc registrul in care trebuie sa pun valoarea
if (strstr(string, "a"))
{
	strcpy(register_var, "eax");
	*flag = 1;
}

if (strstr(string, "b"))
{
	strcpy(register_var, "ebx");
	*flag = 2;
}

if (strstr(string, "c"))
{
	strcpy(register_var, "ecx");
	*flag = 3;
}

if (strstr(string, "d"))
{
	strcpy(register_var, "edx");
	*flag = 4;
}

// vreau sa obtin numarul de 2 cifre si stringul care arata asa:
// "for (a = 0; a < 65; a = a + 1) {"
int number = -1;
if (strstr(string, "<="))
{
	char *p = strstr(string, "<=");
	number = atoi(p + 2);
} else {
	if (strstr(string, "<"))
	{
		char *p = strstr(string, "<");
		number = atoi(p + 1);
	}
}

// afisarile
printf("MOV %s, 0\n", register_var);
printf("start_loop:\n");
printf("CMP %s, %d\n", register_var, number);
printf("%s end_label\n", cond_jump_false);

free(string);
}

// functie care prelucreaza inputul unei instructiuni de tip "if" sau "while"
void convert_if_while_expression(char *buffer) {
char register_var[5] = "";
char register_value[5] = "";
char cond_jump_false[5] = "";
char *string = malloc(BUFFER_SIZE * sizeof(char));
if (!string)
{
	printf("Eroare la alocarea memoriei pentru string.\n");
	exit(1);
}

strcpy(string, buffer);

// gasesc tipul de jump PE CAZUL FALS pe care trebuie sa il fac
if (strstr(string, "=="))
	strcpy(cond_jump_false, "JNE");
else if (strstr(string, "<="))
	strcpy(cond_jump_false, "JG");
else if (strstr(string, ">="))
	strcpy(cond_jump_false, "JL");
else if (strstr(string, ">"))
	strcpy(cond_jump_false, "JLE");
else if (strstr(string, "<"))
	strcpy(cond_jump_false, "JGE");

// gasesc SCALARUL cu care compar variabila initiala
char *token = strtok(buffer, " ()");
token = strtok(NULL, " ()");
if (strcmp(token, "a") == 0)
	strcpy(register_var, "eax");
if (strcmp(token, "b") == 0)
	strcpy(register_var, "ebx");
if (strcmp(token, "c") == 0)
	strcpy(register_var, "ecx");
if (strcmp(token, "d") == 0)
	strcpy(register_var, "edx");

token = strtok(NULL, " ()");
token = strtok(NULL, " ()"); // am aplicat strtok de 2 ori ca sa obtin SCALARUL
// (pls alta metoda nu stiu)
strcpy(register_value, token);

//printez conditia
printf("CMP %s, %s\n", register_var, register_value);
//printez cazul fals
printf("%s end_label\n", cond_jump_false);

free(string);
}

// functie care prelucreaza o secventa de cod
// ea este folosita atat pentru testele simple (doar cu linii propriu zise)
// cat si pentru testele care contin if/while/for
void convert_line(char *buffer) {
char register_var[5] = "";
char mnemonic[5] = "";

char *string = malloc(BUFFER_SIZE * sizeof(char));
if (!string) {
	printf("Eroare la alocarea memoriei pentru string.\n");
	exit(1);
}

strcpy(string, buffer);

// gasesc operatia pe care trebuie sa o fac
if (strstr(string, "&"))
	strcpy(mnemonic, "AND");
else if (strstr(string, "^"))
	strcpy(mnemonic, "XOR");
else if (strstr(string, "|"))
	strcpy(mnemonic, "OR");
else if (strstr(string, "+"))
	strcpy(mnemonic, "ADD");
else if (strstr(string, "-"))
	strcpy(mnemonic, "SUB");
else if (strstr(string, "*"))
	strcpy(mnemonic, "MUL");
else if (strstr(string, "/"))
	strcpy(mnemonic, "DIV");
else if (strstr(string, "<<"))
	strcpy(mnemonic, "SHL");
else if (strstr(string, ">>"))
	strcpy(mnemonic, "SHR");
else
	strcpy(mnemonic, "MOV");

// gasesc registrul in care trebuie sa pun rezultatul
char *token = strtok(string, " ;");
if (strcmp(token, "a") == 0)
	strcpy(register_var, "eax");
if (strcmp(token, "b") == 0)
	strcpy(register_var, "ebx");
if (strcmp(token, "c") == 0)
	strcpy(register_var, "ecx");
if (strcmp(token, "d") == 0)
	strcpy(register_var, "edx");

// gasesc SCALARUL / ALT REGISTRU pe care il aplic variabilei initiale
char token_value[5] = "";
while (token)
{
	strcpy(token_value, token);
	token = strtok(NULL, " ;");
}

char register_value[5] = "";
if (strcmp(token_value, "a") == 0)
	strcpy(register_value, "eax");
else if (strcmp(token_value, "b") == 0)
	strcpy(register_value, "ebx");
else if (strcmp(token_value, "c") == 0)
	strcpy(register_value, "ecx");
else if (strcmp(token_value, "d") == 0)
	strcpy(register_value, "edx");
else
	strcpy(register_value, token_value);
// afisarea in functie de operatia pe care o fac
if (strcmp(mnemonic, "MUL") == 0)
{
	if (strcmp(register_var, "eax") == 0)
		printf("MUL %s\n", register_value);
	else
	{
		printf("MOV eax, %s\n", register_var);
		printf("MUL %s\n", register_value);
		printf("MOV %s, eax\n", register_var);
	}

		int a = 10;
		a = a + 1;
}

else if (strcmp(mnemonic, "DIV") == 0)
{
	if (strcmp(register_value, "0") == 0)
		printf("Error\n");
	else
	{
		if (strcmp(register_var, "eax") == 0)
		{
			printf("MOV eax, a\n");
			printf("DIV %s\n", register_value);
			printf("MOV a, eax\n");
		}

		else
		{
			printf("MOV eax, %s\n", register_var);
			printf("DIV %s\n", register_value);
			printf("MOV %s, eax\n", register_var);
		}
	}
} else {
	printf("%s %s, %s\n", mnemonic, register_var, register_value);
	}
free(string);
}

int main(void)
{
// citesc de la tastatura "linie cu linie" intr-un string denumit "buffer"
char *buffer = malloc(BUFFER_SIZE * sizeof(char));
if (!buffer)
{
	printf("Eroare la alocarea buffer-ului.\n");
	return 1;
}

while (fgets(buffer, BUFFER_SIZE, stdin))
{
	if (buffer[0] == '\n')
		break;
	buffer[strlen(buffer) - 1] = '\0';
	// in buffer am o linie din "datele de intrare"
	if (strstr(buffer, "if"))
	{
		// trebuie sa prelucrez intai expresia din "if (...)"
		convert_if_while_expression(buffer);
		fgets(buffer, BUFFER_SIZE, stdin); // citesc prima expresie din block
		buffer[strlen(buffer) - 1] = '\0';
		// while (strstr(buffer, "}") == NULL) {
		while (buffer[0] != '}')
		{
			int buffer_length = (int)strlen(buffer);
			for (int i = 1; i <= buffer_length; i++) // intrebare
				// de ce paddingul e doar cu un spatiu????
				buffer[i - 1] = buffer[i];
				//buffer[strlen(buffer) - 4] = '\0';
				//printf("PROBA INAINTE:%s\n", buffer);
			convert_line(buffer);

			fgets(buffer, 100, stdin);
			buffer[strlen(buffer) - 1] = '\0';
		}
		printf("end_label:\n");
	} else if (strstr(buffer, "while"))
	{
		printf("start_loop:\n");
		// trebuie sa prelucrez intai expresia din "while (...)"
		convert_if_while_expression(buffer);
		fgets(buffer, BUFFER_SIZE, stdin); // citesc prima expresie din block
		buffer[strlen(buffer) - 1] = '\0';
		// while (strstr(buffer, "}") == NULL) {
		while (buffer[0] != '}')
		{
			int buffer_length = (int)strlen(buffer);
			for (int i = 1; i <= buffer_length; i++) // intrebare
				// de ce paddingul e doar cu un spatiu????
				buffer[i - 1] = buffer[i];
			//buffer[strlen(buffer) - 4] = '\0';
			//printf("PROBA INAINTE:%s\n", buffer);
			convert_line(buffer);
			fgets(buffer, BUFFER_SIZE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
		}
		printf("JMP start_loop\n");
		printf("end_label:\n");
	} else if (strstr(buffer, "for"))
	{
		//printf("proba: aici e segfault\n");
		// trebuie sa prelucrez intai expresia din "for (...)"
		int flag = 0;
		convert_for_expression(buffer, &flag);
		fgets(buffer, BUFFER_SIZE, stdin); // citesc prima expresie din block
		buffer[strlen(buffer) - 1] = '\0';
		// while (strstr(buffer, "}") == NULL) {
		while (buffer[0] != '}')
		{
			int buffer_length = (int)strlen(buffer);
			for (int i = 1; i <= buffer_length; i++)
				buffer[i - 1] = buffer[i];
			convert_line(buffer);
			fgets(buffer, BUFFER_SIZE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
		}

		if (flag == 1)
			printf("ADD eax, 1\n");
		else if (flag == 2)
			printf("ADD ebx, 1\n");
		else if (flag == 3)
			printf("ADD ecx, 1\n");
		else if (flag == 4)
			printf("ADD edx, 1\n");

		printf("JMP start_loop\n");
		printf("end_loop:\n");
	} else {
		convert_line(buffer); // cazul de baza
	}
}

free(buffer);
return 0;
}
