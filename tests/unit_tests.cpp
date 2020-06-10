#include <cstdint>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "nanoid/nanoid.h"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.h"

static std::string _default_dict = "_-0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::size_t _default_size = 21;

class predefined_random : public NANOID_NAMESPACE::crypto_random_base
{
public:
	using result_type = std::uint32_t;
private:
	std::vector<std::uint8_t> _sequence;
public:
	predefined_random& operator=(const predefined_random& other) = delete;
	predefined_random(const predefined_random&) = delete;
	predefined_random(const std::vector<std::uint8_t>& vec) : _sequence(vec) {}

	static constexpr result_type(min)() {
		return std::numeric_limits<std::uint32_t>::min();
	}

	static constexpr result_type(max)() {
		return std::numeric_limits<std::uint32_t>::max();
	}

	result_type operator()() {
		result_type res = _sequence[0];
	}

	void next_bytes(std::uint8_t* buffer, std::size_t size) override
	{
        std::vector<std::uint8_t> bytes;
        for (std::size_t i = 0; i < size; i += _sequence.size())
        {
            bytes.insert(
                std::end(bytes), 
                std::begin(_sequence), 
                std::begin(_sequence) + std::min(size - i, _sequence.size())
            );
        }
        for (std::size_t i = 0; i < size; ++i)
        {
            //Have to revert order, because original uses reversed buffer, so double-reverse.
            buffer[i] = bytes[(size - i - 1)];
        }
	}
};

TEST_CASE("TestDefault")
{
    std::string res = nanoid::generate();
    REQUIRE(res.size() == _default_size);
}

TEST_CASE("TestCustomSize")
{
    const std::size_t size = 10;
    std::string res = nanoid::generate(size);
    REQUIRE(res.size() == size);
}

TEST_CASE("TestCustomAlphabet")
{
    const std::string alphabet = "1234abcd";

    char buff[256];
    snprintf(buff, sizeof(buff), "[%s]{%d}$", alphabet.c_str(), _default_size);
    std::string regex = buff;

    std::string res = nanoid::generate(alphabet);
    REQUIRE(res.size() == _default_size);
    REQUIRE_THAT(res, Catch::Matches(regex));
}

TEST_CASE("TestCustomAlphabetAndSize")
{
    const std::string alphabet = "1234abcd";
    const std::size_t size = 7;
    std::string res = nanoid::generate(alphabet, size);

    char buff[256];
    snprintf(buff, sizeof(buff), "[%s]{%d}$", alphabet.c_str(), size);
    std::string regex = buff;

    REQUIRE(res.size() == size);
    REQUIRE_THAT(res, Catch::Matches(regex));
}

TEST_CASE("TestSingleLetterAlphabet")
{
    const std::string alphabet = "a";
    const std::size_t size = 5;
    std::string res = nanoid::generate(alphabet, size);
    REQUIRE(res == "aaaaa");
}

TEST_CASE("TestPredefinedRandomSequence")
{
    const std::vector<std::uint8_t> seq{ 2, 255, 3, 7, 7, 7, 7, 7, 0, 1 };
    predefined_random r(seq);

    using tp = std::tuple<int, std::string>;

    auto tests = GENERATE(tp(4, "adca"), tp(18, "cbadcbadcbadcbadcc"));

    const std::string alphabet = "abcde";

    CHECK(nanoid::generate(r, alphabet, std::get<int>(tests)) == std::get<std::string>(tests));
}

TEST_CASE("TestAsyncGenerate")
{
    std::string res = nanoid::generate_async().get();
    REQUIRE(res.size() == _default_size);
}

TEST_CASE("TestGeneratesUrlFriendlyIDs")
{
    for (int i = 1; i <= 10; ++i)
    {
        std::string res = nanoid::generate();
        REQUIRE(res.size() == _default_size);

        for (std::size_t j = 0; j < res.size(); ++j)
        {
            REQUIRE(res.find(res[j]) != std::string::npos);
        }
    }
}
//TODO - check this, it uses standard random_device
TEST_CASE("TestHasNoCollisions")
{
    const int count = 100 * 1000;
    std::map<std::string, bool> dictUsed;
    int fails = 0;
    for (std::size_t dummy = 1; dummy <= count; ++dummy)
    {
        std::string result = nanoid::generate();
        auto it = dictUsed.find(result);
        fails += (it != dictUsed.end());
        dictUsed[result] = true;
    }
    CHECK(fails == 0);
}

TEST_CASE("TestFlatDistribution")
{
    const int count = 100 * 1000;
    std::map<char, int> chars;
    for (std::size_t dummy = 1; dummy <= count; ++dummy)
    {
        auto id = nanoid::generate();
        for (std::size_t i = 0; i < _default_size; i++)
        {
            auto c = id[i];
            auto it = chars.find(c);
            if (it == chars.end())
            {
                chars[c] = 0;
            }
            chars[c] += 1;
        }
    }

    for (auto c : chars)
    {
        auto distribution = c.second * _default_dict.size() / (double)(count * _default_size);
        REQUIRE(distribution == Approx(1.0).epsilon(0.05));
    }
}

TEST_CASE("TestMask")
{
    for (auto length = 1; length < 256; length++)
    {
        auto mask1 = (2 << (int)std::floor(std::log(length - 1) / std::log(2))) - 1;
        auto mask2 = (2 << 31 - NANOID_NAMESPACE::impl::clz32((length - 1) | 1)) - 1;
        REQUIRE(mask1 == mask2);
    }
}