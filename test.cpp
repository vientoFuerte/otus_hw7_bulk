#include <gtest/gtest.h>
#include <sstream>
#include "bulk.h"

TEST(BulkTest, SimpleTest) {

    {
      // Сохраняем оригинальный буфер
      auto* old_buf = std::cout.rdbuf();
      
      // Тест 1
      std::stringstream buf1;
      std::cout.rdbuf(buf1.rdbuf());
      std::vector<std::string> commands = {"cmd1", "cmd2", "cmd3"};

for (const auto& cmd : commands) {
  cmd_parser(3, cmd); 

}
      //cmd_parser(3, commands); 
      EXPECT_EQ(buf1.str(), "bulk : cmd1, cmd2, cmd3\n");
      std::cout.rdbuf(old_buf); 
    }
    
    // Тест 2 
    {
      auto* old_buf = std::cout.rdbuf();
      std::stringstream buf2;
      std::cout.rdbuf(buf2.rdbuf());
      std::vector<std::string> commands2 = {"cmd1", "cmd2", "cmd3", "cmd4", "cmd5"};
      
      //cmd_parser(3, commands2); 
        for (const auto& cmd : commands2) {
        //cmd_pool.push_back(cmd);  
        cmd_parser(3, cmd); 
      }
      EXPECT_EQ(buf2.str(), "bulk : cmd1, cmd2, cmd3\nbulk : cmd4, cmd5\n");

      // Восстанавливаем буфер
      std::cout.rdbuf(old_buf);
    }
}

TEST(BulkTest, DynamicBlock) {
    // Сохраняем оригинальный буфер
    auto* old_buf = std::cout.rdbuf();
    
    // Тест 1
    std::stringstream buf1;
    std::cout.rdbuf(buf1.rdbuf());
    std::vector<std::string> commands = {
        "{",
        "cmd1",
        "cmd2",
        "}",
        "cmd3", "cmd4", "cmd5"
    };
    
    for (const auto& cmd : commands){
      cmd_parser(3, cmd); 
    }
    
    EXPECT_EQ(buf1.str(), "bulk : cmd1, cmd2\nbulk : cmd3, cmd4, cmd5\n");
 
    // Восстанавливаем буфер
    std::cout.rdbuf(old_buf);
}

