#pragma once

namespace YT {

using FlatPtr = __UINTPTR_TYPE__;

using Uint8 = __UINT8_TYPE__;
using Uint16 = __UINT16_TYPE__;
using Uint32 = __UINT32_TYPE__;
using Uint64 = __UINT64_TYPE__;

using Int8 = __INT8_TYPE__;
using Int16 = __INT16_TYPE__;
using Int32 = __INT32_TYPE__;
using Int64 = __INT64_TYPE__;

using Size = __SIZE_TYPE__;
using PtrDiff = __PTRDIFF_TYPE__;

using HashCode = __UINTPTR_TYPE__;

}

namespace std {
using nullptr_t = decltype(nullptr);
}