#include <assert.h>
#include <stdio.h>

#include <utf8-validate.h>

#define TEST_ASSERT(expr) \
	assert(expr);

#define RUN_TEST(test)                  \
{                                       \
	printf("running %s\n", # test); \
	test();                         \
}

/* Test data:
 * http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
 */

void test_kosme_validate_mem()
{
	char data[] = "κόσμε";
	TEST_ASSERT(utf8_validate_mem(data, sizeof(data) - 1));
}

void test_kosme_validate_some()
{
	char data[] = "κόσμε";
	utf8_state state = 0;
	TEST_ASSERT(utf8_validate_some(&state, data, sizeof(data) - 1));
	TEST_ASSERT(state == 0);
}

void test_kosme_validate_str()
{
	char data[] = "κόσμε";
	TEST_ASSERT(utf8_validate_str(data));
}

void test_kosme_validate()
{
	char data[] = "κόσμε";
	utf8_state state = 0;
	for (size_t i = 0; i < (sizeof(data) - 1); ++i) {
		TEST_ASSERT(utf8_validate(&state, data[i]));
	}
	TEST_ASSERT(state == 0);
}

void test_dbus_valid()
{
	// test data from the dbus tests

	char data0[] = "";
	TEST_ASSERT(utf8_validate_mem(data0, sizeof(data0) - 1));

	char data1[] = "\xc2\xa9";     /* UTF-8 (c) symbol */
	TEST_ASSERT(utf8_validate_mem(data1, sizeof(data1) - 1));

        char data2[] = "\xef\xbf\xbe"; /* U+FFFE is reserved but Corrigendum 9 says it's OK */
	TEST_ASSERT(utf8_validate_mem(data2, sizeof(data2) - 1));
}

void test_dbus_invalid()
{
	// test data from the dbus tests

	char data0[] = "\xa9"; /* Latin-1 (c) symbol */
	TEST_ASSERT(!utf8_validate_mem(data0, sizeof(data0) - 1));

	char data1[] = "\xed\xa0\x80";   /* UTF-16 surrogates are not valid in UTF-8 */
	TEST_ASSERT(!utf8_validate_mem(data1, sizeof(data1) - 1));
}

void test_bom()
{
	char data[] = "\xef\xbb\xbf";
	TEST_ASSERT(utf8_validate_mem(data, sizeof(data) -1));
}

int main()
{
	RUN_TEST(test_kosme_validate);
	RUN_TEST(test_kosme_validate_mem);
	RUN_TEST(test_kosme_validate_some);
	RUN_TEST(test_kosme_validate_some);
	RUN_TEST(test_kosme_validate_str);
	RUN_TEST(test_dbus_valid);
	RUN_TEST(test_dbus_invalid);
	RUN_TEST(test_bom);
	return 0;
}
