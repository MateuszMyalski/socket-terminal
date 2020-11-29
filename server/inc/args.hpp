#include <stdexcept>
#include <string>
#include <vector>

class ArgumentBase {
 protected:
  std::string name_;
  std::string short_name_;
  std::string man_ = "Description not found.";

 public:
  ArgumentBase(std::string name, std::string short_name)
      : name_(name), short_name_(short_name) {}

  std::string get_man() { return man_; }
  void set_man(std::string man) { man_ = man; }
  std::string get_short_name() { return short_name_; }
  std::string get_long_name() { return name_; }

  virtual void set_param(const std::vector<std::string> &args_list,
                         std::vector<std::string>::const_iterator list_it) = 0;
};

template <typename T>
class Argument;

template <>
class Argument<int> : public ArgumentBase {
 private:
  int arg_value_;

 public:
  Argument(std::string name, std::string short_name, int default_value)
      : ArgumentBase(name, short_name), arg_value_(default_value) {}
  int get_param() { return arg_value_; }

  void set_param(const std::vector<std::string> &args_list,
                 std::vector<std::string>::const_iterator list_it) {
    if (list_it == std::end(args_list))
      throw std::runtime_error("Not enough args");

    try {
      arg_value_ = std::stoi(*(list_it + 1), nullptr, 10);
    } catch (const std::exception &e) {
      throw std::runtime_error("Invalid parameter '" + *list_it + "'");
    }
  }
};

template <>
class Argument<bool> : public ArgumentBase {
 private:
  bool arg_value_;

 public:
  Argument(std::string name, std::string short_name, bool default_value)
      : ArgumentBase(name, short_name), arg_value_(default_value) {}
  bool get_param() { return arg_value_; }

  void set_param(const std::vector<std::string> &args_list,
                 std::vector<std::string>::const_iterator list_it) {
    arg_value_ = (*list_it == name_ || *list_it == short_name_);
  }
};

template <>
class Argument<std::string> : public ArgumentBase {
 private:
  std::string arg_value_;

 public:
  Argument(std::string name, std::string short_name, std::string default_value)
      : ArgumentBase(name, short_name), arg_value_(default_value) {}
  std::string get_param() { return arg_value_; }

  void set_param(const std::vector<std::string> &args_list,
                 std::vector<std::string>::const_iterator list_it) {
    arg_value_ = *(list_it + 1);
  }
};
