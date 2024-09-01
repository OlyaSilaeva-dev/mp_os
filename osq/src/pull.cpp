#include "pull.h"

pull::pull(const pull &other) : _pull_name(other._pull_name){
    this->_data = other._data;
}

pull::pull(std::string &pull_name, search_tree<int, scheme> *data) : _pull_name(pull_name){
    _data = data;
}

pull &pull::operator=(const pull &other) {
    if (this != &other) {
        _pull_name = other._pull_name;
        _data = other._data;
    }
    return  *this;
}

std::string pull::get_pull_name() {
    return this->_pull_name;
}

pull &pull::add_to_pull(int key, scheme value) {
    _data->insert(key, value);
    return *this;
}

scheme pull::get_from_pull(int key) {
    auto result = _data->obtain(key);
    return result;
}

pull &pull::remove_from_the_pull(int key) {
    _data->dispose(key);
    return *this;
}





