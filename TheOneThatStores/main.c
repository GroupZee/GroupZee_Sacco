#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
int MinBal=5000;
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

void loadingAccounts() {
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
    for (int i=0;i<7;i++) {
        printf("Account Number: %s, Name: %s, Balance: %.0f\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].amount);
    }
}


void logs(const char *accountNumber,const char *type,double amount){
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

double Sendcharge(double amount){
double t;
    if(amount>=2001 && amount<=99999){
    t=1000;
    }else if(amount>=100000 && amount<=999999){
    t=3800;
    }else if(amount>=1000000){
    t=5000;
    }
    return amount+t;

}
double WithdrawCharge(double amount){
int t;
    if(amount>=2001 && amount<=99999){
    t=900;
    }else if(amount>=100000 && amount<=999999){
    t=2800;
    }else if(amount>=1000000){
    t=4500;
    }
    return amount+t;
}
int CheckinguserAccount(char *entry1){
for (int i = 0; i < n; i++) {
    if (strcmp(entry1, accounts[i].accountNumber) == 0) {
        printf("Welcome %s.\n", accounts[i].name);
        return 1;
    }
}
    printf("You have entered an incorrect account number.\n");
    return 0;
}

int CheckingPin(char *entry1,char *entry3){
for (int i = 0; i < n; i++) {
    if (strcmp(entry3, accounts[i].pin) == 0 && strcmp(entry1,accounts[i].accountNumber)==0) {
    printf("1. Send Money\n2. Withdraw Money\n3. Check Your Account Balance.\n: ");
    return 1;
    }
}
    printf("Incorrect pin.\n");
    return 0;
}

int Confirm(){
int choice1;
printf("Are you sure you want to proceed with this transaction?\n1. Yes\n2. No\n");
scanf("%d",&choice1);
if(choice1==1){

return 1;
//saveAccounts();
}else if(choice1==2){
printf("\nPlease Wait...");
return 0;
}else{
printf("\nInvalid Input.\n");
}

}

int Withdrawing(double amount,char *entry1,int MinBal){
    for(int i=0;i<n;i++) {
        if(strcmp(entry1, accounts[i].accountNumber) == 0) {
            if((WithdrawCharge(amount)+MinBal) > accounts[i].amount) {
                printf("\nInsufficient Funds to complete transaction.\n");
                return 0;
            }
            if(Confirm()) {
                accounts[i].amount-=(WithdrawCharge(amount)+MinBal);
                printf("Withdrawal successful.\nYour account balance is now UGX.%.0lf\n",accounts[i].amount);
                return 0;
            }else {
                printf("Transaction canceled.\n");
            }
            break;
        }
    }
printf("\nAccount not found");
    return 0;
}

int Sending(char *entry1,char *entry2,double amount,int MinBal){
    int senderFound=0,receiverFound=0;
    // Locate the receiver
    for(int i=0;i<n;i++){
        if(strcmp(entry2,accounts[i].accountNumber)==0){
            receiverFound=1;
            break;
        }
    }
    if(!receiverFound){
        printf("Receiver account not found.\n");
        return -1;
    }
    for(int i=0;i<n;i++){
        if(strcmp(entry1,accounts[i].accountNumber)==0){
            senderFound=1;
            double fee=Sendcharge(amount);
            if((Sendcharge(amount)+MinBal)>accounts[i].amount){
                printf("Insufficient funds.\n");
                return 1;
            }
            if(Confirm()){
                accounts[i].amount-=(Sendcharge(amount));
                for(int j=0;j<n;j++){
                    if(strcmp(entry2,accounts[j].accountNumber)==0){
                        accounts[j].amount+=amount;
                        break;
                    }
                }
                printf("Transaction successful.\nYour account balance is now UGX.%.0lf\n", accounts[i].amount);
                return 0;
            } else{
                printf("Transaction canceled.\n");
                return 2;
            }
        }
    }
    if(!senderFound){
        printf("Sender account not found.\n");
    }
    return -1;
}

int CheckingBalance(char *entry1){
for (int i = 0; i < n; i++) {
    if (strcmp(entry1, accounts[i].accountNumber) == 0) {
        printf("Your account balance is UGX.%.0f\n", accounts[i].amount);
        break;
    }
}
}

void displayingGreeting(){
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);

    if(tm.tm_hour<12){
        printf("Good morning our dear customer\n");
    }else if(tm.tm_hour<17) {
        printf("Good afternoon our dear customer\n");
    }else{
        printf("Good evening our dear customer.\n");
    }
}

int main() {
    char entry1[13], entry2[13], entry3[5];
    double amount;
    int choice;
    int valid;

loadingAccounts();

displayingGreeting();
printf("Please enter your account number: ");
scanf("%12s",&entry1);
if(!CheckinguserAccount(entry1)){
    return 1;
}


printf("Enter your pin: ");
scanf("%4s", entry3);
if(!CheckingPin(entry1,entry3)){
    return 1;
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
            valid=0;
            for(int u=0;u<n;u++) {
                if (strcmp(entry2, accounts[u].accountNumber) == 0) {
                valid=1;
                printf("You are sending money to %s\n", accounts[u].name);
                break;
                }
            }
            if (!valid) {
            printf("Receiver account not found.\n");
            return 0;
            }
            printf("Enter amount to send: ");
            scanf("%.0lf",&amount);
            if (amount<=2000) {
            printf("You cannot send an amount below UGX.2000\n");
            return 0;
            }
            if (Sending(entry1,entry2,amount,MinBal)==0) {
            logs(entry1,"Sent",amount);
            return 0;
            } else {
            printf("Transaction failed.\n");
            }
            break;
        case 2:
            printf("Enter amount to withdraw: ");
            scanf("%lf", &amount);
            if (amount<=2000) {
            printf("Amount below required minimum.\n");
            return 1;
            }else{
            Withdrawing(amount,entry1,MinBal);
           }
           break;
        case 3:
            CheckingBalance(entry1);
            break;

        default:
            printf("Invalid Choice.\n");
            break;
    }

//displayAccounts();
saveAccounts();
    return 0;
}
