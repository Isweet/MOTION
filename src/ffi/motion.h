#ifndef MOTION_FFI_H
#define MOTION_FFI_H

#include <stddef.h>
#include <stdint.h>

#include "base/party.h"
#include "protocols/share_wrapper.h"
#include "secure_type/secure_unsigned_integer.h"

// https://caiorss.github.io/C-Cpp-Notes/CwrapperToQtLibrary.html
#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

typedef encrypto::motion::Party Party;

EXPORT_C Party* motion_party_new(size_t my_id, const char** hosts, const uint16_t* ports, size_t len);
EXPORT_C void   motion_party_run(Party* party);
EXPORT_C void   motion_party_delete(Party* party);

typedef encrypto::motion::ShareWrapper GmwBool;

EXPORT_C GmwBool* motion_gmw_bool_new(Party* party, bool share);
EXPORT_C GmwBool* motion_gmw_bool_constant(Party* party, bool value);
EXPORT_C GmwBool* motion_gmw_bool_copy(GmwBool* share);
EXPORT_C GmwBool* motion_gmw_bool_inv(GmwBool* share);
EXPORT_C GmwBool* motion_gmw_bool_xor(GmwBool* a, GmwBool* b);
EXPORT_C GmwBool* motion_gmw_bool_mux(GmwBool* g, GmwBool* a, GmwBool* b);
EXPORT_C GmwBool* motion_gmw_bool_and(GmwBool* a, GmwBool* b);
EXPORT_C bool     motion_gmw_bool_get(GmwBool* share);
EXPORT_C void     motion_gmw_bool_delete(GmwBool* share);

typedef encrypto::motion::SecureUnsignedInteger GmwNat;

EXPORT_C GmwNat*  motion_gmw_nat_new(Party* party, const bool* share, size_t share_len);
EXPORT_C GmwNat*  motion_gmw_nat_constant(Party* party, const bool* value, size_t value_len);
EXPORT_C GmwNat*  motion_gmw_nat_copy(GmwNat* share);
EXPORT_C GmwNat*  motion_gmw_nat_add(GmwNat* a, GmwNat* b);
EXPORT_C GmwNat*  motion_gmw_nat_sub(GmwNat* a, GmwNat* b);
EXPORT_C GmwNat*  motion_gmw_nat_mul(GmwNat* a, GmwNat* b);
EXPORT_C GmwNat*  motion_gmw_nat_mux(GmwBool* g, GmwNat* a, GmwNat* b);
EXPORT_C GmwBool* motion_gmw_nat_eq(GmwNat* a, GmwNat* b);
EXPORT_C GmwBool* motion_gmw_nat_gt(GmwNat* a, GmwNat* b);
EXPORT_C void     motion_gmw_nat_get(GmwNat* share, bool* buf, size_t buf_len);
EXPORT_C void     motion_gmw_nat_delete(GmwNat* share);

#endif
