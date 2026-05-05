#define MAX_NAME_SIZE 32
#include <Shlobj.h>
#include "file.h"
#include <pathcch.h>
#include <stdint.h>
#include <stdio.h>

enum { NUMBER_OF_PEOPLE = 3, NUMBER_OF_PERSON_MEMBERS = 2 };

typedef struct {
  uint16_t age;
  wchar_t first_name[MAX_NAME_SIZE];
} Person;

int get_path(wchar_t *file_path, wchar_t *directory_path) {
  PWSTR wptr = NULL;
  const KNOWNFOLDERID id = FOLDERID_LocalAppData;
  const wchar_t directory_name[] = L"DefaultCompanyName";
  const wchar_t file_name[] = L"data.txt";
  HRESULT result;
  result = SHGetKnownFolderPath(&id, 0, NULL, &wptr);
  if (FAILED(result)) {
    fwprintf(stdout, L"Error: Failed to get known folder\n");
    return -1;
  }
  result = PathCchCombineEx(directory_path, MAX_PATH, wptr, directory_name,
                            PATHCCH_NONE);
  if (FAILED(result)) {
    fwprintf(stdout, L"Error: Failed to combine directory path\n");
    goto CLEANUP;
  }
  result = PathCchCombineEx(file_path, MAX_PATH, directory_path, file_name,
                            PATHCCH_NONE);
  if (FAILED(result)) {
    fwprintf(stdout, L"Error: Failed to combine file path\n");
    goto CLEANUP;
  }

  if (wptr) {
    CoTaskMemFree(wptr);
  }
  return 0;
CLEANUP:
  if (wptr) {
    CoTaskMemFree(wptr);
  }
  return -1;
}

int create_directory(const wchar_t *directory_path) {
  if (CreateDirectoryW(directory_path, NULL) == 0) {
    DWORD err = GetLastError();
    if (err != ERROR_ALREADY_EXISTS) {
      fwprintf(stdout, L"Error: Failed to create directory\n");
      return -1;
    }
  }
  return 0;
}

int write_file(const wchar_t *file_path) {
  const Person people[NUMBER_OF_PEOPLE] = {{.first_name = L"Billy", .age = 20},
                                           {.first_name = L"Tommy", .age = 99},
                                           {.first_name = L"Larry", .age = 5}};
  FILE *file = _wfopen(file_path, L"w,ccs=UTF-16LE");
  if (!file) {
    fwprintf(stdout, L"Error: Failed to open file\n");
    return -1;
  }
  for (int x = 0; x < NUMBER_OF_PEOPLE; x++) {
    if (fwprintf(file, L"%u %ls\n", people[x].age, people[x].first_name) < 0) {
      fwprintf(stdout, L"Error: Failed to print to file\n");
      fclose(file);
      return -1;
    }
  }
  fclose(file);
  return 0;
}

int read_file(const wchar_t *file_path) {
  Person people[NUMBER_OF_PEOPLE] = {0};
  FILE *file = _wfopen(file_path, L"r,ccs=UTF-16LE");
  if (!file) {
    fwprintf(stdout, L"Error: Failed to open file\n");
    return -1;
  }

  for (int x = 0; x < NUMBER_OF_PEOPLE; x++) {
    if (fwscanf(file, L"%u %31ls", &people[x].age, people[x].first_name) !=
        NUMBER_OF_PERSON_MEMBERS) {
      fwprintf(stdout, L"Error: Failed to read from file\n");
      goto CLEANUP;
    }
  }
  for (int x = 0; x < NUMBER_OF_PEOPLE; x++) {
    if (fwprintf(stdout, L"%i: name: %ls, age: %u\n", x + 1,
                 people[x].first_name, people[x].age) < 0) {
      fwprintf(stdout, L"Error: Failed to print People Data\n");
      goto CLEANUP;
    }
  }
  fclose(file);
  return 0;
CLEANUP:
  fclose(file);
  return -1;
}
