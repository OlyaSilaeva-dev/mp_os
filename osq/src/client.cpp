#include "client.h"

collection find_data_from_collection(search_tree<int, pull>* storage, int key_pull, int key_scheme, int key_collection) {
    pull _pull = storage->obtain(key_pull);
    scheme _scheme = _pull.get_from_pull(key_scheme);
    collection _collection = _scheme.get_from_scheme(key_collection);
    return _collection;
}

void file_processing(const std::string& path, search_tree<int, pull> *storage,
                     const std::function<int(int const &, int const &)>& keys_comparer,
                     allocator *allocator, logger *logger) {

    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        throw;
    }

    std::string operation, object;
    int key_pull, key_scheme, key_collection;

    std::string name;
    int key;

    while (fin >> operation) {
        if (operation == "add") { // add pull/scheme/collection/data
            fin >> object;

            if (object == "pull") { //key pull_name
                fin >> key >> name;
                search_tree<int, scheme> *tree = new b_tree<int, scheme>(24, keys_comparer, nullptr, logger);
                pull new_pull(name, tree);
                storage->insert(key, new_pull);

            } else if (object == "scheme") { //key_pull key scheme_name
                fin >> key_pull >> key >> name;
                search_tree<int, collection> *tree = new b_tree<int, collection>(24, keys_comparer, nullptr, logger);
                scheme new_scheme(name, tree);
                pull _pull = storage->obtain(key_pull);
                _pull.add_to_pull(key, new_scheme);

            } else if (object == "collection") { // key_pull key_scheme key_collection collection_name
                fin >> key_pull >> key_scheme >> key >> name;
                search_tree<int, std::string> *tree = new b_tree<int, std::string>(24, keys_comparer, nullptr, logger);
                collection new_collection(name, tree);
                pull _pull = storage->obtain(key_pull);
                scheme _scheme = _pull.get_from_pull(key_scheme);
                _scheme.add_to_scheme(key, new_collection);

            } else if (object == "data") { // key_pull key_scheme key_collection data_key data
                fin >> key_pull >> key_scheme >> key_collection >> key >> name;
                collection _collection = find_data_from_collection(storage, key_pull, key_scheme, key_collection);
                _collection.add_to_collection(key, name);
            }

        } else if (operation == "delete") { // delete pull/scheme/collection/data
            fin >> object;

            if (object == "pull") { // key_pull
                fin >> key;
                storage->dispose(key);

            } else if (object == "scheme") { // key_pull key_scheme
                fin >> key_pull >> key;
                pull _pull = storage->obtain(key_pull);
                _pull.remove_from_the_pull(key);

            } else if (object == "collection") { // key_pull key_scheme key_collection
                fin >> key_pull >> key_scheme >> key;
                pull _pull = storage->obtain(key_pull);
                scheme _scheme = _pull.get_from_pull(key_scheme);
                _scheme.remove_from_the_scheme(key);

            } else if (object == "data") { // key_pull key_scheme key_collection key_data
                fin >> key_pull >> key_scheme >> key_collection >> key;
                collection _collection = find_data_from_collection(storage, key_pull, key_scheme, key_collection);
                _collection.remove_from_the_collection(key);
            }

        } else if (operation == "read") { // read key_pull key_scheme key_collection key/range data/(data.begin data.end)
            fin >> key_pull >> key_scheme >> key_collection;
            collection _collection = find_data_from_collection(storage, key_pull, key_scheme, key_collection);
            std::string keys;
            fin >> keys;

            if (keys == "key") {
                fin >> key;
                std::string value = _collection.get_record_from_collection(key);
                std::cout << value << std::endl;
            } else {
                int minbound, maxbound;
                fin >> minbound >> maxbound;
                auto result = _collection.get_set_records_from_collection(minbound, maxbound);
                for (const auto &t: result) {
                    std::cout << t.key << ' ' << t.value << std::endl;
                }
            }

        } else if (operation == "change") { // change key_pull key_scheme key_collection key value
            std::string value;
            fin >> key_pull >> key_scheme >> key_collection >> key >> value;
            collection _collection = find_data_from_collection(storage, key_pull, key_scheme, key_collection);
            _collection.remove_from_the_collection(key);
            _collection.add_to_collection(key, value);
        }

    }
}

void dialogue_with_user(search_tree<int, pull> *storage,
                        const std::function<int(int const &, int const &)>& keys_comparer,
                        allocator *allocator, logger *logger) {
    while (true) {
        std::cout << "Choose an operation: \n";
        std::cout << "  a) add: \n";
        std::cout << "  d) delete: \n";
        std::cout << "  r) read: \n";
        std::cout << "  c) change: \n";
        std::cout << "  e) exit: \n";

        char c;
        std::cin >> c;
        switch (c) {
            case 'a':
                std::cout << "Object to add: \n";
                std::cout << "  p) pull: \n";
                std::cout << "  s) scheme: \n";
                std::cout << "  c) collection: \n";
                std::cout << "  r) record: \n";

            case 'd':

            case 'r':

            case 'c':

            case 'e':
                return;

            default:
                break;
        }

    }
}

