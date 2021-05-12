#include "src/server/identity.hpp"

#include <algorithm>

namespace Server {
Identity::Identity(std::string username, std::string password)
    : username(username) {
    set_password(password);
};

std::string const& Identity::get_username() const { return username; };

bool Identity::check_password(std::string password) const {
    remove_if(password.begin(), password.end(), isspace);
    return cmp_password(password);
}

bool Identity::check_username(std::string username) const {
    remove_if(username.begin(), username.end(), isspace);
    return this->username.compare(username) == 0;
}

Password::Password(std::string password) { set_password(password); };

void Password::regenerate_password(){
    // TODO(Mateusz) Password will be cosnt,
    // but some algos can
    // be provided fro eg. password change every x sec. and the
    // algo is know for future users.
    // may use lambda for that?
};

void Password::set_password(std::string& new_password) {
    remove_if(new_password.begin(), new_password.end(), isspace);
    password = new_password;
};
std::string const& Password::get_password() const { return password; };

bool Password::cmp_password(std::string& password) const {
    return (this->password.compare(password) == 0) ||
           (this->password.compare("") == 0);
};
}