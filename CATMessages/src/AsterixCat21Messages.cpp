#include "../include/AsterixCat21Messages.h"
#include <iostream>

AsterixCat21Messages::AsterixCat21Messages(){

};

AsterixCat21Messages::~AsterixCat21Messages(){

};

bool AsterixCat21Messages::decodeData(const char *data, json &cat21_json_data)
{
    json cat21_definition = nlohmann::json::parse(std::ifstream("../CATMessages/categories-definitions/cat021_0.26.json"));
    AsterixCatMessageBase::baseDecodeData(data, cat21_definition, cat21_items_order, cat21_uap_order, cat21_json_data);
    return true;
}

void AsterixCat21Messages::to_json(nlohmann::json &j)
{
}

void AsterixCat21Messages::from_json(Cat21Record &cat_data)
{
}