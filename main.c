#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define FILE_NAME "users.txt"

struct User {
    char cardno[20];
    char name[20];
    int pin;
    unsigned long balance;
};

// Tools
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Functions
void create_account();
int login(struct User *loggedUser);
void menu(struct User *user);
int load_user(const char *cardno, struct User *user);
void update_user(struct User *user);
void transfer_money(struct User *sender);
void show_balance(struct User *user);

int main() {
    int choice;
    struct User current;

    while (1) {
        system("cls");
        set_color(11);
        gotoxy(30, 3);
        printf("=== Moroccan Bank System ===");
        set_color(15);
        gotoxy(30, 6);
        printf("1. Create Account");
        gotoxy(30, 7);
        printf("2. Login");
        gotoxy(30, 8);
        printf("3. Exit");
        gotoxy(30, 10);
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                if (login(&current)) {
                    menu(&current);
                }
                break;
            case 3:
                system("cls");
                gotoxy(30, 10);
                printf("Goodbye.");
                exit(0);
            default:
                gotoxy(30, 12);
                printf("Invalid choice. Try again.");
                getch();
        }
    }
    return 0;
}

void create_account() {
    struct User u;
    FILE *fp = fopen(FILE_NAME, "a");

    if (!fp) {
        printf("Could not open file.\n");
        return;
    }

    system("cls");
    gotoxy(30, 3); set_color(11);
    printf("=== Create Account ===");
    set_color(15);

    gotoxy(30, 6); printf("Enter card number: ");
    scanf("%s", u.cardno);
    gotoxy(30, 7); printf("Enter name: ");
    scanf("%s", u.name);
    gotoxy(30, 8); printf("Set PIN: ");
    scanf("%d", &u.pin);
    gotoxy(30, 9); printf("Initial deposit (DH): ");
    scanf("%lu", &u.balance);

    fprintf(fp, "%s %s %d %lu\n", u.cardno, u.name, u.pin, u.balance);
    fclose(fp);

    gotoxy(30, 11); set_color(10);
    printf("Account created successfully!");
    set_color(15);
    gotoxy(30, 13); printf("Press any key to continue...");
    getch();
}

int login(struct User *loggedUser) {
    char input_card[20];
    int pin, attempts = 0;

    system("cls");
    gotoxy(30, 3); set_color(11);
    printf("=== Login ===");
    set_color(15);
    gotoxy(30, 6); printf("Enter your card number: ");
    scanf("%s", input_card);

    if (!load_user(input_card, loggedUser)) {
        gotoxy(30, 8); set_color(12);
        printf("Account not found.");
        set_color(15);
        gotoxy(30, 10); printf("Press any key to return...");
        getch();
        return 0;
    }

    while (attempts < 3) {
        gotoxy(30, 8); printf("Enter your PIN: ");
        scanf("%d", &pin);
        if (pin == loggedUser->pin) {
            gotoxy(30, 10); set_color(10);
            printf("Login successful. Welcome, %s!", loggedUser->name);
            set_color(15);
            getch();
            return 1;
        } else {
            gotoxy(30, 10); set_color(12);
            printf("Incorrect PIN.");
            set_color(15);
            attempts++;
            getch();
        }
    }

    gotoxy(30, 12); printf("Too many failed attempts.");
    gotoxy(30, 13); printf("Press any key to return...");
    getch();
    return 0;
}

int load_user(const char *cardno, struct User *user) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 0;

    while (fscanf(fp, "%s %s %d %lu", user->cardno, user->name, &user->pin, &user->balance) == 4) {
        if (strcmp(user->cardno, cardno) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void update_user(struct User *user) {
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct User u;

    while (fscanf(fp, "%s %s %d %lu", u.cardno, u.name, &u.pin, &u.balance) == 4) {
        if (strcmp(u.cardno, user->cardno) == 0) {
            fprintf(temp, "%s %s %d %lu\n", user->cardno, user->name, user->pin, user->balance);
        } else {
            fprintf(temp, "%s %s %d %lu\n", u.cardno, u.name, u.pin, u.balance);
        }
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
}

void show_balance(struct User *user) {
    system("cls");
    gotoxy(30, 4); set_color(11);
    printf("=== Account Balance ===");
    set_color(15);
    gotoxy(30, 6); printf("Name: %s", user->name);
    gotoxy(30, 7); printf("Card: %s", user->cardno);
    gotoxy(30, 9); set_color(10);
    printf("Current Balance: %lu DH", user->balance);
    set_color(15);
    gotoxy(30, 11); printf("Press any key to return...");
    getch();
}

void menu(struct User *user) {
    int choice;
    unsigned long amount;

    while (1) {
        system("cls");
        set_color(11);
        gotoxy(30, 3); printf("=== Welcome, %s ===", user->name);
        set_color(15);
        gotoxy(30, 6); printf("1. Check Balance");
        gotoxy(30, 7); printf("2. Deposit");
        gotoxy(30, 8); printf("3. Withdraw");
        gotoxy(30, 9); printf("4. Transfer Money");
        gotoxy(30, 10); printf("5. Logout");
        gotoxy(30, 12); printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: show_balance(user); break;
            case 2:
                gotoxy(30, 14); printf("Enter amount to deposit: ");
                scanf("%lu", &amount);
                user->balance += amount;
                update_user(user);
                gotoxy(30, 16); set_color(10);
                printf("Deposit successful.");
                set_color(15);
                getch();
                break;
            case 3:
                gotoxy(30, 14); printf("Enter amount to withdraw (min 100 DH): ");
                scanf("%lu", &amount);
                if (amount < 100) {
                    gotoxy(30, 16); set_color(12);
                    printf("Minimum withdrawal is 100 DH.");
                } else if (amount > user->balance) {
                    gotoxy(30, 16); set_color(12);
                    printf("Insufficient funds.");
                } else {
                    user->balance -= amount;
                    update_user(user);
                    gotoxy(30, 16); set_color(10);
                    printf("Withdrawal successful.");
                }
                set_color(15);
                getch();
                break;
            case 4:
                transfer_money(user);
                break;
            case 5:
                return;
            default:
                gotoxy(30, 14);
                printf("Invalid choice.");
                getch();
        }
    }
}

void transfer_money(struct User *sender) {
    char receiver_card[20];
    unsigned long amount;
    struct User receiver;

    system("cls");
    gotoxy(30, 3); set_color(11);
    printf("=== Transfer Money ===");
    set_color(15);
    gotoxy(30, 6); printf("Receiver's card number: ");
    scanf("%s", receiver_card);

    if (strcmp(receiver_card, sender->cardno) == 0) {
        gotoxy(30, 8); set_color(12);
        printf("Cannot transfer to yourself.");
        set_color(15);
        getch();
        return;
    }

    if (!load_user(receiver_card, &receiver)) {
        gotoxy(30, 8); set_color(12);
        printf("Receiver not found.");
        set_color(15);
        getch();
        return;
    }

    gotoxy(30, 8); printf("Amount to transfer: ");
    scanf("%lu", &amount);

    if (amount > sender->balance) {
        gotoxy(30, 10); set_color(12);
        printf("Insufficient balance.");
    } else {
        sender->balance -= amount;
        receiver.balance += amount;
        update_user(sender);
        update_user(&receiver);
        gotoxy(30, 10); set_color(10);
        printf("Transfer successful.");
        gotoxy(30, 11); printf("New balance: %lu DH", sender->balance);
    }
    set_color(15);
    gotoxy(30, 13); printf("Press any key to return...");
    getch();
}
