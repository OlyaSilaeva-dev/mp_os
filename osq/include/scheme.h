#ifndef MP_OS_SCHEME_H
#define MP_OS_SCHEME_H

#include <string>
#include <b_tree.h>
#include "collection.h"

class scheme {

private:
    std::string _scheme_name;
    search_tree<int, collection> *_data;

public:
    scheme(
            const scheme &other);

    scheme(
            std::string &scheme_name,
            search_tree<int, collection> *data);

    scheme &operator=(
            scheme const &other);

    scheme(
            scheme &&other) noexcept = default;

    scheme &operator=(
            scheme &&other) noexcept = default;

    ~scheme() = default;

    std::string get_scheme_name();

    scheme &add_to_scheme(int key, collection value);

    collection get_from_scheme(int key);

    scheme &remove_from_the_scheme(int key);
};


#endif //MP_OS_SCHEME_H
