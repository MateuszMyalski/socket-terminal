#ifndef SERVER_IDENTITY_HPP
#define SERVER_IDENTITY_HPP

#include <map>
#include <string>

#include "src/commands/command.hpp"
using namespace Commands;
namespace Server {

class Password {
   public:
    Password(std::string password = "");
    ~Password() = default;

    void regenerate_password();
    void set_password(std::string& new_password);
    std::string const& get_password() const;
    bool cmp_password(std::string& password) const;

   private:
    std::string password;
};

class Identity : protected Password {
   public:
    Identity(std::string username, std::string password,
             const CommandsMap& commands);
    ~Identity() = default;
    std::string const& get_username() const;
    bool check_password(std::string password) const;
    bool check_username(std::string username) const;

   private:
    std::string username;
    const CommandsMap& commands;
};
}
#endif