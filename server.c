#include "server.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
int check_user(char user_name[])//This function checks the existence of the user
{
	FILE *fp;
	FILE *passwd_save;
	int errnum,c;
	char s[100];// This character array is to read the content of every line in the shadow file. 
	char *username;// This pointer is to compare the username with the input username. It is a token for the strtok function.
	fp=fopen("shadow","r");	
	if (fp == NULL) 
	{
   	errnum = errno;
      	fprintf(stderr, "Value of errno: %d\n", errno);
      	fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
	return 0;
	}
	else
	{	int line=0;//this line variable is used to store in which line is the user security data at in the shadow file.
		while(fgets(s,sizeof(s),fp)!=NULL)
		{	
			line++;
			username=strtok(s,":");//strtok is a builtin function in string.h used to get substrings before a delimiter. 
			while(username!=NULL && c!=1)//Here c is just used for iteration and is a part of the logic. After the user is found, we need one more iteration of username so that we get 							      encrypted password		
			{			
			if(strcmp(username,user_name)==0)
			{
				c=-1;	
							
			}
			if(c==0)
			{
				passwd_save=fopen("passwd_for_user","w");
				fputs(username,passwd_save);
				fclose(passwd_save);
				return line;
			}
			strcpy(s,"");
			username=strtok(NULL,":");
			c++;		
			}
		}
	fclose(fp);
	}
}
int check_passwd(char old_passwd[])
{
	FILE *passwd_save;
	int errnum;
	char s[100];
	passwd_save=fopen("passwd_for_user","r");
	if(passwd_save!=NULL)
	{
	while(fgets(s,sizeof(s),passwd_save)!=NULL)
	{
		if(strcmp(decrypt(s),old_passwd)==0)// Here, the password in the shadow file for this user is decrypted and compared with input current password.
		{
			return 1;
		}
	}
	fclose(passwd_save);
	remove("passwd_for_user");
	}
	else
	{
		printf("Wrong password");
		return 0;
	}
	
}

char *encrypt(char passwd[])//Encryption by Caeser-Cipher Method, key=3 here.
{
	int i;
	char ch;
	for(i = 0; passwd[i] != '\0'; ++i)
	{
		ch = passwd[i];
		if(ch >= 'a' && ch <= 'z')
		{
			ch = ch + 3;
			
			if(ch > 'z')
			{
				ch = ch - 'z' + 'a' - 1;
			}
			
			passwd[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z')
		{
			ch = ch + 3;
			
			if(ch > 'Z')
			{
				ch = ch - 'Z' + 'A' - 1;
			}
			
			passwd[i] = ch;
		}
		else
		{
			ch=ch+3;
			passwd[i]=ch;
		}
	}
	return passwd;
}
char *decrypt(char passwd[])//Decryption is just reverse of Encryption and its again by Caeser-Cipher Method, key=3 here.
{
	int i;
	char ch;
	for(i = 0; passwd[i] != '\0'; ++i)
	{
		ch = passwd[i];
		
		if(ch >= 'a' && ch <= 'z')
		{
			ch = ch - 3;
			
			if(ch < 'a')
			{
				ch = ch + 'z' - 'a' + 1;
			}
			
			passwd[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z')
		{
			ch = ch - 3;
			
			if(ch < 'A')
			{
				ch = ch + 'Z' - 'A' + 1;
			}
			
			passwd[i] = ch;
		}
		else
		{
			ch=ch-3;
			passwd[i]=ch;
		}
	}
	return passwd;
	
}
char *new_passwd(char passwd[])
{
	char passwd_new[12];
	printf("Confirm your password: ");
	scanf("%s",passwd_new);
	if(strcmp(passwd_new,passwd)==0)
	{
		printf("Password changed Succesfully!!\n");
		return passwd;
	}
	else
	{
		printf("\nNot Matching! Please Enter the same password!\n ");
		exit(EXIT_FAILURE);
	}
	
}
int append_to_shadow(int line,char username[],char passwd[])//passwd is the new password entered.
{
	int pline=0;//This variable is used to store the line in which our file pointer is at to copy all lines except the user line.
	char s[100],l[100]="";//l character array will contain the user info line to be appended.
	char *token;	
	FILE *shadow;
	FILE *shadow_dup;//This is used to create a duplicate shadow file. We will later rename this file after inserting the content.
	shadow_dup=fopen("shadow_duplicate","w");
	shadow=fopen("shadow","r");	
	while(fgets(s,sizeof(s),shadow)!=NULL)
	{	pline++;
		if(pline!=line)
		{
			fputs(s,shadow_dup);
		}
		if(pline==line)
		{
			//here we are hardcoding the text to be appended to the shadow file as there is no other way to fill in the other parameters.
			strcat(l,username);
			strcat(l,":");
			strcat(l,encrypt(passwd));
			strcat(l,":18349:0:99999:7:::\n");
			fputs(l,shadow_dup);
		}
	}
	fclose(shadow);
	remove("shadow");
	fclose(shadow_dup);
	rename("shadow_duplicate","shadow");
			
}

