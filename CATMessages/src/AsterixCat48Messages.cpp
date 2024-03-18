#include "../include/AsterixCat48Messages.h"
#include <iostream>

AsterixCat48Messages::AsterixCat48Messages(){

};

AsterixCat48Messages::~AsterixCat48Messages(){

};

bool AsterixCat48Messages::decodeData(const char *data, json &cat48_json_data, json &cat48_definition)
{
    AsterixCatMessageBase::baseDecodeData(data, cat48_definition, cat48_items_order, cat48_uap_order, cat48_json_data);
    return true;
}

void AsterixCat48Messages::to_json(nlohmann::json &j)
{
}

void AsterixCat48Messages::from_json(Cat48Record &cat_data)
{
}
