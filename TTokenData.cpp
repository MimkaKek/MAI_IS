#include "TTokenData.hpp"

TTokenData::TTokenData(): files() {
    this->token = "";
    this->df    = 0;
    this->id    = 0;

    this->translation = "";
};

TTokenData::~TTokenData() {};

TTokenData::TTokenData(TTokenData& toCopy) {
    this->token  = toCopy.token;
    this->files  = toCopy.files;
    this->df     = toCopy.df;
    this->id     = toCopy.id;

    this->translation     = toCopy.translation;
}

TTokenData::TTokenData(std::string& token, std::string& translate): files() {
    this->token  = token;
    this->df     = 0;
    this->id     = 0;

    this->translation = translate;
}

TTokenData::TTokenData(std::string& token): files() {
    this->token  = token;
    this->df     = 0;
    this->id     = 0;

    this->translation = "";
}

std::ostream& operator<<(std::ostream& os, TTokenData& tokenData) {
    os << "Token: " << tokenData.token << std::endl;
    return os;
}