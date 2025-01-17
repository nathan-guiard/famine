#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BLOCK_SIZE 8

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

// Fonction de Feistel
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

// Chiffrement des données (en boucle sur les blocs complets uniquement)
void feistel_encrypt(uint8_t *data, size_t size, uint32_t key) {
    uint32_t left, right;

    // Chiffrer uniquement les blocs complets de taille BLOCK_SIZE
    size_t max_blocks = size / BLOCK_SIZE;
    for (size_t i = 0; i < max_blocks * BLOCK_SIZE; i += BLOCK_SIZE) {
        left = *(uint32_t *)(data + i);
        right = *(uint32_t *)(data + i + 4);

        feistel_encrypt_block(&left, &right, key);

        *(uint32_t *)(data + i) = left;
        *(uint32_t *)(data + i + 4) = right;
    }

    // Les octets restants (taille non divisible par BLOCK_SIZE) ne sont pas modifiés.
}

int main(int argc, char *argv[]) {
    uint32_t key;

    // Vérification des arguments
    if (argc == 2) {
        key = generate_random_key();
        printf("Clé générée : %08X\n", key);
    } else if (argc == 3) {
        key = 0x12345678;
    } else {
        fprintf(stderr, "Usage: %s <message> [key]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Récupération du message
    char *input = "123456789";
    size_t len = strlen(input);

    // Allocation de mémoire pour les données
    uint8_t *data = (uint8_t *)malloc(len);
    if (!data) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    memcpy(data, input, len);
    printf("Message original : %s\n", input);

    // Chiffrement
    feistel_encrypt(data, len, key);

    // Affichage du message chiffré
    printf("Message chiffré : ");
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    // Libération de la mémoire
    free(data);
    return EXIT_SUCCESS;
}
