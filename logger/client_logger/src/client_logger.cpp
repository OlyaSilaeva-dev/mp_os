#include <fstream>
#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_all_streams;


client_logger::client_logger(const client_logger &other, const std::string &format_string): _format_string(format_string)
{
    this->_streams = other._streams;
    this->add_to_all_streams();
}

client_logger &client_logger::operator=(const client_logger &other)
        {
    if (this != &other)
    {
        this->_streams = other._streams;
        this->add_to_all_streams();
    }
    return *this;
}

client_logger::client_logger(client_logger &&other) noexcept
{
    this->clear_object();
    this->_streams = std::move(other._streams);
}

client_logger &client_logger::operator=(client_logger &&other) noexcept
        {
    if (this != &other)
    {
        this->clear_object();
        this->_streams = std::move(other._streams);
    }
    return *this;
}

client_logger::~client_logger() noexcept {
    for (auto iter = _streams.begin(); iter != _streams.end(); ++iter)
    {
        auto tmp = _all_streams[iter->first];
        if (tmp.second == 1) {
            tmp.first->close();
            delete tmp.first;
            --tmp.second;
        }
    }
}

void client_logger::add_to_all_streams()
{
    for (auto iter = this->_streams.cbegin(); iter != this->_streams.cend(); ++iter)
        ++_all_streams.find(iter->first)->second.second;
}

void client_logger::clear_object()
{
    for (auto iter = this->_streams.cbegin(); iter != this->_streams.cend(); ++iter)
    {
        size_t to_check = --_all_streams.find(iter->first)->second.second;
        if (to_check == 0) _all_streams.find(iter->first)->second.first->close();
    }
}

void client_logger::insert_in_stream(const std::string &path, std::set<logger::severity> severity_set)
{
    std::ofstream* stream;
    if (path.empty())
    {
        stream =  reinterpret_cast<std::ofstream*>(&std::cout);
    }
    else
    {
        stream = new std::ofstream(path);
    }

    this->_streams.emplace(std::make_pair(path, std::make_pair(stream, std::move(severity_set))));

    if (_all_streams.find(path) != _all_streams.end())
    {
        ++_all_streams.find(path)->second.second;
    }
    else
    {
        _all_streams.emplace(std::make_pair(path, std::make_pair(stream, 1)));
    }
}

logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept
{
    std::string path_of_log_file = "";
    for (const auto &entry : _streams)
    {
        if (entry.second.second.find(severity) != entry.second.second.end())
        {
            path_of_log_file = entry.first;

            std::ofstream* file;
            if (path_of_log_file.empty()) {
                for(int i = 0; _format_string[i] != '\0' && _format_string[i + 1] != '\0'; i++)
                {
                    if(_format_string[i] == '%' && _format_string[i + 1] == 'd')
                    {
                        std::cout << "[" << logger::current_date_to_string() << "] ";
                        i++;
                    }

                    else if(_format_string[i] == '%' && _format_string[i + 1] == 't')
                    {
                        std::cout << "[" << logger::current_time_to_string() << "] ";
                        i++;

                    }

                    else if(_format_string[i] == '%' && _format_string[i + 1] == 's')
                    {
                        std::cout << "[" << logger::severity_to_string(severity) << "] ";
                        i++;
                    }

                    else if(_format_string[i] == '%' && _format_string[i + 1] == 'm')
                    {
                        std::cout << text << ' ';
                        i++;
                    }
                }
                std::cout << std::endl;
            }
            else //в файл
            {
                auto file = path_of_log_file.empty() ? reinterpret_cast<std::ofstream*>(&std::cout) : _all_streams.find(path_of_log_file)->second.first;

                for(int i = 0; _format_string[i] != '\0' && _format_string[i + 1] != '\0'; i++)
                {

                    if(_format_string[i] == '%' && _format_string[i + 1] == 'd')
                    {
                        *file << "[" << logger::current_date_to_string() << "] ";
                        i++;
                    }

                    else if(_format_string[i] == '%' && _format_string[i + 1] == 't')
                    {
                        *file << "[" << logger::current_time_to_string() << "] ";
                        i++;
                    }

                    else if(_format_string[i] == '%' && _format_string[i + 1] == 's')
                    {
                        *file<< "[" << severity_to_string(severity)<< "] ";
                        i++;
                    }

                    else if(_format_string[i] == '%' && _format_string[i + 1] == 'm') {

                        *file << text << " ";
                        i++;
                    }
                }

                *file << std::endl;
            }
        }
    }




    return this;
}



