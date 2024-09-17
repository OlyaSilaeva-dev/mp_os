#include <cstring>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "big_integer.h"


unsigned int multiply_two_digits(const unsigned int &first_multiplier,
                                 const unsigned int &second_multiplier,
                                 unsigned int &carry) {
    constexpr unsigned int shift = sizeof(unsigned int) << 2;
    constexpr unsigned int mask = (1 << shift) - 1;

    unsigned int a_lo = first_multiplier & mask;
    unsigned int a_hi = first_multiplier >> shift;
    unsigned int b_lo = second_multiplier & mask;
    unsigned int b_hi = second_multiplier >> shift;

    unsigned int low_mult = a_lo * b_lo;
    unsigned int mid_mult1 = a_hi * b_lo;
    unsigned int mid_mult2 = a_lo * b_hi;
    unsigned int high_mult = a_hi * b_hi;

    unsigned int result_lo = (low_mult & mask);
    unsigned int result_hi = (low_mult >> shift) + (mid_mult1 & mask) + (mid_mult2 & mask);

    unsigned int result = result_lo | ((result_hi & mask) << shift);

    carry = high_mult + (mid_mult1 >> shift) + (mid_mult2 >> shift) + (result_hi >> shift);

    return result;
}

unsigned int sum_two_digits(const unsigned int &first_summand,
                            const unsigned int &second_summand,
                            unsigned int &carry) {
    constexpr unsigned int shift = sizeof(unsigned int) << 2;
    constexpr unsigned int mask = (1 << shift) - 1;

    unsigned int lower_sum = (first_summand & mask) + (second_summand & mask);
    carry = lower_sum >> shift;

    unsigned int upper_sum = (first_summand >> shift) + (second_summand >> shift) + carry;
    carry = upper_sum >> shift;

    return (lower_sum & mask) | ((upper_sum & mask) << shift);
}


big_integer &big_integer::multiply_by_a_number(big_integer &first_multiplier,
                                               const unsigned int &second_multiplier) const {
    size_t size = first_multiplier.get_digits_count();
    std::vector<uint32_t> result(size + 1, 0);
    unsigned int carry = 0;

    for(size_t i = 0; i < size; i++) {
        unsigned int cur_digit = first_multiplier.get_digit(i);
        unsigned int result_digit = multiply_two_digits(cur_digit, second_multiplier, carry);
        result[i + 1] +=  carry;
        result[i] = sum_two_digits(result[i], result_digit, carry);
        result[i + 1] += carry;
    }

    first_multiplier.initialize_from(result, result.size());
    first_multiplier._remove_leading_zeros();
    return first_multiplier;
}

big_integer &big_integer::trivial_multiplication::multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier) const {

    size_t m = first_multiplier._size;
    size_t n = second_multiplier._size;

    std::vector<unsigned int> result_digits(m + n + 1, 0);
    int result_sign = first_multiplier.sign() * second_multiplier.sign();

    unsigned int carry = 0;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n || carry != 0; ++j) {
            unsigned int a = first_multiplier.get_digit(i);
            unsigned int b = j < n ? second_multiplier.get_digit(j) : 0;

            unsigned int operation_result = multiply_two_digits(a, b, carry);
            unsigned int carry1;
            result_digits[i + j + 1] = sum_two_digits(result_digits[i + j + 1], carry, carry1);
            if(i + j + 1 < m + n)
                result_digits[i + j + 2] += carry1;

            result_digits[i + j] = sum_two_digits(result_digits[i + j], operation_result, carry);
            result_digits[i + j + 1] = sum_two_digits(result_digits[i + j + 1], carry, carry1);
            if(i + j + 2 < m + n + 1)
                result_digits[i + j + 2] += carry1;
        }
    }

    first_multiplier.initialize_from(result_digits, result_digits.size());
    first_multiplier._remove_leading_zeros();
    first_multiplier._sign = result_sign;
    return first_multiplier;
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

void big_integer::_shift_right() {
    if (_size == 0) {
        _add_digit(0);
        return;
    }

    _add_digit(_digits[_size - 1]);
    for (auto i = static_cast<int64_t>(_size) - 2; i > 0; --i) {
        _digits[i] = _digits[i - 1];
    }
    _digits[0] = 0;
}

big_integer big_integer::abs() {
    big_integer result = *this;
    result._sign = 1;
    return result;
}

big_integer &big_integer::reverse() {
    std::vector<uint32_t> vector_result(_size, 0);
    for (size_t i = 0; i < _size; i++) {
        vector_result[i] = _digits[_size - i - 1];
    }
    initialize_from(vector_result, _size);
    return *this;
}

void big_integer::_add_digit(unsigned int digit) {
    if (_size == 0) {
        _digits = new unsigned int[1];
        _digits[0] = digit;
        _size = 1;
    } else if (_size == 1 && _digits[0] == 0) {
        _digits[0] = digit;
    } else {
        auto *new_digits = (unsigned int*)std::realloc(_digits, (_size + 1) * sizeof(unsigned int));
        if (new_digits == nullptr) {
            throw std::bad_alloc();
        }
        new_digits[_size] = digit;
        _digits = new_digits;
        _size++;
    }
}

big_integer &big_integer::trivial_division::divide(
        big_integer &dividend,
        big_integer const &divisor,
        big_integer::multiplication_rule multiplication_rule) const {

    if (divisor.is_equal_to_zero()) {
        throw std::runtime_error("Division by zero");
    }

    if (dividend.is_equal_to_zero()) {
        dividend = big_integer("0");
        return dividend;
    }

    big_integer result("0");
    result._sign = dividend._sign * divisor._sign;

    std::vector<uint32_t> result_digits(dividend._size, 0);

    big_integer current("0");

    big_integer abs_divident = dividend.abs();
    big_integer abs_divisor = divisor;
    abs_divisor = abs_divisor.abs();
    unsigned int curry = 0;

    for (int64_t i = static_cast<int64_t> (dividend._size) - 1; i >= 0; i--) {
        current._shift_right();
        current._digits[0] = dividend.get_digit(i);
        current._remove_leading_zeros();

        uint32_t l = 0, r = UINT32_MAX - 1, result_digit = 0;
        big_integer t("0");
        while (l <= r) {
            uint32_t m = l + ((r - l) >> 1);
            t = abs_divisor * m;
            if (t <= current) {
                result_digit = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }

        result_digits[i] = result_digit;
        current = current - abs_divisor * result_digit;
    }

    result.initialize_from(result_digits, result_digits.size());
    result._remove_leading_zeros();
    dividend = result;
    return dividend;
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
    if (_size != 0) {
        delete[] _digits;
    }
    _sign = 0;
    _size = 0;
    _digits = nullptr;
}

void big_integer::copy_from(
        big_integer const &other) {
    _sign = other._sign;
    _size = other._size;
    _digits = nullptr;
    if (other._digits == nullptr) {
        return;
    }
    try {
        _digits = new unsigned int[other._size];
        std::memcpy(_digits, other._digits, sizeof(unsigned int) * other._size);
    } catch (std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        throw;
    }
}

void big_integer::initialize_from(
        unsigned int const *digits,
        size_t digits_count) {
    if (digits == nullptr) {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0) {
        throw std::logic_error("digits array length must  be GT 0");
    }

    _digits = new unsigned int[digits_count];

    if (_digits == nullptr) {
        return;
    }

    _size = digits_count;
    std::memcpy(_digits, digits, sizeof(unsigned int) * (digits_count));
}

void big_integer::initialize_from(
        std::vector<unsigned int> const &digits,
        size_t digits_count) {
    _digits = nullptr;

    if (digits.empty() || digits_count == 0) {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    try {
        _digits = new unsigned int[digits_count];
    } catch (std::bad_alloc& bad_alloc){
        std::cerr << bad_alloc.what() << std::endl;
        throw;
    }
    _size = digits_count;

    for (auto i = 0; i < digits_count; ++i) {
        _digits[i] = digits[i];
    }
}

void big_integer::initialize_from(std::string const &value, size_t base) {
    _digits = nullptr;

    if (value.empty() || base < 2 || base > 36) {
        throw std::invalid_argument("The base should be in the range from 2 to 36");
    }

    std::vector<unsigned int> v0 = {0};
    big_integer a(v0);

    std::vector<unsigned int> v1 = {1};
    big_integer b_next(v1);


    size_t length = value.size();
    bool is_negative = false;

    for (int i = (int)length - 1; i >= 0; i--) {
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

        for (uint32_t j = 0; j < num; j++) {
            a += b_next;
        }

        big_integer b_next_prev = b_next;
        for (int j = 0; j < base - 1; j++) {
            b_next += b_next_prev;
        }

    }
    a._sign = is_negative ? -1 : 1;

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

template<typename Func>
big_integer big_integer::bitwise_operations(Func op, big_integer const& other) const {
    big_integer result("0");

    size_t max_size = std::max(_size, other._size);
    result._digits = new unsigned int[max_size];
    result._size = max_size;

    for (size_t i = 0; i < max_size; ++i) {
        unsigned int digit_a = (i < _size) ? _digits[i] : 0;
        unsigned int digit_b = (i < other._size) ? other._digits[i] : 0;
        result._digits[i] = op(digit_a, digit_b);
    }

    return result;
}

big_integer &big_integer::_remove_leading_zeros() {
    size_t size_tmp = _size;
    while (size_tmp > 1 && get_digit(size_tmp - 1) == 0) {
        size_tmp--;
    }
    try {
        auto tmp = static_cast<unsigned int*>(realloc(_digits, size_tmp * sizeof(unsigned int)));
        if (tmp == nullptr) {
            throw std::bad_alloc();
        }
        _digits = tmp;
        _size = size_tmp;
    } catch (const std::bad_alloc& e) {
        std::cerr << "memory allocation failed: " << e.what() << std::endl;
    }

    return *this;
}


big_integer &big_integer::change_sign() {
//    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));
    _sign = -_sign;
    return *this;
}

inline size_t big_integer::get_digits_count() const noexcept {
    return _size;
}

inline int big_integer::sign() const noexcept {
    if (is_equal_to_zero()) {
        return 0;
    }

//    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
    return _sign;
}

inline bool big_integer::is_equal_to_zero() const noexcept {
    return *_digits == 0 && _size == 1;
}

inline unsigned int big_integer::get_digit(
        size_t position) const noexcept {

    size_t const digits_count = get_digits_count();
    if (position < digits_count) {
        return _digits[position];
    }
    return 0;
}

big_integer::big_integer(
        unsigned int const *digits,
        size_t _digits_count,
        int sign,
        allocator *allocator) : _sign(sign), _allocator(allocator){
    initialize_from(digits, _digits_count);
}

big_integer::big_integer(
        std::vector<unsigned int> const &digits,
        int sign,
        allocator *allocator) : _sign(sign), _allocator(allocator){
    // TODO: remove additional zeros
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
        std::string const &value_as_string,
        size_t base,
        allocator *allocator) {
    initialize_from(value_as_string, base);
}

big_integer::~big_integer() noexcept{
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
    _digits = other._digits;
    _size = other._size;
    _sign = other._sign;

    other._digits = nullptr;
    other._size = 0;
    other._sign = 1;
}

big_integer &big_integer::operator=(
        big_integer &&other) noexcept {
    clear();

    _digits = other._digits;
    _size = other._size;
    _sign = other._sign;

    other._digits = nullptr;
    other._size = 0;
    other._sign = 1;

    return *this;
}

bool big_integer::operator==(
        big_integer const &other) const {
    if (this->_size != other._size) return false;
    if (this->_sign != other._sign) return false;

    for (int i = 0; i < this->_size; i++) {
        if (this->_digits[i] != other._digits[i]) {
            return false;
        }
    }
    return true;
}

bool big_integer::operator!=(
        big_integer const &other) const {
    return !(*this == other);
}

bool big_integer::operator<(
        big_integer const &other) const {
    if (this->_sign != other._sign) {
        return this->_sign < other._sign;
    }
    if (this->_size != other._size) {
        return this->_size < other._size;
    }
    if (this->_sign == -1) {
        for (int i = (int)this->_size - 1; i >= 0; i--) {
            if (this->_digits[i] != other._digits[i]) {
                return this->_digits[i] > other._digits[i];
            }
        }
    } else {
        for (int i = (int)this->_size - 1; i >= 0; i--) {
            if (this->_digits[i] != other._digits[i]) {
                return this->_digits[i] < other._digits[i];
            }
        }
    }

    return false;
}

bool big_integer::operator>(
        big_integer const &other) const {
    return (*this != other) && !(*this < other);
}

bool big_integer::operator<=(
        big_integer const &other) const {
    return (*this < other) || (*this == other);
}

bool big_integer::operator>=(
        big_integer const &other) const {
    return (*this > other) || (*this == other);
}

big_integer big_integer::operator-() const{
    big_integer result = *this;
    result.change_sign();
    return result;
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

    constexpr unsigned int shift = sizeof(unsigned int) << 2;
    constexpr unsigned int mask = (1 << shift) - 1;

    std::vector<unsigned int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i) {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j) {
            operation_result += (first_value_digit & mask) + (second_value_digit & mask);
            first_value_digit >>= shift;
            second_value_digit >>= shift;
            result_digits[i] |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    result_digits.back() += operation_result;

    auto *maybe_overflowed_digit_ptr = (&*(result_digits.end() - 1));
    if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1)) {
        *maybe_overflowed_digit_ptr ^= (1 << ((sizeof(unsigned int) << 3) - 1));
        ++result_digits.back();
    }

    auto result_digits_count = result_digits.size();
    if (result_digits.back() == 0) {
        --result_digits_count;
    }

    if ((*this).sign() < 0) {
        clear();
        this->_sign = -1;
    } else {
        clear();
        this->_sign = 1;
    }
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
    if (this->_sign != other.sign()) {
        *this += -other;
        return *this;
    }
    if (other.is_equal_to_zero()) {
        return *this;
    }
    if(is_equal_to_zero()) {
        *this = other;
        return change_sign();
    }
    if(*this < other) {
        big_integer tmp = other;
        tmp -= *this;
        *this = tmp;
        return change_sign();
    }
    if (other._sign < 0 && (*this)._sign < 0) {
        this->change_sign();
        (*this) -= (-other);
        return change_sign();
    }

    size_t this_size = this->get_digits_count();
    size_t other_size = other.get_digits_count();
    std::vector<unsigned int> result(this_size,0);

    unsigned int operation_result = 0;
    int carry = 0;

    for (int i = 0; i < this_size; i++) {
        unsigned int digit1 = this->get_digit(i);
        unsigned int digit2 = i < other_size ? other.get_digit(i) : 0;

        if (digit1 < digit2 + carry) {
            result[i] = UINT32_MAX - digit2 - carry + digit1  + 1;
            carry = 1;
        } else {
            result[i] = digit1 - digit2 - carry;
            carry = 0;
        }
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    initialize_from(result, result.size());
    if (carry == 1) {
        this->_sign = -1;
    } else {
        this->_sign = 1;
    }

    return *this;
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
    return multiply(*this, other);
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

const big_integer big_integer::operator*=(
        unsigned int const &value) const{
    return multiply_by_a_number(const_cast<big_integer &>(*this), value);
}

big_integer big_integer::operator*(
        unsigned int const &value) const {
    return big_integer(*this) *= value;
}

big_integer &big_integer::operator/=(big_integer const &other) {
    return divide(*this, other);
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
    big_integer result = (*this) - (((*this) / other) * other);
    if (result._sign == -1) result += other;
    *this = result;
    return *this;
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
    big_integer result = *this;

    for(size_t i = 0; i < result._size; i++) {
        result._digits[i] = ~result._digits[i];
    }
    result.change_sign();

    if (result.is_equal_to_zero()) {
       result._sign = 1;
    }

    return result;
}

big_integer &big_integer::operator&=(
        big_integer const &other) {
    big_integer tmp = bitwise_operations(std::bit_and<unsigned int>(), other);

    if((*this)._sign > 0 && other._sign > 0) {
        tmp._sign = 1;
    } else {
        tmp._sign = -1;
    }
    *this = tmp;
    return *this;
}

big_integer big_integer::operator&(
        big_integer const &other) const {
    return (big_integer)*this &= other;
}

big_integer big_integer::operator&(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator&(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator|=(
        big_integer const &other) {
    big_integer tmp = bitwise_operations(std::bit_or<unsigned int>(), other);

    if (this->_sign < 0 && other._sign < 0) {
        tmp._sign = -1;
    } else {
        tmp._sign = 1;
    }
    *this = tmp;
    return *this;
}

big_integer big_integer::operator|(
        big_integer const &other) const {
    return (big_integer)*this |= other;
}

big_integer big_integer::operator|(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator|(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer &big_integer::operator^=(
        big_integer const &other) {
    big_integer tmp = bitwise_operations(std::bit_xor<unsigned int>(), other);

    if ((*this)._sign == other._sign) {
        tmp._sign = -1;
    } else {
        tmp._sign = 1;
    }
    *this = tmp;
    return *this;
}

big_integer big_integer::operator^(
        big_integer const &other) const {
    return (big_integer)*this ^= other;
}

big_integer big_integer::operator^(
        std::pair<big_integer, allocator *> const &other) const {
    throw not_implemented("big_integer big_integer::operator^(std::pair<big_integer, allocator *> const &) const",
                          "your code should be here...");
}

big_integer& big_integer::operator<<=(size_t shift_value) {
    if (is_equal_to_zero() || shift_value == 0) {
        return *this;
    }

    int value_sign = _sign;
    if (value_sign == -1) {
        change_sign();
    }

    size_t full_digit_shift = shift_value / (sizeof(unsigned int) * 8);
    shift_value %= (sizeof(unsigned int) * 8);

    size_t added_by_shift_at_oldest_digit_digits_count = 0;
    if (_size > 0) {
        unsigned int oldest_digit = _digits[_size - 1];
        int oldest_value_bit_index = 0;
        while (oldest_digit > 1) {
            oldest_digit >>= 1;
            ++oldest_value_bit_index;
        }

        if (oldest_value_bit_index + shift_value >= sizeof(unsigned int) * 8) {
            added_by_shift_at_oldest_digit_digits_count = 1;
        }
    }

    if (added_by_shift_at_oldest_digit_digits_count > 0 || full_digit_shift > 0) {
        size_t new_size = _size + full_digit_shift + added_by_shift_at_oldest_digit_digits_count;
        unsigned int* new_digits = new unsigned int[new_size];

        std::memset(new_digits, 0, sizeof(unsigned int) * new_size);

        std::memcpy(new_digits + full_digit_shift, _digits, sizeof(unsigned int) * _size);

        delete[] _digits;
        _digits = new_digits;
        _size = new_size;
    }

    if (shift_value > 0) {
        unsigned int carry = 0;
        for (size_t i = 0; i < _size; ++i) {
            unsigned int current = _digits[i];
            _digits[i] = (current << shift_value) | carry;
            carry = current >> (sizeof(unsigned int) * 8 - shift_value);
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

//    size_t mask = sizeof(unsigned int) - 1;
    return *this;
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
    if (multiplication_rule == multiplication_rule::trivial) {
        big_integer::trivial_multiplication trivial;
        trivial.multiply(first_multiplier, second_multiplier);
    } else if (multiplication_rule == multiplication_rule::Karatsuba) {
        big_integer::Karatsuba_multiplication Karatsuba;
        Karatsuba.multiply(first_multiplier, second_multiplier);
    } else {
        big_integer::Schonhage_Strassen_multiplication Schonhage_Strassen;
        Schonhage_Strassen.multiply(first_multiplier, second_multiplier);
    }
    return first_multiplier;
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
    if (division_rule == division_rule::trivial) {
        big_integer::trivial_division division;
        return division.divide(dividend, divisor, multiplication_rule);
    } else if (division_rule == division_rule::BurnikelZiegler) {
        big_integer::Burnikel_Ziegler_division division;
        return division.divide(dividend, divisor, multiplication_rule);
    } else {
        big_integer::Newton_division division;
        return division.divide(dividend, divisor, multiplication_rule);
    }
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

    std::vector<unsigned int> result;
    unsigned int carry = 0;
    unsigned int BASE = 1e9;

    for (size_t i = value._size; i > 0; i--) {
        carry = value.get_digit(i - 1);

        for (size_t j = 0; j < result.size(); j++) {
            uint64_t tmp = static_cast<uint64_t>(result[j]) * 4294967296ULL + carry;
            result[j] = tmp % BASE;
            carry = tmp / BASE;
        }

        while (carry > 0) {
            result.push_back(carry % BASE);
            carry /= BASE;
        }
    }

    std::string output;
    if (!result.empty()) {
        output = std::to_string(result.back());
        result.pop_back();

        while (!result.empty()) {
            std::string part = std::to_string(result.back());
            result.pop_back();
            output += std::string(9 - part.length(), '0') + part;
        }
    } else {
        output = "0";
    }

    if (value._sign < 0) {
        stream << "-";
    }

    stream << output;
    return stream;
}

std::istream &operator>>(
        std::istream &stream,
        big_integer &value) {
    throw not_implemented("std::istream &operator>>(std::istream &, big_integer &)", "your code should be here...");
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept {
    return this->_allocator;
}

