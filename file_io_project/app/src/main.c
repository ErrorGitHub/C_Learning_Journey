#include <Objbase.h>
#include <Shlobj.h>
#include <fileapi.h>
#include <pathcch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <winerror.h>
#include <winnt.h>

enum { MAX_PATH_SIZE = MAX_PATH, MAX_NAME_SIZE = 20, NAME_COUNT = 5 };

static int path_creation(wchar_t *path) {
  PWSTR wptr = NULL;
  const wchar_t *directory = L"DefaultCompanyName";
  const wchar_t *filename = L"data.txt";
  KNOWNFOLDERID id = FOLDERID_LocalAppData;
  wchar_t dir[MAX_PATH_SIZE];
  DWORD dw;
  HRESULT hr;

  hr = SHGetKnownFolderPath(&id, 0, NULL, &wptr);

  if (FAILED(hr)) {
    wprintf_s(L"Get Known folder failed: 0x%08lx\n", hr);
    goto EXIT;
  };

  hr = PathCchCombineEx(dir, MAX_PATH_SIZE, wptr, directory, PATHCCH_NONE);
  if (FAILED(hr)) {
    wprintf_s(L"Path combine failed: 0x%08lx\n", hr);
    goto EXIT;
  }

  if (!CreateDirectoryW(dir, NULL)) {
    dw = GetLastError();
    if (dw != ERROR_ALREADY_EXISTS) {
      wprintf_s(L"Create Directory failed: %lu\n", dw);
      goto EXIT;
    }
  }

  hr = PathCchCombineEx(path, MAX_PATH_SIZE, dir, filename, PATHCCH_NONE);
  if (FAILED(hr)) {
    wprintf_s(L"Path combine failed: 0x%08lx\n", hr);
    goto EXIT;
  }

  if (wptr) {
    CoTaskMemFree(wptr);
  }
  return 0;

EXIT:
  if (wptr) {
    CoTaskMemFree(wptr);
  }
  return -1;
}

static int write_to_file(wchar_t *path) {
  wchar_t name[MAX_NAME_SIZE];
  FILE *file = _wfopen(path, L"w");

  if (!file) {
    wprintf_s(L"Failed to open file\n");
    return -1;
  }

  wprintf_s(L"Please enter 5 names:\n");

  for (int x = 0; x < NAME_COUNT; x++) {
    wprintf_s(L"%i: ", x + 1);
    if (wscanf_s(L"%19ls", name, (unsigned)_countof(name)) != 1) {
      wprintf_s(L"Input error\n");
      fclose(file);
      return -1;
    }
    if (fwprintf(file, L"%ls\n", name) < 0) {
      wprintf_s(L"Failed to write to file\n");
      fclose(file);
      return -1;
    }
  }

  _putws(L"");
  fclose(file);
  return 0;
}

static int read_from_file(wchar_t *path) {
  FILE *file = _wfopen(path, L"r");
  int counter = 1;

  if (!file) {
    wprintf_s(L"Failed to open file\n");
    return -1;
  }

  wchar_t name[MAX_NAME_SIZE];

  wprintf_s(L"Names read from file:\n");
  while (fgetws(name, MAX_NAME_SIZE, file)) {
    wprintf_s(L"%i: %ls", counter, name);
    counter++;
  }

  fclose(file);
  return 0;
}

int main(void) {
  wchar_t result[MAX_PATH_SIZE];

  if (path_creation(result) != 0) {
    return -1;
  }

  if (write_to_file(result) != 0) {
    return -1;
  }

  if (read_from_file(result) != 0) {
    return -1;
  }

  return 0;
}
