#include "User.h"
#include "Account.h"

// Base User Constructor
User::User(const std::string& id, const std::string& name) 
    : userID(id), name(name), account(new Account()) {}

// Virtual Destructor
User::~User() = default;
