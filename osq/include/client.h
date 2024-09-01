#ifndef MP_OS_CLIENT_H
#define MP_OS_CLIENT_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include "pull.h"

void file_processing(const std::string& path, search_tree<int, pull> *storage,
                     const std::function<int(int const &, int const &)>& keys_comparer,
                     allocator *allocator, logger *logger);

void dialogue_with_user();

#endif //MP_OS_CLIENT_H
