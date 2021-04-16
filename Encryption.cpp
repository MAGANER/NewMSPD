#include"Encryption.h"

Encryption::key_iv Encryption::get_random_key()
{
	AutoSeededRandomPool rnd;

	SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());

	SecByteBlock iv(AES::BLOCKSIZE);
	rnd.GenerateBlock(iv, iv.size());
	
	return make_pair(key, iv);
}
Encryption::string Encryption::convert_bytes(const SecByteBlock& bytes)
{
	string curr = (const char*)bytes.data();
	string new_key;
	for (size_t i = 0;i<curr.size();++i)
	{
		auto ch = curr[i];
		int code = (int)ch;
		new_key += to_string(code)+'|';
	}
	return new_key;
}
Encryption::SecByteBlock Encryption::convert_to_bytes(const string& key)
{
	string bytes;

	string curr_ch;
	for (auto& ch : key)
	{
		if (ch != '|')curr_ch += ch;
		else
		{
			bytes += (char)atoi(curr_ch.c_str());
			curr_ch.clear();
		}
	}

	SecByteBlock _key(reinterpret_cast<CryptoPP::byte*>(&bytes[0]), AES::DEFAULT_KEYLENGTH);
	return _key;
}

std::string Encryption::encrypt(const key_iv& key_iv, const string& text)
{
	CFB_Mode<AES>::Encryption encryptor(key_iv.first, AES::DEFAULT_KEYLENGTH, key_iv.second);

	CryptoPP::byte* data = (CryptoPP::byte*)text.c_str();
	size_t data_len = strlen((const char*)data) + 1;

	encryptor.ProcessData(data, data, data_len);
	return string((const char*)data);
}
std::string Encryption::decrypt(const key_iv& key_iv, const string& text)
{
	CFB_Mode<AES>::Decryption decryptor(key_iv.first, AES::DEFAULT_KEYLENGTH, key_iv.second);

	CryptoPP::byte* data = (CryptoPP::byte*)text.c_str();
	size_t data_len = strlen((const char*)data) + 1;

	decryptor.ProcessData(data, data, data_len);
	return string((const char*)data);
}