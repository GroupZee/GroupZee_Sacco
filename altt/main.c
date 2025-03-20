#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef struct {
    char accountNumber[13];
    char name[50];
    double amount;
    char pin[5];
} account;
account accounts[] = {
        {"A00011122201","Sheillah",20000,"0145"},
        {"A00011122202","John",29000,"0146"},
        {"A00011122203","Mark",30000,"9012"},
        {"A00011122204","Andrew",434000,"0888"},
        {"A00011122205","Katash",28000,"0141"},
        {"A00011122206","Abraham",2310000,"0501"},
    };
int valid = 1; // Flag to check if input is valid
char entry1[13],entry2[13],entry3[5];
double amount;
int choice;


int main() {
int n, x, i;
n=sizeof(accounts)/sizeof(accounts[0]);
printf("Enter your account number:");
scanf("%12s",&entry1);
//Checking account number
valid=0;
for(i=0;i<=n;i++){
    if(strcmp(entry1,accounts[i].accountNumber)==0){
    valid=1;
    printf("Welcome %s.\n",accounts[i].name);
    }
}
//
if(!valid){
    printf("You have enter an incorrect account number*.");
    return 1;
//Prompt user to re-enter their account number or create an account
}

printf("Enter your pin: ");
scanf("%4s",&entry3);
//Checking the pin
valid=0;
for(int p=0;p<=n;p++){
    if(strcmp(entry3,accounts[p].pin)==0){
    valid=1;
    printf("1.Send Money\n2.Withdraw money\n3.Check your account balance.\n :");
    }
}
if(!valid){
    printf("Incorrect pin.");
    return 0;
}

scanf("%d",&choice);
switch(choice){
    case 1:
        printf("Enter the Account number you wish to send to:");
        scanf("%12s",entry2);
        if(strcmp(entry1,entry2)==0){
            printf("You can't send money to your own account.");
            return 0;
        }
        // Is Receiver a known account??
        else{
            for(i=0;i<=n;i++){
                if (strcmp(entry2,accounts[i].accountNumber)==0){
                    valid = 1;
                    printf("Enter amount to send:");
                    scanf("%.2lf",&amount);
                    printf("Please wait");
                }
            }
        }

    break;
    case 2:
        printf("Enter amount\n");


    break;
    case 3:
        for(i=0;i<=n;i++){
            if (strcmp(entry1,accounts[i].accountNumber)==0){
            valid=1;
            printf("Your account balance is %.0f\n",accounts[i].amount);
            }
        }
    break;
    default:
        printf("Invalid Choice.");
        break;
}




    return 0;
}
