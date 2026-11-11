/*******************************************************************************

  * Section processing composes explicit handlers shared by startup and runtime.

  * Every handler owns its section's validation and fallback. Diagnostics retain
    handler order and distinguish skipped entries from rejected sections.

  * No live configuration or logging state is changed here.

*******************************************************************************/

#include "sections_processing.h"

#include "sections_processing/features.h"
#include "sections_processing/volume_throttling.h"

#include <util/generic/algorithm.h>
#include <util/string/builder.h>

#include <utility>

namespace NCloud::NBlockStore {

bool TConfigSectionsPreparationIssues::HasRejectedSections() const
{
    return AnyOf(
        Sections,
        [](const auto& section) { return HasError(section.RejectionError); });
}

TConfigSectionsPreparationIssues PrepareDynamicConfigSections(
    NProto::TBlockstoreConfig& dynamicConfigCandidate,
    const NProto::TBlockstoreConfig* previousAcceptedDynamicConfig)
{
    TConfigSectionsPreparationIssues result;

    // Give each handler its own issue buffer and collect rejections without
    // preventing the remaining sections from being prepared.
    const auto prepareSection = [&](const TString& path, auto prepare)
    {
        TConfigSectionPreparationIssues section;
        section.Path = path;
        section.RejectionError = prepare(
            dynamicConfigCandidate,
            previousAcceptedDynamicConfig,
            section.Issues);
        if (!section.Issues.empty() || HasError(section.RejectionError)) {
            result.Sections.push_back(std::move(section));
        }
    };

    prepareSection("Features", PrepareFeaturesSection);
    prepareSection("VolumeThrottling", PrepareVolumeThrottlingSection);
    return result;
}

void MergeConfigSections(
    const NProto::TBlockstoreConfig& staticConfig,
    const NProto::TBlockstoreConfig& dynamicConfig,
    NProto::TBlockstoreConfig& result)
{
    MergeVolumeThrottlingSection(staticConfig, dynamicConfig, result);
    MergeFeaturesSection(staticConfig, dynamicConfig, result);
}

TString FormatConfigSectionRejection(
    const TConfigSectionPreparationIssues& section)
{
    return TStringBuilder()
           << "Rejected invalid " << section.Path
           << " configuration: " << section.RejectionError.GetMessage();
}

}   // namespace NCloud::NBlockStore
