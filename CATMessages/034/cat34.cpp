#include "cat34.h"
#include <iostream>

CAT34::CAT34(nlohmann::json cat_def, const char *data)
{
    m_cat_definition = cat_def;
    m_data = data;
};

CAT34::~CAT34(){

};

bool CAT34::decodeData()
{
    decodeHeader();

    for (auto decodeBytes : header_info.uap_list)
    {
        ItemParser testParser(m_cat_definition["items"][CAT34_items_order.at(decodeBytes)]);
        parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
        std::cout << mapping[decodeBytes].dump(4) << std::endl;
    }

    to_json(mapping);
    from_json(cat34_message);

    return true;
}

size_t CAT34::decodeHeader()
{
    header_info.cat_type = static_cast<int>(static_cast<unsigned char>(m_data[0]));
    header_info.message_len = (static_cast<unsigned char>(m_data[1]) << 8) | static_cast<unsigned char>(m_data[2]);

    std::vector<bool> binaryRepresentation;
    size_t byteIndex = 3;

    bool moreBytes;
    do
    {
        std::bitset<8> fspecBits(m_data[byteIndex]);
        moreBytes = fspecBits[0]; // En sağdaki bit, daha fazla FSPEC byte'ı olup olmadığını belirtir

        for (size_t bitIndex = 0; bitIndex < 8; ++bitIndex)
        {
            binaryRepresentation.push_back(fspecBits[bitIndex]); // FSPEC'in geri kalan bitlerini oku
        }

        ++byteIndex;
    } while (moreBytes);

    // UAP list ile karşılaştır, fspec alanını, octet dizilimi ters olduğu için reverse yaptım.
    std::reverse(binaryRepresentation.begin(), binaryRepresentation.end());
    for (auto val = 0; val < binaryRepresentation.size(); val++)
    {
        if (binaryRepresentation[val] == true)
        {

            header_info.uap_list.push_back(CAT34_uap_order[val]);
        }
    }
    parsedBytes += byteIndex;
    return 0;
}

void CAT34::to_json(nlohmann::json &j)
{
    copy_to_Json(j, cat34_json, "000");
    copy_to_Json(j, cat34_json, "010");
    copy_to_Json(j, cat34_json, "020");
    copy_to_Json(j, cat34_json, "030");
    copy_to_Json(j, cat34_json, "041");
    copy_to_Json(j, cat34_json, "050");
    copy_to_Json(j, cat34_json, "060");
    copy_to_Json(j, cat34_json, "070");
    copy_to_Json(j, cat34_json, "090");
    copy_to_Json(j, cat34_json, "100");
    copy_to_Json(j, cat34_json, "110");
    copy_to_Json(j, cat34_json, "120");
}

void CAT34::from_json(Cat34Record &cat_data)
{
    copy_from_Json(cat34_json["000"], "Message Type", cat_data.messageType.messageType);
    copy_from_Json(cat34_json["010"], "SAC", cat_data.dataSourceIdentifier.sac);
    copy_from_Json(cat34_json["010"], "SIC", cat_data.dataSourceIdentifier.sic);
    copy_from_Json(cat34_json["020"], "Sector Number", cat_data.sectorNumber.sectorNumber);
    copy_from_Json(cat34_json["030"], "Time of Day", cat_data.timeOfDay.timeOfDay);
    copy_from_Json(cat34_json["041"], "Antenna Rotation Speed", cat_data.antennaRotationSpeed.antennaRotationSpeed);

    copy_from_Json(cat34_json["050"]["COM"], "NOGO", cat_data.systemConfigurationStatus.com.nogo);
    copy_from_Json(cat34_json["050"]["COM"], "RDPC", cat_data.systemConfigurationStatus.com.rdpc);
    copy_from_Json(cat34_json["050"]["COM"], "RDPR", cat_data.systemConfigurationStatus.com.rdpr);
    copy_from_Json(cat34_json["050"]["COM"], "OVL RDP", cat_data.systemConfigurationStatus.com.ovl_rdp);
    copy_from_Json(cat34_json["050"]["COM"], "OVL XMT", cat_data.systemConfigurationStatus.com.ovl_xmt);
    copy_from_Json(cat34_json["050"]["COM"], "MSC", cat_data.systemConfigurationStatus.com.msc);
    copy_from_Json(cat34_json["050"]["COM"], "TSV", cat_data.systemConfigurationStatus.com.tsv);

    copy_from_Json(cat34_json["050"]["PSR"], "ANT", cat_data.systemConfigurationStatus.psr.ant);
    copy_from_Json(cat34_json["050"]["PSR"], "CH-A/B", cat_data.systemConfigurationStatus.psr.ch_ab);
    copy_from_Json(cat34_json["050"]["PSR"], "OVL", cat_data.systemConfigurationStatus.psr.ovl);
    copy_from_Json(cat34_json["050"]["PSR"], "MSC", cat_data.systemConfigurationStatus.psr.msc);

    copy_from_Json(cat34_json["050"]["SSR"], "ANT", cat_data.systemConfigurationStatus.ssr.ant);
    copy_from_Json(cat34_json["050"]["SSR"], "CH-A/B", cat_data.systemConfigurationStatus.ssr.ch_ab);
    copy_from_Json(cat34_json["050"]["SSR"], "OVL", cat_data.systemConfigurationStatus.ssr.ovl);
    copy_from_Json(cat34_json["050"]["SSR"], "MSC", cat_data.systemConfigurationStatus.ssr.msc);

    copy_from_Json(cat34_json["050"]["MDS"], "ANT", cat_data.systemConfigurationStatus.mds.ant);
    copy_from_Json(cat34_json["050"]["MDS"], "CH-A/B", cat_data.systemConfigurationStatus.mds.ch_ab);
    copy_from_Json(cat34_json["050"]["MDS"], "OVL SUR", cat_data.systemConfigurationStatus.mds.ovl_sur);
    copy_from_Json(cat34_json["050"]["MDS"], "MSC", cat_data.systemConfigurationStatus.mds.msc);
    copy_from_Json(cat34_json["050"]["MDS"], "SCF", cat_data.systemConfigurationStatus.mds.scf);
    copy_from_Json(cat34_json["050"]["MDS"], "DLF", cat_data.systemConfigurationStatus.mds.dlf);
    copy_from_Json(cat34_json["050"]["MDS"], "OVL SCF", cat_data.systemConfigurationStatus.mds.ovl_scf);
    copy_from_Json(cat34_json["050"]["MDS"], "OVL DLF", cat_data.systemConfigurationStatus.mds.ovl_dlf);

    copy_from_Json(cat34_json["060"]["COM"], "RED RDP", cat_data.systemProcessingMode.com.red_rdp);
    copy_from_Json(cat34_json["060"]["COM"], "RED XMT", cat_data.systemProcessingMode.com.red_xmt);

    copy_from_Json(cat34_json["060"]["PSR"], "POL", cat_data.systemProcessingMode.psr.pol);
    copy_from_Json(cat34_json["060"]["PSR"], "RED RAD", cat_data.systemProcessingMode.psr.red_rad);
    copy_from_Json(cat34_json["060"]["PSR"], "STC", cat_data.systemProcessingMode.psr.stc);

    copy_from_Json(cat34_json["060"]["SSR"], "RED RAD", cat_data.systemProcessingMode.ssr.red_rad);

    copy_from_Json(cat34_json["060"]["MDS"], "RED RAD", cat_data.systemProcessingMode.mds.red_rad);
    copy_from_Json(cat34_json["060"]["MDS"], "CLU", cat_data.systemProcessingMode.mds.clu);

    int repetition_item = cat34_json["070"]["REP"].is_null() ? 0 : cat34_json["070"]["REP"].get<int>();
    for(auto idx = 0; idx < repetition_item; idx++)
    {
        Cat34_070_MessageCountValues::CountValue tempValue;
        tempValue.type = cat34_json["070"]["plot_count_values"][idx]["TYP"];
        tempValue.type = cat34_json["070"]["plot_count_values"][idx]["COUNTER"];
        cat_data.messageCountValues.countValues.push_back(tempValue);
    }    

    copy_from_Json(cat34_json["090"], "Range Error", cat_data.collimationError.rangeError);
    copy_from_Json(cat34_json["090"], "Azimuth Error", cat_data.collimationError.azimuthError);

    copy_from_Json(cat34_json["100"], "Rho Start", cat_data.genericPolarWindow.rhoStart);
    copy_from_Json(cat34_json["100"], "Rho End", cat_data.genericPolarWindow.rhoEnd);
    copy_from_Json(cat34_json["100"], "Theta Start", cat_data.genericPolarWindow.thetaStart);
    copy_from_Json(cat34_json["100"], "Theta End", cat_data.genericPolarWindow.thetaEnd);

    copy_from_Json(cat34_json["110"], "Typ", cat_data.dataFilter.typ);

    copy_from_Json(cat34_json["120"], "Height of Data Source", cat_data.positionOfDataSource.heightOfDataSource);
    copy_from_Json(cat34_json["120"], "Latitude", cat_data.positionOfDataSource.latitude);
    copy_from_Json(cat34_json["120"], "Longitude", cat_data.positionOfDataSource.longitude);
}

void CAT34::copy_to_Json(nlohmann::json &j, nlohmann::json &cat_Map, std::string key)
{
    if (j[key].empty())
    {
        std::cout << key << " : message is not exist!" << std::endl;
    }
    else
    {
        for (const auto &item : j.items())
        {
            cat_Map[item.key()] = item.value();
        }
    }
}

template <typename T>
void CAT34::copy_from_Json(const nlohmann::json &j, const std::string &key, T &value)
{
    if (j.contains(key) && !j[key].is_null())
    {
        value = j[key].get<T>();
    }
    else
    {
        //std::cout << key << " : message is not exist!" << std::endl;
    }
}