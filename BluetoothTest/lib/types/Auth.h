#ifndef AUTH
#define AUTH
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
class LoginRequestDTO {
public:
    String email;
    String password;

};
#endif