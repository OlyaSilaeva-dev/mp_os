#include "collection.h"

collection::collection(
        collection const &other) : _collection_name(other._collection_name){
    _data = other._data;
}

collection::collection(
        std::string &collection_name,
        search_tree<int, std::string> *data) : _collection_name(collection_name){
    _data = data;
}

collection &collection::operator=(collection const &other) {
    if (this != &other) {
        _collection_name = other._collection_name;
        _data = other._data;
    }
    return *this;
}

std::string collection::get_collection_name() {
    return _collection_name;
}

collection &collection::add_to_collection(int key, std::string value) {
    this->_data->insert(key, value);
    return *this;
}

collection &collection::remove_from_the_collection(int key) {
    this->_data->dispose(key);
    return *this;
}

std::basic_string<char> collection::get_record_from_collection(int key) {
    return (std::string)(_data->obtain(key));
}

std::vector<typename associative_container<int, std::string>::key_value_pair> collection::get_set_records_from_collection(int minbound, int maxbound) {
    auto result = (std::vector<typename associative_container<int, std::string>::key_value_pair>)(_data->obtain_between(minbound, maxbound,
                                                                                                                        true,
                                                                                                                        true));
    return result;
}











