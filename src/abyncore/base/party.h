#pragma once

#include <fmt/format.h>
#include <omp.h>
#include <memory>
#include <vector>

#include "base/backend.h"
#include "configuration.h"
//#include "crypto/oblivious_transfer/ot_provider.h"
#include "utility/constants.h"
#include "utility/typedefs.h"

namespace ABYN {

class Logger;
using LoggerPtr = std::shared_ptr<Logger>;

class Party {
 public:
  Party() = delete;

  // Let's make only Configuration be copyable
  Party(Party &party) = delete;

  Party(std::vector<Communication::ContextPtr> &parties, std::size_t my_id);

  Party(std::vector<Communication::ContextPtr> &&parties, std::size_t my_id);

  Party(std::initializer_list<Communication::ContextPtr> &&list_parties, std::size_t my_id);

  Party(ConfigurationPtr &configuration)
      : config_(configuration), backend_(std::make_shared<Backend>(config_)) {}

  ~Party();

  ConfigurationPtr GetConfiguration() { return config_; }

  template <MPCProtocol P>
  Shares::SharePtr IN(const std::vector<ENCRYPTO::BitVector> &input, std::size_t party_id) {
    static_assert(P != MPCProtocol::ArithmeticGMW);
    switch (P) {
      case MPCProtocol::BooleanGMW: {
        return backend_->BooleanGMWInput(party_id, input);
      }
      case MPCProtocol::BMR: {
        static_assert(P != MPCProtocol::BMR, "BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error(fmt::format("Unknown protocol with id {}", static_cast<uint>(P))));
      }
    }
  }

  template <MPCProtocol P>
  Shares::SharePtr IN(std::vector<ENCRYPTO::BitVector> &&input, std::size_t party_id) {
    static_assert(P != MPCProtocol::ArithmeticGMW);
    switch (P) {
      case MPCProtocol::BooleanGMW: {
        return backend_->BooleanGMWInput(party_id, std::move(input));
      }
      case MPCProtocol::BMR: {
        static_assert(P != MPCProtocol::BMR, "BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error(fmt::format("Unknown protocol with id {}", static_cast<uint>(P))));
      }
    }
  }

  template <MPCProtocol P>
  Shares::SharePtr IN(const ENCRYPTO::BitVector &input, std::size_t party_id) {
    static_assert(P != MPCProtocol::ArithmeticGMW);
    switch (P) {
      case MPCProtocol::BooleanGMW: {
        return backend_->BooleanGMWInput(party_id, input);
      }
      case MPCProtocol::BMR: {
        static_assert(P != MPCProtocol::BMR, "BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error(fmt::format("Unknown protocol with id {}", static_cast<uint>(P))));
      }
    }
  }

  template <MPCProtocol P>
  Shares::SharePtr IN(ENCRYPTO::BitVector &&input, std::size_t party_id) {
    static_assert(P != MPCProtocol::ArithmeticGMW);
    switch (P) {
      case MPCProtocol::BooleanGMW: {
        return backend_->BooleanGMWInput(party_id, std::move(input));
      }
      case MPCProtocol::BMR: {
        static_assert(P != MPCProtocol::BMR, "BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error(fmt::format("Unknown protocol with id {}", static_cast<uint>(P))));
      }
    }
  }

  template <MPCProtocol P, typename T = std::uint8_t,
            typename = std::enable_if_t<std::is_unsigned_v<T>>>
  Shares::SharePtr IN(const std::vector<T> &input, std::size_t party_id) {
    switch (P) {
      case MPCProtocol::ArithmeticGMW: {
        return backend_->ArithmeticGMWInput(party_id, input);
      }
      case MPCProtocol::BooleanGMW: {
        throw(std::runtime_error(
            fmt::format("Non-binary types have to be converted to BitVectors in BooleanGMW, "
                        "consider using TODO function for the input")));
      }
      case MPCProtocol::BMR: {
        static_assert(P != MPCProtocol::BMR, "BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error(fmt::format("Unknown protocol with id {}", static_cast<uint>(P))));
      }
    }
  }

  template <MPCProtocol P, typename T = std::uint8_t,
            typename = std::enable_if_t<std::is_unsigned_v<T>>>
  Shares::SharePtr IN(std::vector<T> &&input, std::size_t party_id) {
    switch (P) {
      case MPCProtocol::ArithmeticGMW: {
        return backend_->ArithmeticGMWInput(party_id, std::move(input));
      }
      case MPCProtocol::BooleanGMW: {
        throw(std::runtime_error(
            fmt::format("Non-binary types have to be converted to BitVectors in BooleanGMW, "
                        "consider using TODO function for the input")));
      }
      case MPCProtocol::BMR: {
        static_assert(P != MPCProtocol::BMR, "BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error(fmt::format("Unknown protocol with id {}", static_cast<uint>(P))));
      }
    }
  }

  template <MPCProtocol P, typename T = std::uint8_t,
            typename = std::enable_if_t<std::is_unsigned_v<T>>>
  Shares::SharePtr IN(T input, std::size_t party_id) {
    if constexpr (std::is_same_v<T, bool>) {
      static_assert(P != MPCProtocol::ArithmeticGMW, "Invalid input");
      return backend_->BooleanGMWInput(party_id, input);
    } else {
      return IN<P, T>(std::vector<T>{input}, party_id);
    }
  }

  Shares::SharePtr XOR(const Shares::SharePtr &a, const Shares::SharePtr &b) {
    assert(a);
    assert(b);
    assert(a->GetSharingType() != MPCProtocol::ArithmeticGMW);
    assert(a->GetSharingType() == b->GetSharingType());
    switch (a->GetSharingType()) {
      case MPCProtocol::BooleanGMW: {
        return backend_->BooleanGMWXOR(a, b);
      }
      case MPCProtocol::BMR: {
        throw std::runtime_error("BMR protocol is not implemented yet");
        // TODO
      }
      default: {
        throw(std::runtime_error("Unknown protocol"));
      }
    }
  }

  Shares::SharePtr OUT(Shares::SharePtr parent, std::size_t output_owner);

  Shares::SharePtr ADD(const Shares::SharePtr &a, const Shares::SharePtr &b);

  std::size_t GetNumOfParties() { return config_->GetNumOfParties(); }

  /// \brief Establishes connections between the parties.
  void Connect();

  bool IsConnected() { return connected_; }

  /// \brief Evaluates the constructed gates a predefined number of times.
  /// This is realized via repeatedly calling Party::Clear() after each evaluation.
  /// If Connect() was not called yet, it is called automatically at the beginning of this method.
  /// @param repeats Number of iterations.
  void Run(std::size_t repeats = 1);

  /// \brief Destroys all the gates and wires that were constructed until now.
  void Reset();

  /// \brief Interprets the gates and wires as newly created, i.e., Party::Run()
  /// can be executed again.
  void Clear();

  /// \brief Gets a std::vector of std::unique_ptrs to locally constructed ABYN parties connected
  /// via TCP.
  /// @param num_parties Number of ABYN parties.
  /// @param port TCP port offset.
  /// @param logging Enables/disables logging completely.
  static std::vector<std::unique_ptr<Party>> GetNLocalParties(std::size_t num_parties,
                                                              std::uint16_t port,
                                                              bool logging = false);

  const auto &GetLogger() { return backend_->GetLogger(); }

  /// \brief Sends a termination message to all of the connected parties.
  /// In case a TCP connection is used, this will internally be interpreted as a signal to
  /// disconnect.
  ///
  /// This method is executed by the ABYN::Party destructor, but if the parties are run locally,
  /// e.g., for testing purposes, the user SHALL ensure that Party::Finish() is run in parallel
  /// or otherwise the desctructors will likely be called sequentially which will result in a
  /// deadlock, since both connected parties must have sent a termination message and the
  /// destructor will wait for the other party to send the signal.
  /// It is allowed to call Party::Finish() multiple times.
  void Finish();

 private:
  ConfigurationPtr config_;
  BackendPtr backend_;
  bool finished_ = false;
  bool connected_ = false;

  void EvaluateCircuit();
};

using PartyPtr = std::unique_ptr<Party>;
}  // namespace ABYN