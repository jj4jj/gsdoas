// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: logd.proto

#ifndef PROTOBUF_logd_2eproto__INCLUDED
#define PROTOBUF_logd_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace logd {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_logd_2eproto();
void protobuf_AssignDesc_logd_2eproto();
void protobuf_ShutdownFile_logd_2eproto();

class Config;

// ===================================================================

class Config : public ::google::protobuf::Message {
 public:
  Config();
  virtual ~Config();

  Config(const Config& from);

  inline Config& operator=(const Config& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Config& default_instance();

  void Swap(Config* other);

  // implements Message ----------------------------------------------

  Config* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Config& from);
  void MergeFrom(const Config& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string data_dir = 1;
  inline bool has_data_dir() const;
  inline void clear_data_dir();
  static const int kDataDirFieldNumber = 1;
  inline const ::std::string& data_dir() const;
  inline void set_data_dir(const ::std::string& value);
  inline void set_data_dir(const char* value);
  inline void set_data_dir(const char* value, size_t size);
  inline ::std::string* mutable_data_dir();
  inline ::std::string* release_data_dir();
  inline void set_allocated_data_dir(::std::string* data_dir);

  // required string data_file = 2;
  inline bool has_data_file() const;
  inline void clear_data_file();
  static const int kDataFileFieldNumber = 2;
  inline const ::std::string& data_file() const;
  inline void set_data_file(const ::std::string& value);
  inline void set_data_file(const char* value);
  inline void set_data_file(const char* value, size_t size);
  inline ::std::string* mutable_data_file();
  inline ::std::string* release_data_file();
  inline void set_allocated_data_file(::std::string* data_file);

  // @@protoc_insertion_point(class_scope:logd.Config)
 private:
  inline void set_has_data_dir();
  inline void clear_has_data_dir();
  inline void set_has_data_file();
  inline void clear_has_data_file();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* data_dir_;
  ::std::string* data_file_;
  friend void  protobuf_AddDesc_logd_2eproto();
  friend void protobuf_AssignDesc_logd_2eproto();
  friend void protobuf_ShutdownFile_logd_2eproto();

  void InitAsDefaultInstance();
  static Config* default_instance_;
};
// ===================================================================


// ===================================================================

// Config

// required string data_dir = 1;
inline bool Config::has_data_dir() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Config::set_has_data_dir() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Config::clear_has_data_dir() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Config::clear_data_dir() {
  if (data_dir_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_dir_->clear();
  }
  clear_has_data_dir();
}
inline const ::std::string& Config::data_dir() const {
  // @@protoc_insertion_point(field_get:logd.Config.data_dir)
  return *data_dir_;
}
inline void Config::set_data_dir(const ::std::string& value) {
  set_has_data_dir();
  if (data_dir_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_dir_ = new ::std::string;
  }
  data_dir_->assign(value);
  // @@protoc_insertion_point(field_set:logd.Config.data_dir)
}
inline void Config::set_data_dir(const char* value) {
  set_has_data_dir();
  if (data_dir_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_dir_ = new ::std::string;
  }
  data_dir_->assign(value);
  // @@protoc_insertion_point(field_set_char:logd.Config.data_dir)
}
inline void Config::set_data_dir(const char* value, size_t size) {
  set_has_data_dir();
  if (data_dir_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_dir_ = new ::std::string;
  }
  data_dir_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:logd.Config.data_dir)
}
inline ::std::string* Config::mutable_data_dir() {
  set_has_data_dir();
  if (data_dir_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_dir_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:logd.Config.data_dir)
  return data_dir_;
}
inline ::std::string* Config::release_data_dir() {
  clear_has_data_dir();
  if (data_dir_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = data_dir_;
    data_dir_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Config::set_allocated_data_dir(::std::string* data_dir) {
  if (data_dir_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete data_dir_;
  }
  if (data_dir) {
    set_has_data_dir();
    data_dir_ = data_dir;
  } else {
    clear_has_data_dir();
    data_dir_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:logd.Config.data_dir)
}

// required string data_file = 2;
inline bool Config::has_data_file() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Config::set_has_data_file() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Config::clear_has_data_file() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Config::clear_data_file() {
  if (data_file_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_file_->clear();
  }
  clear_has_data_file();
}
inline const ::std::string& Config::data_file() const {
  // @@protoc_insertion_point(field_get:logd.Config.data_file)
  return *data_file_;
}
inline void Config::set_data_file(const ::std::string& value) {
  set_has_data_file();
  if (data_file_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_file_ = new ::std::string;
  }
  data_file_->assign(value);
  // @@protoc_insertion_point(field_set:logd.Config.data_file)
}
inline void Config::set_data_file(const char* value) {
  set_has_data_file();
  if (data_file_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_file_ = new ::std::string;
  }
  data_file_->assign(value);
  // @@protoc_insertion_point(field_set_char:logd.Config.data_file)
}
inline void Config::set_data_file(const char* value, size_t size) {
  set_has_data_file();
  if (data_file_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_file_ = new ::std::string;
  }
  data_file_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:logd.Config.data_file)
}
inline ::std::string* Config::mutable_data_file() {
  set_has_data_file();
  if (data_file_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    data_file_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:logd.Config.data_file)
  return data_file_;
}
inline ::std::string* Config::release_data_file() {
  clear_has_data_file();
  if (data_file_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = data_file_;
    data_file_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Config::set_allocated_data_file(::std::string* data_file) {
  if (data_file_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete data_file_;
  }
  if (data_file) {
    set_has_data_file();
    data_file_ = data_file;
  } else {
    clear_has_data_file();
    data_file_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:logd.Config.data_file)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace logd

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_logd_2eproto__INCLUDED