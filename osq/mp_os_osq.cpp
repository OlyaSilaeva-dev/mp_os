#include "client.h"
#include <client_logger_builder.h>

namespace comparison {
    class int_comparer final {

    public:
        int operator()(
                int const &left,
                int const &right) const noexcept {
            return left - right;
        }

    };

    class stdstring_comparer final {

    public:
        int operator()(
                std::string const &first,
                std::string const &second) const noexcept {
            if (first == second) {
                return 0;
            }
            if (first > second) {
                return 1;
            }
            return -1;
        }
    };

    class ac_kvp_int_stdstring_comparer final {

    public:
        int operator()(
                typename associative_container<int, std::string>::key_value_pair const &first,
                typename associative_container<int, std::string>::key_value_pair const &second) {
            auto keys_comparison_result = int_comparer()(first.key, second.key);
            if (keys_comparison_result != 0) return keys_comparison_result;
            return stdstring_comparer()(first.value, second.value);
        }
    };
}

template<
        typename tkey,
        typename tvalue>
bool compare_results(
        std::vector<typename search_tree<tkey, tvalue>::key_value_pair> const &expected,
        std::vector<typename search_tree<tkey, tvalue>::key_value_pair> const &actual,
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        std::function<int(tvalue const &, tvalue const &)> values_comparer) {
    if (expected.size() != actual.size()) return false;

    for (size_t i = 0; i < expected.size(); ++i) {
        if (keys_comparer(expected[i].key, actual[i].key) != 0) {
            return false;
        }
        if (values_comparer(expected[i].value, actual[i].value) != 0) {
            return false;
        }
    }

    return true;
}

template<
        typename tvalue>
bool compare_obtain_results(
        std::vector<tvalue> const &expected,
        std::vector<tvalue> const &actual,
        std::function<int(tvalue const &, tvalue const &)> values_comparer) {
    if (expected.size() != actual.size()) return false;

    for (size_t i = 0; i < expected.size(); ++i) {
        if (values_comparer(expected[i], actual[i]) != 0) return false;
    }

    return true;
}

logger *create_logger(
        std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
        bool use_console_stream = true,
        logger::severity console_stream_severity = logger::severity::debug) {
    return client_logger_builder().build();

    logger_builder *builder = new client_logger_builder();

    if (use_console_stream) {
        builder->add_console_stream(console_stream_severity);
    }

    for (auto &output_file_stream_setup: output_file_streams_setup) {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }

    logger *built_logger = builder->build();

    delete builder;

    return built_logger;
}

int main(int argc, char* argv[]) {
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>> {
            {"osq_tests_logs.txt", logger::severity::trace}
    });

    logger->trace("testOsq started");

    search_tree<int, pull> *storage = new b_tree<int, pull>(24, keys_comparer, nullptr, logger);

//
//    std::string my_col = "my_collection";
//    auto _collect = new collection(my_col, tree);
//
//    _collect->add_to_collection(1, "hello");

    std::string file = "C:/src/GitHub/mp_os/osq/test.txt";
    file_processing(file, storage, keys_comparer, nullptr, nullptr);
}