#pragma once

#include<string>
#include"../../lib/json.hpp"
#include <fstream>
#include <vector>
#include "../../src/string_conv.h"

#include "../../src/itemparser.h"
#include "../../src/extendablebitsitemparser.h"
#include "../../src/compounditemparser.h"
#include "../../src/fixedbitfielditemparser.h"
#include "../../src/fixedbytesitemparser.h"
#include "../../src/fixedbitsitemparser.h"
#include "../../src/optionalitemparser.h"

class CATMessageBase
{
public:
    CATMessageBase();
    virtual ~CATMessageBase();

    //Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() = 0;

private:
    //Decode First 4 Bytes (FSPEC)
    virtual size_t decodeHeader() = 0;

private:

};