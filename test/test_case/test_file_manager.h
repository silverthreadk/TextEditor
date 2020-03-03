#include <gtest/gtest.h>

#include "file_manager.h"

class FileManagerTest : public testing::Test {
 protected:
    void SetUp() {
        file_manager_ = new FileManager(nullptr, &text_list_);
    }
    void TearDown() {
        delete file_manager_;
    }

    FileManager* file_manager_;
    std::list<std::list<char> > text_list_;
};

TEST_F(FileManagerTest, edit) {
    ASSERT_EQ(file_manager_->edit(), 0);
    ASSERT_EQ(text_list_.size(), 1);
    ASSERT_TRUE(text_list_.back().empty());
}

TEST_F(FileManagerTest, write) {
    ASSERT_EQ(file_manager_->write(), 1);
    ASSERT_EQ(file_manager_->changeFile("test.txt"), 0);
    ASSERT_EQ(file_manager_->edit(), 0);
    ASSERT_EQ(file_manager_->write(), 0);
}

TEST_F(FileManagerTest, changeFile) {
    ASSERT_EQ(file_manager_->changeFile(nullptr), 1);
    ASSERT_EQ(file_manager_->changeFile("test.txt"), 0);
}
