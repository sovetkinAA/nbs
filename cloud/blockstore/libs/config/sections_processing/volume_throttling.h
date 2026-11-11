/*******************************************************************************

Service volume-throttling configuration composition and input validation.

*******************************************************************************/

#pragma once

#include <cloud/blockstore/config/blockstore.pb.h>
#include <cloud/blockstore/config/volume_throttling.pb.h>

#include <cloud/storage/core/libs/common/error.h>

#include <util/generic/string.h>
#include <util/generic/vector.h>

namespace NCloud::NBlockStore {

// Prepare the incoming VolumeThrottling section in dynamicConfigCandidate and
// report detected issues.
// Return success if accepted. On rejection, restore the section from the last
// accepted dynamic configuration in previousAcceptedDynamicConfig, or clear it
// if no previous value exists, and return an error describing the rejection.
NCloud::NProto::TError PrepareVolumeThrottlingSection(
    NProto::TBlockstoreConfig& dynamicConfigCandidate,
    const NProto::TBlockstoreConfig* previousAcceptedDynamicConfig,
    TVector<TString>& issues);

// Replace the throttling section in a protobuf-merged result using its rule
// composition policy without creating an absent section.
// result must contain both configurations combined using protobuf MergeFrom().
void MergeVolumeThrottlingSection(
    const NProto::TBlockstoreConfig& staticConfig,
    const NProto::TBlockstoreConfig& dynamicConfig,
    NProto::TBlockstoreConfig& result);

// Ignore unknown fields and unsupported selectors; report descriptions of
// detected issues and leave config unchanged if a retained coefficient is
// invalid.
NCloud::NProto::TError PrepareStaticVolumeThrottlingConfig(
    NProto::TVolumeThrottlingServiceConfig& config,
    TVector<TString>& issues);

// Prepare dynamic rules, skipping empty Coefficients and retaining deletions;
// report descriptions of detected issues and leave config unchanged on invalid
// active rules.
NCloud::NProto::TError PrepareDynamicVolumeThrottlingConfig(
    NProto::TVolumeThrottlingServiceConfig& config,
    TVector<TString>& issues);

// Build merged config from static and dynamic rules.
// Put dynamic rules with coefficients before static rules not replaced or
// deleted by dynamic rules.
void MergeVolumeThrottlingConfig(
    const NProto::TVolumeThrottlingServiceConfig& staticConfig,
    const NProto::TVolumeThrottlingServiceConfig& dynamicConfig,
    NProto::TVolumeThrottlingServiceConfig* result);

}   // namespace NCloud::NBlockStore
