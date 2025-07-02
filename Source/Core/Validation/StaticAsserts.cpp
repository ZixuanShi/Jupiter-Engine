// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_CONFIG_RELEASE && IS_EDITOR

static_assert(false, "Release configuration should not have editor builds");

#endif