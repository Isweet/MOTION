#ifndef MOTION_FFI_H
#define MOTION_FFI_H

#include <stddef.h>
#include <stdint.h>

// https://caiorss.github.io/C-Cpp-Notes/CwrapperToQtLibrary.html
#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

typedef struct party_config {
  size_t id;
  const char *host;
  uint16_t port;
} party_config;

typedef void* motion_party_config;

EXPORT_C motion_party_config party_config_new(size_t my_id, party_config* party_configs, size_t party_configs_len);

typedef void* motion_party;

EXPORT_C motion_party party_new(size_t my_id, motion_party_config config);
EXPORT_C size_t party_my_id(motion_party);

#endif
