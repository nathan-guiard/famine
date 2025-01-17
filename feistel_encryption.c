#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BLOCK_SIZE 8
#define MAX_MESSAGE_LEN 1024

// Génération d'une clé aléatoire
uint32_t generate_random_key() {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    uint32_t key;
    if (read(fd, &key, sizeof(key)) != sizeof(key)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return key;
}

// Fonction de mélange
uint32_t feistel_function(uint32_t half_block, uint32_t subkey) {
    return (half_block ^ subkey) + ((half_block << 3) | (half_block >> 29));
}

// Chiffrement d'un seul bloc
void feistel_encrypt_block(uint32_t *left, uint32_t *right, uint32_t key) {
    for (int round = 0; round < 8; round++) {
        uint32_t subkey = key ^ (round * 0x1234ABCD);
        uint32_t temp = *right;
        *right = *left ^ feistel_function(*right, subkey);
        *left = temp;
    }
}

// Déchiffrement d'un seul bloc
void feistel_decrypt_block(uint32_t *left, uint32_t *right, uint32_t key) {
    for (int round = 7; round >= 0; round--) {
        uint32_t subkey = key ^ (round * 0x1234ABCD);
        uint32_t temp = *left;
        *left = *right ^ feistel_function(*left, subkey);
        *right = temp;
    }
}

// Padding pour les blocs
size_t add_padding(uint8_t *message, size_t len, size_t block_size) {
    size_t padding_len = block_size - (len % block_size);
    for (size_t i = 0; i < padding_len; i++) {
        message[len + i] = (uint8_t)padding_len;
    }
    return len + padding_len;
}

// Suppression du padding après déchiffrement
size_t remove_padding(uint8_t *message, size_t len) {
    uint8_t padding_len = message[len - 1];
    return len - padding_len;
}

// Chiffrement d'un message complet
void feistel_encrypt_message(uint8_t *message, size_t len, uint32_t key) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        uint32_t *left = (uint32_t *)&message[i];
        uint32_t *right = (uint32_t *)&message[i + 4];
        feistel_encrypt_block(left, right, key);
    }
}

// Déchiffrement d'un message complet
void feistel_decrypt_message(uint8_t *message, size_t len, uint32_t key) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        uint32_t *left = (uint32_t *)&message[i];
        uint32_t *right = (uint32_t *)&message[i + 4];
        feistel_decrypt_block(left, right, key);
    }
}

int main(int argc, char *argv[]) {
    uint32_t key;
    
    // Vérification des arguments
    if (argc == 2) {
        key = generate_random_key();
        printf("Clé générée : %08X\n", key);
    } else if (argc == 3) {
        key = 0x12345678;
    }
    else {
        fprintf(stderr, "Usage: %s <message> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Récupération du message
    char *input = "123456789";

    // Préparation du message
    size_t len = strlen(input);
    if (len > MAX_MESSAGE_LEN) {
        fprintf(stderr, "Erreur: Le message est trop long. Taille maximale autorisée: %d\n", MAX_MESSAGE_LEN);
        return EXIT_FAILURE;
    }

    uint8_t message[MAX_MESSAGE_LEN] = {0}; // Tableau fixe pour stocker le message
    memcpy(message, input, len);
    
    // Padding du message
    size_t padded_len;//= add_padding(message, len, BLOCK_SIZE);

    printf("Message original : %s\n", input);

    // Chiffrement
    feistel_encrypt_message(message, len, key);
    printf("Message chiffré : ");
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", message[i]);
    }
    printf("\n");

    // Déchiffrement
    // feistel_decrypt_message(message, 8, key);
    // padded_len = remove_padding(message, padded_len);

    // printf("Message déchiffré : %.*s\n", (int)8, message);

    return 0;
}
