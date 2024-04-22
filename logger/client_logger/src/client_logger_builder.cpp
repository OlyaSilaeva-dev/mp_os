#include "client_logger_builder.h"
#include "client_logger.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include "nlohmann/json.hpp"

client_logger_builder::client_logger_builder() {}

client_logger_builder::client_logger_builder(const client_logger_builder &other) : _streams(other._streams) {}

client_logger_builder &client_logger_builder::operator=(const client_logger_builder &other)
        {
    if (this != &other) this->_streams = other._streams;
    return *this;
}

client_logger_builder::client_logger_builder(client_logger_builder &&other) noexcept : _streams(std::move(other._streams)) {}

client_logger_builder &client_logger_builder::operator=(client_logger_builder &&other) noexcept
        {
    if (this != &other) this->_streams = std::move(other._streams);
    return *this;
}

client_logger_builder::~client_logger_builder() noexcept {}

client_logger_builder *client_logger_builder::add_file_stream(const std::string &stream_file_path, logger::severity severity)
{
    std::string full_path = "";

    if (!stream_file_path.empty())
    {
        char buffer[MAX_PATH];
        DWORD len = GetFullPathNameA(stream_file_path.c_str(), MAX_PATH, buffer, nullptr);

        if (len > 0 && len < MAX_PATH)
        {
            full_path = buffer;
        } else {
            throw std::runtime_error("Failed to get full path for file stream");
        }
    }

    this->_streams[full_path].insert(severity);
    return this;
}



client_logger_builder *client_logger_builder::add_console_stream(logger::severity severity)
{
    return this->add_file_stream("", severity);
}

client_logger_builder *client_logger_builder::transform_with_configuration(const std::string &configuration_file_path,
                                                                           const std::string &configuration_path)
                                                                           {
    std::ifstream jsonFileStream(configuration_file_path);
    if (!jsonFileStream.is_open())
    {
        throw std::runtime_error("Failed to open configuration file: " + configuration_file_path);
    }

    nlohmann::json data;
    try
    {
        jsonFileStream >> data;
    } catch (const std::exception &ex) {
        throw std::runtime_error("Failed to parse JSON configuration file: " + std::string(ex.what()));
    }

    nlohmann::json element;
    try
    {
        element = data.at(configuration_path);
    } catch (const std::exception &ex) {
        throw std::runtime_error("Configuration path '" + configuration_path + "' hasn't been found in JSON file");
    }

    std::string file = element["file"].get<std::string>();
    std::vector<logger::severity> severities = element["severity"];
    for (const auto &severity : severities)
    {
        if (file.empty())
            add_console_stream(severity);
        else
            add_file_stream(file, severity);
    }
    return this;
}

client_logger_builder *client_logger_builder::clear()
{
    this->_streams.clear();
    return this;
}

logger *client_logger_builder::build() const
{
    auto *logger = new client_logger;

    for (const auto &entry : this->_streams)
    {
        logger->insert_in_stream(entry.first, entry.second);
    }
    return logger;
}
