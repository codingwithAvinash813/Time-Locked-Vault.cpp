#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <iomanip>

using namespace std;

struct Vault {
    string username;
    string password;
    string secret;
    time_t unlockTime;
};

void printBanner() {
    cout << "\n===================================" << endl;
    cout << "      🔐 Time-Locked Vault      " << endl;
    cout << "===================================\n" << endl;
}

void saveVault(Vault &vault) {
    ofstream file("vault.dat", ios::binary);
    if (file) {
        file.write(reinterpret_cast<char*>(&vault), sizeof(Vault));
        file.close();
    }
}

bool loadVault(Vault &vault) {
    ifstream file("vault.dat", ios::binary);
    if (file) {
        file.read(reinterpret_cast<char*>(&vault), sizeof(Vault));
        file.close();
        return true;
    }
    return false;
}

void createVault() {
    printBanner();
    Vault vault;
    cout << "\nEnter username: ";
    cin >> vault.username;
    cout << "Enter password: ";
    cin >> vault.password;
    cin.ignore();
    cout << "Enter secret message: ";
    getline(cin, vault.secret);
    int lockTime;
    cout << "Set unlock time (in seconds): ";
    cin >> lockTime;
    vault.unlockTime = time(0) + lockTime;
    saveVault(vault);
    cout << "\n✅ Vault created successfully! It will unlock after " << lockTime << " seconds.\n";
}

void accessVault() {
    printBanner();
    Vault vault;
    if (!loadVault(vault)) {
        cout << "\n❌ No vault found.\n";
        return;
    }
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    if (username == vault.username && password == vault.password) {
        time_t currentTime = time(0);
        if (currentTime >= vault.unlockTime) {
            cout << "\n🔓 Vault unlocked! Your secret message: " << vault.secret << "\n";
        } else {
            int remainingTime = vault.unlockTime - currentTime;
            cout << "\n⏳ Vault is still locked! Try again in " << remainingTime << " seconds.\n";
        }
    } else {
        cout << "\n🚫 Invalid credentials! Access denied.\n";
    }
}

int main() {
    int choice;
    while (true) {
        printBanner();
        cout << "1️⃣  Create Vault" << endl;
        cout << "2️⃣  Access Vault" << endl;
        cout << "3️⃣  Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: createVault(); break;
            case 2: accessVault(); break;
            case 3: cout << "\n👋 Exiting... Stay safe!\n"; return 0;
            default: cout << "\n❌ Invalid choice! Try again.\n";
        }
        sleep(2);
    }
    return 0;
}
