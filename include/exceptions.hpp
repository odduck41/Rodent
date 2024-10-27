#include <exception>
#include <string>

class BadFlag : public std::exception {
  public:
    explicit BadFlag(const char* data) {
        message_ += data;
    }

    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }

  private:
    std::string message_{"Incorrect flag: "};
};