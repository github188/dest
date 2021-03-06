// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: dssevenum.proto

#ifndef PROTOBUF_dssevenum_2eproto__INCLUDED
#define PROTOBUF_dssevenum_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace Dss {
namespace Event {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_dssevenum_2eproto();
void protobuf_AssignDesc_dssevenum_2eproto();
void protobuf_ShutdownFile_dssevenum_2eproto();


enum CltDssEvent {
  CLT_DSS_CREATCLIENT_REQ = 1,
  DSS_CLT_CREATCLIENT_ACK = 2,
  DSS_CLT_CREATCLIENT_NACK = 3,
  CLT_DSS_SYNCAPPLYLANPORT_REQ = 4,
  DSS_CLT_SYNCAPPLYLANPORT_ACK = 5,
  DSS_CLT_SYNCAPPLYLANPORT_NACK = 6,
  CLT_DSS_SYNCAPPLYWANPORT_REQ = 7,
  DSS_CLT_SYNCAPPLYWANPORT_ACK = 8,
  DSS_CLT_SYNCAPPLYWANPORT_NACK = 9,
  CLT_DSS_ASYNCAPPLYLANPORT_REQ = 10,
  DSS_CLT_ASYNCAPPLYLANPORT_ACK = 11,
  DSS_CLT_ASYNCAPPLYLANPORT_NACK = 12,
  CLT_DSS_ASYNCAPPLYWANPORT_REQ = 13,
  DSS_CLT_ASYNCAPPLYWANPORT_ACK = 14,
  DSS_CLT_ASYNCAPPLYWANPORT_NACK = 15,
  CLT_DSS_RELEASEPORT_CMD = 16,
  CLT_DSS_ADDSWITCH_REQ = 17,
  DSS_CLT_ADDSWITCH_ACK = 18,
  DSS_CLT_ADDSWITCH_NACK = 19,
  CLT_DSS_ADDM2ONESWITCH_REQ = 20,
  DSS_CLT_ADDM2ONESWITCH_ACK = 21,
  DSS_CLT_ADDM2ONESWITCH_NACK = 22,
  CLT_DSS_ADDSRCSWITCH_REQ = 23,
  DSS_CLT_ADDSRCSWITCH_ACK = 24,
  DSS_CLT_ADDSRCSWITCH_NACK = 25,
  CLT_DSS_REMOVESWITCH_CMD = 26,
  CLT_DSS_ADDDUMP_REQ = 27,
  DSS_CLT_ADDDUMP_ACK = 28,
  DSS_CLT_ADDDUMP_NACK = 29,
  CLT_DSS_REMOVDUMP_CMD = 30,
  DSS_CLT_SRVLOST_NTF = 31,
  CLT_DSS_DESTROYCLIENT_CMD = 32,
  CLT_DSS_ASYNCADDBRIDGE_REQ = 33,
  DSS_CLT_ASYNCADDBRIDGE_ACK = 34,
  DSS_CLT_ASYNCADDBRIDGE_NACK = 35,
  CLT_DSS_SYNCADDBRIDGE_REQ = 36,
  DSS_CLT_SYNCADDBRIDGE_ACK = 37,
  DSS_CLT_SYNCADDBRIDGE_NACK = 38,
  CLT_DSS_REMOVEBRIDGE_CMD = 39,
  DSS_CLT_PING = 40,
  CLT_DSS_PONG = 41,
  CLT_DSS_SENDTOBRIDGE_REQ = 42,
  DSS_CLT_SENDTOBRIDGE_ACK = 43,
  DSS_CLT_SENDTOBRIDGE_NACK = 44,
  CLT_DSS_RECEIVEFROMBRIDGE_REQ = 45,
  DSS_CLT_RECEIVEFROMBRIDGE_ACK = 46,
  DSS_CLT_RECEIVEFROMBRIDGE_NACK = 47,
  CLT_DSS_STOPSENDTOBRIDGE_CMD = 48,
  CLT_DSS_STOPRECEIVEFROMBRIDGE_CMD = 49,
  CLT_DSS_ALIVEDETECT_REQ = 50,
  DSS_CLT_ALIVEDETECT_RSP = 51,
  CLT_DSS_SYNCATTACHLANIPOFPORT_REQ = 52,
  DSS_CLT_SYNCATTACHLANIPOFPORT_ACK = 53,
  DSS_CLT_SYNCATTACHLANIPOFPORT_NACK = 54,
  CLT_DSS_SYNCATTACHWANIPOFPORT_REQ = 55,
  DSS_CLT_SYNCATTACHWANIPOFPORT_ACK = 56,
  DSS_CLT_SYNCATTACHWANIPOFPORT_NACK = 57,
  CLT_DSS_ASYNCATTACHLANIPOFPORT_REQ = 58,
  DSS_CLT_ASYNCATTACHLANIPOFPORT_ACK = 59,
  DSS_CLT_ASYNCATTACHLANIPOFPORT_NACK = 60,
  CLT_DSS_ASYNCATTACHWANIPOFPORT_REQ = 61,
  DSS_CLT_ASYNCATTACHWANIPOFPORT_ACK = 62,
  DSS_CLT_ASYNCATTACHWANIPOFPORT_NACK = 63,
  DSS_CLT_SRVREGISTER_NTF = 64,
  DSS_CLT_SYNCAPPLYLANPORT_REPLY = 65,
  DSS_CLT_ASYNCAPPLYLANPORT_REPLY = 66,
  DSS_CLT_SYNCAPPLYWANPORT_REPLY = 67,
  DSS_CLT_ASYNCAPPLYWANPORT_REPLY = 68,
  CLT_DSS_SYNCAPPLYCONSLANPORT_REQ = 69,
  DSS_CLT_SYNCAPPLYCONSLANPORT_REPLY = 70,
  CLT_DSS_ASYNCAPPLYCONSLANPORT_REQ = 71,
  DSS_CLT_ASYNCAPPLYCONSLANPORT_REPLY = 72
};
bool CltDssEvent_IsValid(int value);
const CltDssEvent CltDssEvent_MIN = CLT_DSS_CREATCLIENT_REQ;
const CltDssEvent CltDssEvent_MAX = DSS_CLT_ASYNCAPPLYCONSLANPORT_REPLY;
const int CltDssEvent_ARRAYSIZE = CltDssEvent_MAX + 1;

const ::google::protobuf::EnumDescriptor* CltDssEvent_descriptor();
inline const ::std::string& CltDssEvent_Name(CltDssEvent value) {
  return ::google::protobuf::internal::NameOfEnum(
    CltDssEvent_descriptor(), value);
}
inline bool CltDssEvent_Parse(
    const ::std::string& name, CltDssEvent* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CltDssEvent>(
    CltDssEvent_descriptor(), name, value);
}
enum iptype {
  IpTypeUnknown = 0,
  eIpV4 = 1,
  eIpV6 = 2
};
bool iptype_IsValid(int value);
const iptype iptype_MIN = IpTypeUnknown;
const iptype iptype_MAX = eIpV6;
const int iptype_ARRAYSIZE = iptype_MAX + 1;

const ::google::protobuf::EnumDescriptor* iptype_descriptor();
inline const ::std::string& iptype_Name(iptype value) {
  return ::google::protobuf::internal::NameOfEnum(
    iptype_descriptor(), value);
}
inline bool iptype_Parse(
    const ::std::string& name, iptype* value) {
  return ::google::protobuf::internal::ParseNamedEnum<iptype>(
    iptype_descriptor(), name, value);
}
enum DssObj {
  DssObjUnknown = 0,
  DSS_CONF = 1,
  DSS_NU = 2
};
bool DssObj_IsValid(int value);
const DssObj DssObj_MIN = DssObjUnknown;
const DssObj DssObj_MAX = DSS_NU;
const int DssObj_ARRAYSIZE = DssObj_MAX + 1;

const ::google::protobuf::EnumDescriptor* DssObj_descriptor();
inline const ::std::string& DssObj_Name(DssObj value) {
  return ::google::protobuf::internal::NameOfEnum(
    DssObj_descriptor(), value);
}
inline bool DssObj_Parse(
    const ::std::string& name, DssObj* value) {
  return ::google::protobuf::internal::ParseNamedEnum<DssObj>(
    DssObj_descriptor(), name, value);
}
enum DsType {
  DsTypeUnknown = 0,
  DSRTP = 1,
  DSRTCP = 2
};
bool DsType_IsValid(int value);
const DsType DsType_MIN = DsTypeUnknown;
const DsType DsType_MAX = DSRTCP;
const int DsType_ARRAYSIZE = DsType_MAX + 1;

const ::google::protobuf::EnumDescriptor* DsType_descriptor();
inline const ::std::string& DsType_Name(DsType value) {
  return ::google::protobuf::internal::NameOfEnum(
    DsType_descriptor(), value);
}
inline bool DsType_Parse(
    const ::std::string& name, DsType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<DsType>(
    DsType_descriptor(), name, value);
}
enum SecMode {
  SEC_MODE_NONE = 0,
  SEC_MODE_ECB = 1,
  SEC_MODE_CBC = 2,
  SEC_MODE_CFB = 3,
  SEC_MODE_OFB = 4,
  SEC_MODE_MAC = 5,
  SEC_MODE_CTR = 6
};
bool SecMode_IsValid(int value);
const SecMode SecMode_MIN = SEC_MODE_NONE;
const SecMode SecMode_MAX = SEC_MODE_CTR;
const int SecMode_ARRAYSIZE = SecMode_MAX + 1;

const ::google::protobuf::EnumDescriptor* SecMode_descriptor();
inline const ::std::string& SecMode_Name(SecMode value) {
  return ::google::protobuf::internal::NameOfEnum(
    SecMode_descriptor(), value);
}
inline bool SecMode_Parse(
    const ::std::string& name, SecMode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SecMode>(
    SecMode_descriptor(), name, value);
}
enum SecAlg {
  SEC_ALG_NONE = 0,
  SEC_ALG_SM1 = 1,
  SEC_ALG_SSF33 = 2,
  SEC_ALG_SM4 = 3,
  SEC_ALG_DES = 4,
  SEC_ALG_AES = 5,
  SEC_ALG_3DES = 6,
  SEC_ALG_RSA = 7,
  SEC_ALG_SM2_1 = 8,
  SEC_ALG_SM2_2 = 9,
  SEC_ALG_SM2_3 = 10,
  SEC_ALG_SM3 = 11,
  SEC_ALG_SHA1 = 12,
  SEC_ALG_SHA256 = 13
};
bool SecAlg_IsValid(int value);
const SecAlg SecAlg_MIN = SEC_ALG_NONE;
const SecAlg SecAlg_MAX = SEC_ALG_SHA256;
const int SecAlg_ARRAYSIZE = SecAlg_MAX + 1;

const ::google::protobuf::EnumDescriptor* SecAlg_descriptor();
inline const ::std::string& SecAlg_Name(SecAlg value) {
  return ::google::protobuf::internal::NameOfEnum(
    SecAlg_descriptor(), value);
}
inline bool SecAlg_Parse(
    const ::std::string& name, SecAlg* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SecAlg>(
    SecAlg_descriptor(), name, value);
}
enum CarrierType {
  CHINA_NONE = 0,
  CHINA_MOBILE = 1,
  CHINA_UNICOM = 2,
  CHINA_TELECOM = 3
};
bool CarrierType_IsValid(int value);
const CarrierType CarrierType_MIN = CHINA_NONE;
const CarrierType CarrierType_MAX = CHINA_TELECOM;
const int CarrierType_ARRAYSIZE = CarrierType_MAX + 1;

const ::google::protobuf::EnumDescriptor* CarrierType_descriptor();
inline const ::std::string& CarrierType_Name(CarrierType value) {
  return ::google::protobuf::internal::NameOfEnum(
    CarrierType_descriptor(), value);
}
inline bool CarrierType_Parse(
    const ::std::string& name, CarrierType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CarrierType>(
    CarrierType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace Event
}  // namespace Dss

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::CltDssEvent>() {
  return ::Dss::Event::CltDssEvent_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::iptype>() {
  return ::Dss::Event::iptype_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::DssObj>() {
  return ::Dss::Event::DssObj_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::DsType>() {
  return ::Dss::Event::DsType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::SecMode>() {
  return ::Dss::Event::SecMode_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::SecAlg>() {
  return ::Dss::Event::SecAlg_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Dss::Event::CarrierType>() {
  return ::Dss::Event::CarrierType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_dssevenum_2eproto__INCLUDED
