// Copyright 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#include "tink/aead/aead_config.h"

#include "tink/config.h"
#include "tink/registry.h"
#include "tink/aead/aead_catalogue.h"
#include "tink/mac/mac_config.h"
#include "tink/util/status.h"
#include "proto/config.pb.h"


using google::crypto::tink::RegistryConfig;

namespace crypto {
namespace tink {

namespace {

google::crypto::tink::RegistryConfig* GenerateRegistryConfig() {
  google::crypto::tink::RegistryConfig* config =
      new google::crypto::tink::RegistryConfig();
  config->MergeFrom(MacConfig::Latest());
  config->add_entry()->MergeFrom(*Config::GetTinkKeyTypeEntry(
      AeadConfig::kCatalogueName, AeadConfig::kPrimitiveName,
      "AesCtrHmacAeadKey", 0, true));
  config->add_entry()->MergeFrom(*Config::GetTinkKeyTypeEntry(
      AeadConfig::kCatalogueName, AeadConfig::kPrimitiveName,
      "AesGcmKey", 0, true));
  config->add_entry()->MergeFrom(*Config::GetTinkKeyTypeEntry(
      AeadConfig::kCatalogueName, AeadConfig::kPrimitiveName,
      "AesEaxKey", 0, true));
  config->add_entry()->MergeFrom(*Config::GetTinkKeyTypeEntry(
      AeadConfig::kCatalogueName, AeadConfig::kPrimitiveName,
      "XChaCha20Poly1305Key", 0, true));
  config->set_config_name("TINK_AEAD");
  return config;
}

}  // anonymous namespace

constexpr char AeadConfig::kCatalogueName[];
constexpr char AeadConfig::kPrimitiveName[];

// static
const google::crypto::tink::RegistryConfig& AeadConfig::Latest() {
  static const auto config = GenerateRegistryConfig();
  return *config;
}

// static
util::Status AeadConfig::Register() {
  auto status = MacConfig::Register();
  if (!status.ok()) return status;
  status = Registry::AddCatalogue(kCatalogueName, new AeadCatalogue());
  if (!status.ok()) return status;
  return Config::Register(Latest());
}

}  // namespace tink
}  // namespace crypto
