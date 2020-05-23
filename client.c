#include "server.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
int main(int argc,char *argv[])
{	int line;//line is used to store the line in which user security information is stored in shadow file
	char old_pass[12],new_pass[12];//password must be between 8 and 12 characters
	printf("The user you want to change password for is: %s\n",argv[1]);
 	if(check_user(argv[1]))
	{
		printf("USER FOUND");
		line=check_user(argv[1]);
		printf("\nEnter the current password: ");
		scanf("%s",old_pass);
		check_passwd(old_pass);/*This function is used to check the old/current password. This compares the entered password with the encrypted password in the shadow file.
					This function intern calls another function new_passwd whicb is used to perform the operation of accepting a new password and encrypting it into the 						shadow file */
		if(check_passwd(old_pass))
		{
			remove("passwd_for_user");//this is to delete the file where only the password is stored of the user in-case its not deleted.
			label://This label is used in the goto function/command which comes few lines after this. It is used to reenter password of conditions arent satisfied.
			printf("Enter the new password: ");
			scanf("%s",new_pass);
			if(strlen(new_pass)<8)
			{
				printf("Password length must be between 8 and 12 characters!\n");
				goto label;
			}
			//new_passwd is a function to perform the operation of confirming the new password.	
			if(strcpy(new_pass,new_passwd(new_pass)))/*here first new_passwd function will be called and then later, will be copied to new_pass variable. Condition will be true
								if new_pass isnt empty. new_pass will be empty only when you do not enter correct password while confirming.*/
			{
				
				append_to_shadow(line,argv[1],new_pass);
			}	
		}		
	}
	else
	{
		printf("USER NOT FOUND");
	}
	printf("\n");
	
}
