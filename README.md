# nanoid-cpp
[![Build Status](https://travis-ci.org/mcmikecreations/nanoid_cpp.svg?branch=master)](https://travis-ci.org/mcmikecreations/nanoid_cpp)
[![License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](LICENSE)

A tiny, URL-friendly, unique string ID generator for C++, implementation of [ai's](https://github.com/ai) [nanoid](https://github.com/ai/nanoid)!

**Can be Safe.** It uses `mt19937`, a strong random generator with a seed from `random_device` by default, which is [not cryptographically secure](https://github.com/altf4/untwister). However, the library can be expanded using an algorithm of choice (from OpenSSL, Crypto++, Botan, LibCrypto) using the [API](#custom-random-bytes-generator).

**Compact.** It uses more symbols than UUID (`A-Za-z0-9_-`)
and has the same number of unique options in just 22 symbols instead of 36.

**Fast.** Nanoid is as fast as UUID but can be used in URLs.

## Install

Install with cmake:

1. Copy git repo to your destination
2. Use `add_subdirectory` and `target_link_libraries` to add to your project.
3. Or use `cmake` and `make` to build the project and use it as a library.

## Usage

### Normal

The default method uses URL-friendly symbols (`A-Za-z0-9_-`) and returns an ID
with 21 characters (to have a collision probability similar to UUID v4).

```cpp
std::string id = nanoid::generate();
//id := "Uakgb_J5m9g-0JDMbcJqLJ"
```

Symbols `-,.()` are not encoded in the URL. If used at the end of a link
they could be identified as a punctuation symbol.

If you want to reduce ID length (and increase collisions probability),
you can pass the size as an argument:

```cpp
std::string id = nanoid::generate(10);
//id := "IRFa-VaY2b"
```

### Async

The async method can be used to simplify writing of asynchronous code.

```cpp
auto id_fut = nanoid::generate_async();
//unrelated code here
std::string id = id_fut.get();
//id := "Uakgb_J5m9g-0JDMbcJqLJ"
```

### Custom Alphabet or Length

If you want to change the ID's alphabet or length
you can pass alphabet and size.

```cpp
std::string id = nanoid::generate("1234567890abcdef", 10);
//id := "4f90d13a42"
```

Alphabet must contain 256 symbols or less.
Otherwise, the generator will not be secure.


### Custom Random Bytes Generator

You can replace the default safe random generator using the `nanoid::crypto_random_base` or `nanoid::crypto_random<G>` class.
For instance, to use a seed-based generator.

```cpp
auto random = nanoid::crypto_random<std::ranlux24>(10);
auto id = nanoid::generate(random, "abcdef", 10);
//id := "fbaefaadeb"
```

## Credits

- [ai](https://github.com/ai) - [nanoid](https://github.com/ai/nanoid)
- [codeyu](https://github.com/codeyu) - [nanoid-net](https://github.com/codeyu/nanoid-net/)

## License

The MIT License (MIT). Please see [License File](LICENSE) for more information.
