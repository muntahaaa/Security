#include <iostream>
#include <string>

using namespace std;

string generateKey(string str, string key) {
    int x = str.length();

    for (int i = 0;; i++) {
        if (x == i)
            i = 0;
        if (key.length() == str.length())
            break;
        key += (key[i]);
    }
    return key;
}

string cipherText(string str, string key) {
    string cipher_text = "";

    for (int i = 0; i < str.length(); i++) {
        int x = (str[i] + key[i]) % 26;
        x += 'A';
        cipher_text += (char) (x);
    }
    return cipher_text;
}

string originalText(string cipher_text, string key) {
    string orig_text = "";

    for (int i = 0; i < cipher_text.length() && i < key.length(); i++) {
        int x = (cipher_text[i] - key[i] + 26) % 26;
        x += 'A';
        orig_text += (char) (x);
    }
    return orig_text;
}

string LowerToUpper(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (islower(s[i])) {
            s[i] = toupper(s[i]);
        }
    }
    return s;
}

int main() {
    string Str = "GoodMorning";
    string Keyword = "Monarchy";

    string str = LowerToUpper(Str);
    string keyword = LowerToUpper(Keyword);

    string key = generateKey(str, keyword);
    string cipher_text = cipherText(str, key);

    cout << "Ciphertext: " << cipher_text << endl;

    cout << "Original/Decrypted Text: "
         << originalText(cipher_text, key) << endl;

    return 0;
}
