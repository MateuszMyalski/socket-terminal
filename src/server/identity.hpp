#ifndef SERVER_IDENTITY_HPP
#define SERVER_IDENTITY_HPP
#include <string>

namespace Server {

class Password {
   public:
    Password(std::string password = "");
    ~Password() = default;

    void regenerate_password();
    void set_password(std::string& new_password);
    std::string get_password();
    bool cmp_password(std::string& password);

   private:
    std::string password;
};

class Identity : protected Password {
   public:
    Identity(std::string username, std::string password);
    ~Identity() = default;

   private:
    std::string username;
};
}
#endif