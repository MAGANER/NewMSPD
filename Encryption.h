#pragma once
#include"rijndael.h"
#include"osrng.h"
#include<utility>
#include<string>

namespace Encryption
{
	using namespace CryptoPP;
	using namespace std;
	typedef pair<SecByteBlock, SecByteBlock> key_iv;

	key_iv get_random_key();
	string convert_bytes(const SecByteBlock& bytes);
	SecByteBlock convert_to_bytes(const string& key);
};