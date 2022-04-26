#include "base/party.h"
#include "utility/bit_vector.h"
#include "communication/communication_layer.h"
#include "communication/tcp_transport.h"

#include "motion.h"

#include <iostream>
#include <boost/exception/diagnostic_information.hpp>

using namespace encrypto::motion;

// MOTION Party

Party* motion_party_new(size_t my_id, int* others_tcp_sockets, size_t others_tcp_sockets_len) {
  communication::TcpSetupHelper helper(my_id, others_tcp_sockets_len + 1);
  std::vector<int> others_tcp_sockets_vec(others_tcp_sockets_len + 1);
  for (size_t i = 0; i < others_tcp_sockets_len; i++) {
    if (i < my_id) {
      others_tcp_sockets_vec[i] = others_tcp_sockets[i];
    } else {
      others_tcp_sockets_vec[i + 1] = others_tcp_sockets[i];
    }
  }
  auto communication_layer = std::make_unique<communication::CommunicationLayer>(my_id, helper.UseConnections(others_tcp_sockets_vec));
  return new Party(std::move(communication_layer));
}

void motion_party_run(Party* party) {
  party->Run();
  party->Finish();
}

void motion_party_delete(Party* party) {
  delete party;
}

// MOTION Boolean

GmwBool* motion_gmw_bool_new(Party* party, bool share) {
  BitVector share_bv(share);
  std::span input(&share_bv, 1);
  return new ShareWrapper(party->SharedIn<MpcProtocol::kBooleanGmw>(input));
}

GmwBool* motion_gmw_bool_constant(Party* party, bool value) {
  BitVector value_bv(value);
  std::span input(&value_bv, 1);
  return new ShareWrapper(party->In<MpcProtocol::kBooleanGmw>(input));
}

GmwBool* motion_gmw_bool_and(GmwBool* a, GmwBool* b) {
  return new ShareWrapper(*a & *b);
}

bool motion_gmw_bool_reify(GmwBool* share) {
  return share->As<bool>();
}

void motion_gmw_bool_delete(GmwBool* share) {
  delete share;
}
