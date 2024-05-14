#include <iostream>
#include <string>

using namespace std;

string decrypt(string msg, int shift) {
    string decryptedMsg = "";

    for (int i = 0; i < msg.length(); i++) {
        char c = msg[i];

        if (isalpha(c) && islower(c)) {
            c = ((c - 'a' - shift + 26) % 26) + 'a';
        }
        else if (isalpha(c) && isupper(c)) {
            c = ((c - 'A' - shift + 26) % 26) + 'A';
        }

        decryptedMsg += c;
    }

    return decryptedMsg;
}

int main() {
    string msg;
    int shift;

    cout << "Enter encrypted message: ";
    getline(cin, msg);

    cout << "Enter shift: ";
    cin >> shift;

    cout << "Decrypted Message: " << decrypt(msg, shift) << endl;

    return 0;
}
