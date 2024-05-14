#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "structures.h"

using namespace std;

void AddRoundKey(unsigned char *state, unsigned char *roundKey)
{
	for (int i = 0; i < 16; i++)
	{
		state[i] ^= roundKey[i];
	}
}

void SubBytes(unsigned char *state)
{
	for (int i = 0; i < 16; i++)
	{
		state[i] = s[state[i]];
	}
}

void ShiftRows(unsigned char *state)
{
	unsigned char tmp[16];

	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++)
	{
		state[i] = tmp[i];
	}
}

void MixColumns(unsigned char *state)
{
	unsigned char tmp[16];

	tmp[0] = (unsigned char)mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	tmp[1] = (unsigned char)state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	tmp[2] = (unsigned char)state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	tmp[3] = (unsigned char)mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];

	tmp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	tmp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	tmp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	tmp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];

	tmp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	tmp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	tmp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	tmp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];

	tmp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	tmp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	tmp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	tmp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];

	for (int i = 0; i < 16; i++)
	{
		state[i] = tmp[i];
	}
}

void Round(unsigned char *state, unsigned char *key)
{
	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, key);
}

void FinalRound(unsigned char *state, unsigned char *key)
{
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, key);
}

void generateCounter(unsigned char * counter, int length){
    srand(time(0));
    for(int i=0; i<length; i++){
        counter[i] = (unsigned char) rand() % 256;
    }
}

void AESEncrypt(unsigned char *message, unsigned char *expandedKey, unsigned char *encryptedMessage,
unsigned char *CounterMessage)
{
	unsigned char state[16]; // Stores the first 16 bytes of original message

	for (int i = 0; i < 16; i++)
	{
		state[i] = CounterMessage[i];
	}

	int numberOfRounds = 9;

	AddRoundKey(state, expandedKey); // Initial round

	for (int i = 0; i < numberOfRounds; i++)
	{
		Round(state, expandedKey + (16 * (i + 1)));
	}

	FinalRound(state, expandedKey + 160);

	for (int i = 0; i < 16; i++)
	{
		encryptedMessage[i] = state[i] ^ message[i];
	}
}

void storeEncryptedMessageToFile(const char *filename, const unsigned char *encryptedMessage, int paddedMessageLen)
{

	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile)
	{
		std::cerr << "Error: Failed to open file for writing." << std::endl;
		return;
	}

	outFile << "Encrypted message in hex:" << std::endl;
	for (int i = 0; i < paddedMessageLen; i++)
	{
		outFile << std::hex << static_cast<int>(encryptedMessage[i]) << " ";
	}

	outFile.close();

	std::cout << "Encrypted message has been stored in the file: " << filename << std::endl;
}

void mainAES()
{

	char message[1024];
	const char *inputFilename = "input.txt";

	ifstream infiles(inputFilename);
	if (!infiles.is_open())
	{
		std::cerr << "Error: Failed to open input file." << std::endl;
		return ;
	}

	infiles.getline(message, sizeof(message));
	cout << message << endl;

	// Pad message to 16 bytes=128 bits
	int originalLen = strlen((const char *)message);

	int paddedMessageLen = originalLen;

	if ((paddedMessageLen % 16) != 0)
	{
		paddedMessageLen = ((paddedMessageLen / 16) + 1) * 16;
	}
    
    unsigned char *counter= new unsigned char[paddedMessageLen];
    generateCounter(counter,paddedMessageLen);
	unsigned char *paddedMessage = new unsigned char[paddedMessageLen];
	for (int i = 0; i < paddedMessageLen; i++)
	{
		if (i >= originalLen)
		{
			paddedMessage[i] = 0;
		}
		else
		{
			paddedMessage[i] = message[i];
		}
	}

	unsigned char *encryptedMessage = new unsigned char[paddedMessageLen];

	string str;
	ifstream infile;
	infile.open("keyfile", ios::in | ios::binary);

	if (infile.is_open())
	{
		getline(infile, str); // The first line of file should be the key
		infile.close();
	}

	else
		cout << "Unable to open file";

	istringstream hex_chars_stream(str);
	unsigned char key[16];
	int i = 0;
	unsigned int c;
	while (hex_chars_stream >> hex >> c) // store key in hexadecimal format
	{
		key[i] = c;
		i++;
	}

	unsigned char expandedKey[176];

	KeyExpansion(key, expandedKey);

	for (int i = 0; i < paddedMessageLen; i += 16)
	{
		AESEncrypt(paddedMessage + i, expandedKey, encryptedMessage + i,counter+i);
	}
	 cout << "Encrypted message :" << endl;
	for (int i = 0; i < paddedMessageLen; i++)
	{
		cout << encryptedMessage[i];
		cout << " ";
	}

	cout << "\nEncrypted message in hex:" << endl;
	for (int i = 0; i < paddedMessageLen; i++)
	{
		cout << hex << (int)encryptedMessage[i];
		cout << " ";
	}

	cout << endl;

	const char *filename = "encrypted_message.txt";
	storeEncryptedMessageToFile(filename, encryptedMessage, paddedMessageLen);

	// Write the encrypted string out to file "message.aes"
	ofstream outfile;
	outfile.open("message.aes", ios::out | ios::binary);
	if (outfile.is_open())
	{
		outfile.write(reinterpret_cast<const char *>(encryptedMessage), sizeof(encryptedMessage));
		outfile.close();
	}

	else
		cout << "Unable to open file";

	// Free memory
	delete[] paddedMessage;
	delete[] encryptedMessage;

	return ;
}