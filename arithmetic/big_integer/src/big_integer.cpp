#include <cstring>
#include <stdexcept>
#include <vector>

#include "big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const {
    throw not_implemented(
            "big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)",
            "your code should be here...");
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const {
    throw not_implemented(
            "big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)",
            "your code should be here...");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const {
    throw not_implemented(
            "big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)",
            "your code should be here...");
}

big_integer &big_integer::trivial_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {
    throw not_implemented(
            "big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {
    throw not_implemented(
            "big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::Newton_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {
    throw not_implemented(
            "big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::Newton_division::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {
    throw not_implemented(
            "big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {
    throw not_implemented(
            "big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {
    throw not_implemented(
            "big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)",
            "your code should be here...");
}

void big_integer::clear() {
    _oldest_digit = 0;
    delete[] _other_digits;
    _other_digits = nullptr;
}

void big_integer::copy_from(
        big_integer const &other) {
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;
    if (other._other_digits == nullptr) {
        return;
    }

    _other_digits = new unsigned int[*other._other_digits];
    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
}

void big_integer::initialize_from(
        int const *digits,
        size_t digits_count) {
    if (digits == nullptr) {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0) {
        throw std::logic_error("digits array length must  be GT 0");
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits = (digits_count == 1
                     ? nullptr
                     : new unsigned int[digits_count]);

    if (_other_digits == nullptr) {
        return;
    }

    *_other_digits = (unsigned int) digits_count;

    std::memcpy(_other_digits + 1, digits, sizeof(unsigned int) * (digits_count - 1));
}

void big_integer::initialize_from(
        std::vector<int> const &digits,
        size_t digits_count) {
    _other_digits = nullptr;

    if (digits.empty() || digits_count == 0) {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1) {
        return;
    }

    _other_digits = new unsigned int[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i) {
        _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
    }
}

void big_integer::initialize_from(std::string const &value, size_t base) {
    _other_digits = nullptr;

    if (value.empty() || base < 2 || base > 36) {
        throw std::invalid_argument("The base should be in the range from 2 to 36");
    }

    std::vector<int> v0 = {0};
    big_integer a(v0);

    std::vector<int> v1 = {1};
    big_integer b_next(v1);


    size_t length = value.size();
//    std::vector<uint32_t> digits;
//    uint32_t accumulated_value = 0;
//    uint32_t base_tmp = 1;

    bool is_negative = false;

    // Проход по строке с начала (little-endian)
    for (int i = length - 1; i >= 0; i--) {
        uint32_t num;
        if (value[i] >= 'a' && value[i] <= 'z') {
            num = (uint32_t)(value[i] - 'a') + 10;
        } else if (value[i] >= 'A' && value[i] <= 'Z') {
            num = (uint32_t)(value[i] - 'A') + 10;
        } else if (value[i] >= '0' && value[i] <= '9') {
            num = (uint32_t)(value[i] - '0');
        } else if (value[i] == '-') {
            is_negative = true;
            continue;
        } else {
            throw std::invalid_argument("Invalid character in the input string");
        }

        if (num >= base) {
            throw std::invalid_argument("Invalid digit for the provided base");
        }

        for (int j = 0; j < num; j++) {
            a += b_next;
        }

        big_integer b_next_prev = b_next;
        for (int j = 0; j < base - 1; j++) {
            b_next += b_next_prev;
        }
//
//        // Если base_tmp переполняется или мы обработали последний символ
//        if (base_tmp > UINT_MAX / base || i == length - 1) {
//            digits.push_back(accumulated_value);
//            accumulated_value = 0;
//            base_tmp = 1;
//            ++digits_count;
//        } else {
//            base_tmp *= base;
//        }
    }

    // Инициализация _oldest_digit и _other_digits
//    if (!digits.empty()) {
//        _oldest_digit = is_negative ?  -static_cast<int>(digits.back()) : static_cast<int>(digits.back());
//        digits.pop_back();
//    }
//
//    if (!digits.empty()) {
//        _other_digits = new uint32_t[digits.size() + 1];
//        _other_digits[0] = digits_count;
//        std::copy(digits.begin(), digits.end(), _other_digits + 1);
//    }
    copy_from(a);
}

void big_integer::print_byte(
        std::ostream &stream,
        unsigned char byte_value) {
    for (int i = 0; i < 8; i++) {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
        std::ostream &stream,
        int value) {
    auto *p = (unsigned char *) &value;
    for (int i = 0; i < sizeof(int); i++) {
        print_byte(stream, *p++);
        stream << ' ';
    }
}

void big_integer::dump_value(std::ostream &stream) const {

    for (auto i = 0; i < get_digits_count(); ++i)
    {
        auto digit = get_digit(i);
        dump_int_value(stream, *reinterpret_cast<int *>(&digit));
        stream << ' ';
    }
}

big_integer &big_integer::change_sign() {
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));

    return *this;
}

inline int big_integer::get_digits_count() const noexcept {
    return static_cast<int>(_other_digits == nullptr
                            ? 1
                            : *_other_digits);
}

inline int big_integer::sign() const noexcept {
    if (is_equal_to_zero()) {
        return 0;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept {
    return _oldest_digit == 0 && _other_digits == nullptr;
}

inline unsigned int big_integer::get_digit(
        int position) const noexcept {
    if (_other_digits == nullptr) {
        return position == 0
               ? _oldest_digit
               : 0;
    }

    int const digits_count = get_digits_count();
    if (position < digits_count - 1) {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1) {
        return _oldest_digit;
    }

    return 0;
}

big_integer::big_integer(
        int const *digits,
        size_t _digits_count,
        allocator *allocator) {
    initialize_from(digits, _digits_count);
}

big_integer::big_integer(
        std::vector<int> const &digits,
        allocator *allocator) {
    // TODO: remove additional zeros
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
        std::string const &value_as_string,
        size_t base,
        allocator *allocator) {
    initialize_from(value_as_string, base);
}

big_integer::~big_integer() {
    clear();
}

big_integer::big_integer(
        big_integer const &other) {
    copy_from(other);
}

big_integer &big_integer::operator=(
        big_integer const &other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }

    return *this;
}

big_integer::big_integer(
        big_integer &&other) noexcept {

}

big_integer &big_integer::operator=(
        big_integer &&other) noexcept {
    return *this;
}

bool big_integer::operator==(
        big_integer const &other) const {
    throw not_implemented("bool big_integer::operator==(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator!=(
        big_integer const &other) const {
    throw not_implemented("bool big_integer::operator!=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<(
        big_integer const &other) const {
    throw not_implemented("bool big_integer::operator<(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>(
        big_integer const &other) const {
    throw not_implemented("bool big_integer::operator>(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<=(
        big_integer const &other) const {
    throw not_implemented("bool big_integer::operator<=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>=(
        big_integer const &other) const {
    throw not_implemented("bool big_integer::operator>=(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-() const {
    throw not_implemented("big_integer big_integer::operator-() const", "your code should be here...");
}

big_integer &big_integer::operator+=(
        big_integer const &other) {
    if (other.is_equal_to_zero()) {
        return *this;
    }

    if (is_equal_to_zero()) {
        return *this = other;
    }

    if (sign() == -1) {
        change_sign();
        *this += -other;
        return change_sign();
    }

    if (other.sign() == -1) {
        return *this -= -other;
    }

    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);

    unsigned int operation_result = 0;

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i) {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j) {
            operation_result += (first_value_digit & mask) + (second_value_digit & mask);
            first_value_digit >>= shift;
            second_value_digit >>= shift;
            *reinterpret_cast<unsigned int *>(&result_digits[i]) |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    result_digits.back() += *reinterpret_cast<int *>(&operation_result);

    auto *maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int *>(&*(result_digits.end() - 2));
    if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1)) {
        *maybe_overflowed_digit_ptr ^= (1 << ((sizeof(unsigned int) << 3) - 1));
        ++result_digits.back();
    }

    auto result_digits_count = result_digits.size();
    if (result_digits.back() == 0) {
        --result_digits_count;
    }

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator+(
        big_integer const &other) const {
    return big_integer(*this) += other;
}

big_integer big_integer::operator+(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator+(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator-=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator-=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator-(
        big_integer const &other) const {
    return big_integer(*this) -= other;
}

big_integer big_integer::operator-(
        std::pair<big_integer, allocator *> const &other) const {
    return big_integer(*this).change_sign();
}

big_integer &big_integer::operator*=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator*=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator*(
        big_integer const &other) const {
    return big_integer(*this) *= other;
}

big_integer big_integer::operator*(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator*(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator/=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator/=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator/(
        big_integer const &other) const {
    return big_integer(*this) /= other;
}

big_integer big_integer::operator/(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator/(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator%=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator%=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator%(
        big_integer const &other) const {
    return big_integer(*this) %= other;
}

big_integer big_integer::operator%(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator%(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer big_integer::operator~() const {
    throw not_implemented("big_integer big_integer::operator~() const", "your code should be here...");
}

big_integer &big_integer::operator&=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator&=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator&(
        big_integer const &other) const {
    throw not_implemented("big_integer big_integer::operator&(big_integer const &) const",
                          "your code should be here...");
}

big_integer big_integer::operator&(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator&(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}


big_integer &big_integer::operator|=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator|=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator|(
        big_integer const &other) const {
    throw not_implemented("big_integer big_integer::operator|(big_integer const &) const",
                          "your code should be here...");
}

big_integer big_integer::operator|(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator|(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator^=(
        big_integer const &other) {
    throw not_implemented("big_integer &big_integer::operator^=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator^(
        big_integer const &other) const {
    throw not_implemented("big_integer big_integer::operator^(big_integer const &) const",
                          "your code should be here...");
}

big_integer big_integer::operator^(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator^(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator<<=(
        size_t shift_value) {
    if (is_equal_to_zero() || shift_value == 0) {
        return *this;
    }

    auto value_sign = sign();
    if (value_sign == -1) {
        change_sign();
    }

    auto const added_by_shift_at_other_digits_digits_count = shift_value / (sizeof(unsigned int) << 3);
    shift_value %= (sizeof(unsigned int) << 3);

    auto added_by_shift_at_oldest_digit_digits_count = 0;
    if (_oldest_digit != 0) {
        unsigned int oldest_digit = *reinterpret_cast<unsigned int *>(&_oldest_digit);
        int oldest_value_bit_index = 0;
        while (oldest_digit != 1) {
            oldest_digit >>= 1;
            ++oldest_value_bit_index;
        }

        if (oldest_value_bit_index + shift_value > (sizeof(int) << 3) - 2) {
            ++added_by_shift_at_oldest_digit_digits_count;
        }
    }

    if (added_by_shift_at_oldest_digit_digits_count != 0 || added_by_shift_at_other_digits_digits_count != 0) {
        auto const added_digits_count =
                added_by_shift_at_oldest_digit_digits_count + added_by_shift_at_other_digits_digits_count;

        if (_other_digits == nullptr) {
            _other_digits = new unsigned int[added_digits_count + 1];
            *_other_digits = added_digits_count + 1;
            std::memset(_other_digits + 1, 0, sizeof(unsigned int) * (added_digits_count - 1));
            if (added_by_shift_at_oldest_digit_digits_count != 0) {
                _other_digits[*_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            } else {
                _other_digits[*_other_digits - 1] = 0;
            }
        } else {
            auto *new_digits = new unsigned int[added_digits_count + *_other_digits];
            std::memset(new_digits + 1, 0, sizeof(unsigned int) * added_digits_count);
            if (added_by_shift_at_oldest_digit_digits_count != 0) {
                new_digits[added_digits_count + *_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            std::memcpy(new_digits + 1 + added_by_shift_at_other_digits_digits_count, _other_digits + 1,
                        sizeof(unsigned int) * (*_other_digits - 1));
            *new_digits = *_other_digits + added_digits_count;

            clear();
            _other_digits = new_digits;
        }
    }

    if (shift_value != 0) {
        auto const digits_count = get_digits_count();
        unsigned int part_to_move_to_next_digit = 0;
        for (auto i = 0; i < digits_count; ++i) {
            auto digit_value = get_digit(i);
            auto *digit_address = i == digits_count - 1
                                  ? reinterpret_cast<unsigned int *>(&_oldest_digit)
                                  : _other_digits + 1 + i;
            *digit_address <<= shift_value;
            *digit_address |= part_to_move_to_next_digit;
            part_to_move_to_next_digit = digit_value >> ((sizeof(unsigned int) << 3) - shift_value);
        }
    }

    if (value_sign == -1) {
        change_sign();
    }

    return *this;
}

big_integer big_integer::operator<<(
        size_t shift) const {
    return big_integer(*this) <<= shift;
}

big_integer big_integer::operator<<(
        std::pair<size_t, allocator *> const &shift) const {
    throw not_implemented("big_integer big_integer::operator<<(std::pair<size_t, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator>>=(
        size_t shift) {
    throw not_implemented("big_integer &big_integer::operator>>=(size_t)", "your code should be here...");
}

big_integer big_integer::operator>>(
        size_t shift) const {
    throw not_implemented("big_integer big_integer::operator>>(size_t) const", "your code should be here...");
}

big_integer big_integer::operator>>(
        std::pair<size_t, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator>>(std::pair<size_t, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator,
        big_integer::multiplication_rule multiplication_rule) {
    throw not_implemented(
            "big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer big_integer::multiply(
        big_integer const &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator,
        big_integer::multiplication_rule multiplication_rule) {
    throw not_implemented(
            "big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule) {
    throw not_implemented(
            "big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer big_integer::divide(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule) {
    throw not_implemented(
            "big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer &big_integer::modulo(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule) {
    throw not_implemented(
            "big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)",
            "your code should be here...");
}

big_integer big_integer::modulo(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule,
        big_integer::multiplication_rule multiplication_rule) {
    throw not_implemented(
            "big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)",
            "your code should be here...");
}

std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value) {
    throw not_implemented("std::ostream &operator<<(std::ostream &, big_integer const &)",
                          "your code should be here...");
}

std::istream &operator>>(
        std::istream &stream,
        big_integer &value) {
    throw not_implemented("std::istream &operator>>(std::istream &, big_integer &)", "your code should be here...");
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept {
    return this->_allocator;
}