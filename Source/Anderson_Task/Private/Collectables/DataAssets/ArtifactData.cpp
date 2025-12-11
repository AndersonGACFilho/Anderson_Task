#include "Collectables/DataAssets/ArtifactData.h"

FPrimaryAssetId UArtifactData::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("Artifact", GetFName());
}
