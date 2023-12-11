#pragma once

#include <vector>
#include <string>

#pragma pack(push, 1) // Hizalamayı 1 byte'a ayarla

struct Cat34_000_MessageType
{
    unsigned int messageType;
};

struct Cat34_010_DataSourceIdentifier
{
    unsigned int sac; // System Area Code
    unsigned int sic; // System Identification Code
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
        bool nogo: 1;
        bool rdpc: 1;
        bool rdpr: 1;
        bool ovl_rdp: 1;
        bool ovl_xmt: 1;
        bool msc: 1;
        bool tsv: 1;
        unsigned int unused1 : 1;
    } com;

    struct PSR
    {
        bool ant: 1;
        int ch_ab: 2;
        bool ovl: 1;
        bool msc: 1;
        unsigned int unused1 : 3; 
    } psr;

    struct SSR
    {
        bool ant: 1;
        int ch_ab: 2;
        bool ovl: 1;
        bool msc: 1;
        unsigned int unused1: 3;
    } ssr;

    struct MDS
    {
        bool ant: 1;
        int ch_ab: 2;
        bool ovl_sur: 1;
        bool msc: 1;
        bool scf: 1;
        bool dlf: 1;
        bool ovl_scf: 1;
        bool ovl_dlf: 1;
        unsigned int unused1: 7;
    } mds;

    struct Available
    {
        bool com_available;
        bool psr_available;
        bool ssr_available;
        bool mds_available;
    } available_map;
};

struct Cat34_060_SystemProcessingMode
{
    struct COM
    {
        unsigned int unused1: 1;
        int red_rdp : 3;
        int red_xmt : 3;
        unsigned int unused2: 1;
    } com;

    struct PSR
    {
        bool pol: 1;
        int red_rad: 3;
        int stc: 2;
        unsigned int unused1: 2;
    } psr;

    struct SSR
    {
        int red_rad : 3;
        unsigned int unused1: 5;
    } ssr;

    struct MDS
    {
        int red_rad : 3;
        bool clu: 1;
        unsigned int unused1: 4;
    } mds;

    struct Available
    {
        bool com_available;
        bool psr_available;
        bool ssr_available;
        bool mds_available;
    } available_map;
};

struct Cat34_070_MessageCountValues
{
    struct CountValue
    {
        int type: 5;
        int counter: 11;
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
    Cat34_000_MessageType messageType;
    Cat34_010_DataSourceIdentifier dataSourceIdentifier;
    Cat34_020_SectorNumber sectorNumber;
    Cat34_030_TimeOfDay timeOfDay;
    Cat34_041_AntennaRotationSpeed antennaRotationSpeed;
    Cat34_050_SystemConfigurationStatus systemConfigurationStatus;
    Cat34_060_SystemProcessingMode systemProcessingMode;
    Cat34_070_MessageCountValues messageCountValues;
    Cat34_090_CollimationError collimationError;
    Cat34_100_GenericPolarWindow genericPolarWindow;
    Cat34_110_DataFilter dataFilter;
    Cat34_120_3DPositionOfDataSource positionOfDataSource;

    // Default constructor
    Cat34Record() {}
};

#pragma pack(pop) // Önceki hizalama ayarını geri yükle