#include <Arduino.h>
#include "Pixel.h"
#include <unity.h>

using namespace Pixel;

ColorHelper colorHelper;

// Unity generated initial setup function
// void setUp(void) {
// // set stuff up here
// }

// Unity generated after action function
// void tearDown(void) {
// // clean stuff up here
// }

void test_initialize_strip_check(void)
{
    TEST_ASSERT_EQUAL(0, Pixels.ReadPixel(0).r);
}

void test_get_rcolorop(void)
{
    Color testColor = {130, 130, 130};
    TEST_ASSERT_EQUAL((130 / 255), colorHelper.GetColorOp(testColor).r);
}

void test_set_rcolorop(void)
{
    Color baseColor = {130, 130, 130};
    ColorOp testColorOp = colorHelper.GetColorOp(baseColor);
    TEST_ASSERT_EQUAL(130, colorHelper.SetColorOp(testColorOp).r);
}

void test_merge_rpixel_normal(void)
{
    Color baseColor = {130,130,130};
    Color newColor = {26,26,26};

    TEST_ASSERT_EQUAL(78, colorHelper.MergePixel(baseColor, newColor, 0.5, Normal).r);
}

void test_merge_rpixel_add(void)
{
    Color baseColor = {130,130,130};
    Color newColor = {26,26,26};

    TEST_ASSERT_EQUAL(143, colorHelper.MergePixel(baseColor, newColor, 0.5, Add).r);
}

void test_merge_rpixel_multiply(void)
{
    Color baseColor = {130,130,130};
    Color newColor = {26,26,26};

    TEST_ASSERT_INT_WITHIN(1, 71, colorHelper.MergePixel(baseColor, newColor, 0.5, Multiply).r);
}

void test_merge_rpixel_multiply_alt1(void)
{
    Color baseColor = {46,121,36};
    Color newColor = {38,77,203};

    TEST_ASSERT_INT_WITHIN(1, 34, colorHelper.MergePixel(baseColor, newColor, 0.3, Multiply).r);
}

void test_merge_rpixel_multiply_alt2(void)
{
    Color baseColor = {121,86,36};
    Color newColor = {118,227,141};

    TEST_ASSERT_INT_WITHIN(1, 56, colorHelper.MergePixel(baseColor, newColor, 1, Multiply).r);
}

void test_merge_rpixel_screen(void)
{
    Color baseColor = {130,130,130};
    Color newColor = {26,26,26};

    TEST_ASSERT_INT_WITHIN(1, 137, colorHelper.MergePixel(baseColor, newColor, 0.5, Screen).r);
}

void test_merge_rpixel_screen_alt1(void)
{
    Color baseColor = {46,121,36};
    Color newColor = {38,77,203};

    TEST_ASSERT_INT_WITHIN(1, 55, colorHelper.MergePixel(baseColor, newColor, 0.3, Screen).r);
}

void test_merge_rpixel_screen_alt2(void)
{
    Color baseColor = {121,86,36};
    Color newColor = {118,227,141};

    TEST_ASSERT_INT_WITHIN(1, 183, colorHelper.MergePixel(baseColor, newColor, 1, Screen).r);
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!

    Pixels.begin();
    
    RUN_TEST(test_initialize_strip_check);
    RUN_TEST(test_get_rcolorop);
    RUN_TEST(test_set_rcolorop);
    RUN_TEST(test_merge_rpixel_normal);
    RUN_TEST(test_merge_rpixel_add);
    RUN_TEST(test_merge_rpixel_multiply);
    RUN_TEST(test_merge_rpixel_multiply_alt1);
    RUN_TEST(test_merge_rpixel_multiply_alt2);
    RUN_TEST(test_merge_rpixel_screen);
    RUN_TEST(test_merge_rpixel_screen_alt1);
    RUN_TEST(test_merge_rpixel_screen_alt2);
}

void loop() {
    UNITY_END(); // stop unit testing
}