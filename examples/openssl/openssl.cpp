#include "nanoid/nanoid.h"
#include <openssl/rand.h>
#include <iostream>

class openssl_random : public NANOID_NAMESPACE::crypto_random_base
{
public:
    using result_type = std::uint32_t;

    openssl_random& operator=(const openssl_random& other) = delete;
    openssl_random(const openssl_random&) = delete;
    // Seeding can happen in the constructor or globally beforehand. An engine can be used here too.
    explicit openssl_random() = default;

    static constexpr result_type(min)() {
        return std::numeric_limits<std::uint32_t>::min();
    }

    static constexpr result_type(max)() {
        return std::numeric_limits<std::uint32_t>::max();
    }

    result_type operator()() {
        result_type res = 0;
        RAND_bytes(reinterpret_cast<unsigned char*>(&res), (int)sizeof(result_type));
        return res;
    }

    void next_bytes(std::uint8_t* buffer, std::size_t size) override
    {
        RAND_bytes(buffer, (int)size);
    }
};

int main()
{
    nanoid::crypto_random<openssl_random> random;
    for (int i = 0; i < 5; ++i)
    {
        std::cout << nanoid::generate(random, "abcdef", 10) << std::endl;
    }
    return 0;
}
