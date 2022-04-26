#ifndef MOTION_FFI_H
#define MOTION_FFI_H

#include <stddef.h>
#include <stdint.h>

#include "base/party.h"
#include "protocols/share_wrapper.h"

// https://caiorss.github.io/C-Cpp-Notes/CwrapperToQtLibrary.html
#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

typedef encrypto::motion::Party Party;

EXPORT_C Party* motion_party_new(size_t my_id, int *others_tcp_sockets, size_t others_tcp_sockets_len);
EXPORT_C void   motion_party_run(Party* party);
EXPORT_C void   motion_party_delete(Party* party);

typedef encrypto::motion::ShareWrapper GmwBool;

EXPORT_C GmwBool* motion_gmw_bool_new(Party* party, bool share);
EXPORT_C GmwBool* motion_gmw_bool_constant(Party* party, bool value);
EXPORT_C GmwBool* motion_gmw_bool_and(GmwBool* a, GmwBool* b);
EXPORT_C bool     motion_gmw_bool_reify(GmwBool* share);
EXPORT_C void     motion_gmw_bool_delete(GmwBool* share);

#endif
