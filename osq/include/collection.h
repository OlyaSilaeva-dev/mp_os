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
            collection const &other);

    collection(
            std::string &collection_name,
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

    std::basic_string<char> get_record_from_collection(int key);

    std::vector<typename associative_container<int, std::string>::key_value_pair> get_set_records_from_collection(int minbound, int maxbound);

    collection &remove_from_the_collection(int key);
};


#endif //MP_OS_COLLECTION_H
