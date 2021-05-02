#include "src/server/identity.hpp"
namespace Server {
Identity::Identity(std::string username, std::string password)
    : username(username) {
    set_password(password);
};

std::string Identity::get_username() { return username; };

Password::Password(std::string password) { set_password(password); };

void Password::regenerate_password(){
    // TODO(Mateusz) Password will be cosnt,
    // but some algos can
    // be provided fro eg. password change every x sec. and the
    // algo is know for future users.
    // may use lambda for that?
};

void Password::set_password(std::string& new_password) {
    password = new_password;
};
std::string Password::get_password() { return password; };

bool Password::cmp_password(std::string& password) {
    return (this->password == password) || (this->password == "");
};
}