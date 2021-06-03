#include "server/identity.hpp"

#include "commands/command.hpp"
#include "gtest/gtest.h"

using namespace Server;
namespace {
class IdentityTest : public ::testing::Test {
   public:
    std::shared_ptr<Identity> user;

    std::string username = "user1";
    std::string password = "uSeR1";

    void SetUp() override {
        CommandsMap cmd_set{
            {"debug", std::shared_ptr<Command>{std::make_shared<Command>()}}};
        user = std::make_shared<Identity>(username, password, cmd_set);
    };
};

class PasswordTest : public ::testing::Test {
   public:
    std::shared_ptr<Password> password;

    void SetUp() override { password = std::make_shared<Password>(""); };
};

TEST_F(IdentityTest, check_password) {
    ASSERT_TRUE(user->check_password(password));
    ASSERT_FALSE(user->check_password("xxx"));
}

TEST_F(IdentityTest, check_username) {
    ASSERT_TRUE(user->check_username(username));
    ASSERT_FALSE(user->check_password("xxx"));
}

TEST_F(IdentityTest, command_map) {
    auto cmd = user->get_command_map();
    ASSERT_TRUE(cmd.find("debug") != cmd.end());
    ASSERT_TRUE(cmd.find("xxx") == cmd.end());
}

TEST_F(PasswordTest, any_password_valid) {
    std::string pass = "";
    ASSERT_TRUE(password->cmp_password(pass));

    pass = "xxx";
    ASSERT_TRUE(password->cmp_password(pass));
}

TEST_F(PasswordTest, set_password) {
    std::string pass = "mYpAssWord";
    password->set_password(pass);

    ASSERT_TRUE(password->cmp_password(pass));

    pass = "xxx";
    ASSERT_FALSE(password->cmp_password(pass));
}

TEST_F(PasswordTest, get_password) {
    std::string pass = "mYpAssWord";
    password->set_password(pass);

    ASSERT_TRUE(password->get_password() == pass);
}
}