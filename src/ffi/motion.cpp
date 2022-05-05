#include "utility/bit_vector.h"
#include "communication/communication_layer.h"
#include "communication/tcp_transport.h"
#include "communication/shared_transport.h"

#include "motion.h"

using namespace encrypto::motion;

// MOTION Transports

std::pair<size_t, std::vector<communication::SharedTransport>>* motion_transports_new(size_t my_id, const char** hosts, const uint16_t* ports, size_t len) {
  communication::TcpPartiesConfiguration config(len);

  for (size_t i = 0; i < len; i++) {
    config.at(i) = std::make_pair(hosts[i], ports[i]);
  }

  communication::TcpSetupHelper helper(my_id, config);
  std::vector<std::unique_ptr<communication::Transport>> tcp_transports = helper.SetupConnections();

  std::vector<communication::SharedTransport> shared_transports = std::vector<communication::SharedTransport>(len);

  for (size_t i = 0; i < len; i++) {
    shared_transports[i] = communication::SharedTransport(std::move(tcp_transports[i]));
  }

  return new std::pair<size_t, std::vector<communication::SharedTransport>>(my_id, shared_transports);
}

void motion_transports_delete(std::pair<size_t, std::vector<communication::SharedTransport>>* transports) {
  size_t len = transports->second.size();

  for (size_t i = 0; i < len; i++) {
    if (i == transports->first) {
      continue;
    }

    transports->second[i].Drop();
  }

  delete transports;
}

// MOTION Party

Party* motion_party_new(size_t my_id, std::pair<size_t, std::vector<communication::SharedTransport>>* shared_transports) {
  size_t len = shared_transports->second.size();
  std::vector<std::unique_ptr<communication::Transport>> transports(len);

  for (size_t i = 0; i < len; i++) {
    transports[i] = std::make_unique<communication::SharedTransport>(shared_transports->second[i]);
  }

  auto communication_layer = std::make_unique<communication::CommunicationLayer>(my_id, std::move(transports));
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
  BitVector share_bv(1, share);
  std::span input(&share_bv, 1);
  return new ShareWrapper(party->SharedIn<MpcProtocol::kBooleanGmw>(input));
}

GmwBool* motion_gmw_bool_constant(Party* party, bool value) {
  value = party->GetConfiguration()->GetMyId() == 0 ? value : false;
  BitVector value_bv(1, value);
  std::span input(&value_bv, 1);
  return new ShareWrapper(party->SharedIn<MpcProtocol::kBooleanGmw>(input));
}

GmwBool* motion_gmw_bool_copy(GmwBool* share) {
  return new ShareWrapper(*share);
}

GmwBool* motion_gmw_bool_inv(GmwBool* share) {
  return new ShareWrapper(~*share);
}

GmwBool* motion_gmw_bool_xor(GmwBool* a, GmwBool* b) {
  return new ShareWrapper(*a ^ *b);
}

GmwBool* motion_gmw_bool_mux(GmwBool* g, GmwBool* a, GmwBool* b) {
  return new ShareWrapper(g->Mux(*a, *b));
}

GmwBool* motion_gmw_bool_and(GmwBool* a, GmwBool* b) {
  return new ShareWrapper(*a & *b);
}

bool motion_gmw_bool_get(GmwBool* share) {
  return share->As<bool>();
}

void motion_gmw_bool_delete(GmwBool* share) {
  delete share;
}

GmwNat* motion_gmw_nat_new(Party* party, const bool* share, size_t share_len) {
  std::vector<BitVector<StdAllocator>> input;
  for (size_t i = 0; i < share_len; i++) {
    input.emplace_back(1, share[i]);
  }
  return new SecureUnsignedInteger(party->SharedIn<MpcProtocol::kBooleanGmw>(input));
}

GmwNat* motion_gmw_nat_constant(Party* party, const bool* value, size_t value_len) {
  std::vector<BitVector<StdAllocator>> input;
  bool p0 = party->GetConfiguration()->GetMyId();
  for (size_t i = 0; i < value_len; i++) {
    input.emplace_back(1, p0 ? value[i] : false);
  }
  return new SecureUnsignedInteger(party->SharedIn<MpcProtocol::kBooleanGmw>(input));
}

GmwNat* motion_gmw_nat_copy(GmwNat* share) {
  return new SecureUnsignedInteger(*share);
}

GmwNat* motion_gmw_nat_add(GmwNat* a, GmwNat* b) {
  return new SecureUnsignedInteger(*a + *b);
}

GmwNat* motion_gmw_nat_sub(GmwNat* a, GmwNat* b) {
  return new SecureUnsignedInteger(*a - *b);
}

GmwNat* motion_gmw_nat_mul(GmwNat* a, GmwNat* b) {
  return new SecureUnsignedInteger(*a * *b);
}

GmwNat* motion_gmw_nat_mux(GmwBool* g, GmwNat* a, GmwNat* b) {
  return new SecureUnsignedInteger(g->Mux(a->Get(), b->Get()));
}

GmwBool* motion_gmw_nat_eq(GmwNat* a, GmwNat* b) {
  return new ShareWrapper(*a == *b);
}

GmwBool* motion_gmw_nat_gt(GmwNat* a, GmwNat* b) {
  return new ShareWrapper(*a > *b);
}

void motion_gmw_nat_get(GmwNat* share, bool* buf, size_t buf_len) {
  auto output = share->Get().As<std::vector<BitVector<StdAllocator>>>();
  assert(buf_len == output.size());
  for (size_t i = 0; i < buf_len; i++) {
    buf[i] = output[i][0];
  }
}

void motion_gmw_nat_delete(GmwNat* share) {
  delete share;
}
