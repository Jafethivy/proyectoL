#include "argon2id.h"

Argon2id::Argon2id() {
    if (sodium_init() < 0) {
        throw std::runtime_error("No se pudo inicializar libsodium");
    }
}

Argon2id::~Argon2id() {}

std::string hashPassword(const std::string& password) {
    char hashed_password[crypto_pwhash_STRBYTES];

    // Opciones de seguridad:
    // - INTERACTIVE: ~0.1s, 64MB - Para login web
    // - MODERATE: ~0.7s, 256MB - Para apps de escritorio  
    // - SENSITIVE: ~3.5s, 1GB - Para datos críticos

    if (crypto_pwhash_str(
        hashed_password,
        password.c_str(),
        password.length(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,  // 2 iteraciones
        crypto_pwhash_MEMLIMIT_INTERACTIVE   // 64 MB
    ) != 0) {
        throw std::runtime_error("Error: memoria insuficiente para hashing");
    }

    // El resultado incluye: algoritmo, versión, memoria, iteraciones, salt, hash
    // Ejemplo: $argon2id$v=19$m=65536,t=3,p=4$...
    return std::string(hashed_password);
}

bool verifyPassword(const std::string& password, const std::string& hash) {
    // crypto_pwhash_str_verify extrae automáticamente los parámetros del hash
    int result = crypto_pwhash_str_verify(
        hash.c_str(),
        password.c_str(),
        password.length()
    );

    // Seguridad adicional: limpiar password de memoria después de usar
    sodium_memzero(const_cast<char*>(password.c_str()), password.length());

    return result == 0; // 0 = éxito, -1 = fallo
}

bool needsRehash(const std::string& hash) {
    return crypto_pwhash_str_needs_rehash(
        hash.c_str(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    ) == 1; // 1 = necesita rehash, 0 = está actualizado
}