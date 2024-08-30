#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H

#include <iostream>
#include <map>
#include <vector>

#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>

unsigned int sum_two_digits(
        const unsigned int &first_value,
        const unsigned int &second_value,
        unsigned int &carry);

unsigned int multiply_two_digits(
        const unsigned int &first_multiplier,
        const unsigned int &second_multiplier,
        unsigned int &carry);



class big_integer final:
    allocator_guardant
{
public:

    enum class multiplication_rule
    {
        trivial,
        Karatsuba,
        SchonhageStrassen
    };

private:

    class multiplication
    {
    public:

        virtual ~multiplication() noexcept = default;

        virtual big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const = 0;
    };

    class trivial_multiplication final:
        public multiplication
    {

    public:
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;

    };

    class Karatsuba_multiplication final:
        public multiplication
    {

    public:

        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;

    };

    class Schonhage_Strassen_multiplication final:
        public multiplication
    {

    public:

        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;

    };

public:

    enum class division_rule
    {
        trivial,
        Newton,
        BurnikelZiegler
    };

private:

    class division
    {

    public:

        virtual ~division() noexcept = default;

    public:

        virtual big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;

        virtual big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;

    };

    class trivial_division final:
        public division
    {
    public:
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

    };

    class Newton_division final:
        public division
    {

    public:

        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

    };

    class Burnikel_Ziegler_division final:
        public division
    {

    public:

        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;

    };


private:

    void clear();

    void copy_from(
            big_integer const &other);

    void initialize_from(
            unsigned int const *digits,
            size_t digits_count);

    void initialize_from(
            std::vector<unsigned int> const &digits,
            size_t digits_count);

    void initialize_from(
            std::string const &value,
            size_t base);

private:

    static void print_byte(
            std::ostream &stream,
            unsigned char byte_value);

    static void dump_int_value(
            std::ostream &stream,
            int value);

public:

    void dump_value(
            std::ostream &stream) const;


private:

    int _sign;
    unsigned int *_digits;
    size_t _size;
    allocator* _allocator;

private:
    template<typename Func>
        big_integer bitwise_operations(Func op, big_integer const& other) const;

    big_integer &_remove_leading_zeros();

    big_integer &reverse();

    big_integer abs();

    void _shift_right();

    void _add_digit(unsigned int digit);

    big_integer &multiply_by_a_number(
            big_integer &first_multiplier,
            unsigned int const &second_multiplier) const;


public:
    big_integer &change_sign();

    inline size_t get_digits_count() const noexcept;

    inline int sign() const noexcept;

    inline bool is_equal_to_zero() const noexcept;

    inline unsigned int get_digit(size_t position) const noexcept;

public:

    big_integer(
        unsigned int const *digits,
        size_t digits_count,
        int sign = 1,
        allocator *allocator = nullptr);

    explicit big_integer(
        std::vector<unsigned int> const &digits,
        int sign = 1,
        allocator *allocator = nullptr);

    explicit big_integer(
        std::string const &value_as_string,
        size_t base = 10,
        allocator *allocator = nullptr);

public:

    ~big_integer() noexcept;

    big_integer(
        big_integer const &other);

    big_integer &operator=(
        big_integer const &other);

    big_integer(
        big_integer &&other) noexcept;

    big_integer &operator=(
        big_integer &&other) noexcept;

public:

    bool operator==(
        big_integer const &other) const;

    bool operator!=(
        big_integer const &other) const;

public:

    bool operator<(
        big_integer const &other) const;

    bool operator>(
        big_integer const &other) const;

    bool operator<=(
        big_integer const &other) const;

    bool operator>=(
        big_integer const &other) const;

public:

    big_integer operator-() const;

    big_integer &operator+=(
        big_integer const &other);

    big_integer operator+(
        big_integer const &other) const;

    big_integer operator+(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator-=(
        big_integer const &other);

    big_integer operator-(
        big_integer const &other) const;

    big_integer operator-(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator*=(
        big_integer const &other);

    big_integer operator*(
        big_integer const &other) const;

    big_integer operator*(
        std::pair<big_integer, allocator *> const &other) const;

    const big_integer operator*=(
            unsigned int const &value) const;

    big_integer operator*(
            unsigned int const &value) const;

    big_integer &operator/=(
        big_integer const &other);

    big_integer operator/(
        big_integer const &other) const;

    big_integer operator/(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator%=(
        big_integer const &other);

    big_integer operator%(
        big_integer const &other) const;

    big_integer operator%(
        std::pair<big_integer, allocator *> const &other) const;

public:

    big_integer operator~() const;

    big_integer &operator&=(
        big_integer const &other);

    big_integer operator&(
        big_integer const &other) const;

    big_integer operator&(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator|=(
        big_integer const &other);

    big_integer operator|(
        big_integer const &other) const;

    big_integer operator|(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator^=(
        big_integer const &other);

    big_integer operator^(
        big_integer const &other) const;

    big_integer operator^(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator<<=(
        size_t shift);

    big_integer operator<<(
        size_t shift) const;

    big_integer operator<<(
        std::pair<size_t, allocator *> const &shift) const;

    big_integer &operator>>=(
        size_t shift);

    big_integer operator>>(
        size_t shift) const;

    big_integer operator>>(
        std::pair<size_t, allocator *> const &shift) const;

public:

    static big_integer &multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer multiply(
        big_integer const &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer divide(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &modulo(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer modulo(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

public:

    friend std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value);

    friend std::istream &operator>>(
        std::istream &stream,
        big_integer &value);

private:

    [[nodiscard]] allocator *get_allocator() const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H