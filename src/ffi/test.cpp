#include "base/party.h"
#include "protocols/share_wrapper.h"
#include "utility/bit_vector.h"
#include "communication/communication_layer.h"
#include "communication/tcp_transport.h"

using namespace encrypto::motion;

Party MakeParty(size_t my_id, const char **hosts, ushort *ports, size_t len) {
  communication::TcpPartiesConfiguration parties_configuration(len);

  for (size_t party_id = 0; party_id < len; party_id++) {
    std::string host(hosts[party_id]);
    ushort port = ports[party_id];
    parties_configuration.at(party_id) = std::make_pair(std::string(hosts[party_id]), ports[party_id]);
  }

  communication::TcpSetupHelper helper(my_id, parties_configuration);
  auto communication_layer = std::make_unique<communication::CommunicationLayer>(my_id, helper.SetupConnections());
  return Party(std::move(communication_layer));
}

ShareWrapper MakeBoolShare(Party& party, bool share) {
  BitVector share_bv(1, share);
  std::span input(&share_bv, 1);
  return ShareWrapper(party.SharedIn<MpcProtocol::kBooleanGmw>(input));
}

int main(int argc, char *argv[]) {
  size_t my_id = atoi(argv[1]);
  const char *hosts[2] = { "127.0.0.1", "127.0.0.1" };
  ushort ports[2] = { 23000, 23001 };
  Party party = MakeParty(my_id, hosts, ports, 2);
  ShareWrapper a = MakeBoolShare(party, my_id == 0 ? false : true);
  ShareWrapper b = MakeBoolShare(party, my_id == 0 ? false : true);
  ShareWrapper c(a & b);

  party.Run();
  party.Finish();

  std::cout << my_id << ": " << c.As<bool>() << std::endl;

  return 0;
}
