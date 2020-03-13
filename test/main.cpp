#include <gtest/gtest.h>

#include "test_string_utils.h"
#include "test_file_manager.h"
#include "test_cursor.h"

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}