#pragma once
#include <sodium.h>
#include <stdexcept>
#include <string>
#include <QDebug>

class Argon2id {
public: 
	Argon2id();
	~Argon2id();

	std::string hashPassword(const std::string& password);
	bool verifyPassword(const std::string& password, const std::string& hash);
	bool needsRehash(const std::string& hash);
};