#pragma once
#include "wchar.h"

int get_path(wchar_t *file_path, wchar_t *directory_path);

int create_directory(const wchar_t *directory_path);

int write_file(const wchar_t *file_path);

int read_file(const wchar_t *file_path);
