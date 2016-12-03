#pragma once

#define HANA_CONFIG_VERSION(version,revision,patch) \
    (((version)<<24)+((revision)<<16)+(patch))

#define HANA_MAJOR_VERSION 1
#define HANA_MINOR_VERSION 0
#define HANA_PATCH_VERSION 2

#define HANA_VERSION \
    HANA_CONFIG_VERSION(HANA_MAJOR_VERSION,HANA_MINOR_VERSION,HANA_PATCH_VERSION)