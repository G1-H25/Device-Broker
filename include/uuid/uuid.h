//! @file uuid.h
//! @author Jennifer Gott
//! @brief Functions for generating UUIDs
//! @version 0.1
//! @date 2025-10-24
//! @copyright Copyright (c) 2025, released under the MIT license

#ifndef INCLUDE_UUID_UUID_H_
#define INCLUDE_UUID_UUID_H_

#include <array>
#include <cstdint>
#include <cstdio>

// Enable std::string support if available (disable for constrained embedded systems if needed)
#ifndef UUID_DISABLE_STRING_SUPPORT
#include <string>
#define UUID_STRING_SUPPORT_ENABLED
#endif

namespace uuid {

/**
 * @brief UUID version enumeration
 *
 * Specifies the UUID version as defined in RFC 4122
 */
enum class UuidVersion : uint8_t {
    NAME_BASED_MD5 = 3,  ///< UUID version 3 (Name-based using MD5)
    RANDOM = 4           ///< UUID version 4 (Random)
};

/**
 * @brief Base UUID template structure
 *
 * Template structure for UUID generation, providing common fields
 * and methods. Specialized for different UUID versions.
 *
 * @tparam version The UUID version to generate
 */
template <UuidVersion version>
struct MyUuid {
    uint32_t time_low = 0;                   ///< Low 32 bits of time field
    uint16_t time_mid = 0;                   ///< Middle 16 bits of time field
    uint16_t time_hi_and_version = 0;        ///< High 12 bits of time + 4-bit version
    uint8_t clock_seq_hi_and_reserved = 0;   ///< High 6 bits of clock sequence + 2-bit variant
    uint8_t clock_seq_low = 0;               ///< Low 8 bits of clock sequence
    uint8_t node[6] = {0};                   ///< 48-bit node identifier

    /**
     * @brief Convert UUID to string format (safe array version)
     *
     * Converts the UUID to the standard hyphenated string representation:
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * This version is thread-safe and does not allocate memory.
     *
     * @param output Reference to std::array to store the UUID string (36 chars + null terminator)
     */
    void to_string(std::array<char, 37>& output) const {
        snprintf(output.data(), output.size(),
                "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                time_low,
                time_mid,
                time_hi_and_version,
                clock_seq_hi_and_reserved,
                clock_seq_low,
                node[0], node[1], node[2], node[3], node[4], node[5]);
    }

#ifdef UUID_STRING_SUPPORT_ENABLED
    /**
     * @brief Convert UUID to string format (std::string version)
     *
     * Converts the UUID to the standard hyphenated string representation:
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * This version returns a std::string for convenience but allocates memory.
     *
     * @return std::string containing the formatted UUID
     */
    std::string to_string() const {
        std::array<char, 37> buffer;
        to_string(buffer);
        return std::string(buffer.data());
    }
#endif

 protected:
    /**
     * @brief Clear all UUID fields to zero
     */
    void clear_fields() {
        time_low = 0;
        time_mid = 0;
        time_hi_and_version = 0;
        clock_seq_hi_and_reserved = 0;
        clock_seq_low = 0;
        for (int i = 0; i < 6; i++) {
            node[i] = 0;
        }
    }
};

/**
 * @brief UUID version 3 specialization
 *
 * Name-based UUID using MD5 hashing. This implementation generates
 * a deterministic UUID from a 32-bit input value by distributing
 * the bits across the UUID fields.
 */
template <>
struct MyUuid<UuidVersion::NAME_BASED_MD5> {
    uint32_t time_low = 0;                   ///< Low 32 bits of time field
    uint16_t time_mid = 0;                   ///< Middle 16 bits of time field
    uint16_t time_hi_and_version = 0;        ///< High 12 bits of time + 4-bit version
    uint8_t clock_seq_hi_and_reserved = 0;   ///< High 6 bits of clock sequence + 2-bit variant
    uint8_t clock_seq_low = 0;               ///< Low 8 bits of clock sequence
    uint8_t node[6] = {0};                   ///< 48-bit node identifier

    /**
     * @brief Default constructor
     *
     * Creates an empty UUID v3 with all fields initialized to zero
     */
    MyUuid() = default;

    /**
     * @brief Construct UUID v3 from uint32_t input
     *
     * Creates a deterministic UUID version 3 from a 32-bit input value.
     * The input is distributed across the UUID fields using bit operations.
     *
     * @param input 32-bit input value to generate UUID from
     */
    explicit MyUuid(uint32_t input) {
        generate_v3_from_uint32(input);
    }

    /**
     * @brief Generate UUID v3 deterministically from uint32_t
     *
     * Distributes the 32-bit input value across all UUID fields:
     * - Sets version to 3 (bits 12-15 of time_hi_and_version)
     * - Sets variant to 10b (RFC 4122) in clock_seq_hi_and_reserved
     * - Spreads input bits across remaining fields
     *
     * @param input 32-bit input value to generate UUID from
     */
    void generate_v3_from_uint32(uint32_t input) {
        clear_fields();

        // Set version to 3 (0011 in bits 12-15)
        time_hi_and_version |= (0x3 << 12);

        // Set variant to 10 (RFC 4122) in bits 6-7 of clock_seq_hi_and_reserved
        clock_seq_hi_and_reserved |= (0x2 << 6);  // 0x2 = 0b10

        // Distribute the 32-bit input across the UUID fields
        // Fill time_low (32 bits) with lower 32 bits of input
        time_low = input;

        // Fill time_mid (16 bits) with next 16 bits
        time_mid = static_cast<uint16_t>((input >> 16) & 0xFFFF);

        // Fill time_hi_and_version (12 bits for hash) with next 12 bits
        // (upper 4 bits are already set to version 3)
        time_hi_and_version |= static_cast<uint16_t>((input >> 24) & 0x0FFF);

        // Fill clock sequence (16 bits total)
        clock_seq_hi_and_reserved |= static_cast<uint8_t>((input >> 28) & 0x3F);  // Lower 6 bits
        clock_seq_low = static_cast<uint8_t>((input >> 30) & 0xFF);

        // Fill node (48 bits) with remaining bits, padded with deterministic pattern
        uint32_t remaining = input >> 31;  // Get the highest bit
        node[0] = static_cast<uint8_t>(remaining | (input & 0xFF));
        node[1] = static_cast<uint8_t>((input >> 8) & 0xFF);
        node[2] = static_cast<uint8_t>((input >> 16) & 0xFF);
        node[3] = static_cast<uint8_t>((input >> 24) & 0xFF);
        node[4] = static_cast<uint8_t>((input >> 2) & 0xFF);
        node[5] = static_cast<uint8_t>((input >> 10) & 0xFF);
    }

    /**
     * @brief Convert UUID to string format (safe array version)
     *
     * Converts the UUID to the standard hyphenated string representation:
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * This version is thread-safe and does not allocate memory.
     *
     * @param output Reference to std::array to store the UUID string (36 chars + null terminator)
     */
    void to_string(std::array<char, 37>& output) const {
        snprintf(output.data(), output.size(),
                "%02x%02x%02x%02x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                static_cast<uint8_t>(time_low >> 24),
                static_cast<uint8_t>(time_low >> 16),
                static_cast<uint8_t>(time_low >> 8),
                static_cast<uint8_t>(time_low),
                time_mid,
                time_hi_and_version,
                clock_seq_hi_and_reserved,
                clock_seq_low,
                node[0], node[1], node[2], node[3], node[4], node[5]);
    }

#ifdef UUID_STRING_SUPPORT_ENABLED
    /**
     * @brief Convert UUID to string format (std::string version)
     *
     * Converts the UUID to the standard hyphenated string representation:
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * This version returns a std::string for convenience but allocates memory.
     *
     * @return std::string containing the formatted UUID
     */
    std::string to_string() const {
        std::array<char, 37> buffer;
        to_string(buffer);
        return std::string(buffer.data());
    }
#endif

 private:
    /**
     * @brief Clear all UUID fields to zero
     */
    void clear_fields() {
        time_low = 0;
        time_mid = 0;
        time_hi_and_version = 0;
        clock_seq_hi_and_reserved = 0;
        clock_seq_low = 0;
        for (int i = 0; i < 6; i++) {
            node[i] = 0;
        }
    }
};

/**
 * @brief UUID version 4 specialization
 *
 * Random UUID generation. This implementation generates a UUID from
 * a 32-bit random input value (e.g., from ESP-IDF's random number generator).
 */
template <>
struct MyUuid<UuidVersion::RANDOM> {
    uint32_t time_low = 0;                   ///< Low 32 bits of time field (random)
    uint16_t time_mid = 0;                   ///< Middle 16 bits of time field (random)
    uint16_t time_hi_and_version = 0;        ///< High 12 bits of time + 4-bit version
    uint8_t clock_seq_hi_and_reserved = 0;   ///< High 6 bits of clock sequence + 2-bit variant
    uint8_t clock_seq_low = 0;               ///< Low 8 bits of clock sequence (random)
    uint8_t node[6] = {0};                   ///< 48-bit node identifier (random)

    /**
     * @brief Default constructor
     *
     * Creates an empty UUID v4 with all fields initialized to zero.
     * Call generate_v4_from_random() to populate with random data.
     */
    MyUuid() = default;

    /**
     * @brief Construct UUID v4 from random input
     *
     * Creates a UUID version 4 from a 32-bit random input value.
     * The dummy parameter distinguishes this constructor from the v3 constructor.
     *
     * @param random_input 32-bit random value to generate UUID from
     * @param dummy Unused parameter for constructor overload resolution
     */
    MyUuid(uint32_t random_input, int dummy) {
        (void)dummy;  // Suppress unused parameter warning
        generate_v4_from_random(random_input);
    }

    /**
     * @brief Generate UUID v4 from 32-bit random input
     *
     * Distributes the 32-bit random value across all UUID fields:
     * - Sets version to 4 (bits 12-15 of time_hi_and_version)
     * - Sets variant to 10b (RFC 4122) in clock_seq_hi_and_reserved
     * - Spreads random bits across remaining fields
     *
     * @param random_input 32-bit random value (e.g., from esp_random())
     */
    void generate_v4_from_random(uint32_t random_input) {
        clear_fields();

        // Set version to 4 (0100 in bits 12-15)
        time_hi_and_version |= (0x4 << 12);

        // Set variant to 10 (RFC 4122) in bits 6-7 of clock_seq_hi_and_reserved
        clock_seq_hi_and_reserved |= (0x2 << 6);  // 0x2 = 0b10

        // Distribute the 32-bit random input across the UUID fields
        // Fill time_low (32 bits) with the random input
        time_low = random_input;

        // Fill time_mid (16 bits) with shifted random bits
        time_mid = static_cast<uint16_t>((random_input >> 16) & 0xFFFF);

        // Fill time_hi_and_version (12 bits) with more shifted random bits
        // (upper 4 bits are already set to version 4)
        time_hi_and_version |= static_cast<uint16_t>((random_input >> 24) & 0x0FFF);

        // Fill clock sequence with remaining random bits
        clock_seq_hi_and_reserved |= static_cast<uint8_t>((random_input >> 28) & 0x3F);
        clock_seq_low = static_cast<uint8_t>((random_input >> 30) & 0xFF);

        // Fill node with remaining bits and additional entropy from the random input
        node[0] = static_cast<uint8_t>(random_input & 0xFF);
        node[1] = static_cast<uint8_t>((random_input >> 8) & 0xFF);
        node[2] = static_cast<uint8_t>((random_input >> 16) & 0xFF);
        node[3] = static_cast<uint8_t>((random_input >> 24) & 0xFF);
        node[4] = static_cast<uint8_t>((random_input >> 2) & 0xFF);
        node[5] = static_cast<uint8_t>((random_input >> 10) & 0xFF);
    }

    /**
     * @brief Convert UUID to string format (safe array version)
     *
     * Converts the UUID to the standard hyphenated string representation:
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * This version is thread-safe and does not allocate memory.
     *
     * @param output Reference to std::array to store the UUID string (36 chars + null terminator)
     */
    void to_string(std::array<char, 37>& output) const {
        snprintf(output.data(), output.size(),
                "%02x%02x%02x%02x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                static_cast<uint8_t>(time_low << 24),
                static_cast<uint8_t>(time_low << 16),
                static_cast<uint8_t>(time_low << 8),
                static_cast<uint8_t>(time_low),
                time_mid,
                time_hi_and_version,
                clock_seq_hi_and_reserved,
                clock_seq_low,
                node[0], node[1], node[2], node[3], node[4], node[5]);
    }

#ifdef UUID_STRING_SUPPORT_ENABLED
    /**
     * @brief Convert UUID to string format (std::string version)
     *
     * Converts the UUID to the standard hyphenated string representation:
     * xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
     *
     * This version returns a std::string for convenience but allocates memory.
     *
     * @return std::string containing the formatted UUID
     */
    std::string to_string() const {
        std::array<char, 37> buffer;
        to_string(buffer);
        return std::string(buffer.data());
    }
#endif

 private:
    /**
     * @brief Clear all UUID fields to zero
     */
    void clear_fields() {
        time_low = 0;
        time_mid = 0;
        time_hi_and_version = 0;
        clock_seq_hi_and_reserved = 0;
        clock_seq_low = 0;
        for (int i = 0; i < 6; i++) {
            node[i] = 0;
        }
    }
};

/**
 * @brief Type alias for UUID version 3 (name-based, MD5)
 *
 * Usage examples:
 * @code
 *   // Using std::array (thread-safe, no allocation)
 *   UUIDv3 uuid(12345);
 *   std::array<char, 37> buffer;
 *   uuid.to_string(buffer);
 *   printf("%s\n", buffer.data());
 *
 *   // Using std::string (if enabled, easier but allocates)
 *   std::string uuid_str = uuid.to_string();
 *   printf("%s\n", uuid_str.c_str());
 * @endcode
 */
using UUIDv3 = MyUuid<UuidVersion::NAME_BASED_MD5>;

/**
 * @brief Type alias for UUID version 4 (random)
 *
 * Usage examples:
 * @code
 *   // Generate UUID v4 from random value
 *   UUIDv4 uuid1(0x12345678, 0);
 *
 *   // Using std::array (thread-safe, no allocation)
 *   std::array<char, 37> buffer;
 *   uuid1.to_string(buffer);
 *   printf("%s\n", buffer.data());
 *
 *   // Using std::string (if enabled)
 *   std::string uuid_str = uuid1.to_string();
 *
 *   // Or create empty and generate later
 *   UUIDv4 uuid2;
 *   uuid2.generate_v4_from_random(esp_random());
 * @endcode
 */
using UUIDv4 = MyUuid<UuidVersion::RANDOM>;


}  // namespace uuid

#endif  // INCLUDE_UUID_UUID_H_
