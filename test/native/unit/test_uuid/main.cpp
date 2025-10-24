/**
 * @file main.cpp
 * @author Jennifer Gott
 * @brief Unit tests for UUID generation (deterministic and random)
 * @version 0.1
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025, released under the MIT license
 */

#include <unity.h>
#include <array>
#include <string>
#include <cstring>
#include "uuid/uuid.h"

// Test that UUIDv3 generates deterministic output from same input
void test_uuid_v3_deterministic_generation() {
    // Arrange
    constexpr uint32_t input = 0x12345678;

    // Act
    uuid::UUIDv3 uuid1(input);
    uuid::UUIDv3 uuid2(input);

    // Assert - same input should produce identical UUIDs
    TEST_ASSERT_EQUAL_UINT32(uuid1.time_low, uuid2.time_low);
    TEST_ASSERT_EQUAL_UINT16(uuid1.time_mid, uuid2.time_mid);
    TEST_ASSERT_EQUAL_UINT16(uuid1.time_hi_and_version, uuid2.time_hi_and_version);
    TEST_ASSERT_EQUAL_UINT8(uuid1.clock_seq_hi_and_reserved, uuid2.clock_seq_hi_and_reserved);
    TEST_ASSERT_EQUAL_UINT8(uuid1.clock_seq_low, uuid2.clock_seq_low);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(uuid1.node, uuid2.node, 6);
}

// Test that UUIDv3 generates different output from different inputs
void test_uuid_v3_different_inputs_different_outputs() {
    // Arrange
    constexpr uint32_t input1 = 0x12345678;
    constexpr uint32_t input2 = 0x87654321;

    // Act
    uuid::UUIDv3 uuid1(input1);
    uuid::UUIDv3 uuid2(input2);

    // Assert - different inputs should produce different UUIDs
    bool is_different = (uuid1.time_low != uuid2.time_low) ||
                       (uuid1.time_mid != uuid2.time_mid) ||
                       (uuid1.time_hi_and_version != uuid2.time_hi_and_version) ||
                       (uuid1.clock_seq_hi_and_reserved != uuid2.clock_seq_hi_and_reserved) ||
                       (uuid1.clock_seq_low != uuid2.clock_seq_low) ||
                       (memcmp(uuid1.node, uuid2.node, 6) != 0);

    TEST_ASSERT_TRUE(is_different);
}

// Test that UUIDv3 has correct version bits (version 3)
void test_uuid_v3_version_bits() {
    // Arrange
    constexpr uint32_t input = 0x12345678;

    // Act
    uuid::UUIDv3 uuid(input);

    // Assert - version should be 3 (bits 12-15 of time_hi_and_version)
    uint8_t version = (uuid.time_hi_and_version >> 12) & 0x0F;
    TEST_ASSERT_EQUAL_UINT8(3, version);
}

// Test that UUIDv3 has correct variant bits (RFC 4122)
void test_uuid_v3_variant_bits() {
    // Arrange
    constexpr uint32_t input = 0x12345678;

    // Act
    uuid::UUIDv3 uuid(input);

    // Assert - variant should be 10b (bits 6-7 of clock_seq_hi_and_reserved)
    uint8_t variant = (uuid.clock_seq_hi_and_reserved >> 6) & 0x03;
    TEST_ASSERT_EQUAL_UINT8(0x02, variant);  // 0b10
}

// Test UUIDv3 string output format with std::array
void test_uuid_v3_string_format_array() {
    // Arrange
    constexpr uint32_t input = 0x12345678;
    uuid::UUIDv3 uuid(input);
    std::array<char, 37> buffer;

    // Act
    uuid.to_string(buffer);

    // Assert
    // Check length (36 chars + null terminator)
    TEST_ASSERT_EQUAL_size_t(36, strlen(buffer.data()));

    // Check format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    TEST_ASSERT_EQUAL_CHAR('-', buffer[8]);
    TEST_ASSERT_EQUAL_CHAR('-', buffer[13]);
    TEST_ASSERT_EQUAL_CHAR('-', buffer[18]);
    TEST_ASSERT_EQUAL_CHAR('-', buffer[23]);
    TEST_ASSERT_EQUAL_CHAR('\0', buffer[36]);

    // Check version digit (should be '3')
    TEST_ASSERT_EQUAL_CHAR('3', buffer[14]);
}

// Test UUIDv3 string output format with std::string
void test_uuid_v3_string_format_std_string() {
    // Arrange
    constexpr uint32_t input = 0x12345678;
    uuid::UUIDv3 uuid(input);

    // Act
    std::string uuid_str = uuid.to_string();

    // Assert
    // Check length
    TEST_ASSERT_EQUAL_size_t(36, uuid_str.length());

    // Check format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    TEST_ASSERT_EQUAL_CHAR('-', uuid_str[8]);
    TEST_ASSERT_EQUAL_CHAR('-', uuid_str[13]);
    TEST_ASSERT_EQUAL_CHAR('-', uuid_str[18]);
    TEST_ASSERT_EQUAL_CHAR('-', uuid_str[23]);

    // Check version digit (should be '3')
    TEST_ASSERT_EQUAL_CHAR('3', uuid_str[14]);
}

// Test that both string methods produce identical output
void test_uuid_v3_string_methods_consistency() {
    // Arrange
    constexpr uint32_t input = 0xABCDEF01;
    uuid::UUIDv3 uuid(input);

    // Act
    std::array<char, 37> buffer;
    uuid.to_string(buffer);
    std::string uuid_str = uuid.to_string();

    // Assert
    TEST_ASSERT_EQUAL_STRING(buffer.data(), uuid_str.c_str());
}

// Test UUIDv3 with edge case: zero input
void test_uuid_v3_zero_input() {
    // Arrange
    constexpr uint32_t input = 0x00000000;

    // Act
    uuid::UUIDv3 uuid(input);

    // Assert - version and variant bits should still be set correctly
    uint8_t version = (uuid.time_hi_and_version >> 12) & 0x0F;
    uint8_t variant = (uuid.clock_seq_hi_and_reserved >> 6) & 0x03;
    TEST_ASSERT_EQUAL_UINT8(3, version);
    TEST_ASSERT_EQUAL_UINT8(0x02, variant);
}

// Test UUIDv3 with edge case: max input
void test_uuid_v3_max_input() {
    // Arrange
    constexpr uint32_t input = 0xFFFFFFFF;

    // Act
    uuid::UUIDv3 uuid(input);

    // Assert - version and variant bits should still be set correctly
    uint8_t version = (uuid.time_hi_and_version >> 12) & 0x0F;
    uint8_t variant = (uuid.clock_seq_hi_and_reserved >> 6) & 0x03;
    TEST_ASSERT_EQUAL_UINT8(3, version);
    TEST_ASSERT_EQUAL_UINT8(0x02, variant);
}

// Test UUIDv4 has correct version bits (version 4)
void test_uuid_v4_version_bits() {
    // Arrange
    constexpr uint32_t random_input = 0x12345678;

    // Act
    uuid::UUIDv4 uuid(random_input, 0);

    // Assert - version should be 4 (bits 12-15 of time_hi_and_version)
    uint8_t version = (uuid.time_hi_and_version >> 12) & 0x0F;
    TEST_ASSERT_EQUAL_UINT8(4, version);
}

// Test UUIDv4 has correct variant bits (RFC 4122)
void test_uuid_v4_variant_bits() {
    // Arrange
    constexpr uint32_t random_input = 0x12345678;

    // Act
    uuid::UUIDv4 uuid(random_input, 0);

    // Assert - variant should be 10b (bits 6-7 of clock_seq_hi_and_reserved)
    uint8_t variant = (uuid.clock_seq_hi_and_reserved >> 6) & 0x03;
    TEST_ASSERT_EQUAL_UINT8(0x02, variant);  // 0b10
}

// Test UUIDv4 string format
void test_uuid_v4_string_format() {
    // Arrange
    constexpr uint32_t random_input = 0x12345678;
    uuid::UUIDv4 uuid(random_input, 0);
    std::array<char, 37> buffer;

    // Act
    uuid.to_string(buffer);

    // Assert
    // Check format
    TEST_ASSERT_EQUAL_size_t(36, strlen(buffer.data()));
    TEST_ASSERT_EQUAL_CHAR('-', buffer[8]);
    TEST_ASSERT_EQUAL_CHAR('-', buffer[13]);
    TEST_ASSERT_EQUAL_CHAR('-', buffer[18]);
    TEST_ASSERT_EQUAL_CHAR('-', buffer[23]);

    // Check version digit (should be '4')
    TEST_ASSERT_EQUAL_CHAR('4', buffer[14]);
}

// Test UUIDv4 default constructor
void test_uuid_v4_default_constructor() {
    // Arrange & Act
    uuid::UUIDv4 uuid;

    // Assert - all fields should be zero initially
    TEST_ASSERT_EQUAL_UINT32(0, uuid.time_low);
    TEST_ASSERT_EQUAL_UINT16(0, uuid.time_mid);
    TEST_ASSERT_EQUAL_UINT16(0, uuid.time_hi_and_version);
    TEST_ASSERT_EQUAL_UINT8(0, uuid.clock_seq_hi_and_reserved);
    TEST_ASSERT_EQUAL_UINT8(0, uuid.clock_seq_low);
}

// Test UUIDv4 generate_v4_from_random method
void test_uuid_v4_generate_from_random() {
    // Arrange
    uuid::UUIDv4 uuid;
    constexpr uint32_t random_input = 0xABCDEF01;

    // Act
    uuid.generate_v4_from_random(random_input);

    // Assert - version and variant should be set
    uint8_t version = (uuid.time_hi_and_version >> 12) & 0x0F;
    uint8_t variant = (uuid.clock_seq_hi_and_reserved >> 6) & 0x03;
    TEST_ASSERT_EQUAL_UINT8(4, version);
    TEST_ASSERT_EQUAL_UINT8(0x02, variant);
}

// Test that same random input to UUIDv4 produces deterministic output
void test_uuid_v4_deterministic_with_same_random_input() {
    // Arrange
    constexpr uint32_t random_input = 0x12345678;

    // Act
    uuid::UUIDv4 uuid1(random_input, 0);
    uuid::UUIDv4 uuid2(random_input, 0);

    // Assert - same random input should produce identical UUIDs
    TEST_ASSERT_EQUAL_UINT32(uuid1.time_low, uuid2.time_low);
    TEST_ASSERT_EQUAL_UINT16(uuid1.time_mid, uuid2.time_mid);
    TEST_ASSERT_EQUAL_UINT16(uuid1.time_hi_and_version, uuid2.time_hi_and_version);
    TEST_ASSERT_EQUAL_UINT8(uuid1.clock_seq_hi_and_reserved, uuid2.clock_seq_hi_and_reserved);
    TEST_ASSERT_EQUAL_UINT8(uuid1.clock_seq_low, uuid2.clock_seq_low);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(uuid1.node, uuid2.node, 6);
}

// Test that different random inputs produce different UUIDs
void test_uuid_v4_different_inputs_different_outputs() {
    // Arrange
    constexpr uint32_t random_input1 = 0x12345678;
    constexpr uint32_t random_input2 = 0x87654321;

    // Act
    uuid::UUIDv4 uuid1(random_input1, 0);
    uuid::UUIDv4 uuid2(random_input2, 0);

    // Assert - different inputs should produce different UUIDs
    bool is_different = (uuid1.time_low != uuid2.time_low) ||
                       (uuid1.time_mid != uuid2.time_mid) ||
                       (uuid1.time_hi_and_version != uuid2.time_hi_and_version) ||
                       (uuid1.clock_seq_hi_and_reserved != uuid2.clock_seq_hi_and_reserved) ||
                       (uuid1.clock_seq_low != uuid2.clock_seq_low) ||
                       (memcmp(uuid1.node, uuid2.node, 6) != 0);

    TEST_ASSERT_TRUE(is_different);
}

// Test UUIDv3 and UUIDv4 with same input produce different UUIDs
void test_uuid_v3_and_v4_different() {
    // Arrange
    constexpr uint32_t input = 0x12345678;

    // Act
    uuid::UUIDv3 uuid3(input);
    uuid::UUIDv4 uuid4(input, 0);

    // Assert - they should have different version numbers at minimum
    uint8_t version3 = (uuid3.time_hi_and_version >> 12) & 0x0F;
    uint8_t version4 = (uuid4.time_hi_and_version >> 12) & 0x0F;

    TEST_ASSERT_EQUAL_UINT8(3, version3);
    TEST_ASSERT_EQUAL_UINT8(4, version4);
    TEST_ASSERT_NOT_EQUAL(version3, version4);
}

// Test multiple sequential UUIDv3 generation for consistency
void test_uuid_v3_sequential_generation_consistency() {
    // Arrange
    constexpr uint32_t base_input = 1000;
    constexpr int count = 10;
    uuid::UUIDv3 uuids[count];

    // Act - generate UUIDs from sequential inputs
    for (int i = 0; i < count; i++) {
        uuids[i] = uuid::UUIDv3(base_input + i);
    }

    // Assert - all should have correct version and be unique
    for (int i = 0; i < count; i++) {
        uint8_t version = (uuids[i].time_hi_and_version >> 12) & 0x0F;
        TEST_ASSERT_EQUAL_UINT8(3, version);

        // Check uniqueness against previous UUIDs
        for (int j = 0; j < i; j++) {
            bool is_different = (uuids[i].time_low != uuids[j].time_low) ||
                               (uuids[i].time_mid != uuids[j].time_mid) ||
                               (uuids[i].time_hi_and_version != uuids[j].time_hi_and_version);
            TEST_ASSERT_TRUE(is_different);
        }
    }
}

int main(int argc, char const *argv[]) {
    UNITY_BEGIN();

    // UUIDv3 tests
    RUN_TEST(test_uuid_v3_deterministic_generation);
    RUN_TEST(test_uuid_v3_different_inputs_different_outputs);
    RUN_TEST(test_uuid_v3_version_bits);
    RUN_TEST(test_uuid_v3_variant_bits);
    RUN_TEST(test_uuid_v3_string_format_array);
    RUN_TEST(test_uuid_v3_string_format_std_string);
    RUN_TEST(test_uuid_v3_string_methods_consistency);
    RUN_TEST(test_uuid_v3_zero_input);
    RUN_TEST(test_uuid_v3_max_input);
    RUN_TEST(test_uuid_v3_sequential_generation_consistency);

    // UUIDv4 tests
    RUN_TEST(test_uuid_v4_version_bits);
    RUN_TEST(test_uuid_v4_variant_bits);
    RUN_TEST(test_uuid_v4_string_format);
    RUN_TEST(test_uuid_v4_default_constructor);
    RUN_TEST(test_uuid_v4_generate_from_random);
    RUN_TEST(test_uuid_v4_deterministic_with_same_random_input);
    RUN_TEST(test_uuid_v4_different_inputs_different_outputs);

    // Cross-version tests
    RUN_TEST(test_uuid_v3_and_v4_different);

    return UNITY_END();
}

