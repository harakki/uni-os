#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Elf64_Shdr get_section_header(FILE* file, Elf64_Ehdr* header, int index);

int main(int argc, char** argv) {
  const char* elf_file;
  if (argc > 1) {
    elf_file = argv[1];
  } else {
    fprintf(stderr, "Usage: provide path to ELF file as program argument.\n");
    return 1;
  }

  char sname[32];

  Elf64_Ehdr header;
  Elf64_Shdr sheader;
  Elf64_Shdr symtab;
  Elf64_Shdr strtab;
  Elf64_Sym sym;

  FILE* file = fopen(elf_file, "rb");

  // Чтение заголовка ELF-файла
  fread(&header, sizeof(header), 1, file);

  for (int i = 0; i < header.e_shnum; i++) {
    Elf64_Shdr sheader = get_section_header(file, &header, i);

    if (i == 4) {
      symtab = (Elf64_Shdr)sheader;
    }
    if (i == 5) {
      strtab = (Elf64_Shdr)sheader;
    }
  }

  // Check if symbols table is empty
  if (symtab.sh_size == 0 || symtab.sh_entsize == 0) {
    fclose(file);
    return 0;
  }

  for (int i = 0; i < symtab.sh_size / symtab.sh_entsize; i++) {
    fseek(file, symtab.sh_offset + symtab.sh_entsize * i, SEEK_SET);
    fread(&sym, sizeof(Elf64_Sym), 1, file);

    // Check if symbol type is functions
    if (ELF64_ST_TYPE(sym.st_info) == STT_FUNC && sym.st_name != 0) {
      fseek(file, strtab.sh_offset + sym.st_name, SEEK_SET);
      fread(sname, 1, 32, file);

      // Print only defined by developer functions
      if (sym.st_shndx != SHN_UNDEF) {
        fprintf(stdout, "%s\n", sname);
      }
    }
  }
  return 0;
}

Elf64_Shdr get_section_header(FILE* file, Elf64_Ehdr* header, int index) {
  Elf64_Shdr sheader;
  fseek(file, header->e_shoff + header->e_shentsize * index, SEEK_SET);
  fread(&sheader, sizeof(Elf64_Shdr), 1, file);
  return sheader;
}
