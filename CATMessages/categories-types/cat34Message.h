#pragma once

#include <vector>
#include <string>

#pragma pack(push, 1) // Hizalamayı 1 byte'a ayarla

//CAT_definitionda bitfield alanlar unsigned int olarak tanımlanmıştır.
struct Cat34_000_MessageType
{
    unsigned int messageType;
};

struct Cat34_010_DataSourceIdentifier
{
    unsigned int sac;
    unsigned int sic;
};

struct Cat34_020_SectorNumber
{
    unsigned int sectorNumber;
};

struct Cat34_030_TimeOfDay
{
    unsigned int timeOfDay;
};

struct Cat34_041_AntennaRotationSpeed
{
    unsigned int antennaRotationSpeed;
};

struct Cat34_050_SystemConfigurationStatus
{
    struct COM
    {
        unsigned int nogo;
        unsigned int rdpc;
        unsigned int rdpr;
        unsigned int ovl_rdp;
        unsigned int ovl_xmt;
        unsigned int msc;
        unsigned int tsv;
    } com;

    struct PSR
    {
        unsigned int ant;
        unsigned int ch_ab;
        unsigned int ovl;
        unsigned int msc;
    } psr;

    struct SSR
    {
        unsigned int ant;
        unsigned int ch_ab;
        unsigned int ovl;
        unsigned int msc;
    } ssr;

    struct MDS
    {
        unsigned int ant;
        unsigned int ch_ab;
        unsigned int ovl_sur;
        unsigned int msc;
        unsigned int scf;
        unsigned int dlf;
        unsigned int ovl_scf;
        unsigned int ovl_dlf;
    } mds;

    struct Available
    {
        unsigned int com_available;
        unsigned int psr_available;
        unsigned int ssr_available;
        unsigned int mds_available;
    } available_map;
};

struct Cat34_060_SystemProcessingMode
{
    struct COM
    {
        unsigned int red_rdp;
        unsigned int red_xmt;
    } com;

    struct PSR
    {
        unsigned int pol;
        unsigned int red_rad;
        unsigned int stc;
    } psr;

    struct SSR
    {
        unsigned int red_rad;
    } ssr;

    struct MDS
    {
        unsigned int red_rad;
        unsigned int clu;
    } mds;

    struct Available
    {
        unsigned int com_available;
        unsigned int psr_available;
        unsigned int ssr_available;
        unsigned int mds_available;
    } available_map;
};

struct Cat34_070_MessageCountValues
{
    struct CountValue
    {
        unsigned int type;
        unsigned int counter;
    };
    std::vector<CountValue> countValues;
};

struct Cat34_090_CollimationError
{
    int rangeError;
    int azimuthError;
};

struct Cat34_100_GenericPolarWindow
{
    unsigned int rhoStart;
    unsigned int rhoEnd;
    unsigned int thetaStart;
    unsigned int thetaEnd;
};

struct Cat34_110_DataFilter
{
    unsigned int typ;
};

struct Cat34_120_3DPositionOfDataSource
{
    int heightOfDataSource;
    int latitude;
    int longitude;
};

struct Cat34Record
{
    Cat34_000_MessageType messageType{0};
    Cat34_010_DataSourceIdentifier dataSourceIdentifier{0};
    Cat34_020_SectorNumber sectorNumber{0};
    Cat34_030_TimeOfDay timeOfDay{0};
    Cat34_041_AntennaRotationSpeed antennaRotationSpeed{0};
    Cat34_050_SystemConfigurationStatus systemConfigurationStatus{0};
    Cat34_060_SystemProcessingMode systemProcessingMode{0};
    Cat34_070_MessageCountValues messageCountValues;
    Cat34_090_CollimationError collimationError{0};
    Cat34_100_GenericPolarWindow genericPolarWindow{0};
    Cat34_110_DataFilter dataFilter{0};
    Cat34_120_3DPositionOfDataSource positionOfDataSource{0};

    Cat34Record() {}
};

#pragma pack(pop) // Önceki hizalama ayarını geri yükle