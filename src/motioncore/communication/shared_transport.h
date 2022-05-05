#pragma once

#include "transport.h"

namespace encrypto::motion::communication {

  class SharedTransport : public Transport {
  public:
    SharedTransport() : repr_() {}

    SharedTransport(std::shared_ptr<Transport> repr) : repr_(repr) {};

    SharedTransport(const SharedTransport& other) : repr_(other.repr_) {};

    SharedTransport &operator=(const SharedTransport& other) {
      repr_ = other.repr_;
      return *this;
    }

    ~SharedTransport() {};

    void SendMessage(std::vector<std::uint8_t>&& message) {
      repr_->SendMessage(message);
    }

    void SendMessage(const std::vector<std::uint8_t>& message) {
      repr_->SendMessage(message);
    }

    bool Available() const {
      return repr_->Available();
    }

    std::optional<std::vector<std::uint8_t>> ReceiveMessage() {
      return repr_->ReceiveMessage();
    }

    void ShutdownSend() {
      return;
    }

    void Shutdown() {
      return;
    }

    void Drop() {
      repr_->Shutdown();
    }

  private:
    std::shared_ptr<Transport> repr_;
  };
}

/*

namespace encrypto::motion::communication {

class SharedTransport : public Transport {
 public:
 SharedTransport(std::shared_ptr<Transport> repr) : repr_(repr) {};
  ~SharedTransport() = default;
 private:
  std::shared_ptr<Transport> repr_;
};
*/
