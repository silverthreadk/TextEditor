#include <gtest/gtest.h>

#include "cursor.h"
#include "file_manager.h"

class CursorTest : public testing::Test {
 protected:
    void SetUp() {
        file_manager_ = new FileManager(nullptr, &text_list_);
        file_manager_->edit();
        cursor_ = new Cursor(text_list_.begin(), (text_list_.begin())->begin());
    }
    void TearDown() {
        delete file_manager_;
        delete cursor_;
    }

    FileManager* file_manager_;
    Cursor* cursor_;
    std::list<std::list<char> > text_list_;
};

TEST_F(CursorTest, moveCursorInNewDocument) {
    ASSERT_EQ(cursor_->moveToLeft(), 1);
    ASSERT_EQ(cursor_->moveToRight(), 1);
    ASSERT_EQ(cursor_->moveToUp(), 1);
    ASSERT_EQ(cursor_->moveToDown(text_list_.size()), 1);
    ASSERT_EQ(cursor_->moveToBeginning(), 1);
    ASSERT_EQ(cursor_->moveToEnd(), 1);
}
