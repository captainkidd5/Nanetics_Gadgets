#ifndef AUTH
#define AUTH
#include <ArduinoJson.h>
class LoginRequestDTO {
public:
    String email;
    String password;

};
#endif