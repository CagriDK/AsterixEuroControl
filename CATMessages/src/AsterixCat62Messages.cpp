#include "../include/AsterixCat62Messages.h"
#include <iostream>

AsterixCat62Messages::AsterixCat62Messages(){

};

AsterixCat62Messages::~AsterixCat62Messages(){

};

bool AsterixCat62Messages::decodeData(const char *data, json &cat62_json_data, json &cat62_definition, bool use_sample, json &cat62_sample)
{
    AsterixCatMessageBase::baseDecodeData(data, cat62_definition, cat62_items_order, cat62_uap_order, cat62_json_data, use_sample, cat62_sample);
    return true;
}

void AsterixCat62Messages::to_json(nlohmann::json &j)
{
}

void AsterixCat62Messages::from_json(Cat62Record &cat_data)
{
}
