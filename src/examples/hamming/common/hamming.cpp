// MIT License
//
// Copyright (c) 2019 Oleksandr Tkachenko, Daniel Demmler, Tobias Kussel
// Cryptography and Privacy Engineering Group (ENCRYPTO)
// TU Darmstadt, Germany
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "hamming.h"

#include <cstddef>
#include <vector>
#include <limits>

#include "protocols/share_wrapper.h"
#include "secure_type/secure_unsigned_integer.h"
#include "utility/config.h"

// abbreviate namespace
namespace mo = encrypto::motion;

void EvaluateProtocol(mo::PartyPointer& party, std::uint32_t value) {
  mo::SecureUnsignedInteger zero(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 0), 0));
  mo::SecureUnsignedInteger one(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 1), 0));

  std::vector<mo::SecureUnsignedInteger> inputA(value);
  std::vector<mo::SecureUnsignedInteger> inputB(value);

  for (uint32_t i = 0; i < value / 4; i++) {
    inputA[4 * i]     = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 1), 0));
    inputA[4 * i + 1] = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 0), 0));
    inputA[4 * i + 2] = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 1), 0));
    inputA[4 * i + 3] = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 1), 0));
  }

  for (uint32_t i = 0; i < value / 4; i++) {
    inputB[4 * i]     = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 1), 1));
    inputB[4 * i + 1] = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 0), 1));
    inputB[4 * i + 2] = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 1), 1));
    inputB[4 * i + 3] = mo::SecureUnsignedInteger(party->In<mo::MpcProtocol::kBooleanGmw>(mo::ToInput((uint32_t) 0), 1));
  }

  auto distance = zero;

  for (uint32_t i = 0; i < value; i++) {
    distance += (inputA[i] == inputB[i]).Mux(zero.Get(), one.Get());
  }

  // construct an output gate
  auto output = distance.Out();

  // run the protocol
  party->Run();
  party->Finish();

  // retrieve the result
  auto result = output.As<std::uint32_t>();
  // print the result into the terminal
  std::cout << "Distance " << result << std::endl;
}
