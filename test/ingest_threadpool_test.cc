// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include <cstdint>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "gtest/gtest.h"

#include "adapters/simdjson_adapter.h"
#include <device/file_system_disk.h>

using handler_t = fishstore::environment::ThreadPoolIoHandler;

#define CLASS IngestTest_ThreadPool
#include "ingest_test.h"
#undef CLASS

#ifdef USE_EZPSF
#define CLASS IngestTest_ThreadPoolEzPsf
#else
#define CLASS IngestTest_ThreadPool
#endif
#include "ingest_test.h"
#undef CLASS

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}  
