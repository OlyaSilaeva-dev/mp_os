//
// Created by olya- on 31.08.2024.
//

#include "scheme.h"

scheme::scheme(const scheme &other) : _scheme_name(other._scheme_name) {
    _data = other._data;
}

scheme::scheme(std::string &scheme_name, search_tree<int, collection> *data) : _scheme_name(scheme_name){
    _data = data;
}

scheme &scheme::operator=(const scheme &other) {
    if (this != &other) {
        this->_scheme_name = other._scheme_name;
        this->_data = other._data;
    }
    return *this;
}

std::string scheme::get_scheme_name() {
    return _scheme_name;
}

scheme &scheme::add_to_scheme(int key, collection value) {
    _data->insert(key,value);
    return *this;
}

collection scheme::get_from_scheme(int key) {
    auto result = _data->obtain(key);
    return result;
}

scheme &scheme::remove_from_the_scheme(int key) {
    _data->dispose(key);
    return *this;
}





