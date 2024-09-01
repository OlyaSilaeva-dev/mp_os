#ifndef MP_OS_PULL_H
#define MP_OS_PULL_H

#include <string>
#include "b_tree.h"
#include "scheme.h"

class pull {

private:
    std::string _pull_name;
    search_tree<int, scheme> *_data;

public:
    pull(const pull &_pull);

    pull(
            std::string &pull_name,
    search_tree<int, scheme> *data);

    pull &operator=(
            pull const &other);

    pull(
            pull &&other) noexcept = default;

    pull &operator=(
            pull &&other) noexcept = default;

    ~pull() = default;

    std::string get_pull_name();

    pull &add_to_pull(int key, scheme value);

    scheme get_from_pull(int key);

    pull &remove_from_the_pull(int key);
};


#endif //MP_OS_PULL_H
