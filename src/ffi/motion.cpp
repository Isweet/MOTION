#include "base/party.h"
#include "communication/communication_layer.h"
#include "communication/tcp_transport.h"

#include "motion.h"

using namespace encrypto::motion;

motion_party_config party_config_new(size_t my_id, party_config* party_configs, size_t party_configs_len) {
  communication::TcpPartiesConfiguration parties_configuration(party_configs_len);
  for (size_t i = 0; i < party_configs_len; i++) {
    parties_configuration.at(party_configs[i].id) = std::make_pair(std::string(party_configs[i].host), party_configs[i].port);
  }

  return new communication::TcpSetupHelper(my_id, parties_configuration);
}

motion_party party_new(size_t my_id, motion_party_config config) {
  communication::TcpSetupHelper* helper = static_cast<communication::TcpSetupHelper*>(config);
  auto communication_layer = std::make_unique<communication::CommunicationLayer>(my_id, helper->SetupConnections());
  return new Party(std::move(communication_layer));
}

size_t party_my_id(motion_party party_) {
  Party* party = static_cast<Party*>(party_);
  return party->GetConfiguration()->GetMyId();
}
