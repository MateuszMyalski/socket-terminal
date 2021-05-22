#include "commands/command_dispatcher.hpp"

#include <algorithm>

#include "commands/command.hpp"
#include "gtest/gtest.h"

using namespace Commands;
namespace {

TEST(command_dispatcher_test, parse_querry) {
    CommandDispatcher command_dispatcher(nullptr);
    std::vector<std::string> ref_args = {"ABC", "def", "gh", "ij", "kl"};

    std::string querry_a("ABC def gh ij kl");
    std::vector<std::string> args_a;
    command_dispatcher.parse_querry(querry_a, args_a);
    EXPECT_EQ(ref_args, args_a);

    std::string querry_b("ABC \"def\" \"gh\" ij kl");
    std::vector<std::string> args_b;
    command_dispatcher.parse_querry(querry_b, args_b);
    EXPECT_EQ(ref_args, args_b);
}

TEST(command_dispatcher_test, parse_querry_with_escape) {
    CommandDispatcher command_dispatcher(nullptr);
    std::vector<std::string> ref_args = {"ABC", "def", "g\\h", "ij", "kl"};
    std::replace(ref_args[1].begin(), ref_args[1].end(), '}', '"');

    std::string querry_a("ABC def g\\}h ij kl");
    std::replace(querry_a.begin(), ref_args[1].end(), '}', '\\');
    std::vector<std::string> args_a;
    command_dispatcher.parse_querry(querry_a, args_a);
    EXPECT_EQ(ref_args, args_a);
}

TEST(command_dispatcher_test, register_commands) {
    std::string ctx;
    CommandDispatcher command_dispatcher(static_cast<void *>(&ctx));

    std::vector<std::string> valid_args = {"cmd", "abc", "defg"};
    auto command = std::shared_ptr<Command>{std::make_shared<Command>()};

    command_dispatcher.register_command("cmd", command);
    bool found = command_dispatcher.dispatch(valid_args);

    EXPECT_TRUE(found);
    EXPECT_EQ("cmd abc defg", ctx);

    std::vector<std::string> invalid_args = {"xxx", "cmd", "defg"};
    found = command_dispatcher.dispatch(invalid_args);
    EXPECT_FALSE(found);
}

TEST(command_dispatcher_test, register_multiple_commands) {
    std::string ctx;
    CommandDispatcher command_dispatcher(static_cast<void *>(&ctx));

    CommandsMap commands;
    std::vector<std::string> args = {"cmd", "abc", "defg"};
    commands["cmd_1"] = std::shared_ptr<Command>{std::make_shared<Command>()};
    commands["cmd_2"] = std::shared_ptr<Command>{std::make_shared<Command>()};
    commands["cmd_3"] = std::shared_ptr<Command>{std::make_shared<Command>()};

    command_dispatcher.register_multiple_command(commands);

    args[0] = "cmd_1";
    ctx.clear();
    bool found = command_dispatcher.dispatch(args);
    EXPECT_TRUE(found);
    EXPECT_EQ("cmd_1 abc defg", ctx);

    args[0] = "cmd_2";
    ctx.clear();
    found = command_dispatcher.dispatch(args);
    EXPECT_TRUE(found);
    EXPECT_EQ("cmd_2 abc defg", ctx);

    args[0] = "cmd_3";
    ctx.clear();
    found = command_dispatcher.dispatch(args);
    EXPECT_TRUE(found);
    EXPECT_EQ("cmd_3 abc defg", ctx);

    args[0] = "invalid_cmd";
    ctx.clear();
    found = command_dispatcher.dispatch(args);
    EXPECT_FALSE(found);
}
}