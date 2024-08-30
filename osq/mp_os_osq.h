#ifndef MP_OS_MP_OS_OSQ_H
#define MP_OS_MP_OS_OSQ_H

#include "b_tree.h"

#include <string>


class pull {

private:
    class scheme {

    private:
        class collection {
            std::string _collection_name;
            b_tree<uint32_t, std::string> _data;
        };

        std::string _scheme_name;
        b_tree<uint32_t, collection> _collections;
    };

    std::string _pull_name;
    b_tree<uint32_t, scheme> _schemes;


};





#endif //MP_OS_MP_OS_OSQ_H
