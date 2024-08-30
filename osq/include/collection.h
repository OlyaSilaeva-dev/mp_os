//
// Created by olya- on 30.08.2024.
//

#ifndef MP_OS_COLLECTION_H
#define MP_OS_COLLECTION_H

#include <string>
#include <b_tree.h>

class collection {

private:
    std::string _collection_name;
    search_tree<int, std::string> *_data;

public:
    collection(
            std::string collection_name,
            search_tree<int, std::string> *data);

    collection &operator=(
            collection const &other);

    collection(
            collection &&other) noexcept = default;

    collection &operator=(
            collection &&other) noexcept = default;

    ~collection() = default;

    std::string get_collection_name();

    collection &add_to_collection(int key, std::string value);

    std::basic_string<char> get_from_collection(int key);

    collection &remove_from_the_collection(int key);
};


#endif //MP_OS_COLLECTION_H
