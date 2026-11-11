/*******************************************************************************

Feature configuration composition for Blockstore sources. Apply the named
feature policy after the aggregate has been merged using protobuf rules.

*******************************************************************************/

#pragma once

#include <cloud/blockstore/config/blockstore.pb.h>

#include <cloud/storage/core/libs/common/error.h>

#include <util/generic/string.h>
#include <util/generic/vector.h>

namespace NCloud::NBlockStore {

// Prepare the incoming Features section in dynamicConfigCandidate and report
// detected issues.
// Return success if accepted. On rejection, restore the section from the last
// accepted dynamic configuration in previousAcceptedDynamicConfig, or clear it
// if no previous value exists, and return an error describing the rejection.
NCloud::NProto::TError PrepareFeaturesSection(
    NProto::TBlockstoreConfig& dynamicConfigCandidate,
    const NProto::TBlockstoreConfig* previousAcceptedDynamicConfig,
    TVector<TString>& issues);

// Replace feature definitions in result by name, keeping the first occurrence
// within each source and the static record positions.
// result must contain both configurations combined using protobuf MergeFrom().
void MergeFeaturesSection(
    const NProto::TBlockstoreConfig& staticConfig,
    const NProto::TBlockstoreConfig& dynamicConfig,
    NProto::TBlockstoreConfig& result);

}   // namespace NCloud::NBlockStore
