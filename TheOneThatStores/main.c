#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_ACCOUNTS 100

typedef struct {
    char accountNumber[13];
    char name[50];
    double amount;
    char pin[5];
} account;

typedef struct {
    char accountNumber[13];
    char type[9];
    double amount;
    time_t stamp;
} transactions;




account accounts[MAX_ACCOUNTS];
int n = 0; // Number of accounts loaded

void loadAccounts() {
    FILE *file = fopen("accounts.dat", "rb");
    if (file) {
        while (fread(&accounts[n], sizeof(account), 1, file) == 1) {
            n++;
        }
        fclose(file);
    }
}

void saveAccounts() {
    FILE *file = fopen("accounts.dat", "wb");
    if (file) {
        fwrite(accounts, sizeof(account), n, file);
        fclose(file);
    }
}

void displayAccounts() {
    for (int i = 0; i < n; i++) {
        printf("Account Number: %s, Name: %s, Balance: %.0f\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].amount);
    }
}

void logs(transactions *tt,const char *accountNumber,const char *type,double amount){
FILE *file=fopen("logs.dat","ab");
if(file){
    transactions tt;
    strcpy(tt.accountNumber, accountNumber);
    strcpy(tt.type, type);
    tt.amount=amount;
    tt.stamp=time(NULL);
    fwrite(&tt,sizeof(transactions),1,file);
    fclose(file);
}else{
    printf("No data");
}

}



int main() {
    char entry1[13], entry2[13], entry3[5];
    double amount;
    int choice;
    int valid = 0;

    loadAccounts();

    printf("Welcome to GroupZee Sacco.\nEnter your account number: ");
    scanf("%12s", entry1);

    // Check account number
    for (int i = 0; i < n; i++) {
        if (strcmp(entry1, accounts[i].accountNumber) == 0) {
            valid = 1;
            printf("Welcome %s.\n", accounts[i].name);
            break;
        }
    }

    if (!valid) {
        printf("You have entered an incorrect account number.\n");
        return 1;
    }

    printf("Enter your pin: ");
    scanf("%4s", entry3);

    // Check the pin
    valid = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(entry3, accounts[i].pin) == 0) {
            valid = 1;
            printf("1. Send Money\n2. Withdraw Money\n3. Check Your Account Balance.\n: ");
            break;
        }
    }

    if (!valid) {
        printf("Incorrect pin.\n");
        return 0;
    }

    scanf("%d", &choice);
    switch (choice) {
        case 1:
            printf("Enter the Account number you wish to send to: ");
            scanf("%12s", entry2);
            if (strcmp(entry1, entry2) == 0) {
                printf("You can't send money to your own account.\n");
                return 0;
            }
            // Check if the receiver is a known account
            valid = 0;
            for (int i = 0; i < n; i++) {
                if (strcmp(entry2, accounts[i].accountNumber) == 0) {
                    valid = 1;
                    printf("You are sending money to %s\n",accounts[i].name);
                    printf("Enter amount to send: ");
                    scanf("%lf", &amount);

                    for (int j = 0; j < n; j++) {
                        if (strcmp(entry1, accounts[j].accountNumber) == 0) {
                            accounts[j].amount -= amount; // Deduct
                        }
                        if (strcmp(entry2, accounts[j].accountNumber) == 0) {
                            accounts[j].amount += amount; // Add
                        }
                    }
                    printf("Success...");
                    saveAccounts("accounts.dat");// Save changes
                    logs("logs.dat",entry1,"Sent",amount);
                    break;
                }
//Supposed to make the code display the receivers name

            }
            if (!valid) {
                printf("Receiver account not found.\n");
            }
            break;

        case 2:
            printf("Enter amount to withdraw: ");
            scanf("%lf", &amount);
            // Withdrawal logic here
            // Assuming the account has sufficient balance
            for (int i = 0; i < n; i++) {
                if (strcmp(entry1, accounts[i].accountNumber) == 0) {
                    accounts[i].amount -= amount; // Deduct from account
                    saveAccounts("accounts.dat"); // Save changes
                    loadAccounts("accounts.dat");
                    printf("Withdrawal successful.\nYour account balance is now %.0lf",accounts[i].amount);
                    break;
                }
                logs("logs.dat",entry1,"Withdrew",amount);
            }
            break;

        case 3:
            for (int i = 0; i < n; i++) {
                if (strcmp(entry1, accounts[i].accountNumber) == 0) {
                    printf("Your account balance is %.f\n", accounts[i].amount);
                    break;
                }
            }
            break;

        default:
            printf("Invalid Choice.\n");
            break;
    }

    //

    return 0;
}
