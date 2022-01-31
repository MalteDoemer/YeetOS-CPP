/*
 * Copyright 2021 Malte Dömer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <Types.hpp>

namespace YT {

/// IntegralConstant
template<typename T, T val>
struct IntegralConstant {
    using ValueType = T;
    static constexpr T value = val;
    constexpr operator T() const noexcept {
        return value;
    }
    constexpr T operator()() const noexcept {
        return value;
    }
};

/// declval
template<typename T>
auto declval() -> T&&;

/// BoolConstant
template<bool b>
struct BoolConstant : public IntegralConstant<bool, b> {};

/// TrueType
struct TrueType : public BoolConstant<true> {};

/// FalseType
struct FalseType : public BoolConstant<false> {};

namespace Detail {

template<typename>
struct RemoveCV;

template<bool condition, typename TypeIfTrue, typename TypeIfFalse>
struct Conditional;

/// TypeList
template<typename...>
struct TypeList {};

/// TypeList
template<typename T, typename... U>
struct TypeList<T, U...> {
    static constexpr usize size = sizeof(T);
};

/// TypeSelect
template<usize s, typename T, bool = (s <= T::size)>
struct TypeSelect;

/// TypeSelect
template<usize s, typename UInt, typename... UInts>
struct TypeSelect<s, TypeList<UInt, UInts...>, true> {
    using Type = UInt;
};

/// TypeSelect
template<usize s, typename UInt, typename... UInts>
struct TypeSelect<s, TypeList<UInt, UInts...>, false> : TypeSelect<s, TypeList<UInts...>> {};

/// LogicalOr
template<bool...>
struct LogicalOr;

/// LogicalOr
template<>
struct LogicalOr<> : public FalseType {};

/// LogicalOr
template<bool B1>
struct LogicalOr<B1> : public BoolConstant<B1> {};

/// LogicalOr
template<bool B1, bool B2>
struct LogicalOr<B1, B2> : public Conditional<B1, BoolConstant<B1>, BoolConstant<B2>>::Type {};

/// LogicalOr
template<bool B1, bool B2, bool B3, bool... Bn>
struct LogicalOr<B1, B2, B3, Bn...> : public Conditional<B1, BoolConstant<B1>, LogicalOr<B2, B3, Bn...>>::Type {};

/// LogicalAnd
template<bool...>
struct LogicalAnd;

/// LogicalAnd
template<>
struct LogicalAnd<> : public TrueType {};

/// LogicalAnd
template<bool B1>
struct LogicalAnd<B1> : public BoolConstant<B1> {};

/// LogicalAnd
template<bool B1, bool B2>
struct LogicalAnd<B1, B2> : public Conditional<B1, BoolConstant<B2>, BoolConstant<B1>>::Type {};

/// LogicalAnd
template<bool B1, bool B2, bool B3, bool... Bn>
struct LogicalAnd<B1, B2, B3, Bn...> : public Conditional<B1, LogicalAnd<B2, B3, Bn...>, BoolConstant<B1>>::Type {};

/// LogicalNot
template<bool B>
struct LogicalNot : public BoolConstant<!B> {};

/// DependantFalse
template<typename T>
struct DependantFalse : BoolConstant<false> {};

/// IsVoid
template<typename T>
struct IsVoid : public BoolConstant<__is_void(T)> {};

template<typename>
struct IsNullPointerHelper : public FalseType {};

template<>
struct IsNullPointerHelper<decltype(nullptr)> : public TrueType {};

/// IsNullPointer
template<typename T>
struct IsNullPointer : public IsNullPointerHelper<typename RemoveCV<T>::Type> {};

/// IsIntegral
template<typename T>
struct IsIntegral : public BoolConstant<__is_integral(T)> {};

/// IsFloatingPoint
template<typename T>
struct IsFloatingPoint : public BoolConstant<__is_floating_point(T)> {};

/// IsArray
template<typename T>
struct IsArray : public BoolConstant<__is_array(T)> {};

/// IsEnum
template<typename T>
struct IsEnum : public BoolConstant<__is_enum(T)> {};

/// IsUnion
template<typename T>
struct IsUnion : public BoolConstant<__is_union(T)> {};

/// IsClass
template<typename T>
struct IsClass : public BoolConstant<__is_class(T)> {};

/// IsFunction
template<typename T>
struct IsFunction : public BoolConstant<__is_function(T)> {};

/// IsPointer
template<typename T>
struct IsPointer : public BoolConstant<__is_pointer(T)> {};

/// IsLvalueReference
template<typename T>
struct IsLvalueReference : public BoolConstant<__is_lvalue_reference(T)> {};

/// IsRvalueReference
template<typename T>
struct IsRvalueReference : public BoolConstant<__is_lvalue_reference(T)> {};

/// IsMemberObjectPointer
template<typename T>
struct IsMemberObjectPointer : public BoolConstant<__is_member_object_pointer(T)> {};

/// IsMemberFunctionPointer
template<typename T>
struct IsMemberFunctionPointer : public BoolConstant<__is_member_function_pointer(T)> {};

/// IsFundamental
template<typename T>
struct IsFundamental : public BoolConstant<__is_fundamental(T)> {};

/// IsArithmetic
template<typename T>
struct IsArithmetic : public BoolConstant<__is_arithmetic(T)> {};

/// IsScalar
template<typename T>
struct IsScalar : public BoolConstant<__is_scalar(T)> {};

/// IsObject
template<typename T>
struct IsObject : public BoolConstant<__is_object(T)> {};

/// IsCompound
template<typename T>
struct IsCompound : public BoolConstant<__is_compound(T)> {};

/// IsReference
template<typename T>
struct IsReference : public BoolConstant<__is_reference(T)> {};

/// IsMemberPointer
template<typename T>
struct IsMemberPointer : public BoolConstant<__is_member_pointer(T)> {};

/// IsConst
template<typename T>
struct IsConst : public BoolConstant<__is_const(T)> {};

/// IsVolatile
template<typename T>
struct IsVolatile : public BoolConstant<__is_volatile(T)> {};

/// IsTrivial
template<typename T>
struct IsTrivial : public BoolConstant<__is_trivial(T)> {};

/// IsTriviallyCopyable
template<typename T>
struct IsTriviallyCopyable : public BoolConstant<__is_trivially_copyable(T)> {};

/// IsStandardLayout
template<typename T>
struct IsStandardLayout : public BoolConstant<__is_standard_layout(T)> {};

/// HasUniqueObjectRepresentations
template<typename T>
struct HasUniqueObjectRepresentations : public BoolConstant<__has_unique_object_representations(T)> {};

/// IsEmpty
template<typename T>
struct IsEmpty : public BoolConstant<__is_empty(T)> {};

/// IsPolymorphic
template<typename T>
struct IsPolymorphic : public BoolConstant<__is_polymorphic(T)> {};

/// IsAbstract
template<typename T>
struct IsAbstract : public BoolConstant<__is_abstract(T)> {};

/// IsFinal
template<typename T>
struct IsFinal : public BoolConstant<__is_final(T)> {};

/// IsAggregate
template<typename T>
struct IsAggregate : public BoolConstant<__is_aggregate(T)> {};

/// IsSigned
template<typename T>
struct IsSigned : public BoolConstant<__is_signed(T)> {};

/// IsUnsigned
template<typename T>
struct IsUnsigned : public BoolConstant<__is_unsigned(T)> {};

template<typename>
struct IsBoundedArray : public FalseType {};

/// IsBoundedArray
template<typename T, usize N>
struct IsBoundedArray<T[N]> : public TrueType {};

template<typename>
struct IsUnboundedArray : public FalseType {};

/// IsUnboundedArray
template<typename T>
struct IsUnboundedArray<T[]> : public TrueType {};

/// IsConstructible
template<typename T, typename... Args>
struct IsConstructible : public BoolConstant<__is_constructible(T, Args...)> {};

/// IsTriviallyConstructible
template<typename T, typename... Args>
struct IsTriviallyConstructible : public BoolConstant<__is_trivially_constructible(T, Args...)> {};

/// IsNothrowConstructible
template<typename T, typename... Args>
struct IsNothrowConstructible : public BoolConstant<__is_nothrow_constructible(T, Args...)> {};

/// IsDefaultConstructible
template<typename T>
struct IsDefaultConstructible : public IsConstructible<T> {};

/// IsTriviallyDefaultConstructible
template<typename T>
struct IsTriviallyDefaultConstructible : public IsTriviallyConstructible<T> {};

/// IsNothrowDefaultConstructible
template<typename T>
struct IsNothrowDefaultConstructible : public IsNothrowConstructible<T> {};

/// IsCopyConstructible
template<typename T>
struct IsCopyConstructible : public IsConstructible<T, const T&> {};

/// IsTriviallyCopyConstructible
template<typename T>
struct IsTriviallyCopyConstructible : public IsTriviallyConstructible<T, const T&> {};

/// IsNothrowCopyConstructible
template<typename T>
struct IsNothrowCopyConstructible : public IsNothrowConstructible<T, const T&> {};

/// IsMoveConstructible
template<typename T>
struct IsMoveConstructible : public IsConstructible<T, T&&> {};

/// IsTriviallyMoveConstructible
template<typename T>
struct IsTriviallyMoveConstructible : public IsTriviallyConstructible<T, T&&> {};

/// IsNothrowMoveConstructible
template<typename T>
struct IsNothrowMoveConstructible : public IsNothrowConstructible<T, T&&> {};

/// IsAssignable
template<typename T, typename U>
struct IsAssignable : public BoolConstant<__is_assignable(T, U)> {};

/// IsTriviallyAssignable
template<typename T, typename U>
struct IsTriviallyAssignable : public BoolConstant<__is_trivially_assignable(T, U)> {};

/// IsNothrowAssignable
template<typename T, typename U>
struct IsNothrowAssignable : public BoolConstant<__is_nothrow_assignable(T, U)> {};

/// IsCopyAssignable
template<typename T>
struct IsCopyAssignable : public IsAssignable<T&, const T&> {};

/// IsTriviallyCopyAssignable
template<typename T>
struct IsTriviallyCopyAssignable : public IsTriviallyAssignable<T&, const T&> {};

/// IsNothrowCopyAssignable
template<typename T>
struct IsNothrowCopyAssignable : public IsNothrowAssignable<T&, const T&> {};

/// IsMoveAssignable
template<typename T>
struct IsMoveAssignable : public IsAssignable<T&, T&&> {};

/// IsTriviallyMoveAssignable
template<typename T>
struct IsTriviallyMoveAssignable : public IsTriviallyAssignable<T&, T&&> {};

/// IsNothrowMoveAssignable
template<typename T>
struct IsNothrowMoveAssignable : public IsNothrowAssignable<T&, T&&> {};

template<typename T, typename U>
struct IsSame;

template<typename T>
struct RemoveAllExtents;

template<typename T>
auto destruct_test(T&&) -> decltype(declval<T&>().~T(), TrueType());

auto destruct_test(...) -> FalseType;

template<typename T,
         bool = LogicalOr<IsVoid<T>::value, IsFunction<T>::value, IsUnboundedArray<T>::value>::value,
         bool = LogicalOr<IsReference<T>::value, IsScalar<T>::value>::value>
struct IsDestructibleHelper;

template<typename T>
struct IsDestructibleHelper<T, false, false> :
    public decltype(destruct_test(declval<typename RemoveAllExtents<T>::Type>())) {};

template<typename T>
struct IsDestructibleHelper<T, true, false> : public FalseType {};

template<typename T>
struct IsDestructibleHelper<T, false, true> : public TrueType {};

/// IsDestructible
template<typename T>
struct IsDestructible : public IsDestructibleHelper<T> {};

/// IsTriviallyDestructible
template<typename T>
struct IsTriviallyDestructible : public BoolConstant<__is_trivially_destructible(T)> {};

template<typename T>
auto nt_destruct_test(T&&) -> BoolConstant<noexcept(declval<T&>().~T())>;
auto nt_destruct_test(...) -> FalseType;

template<typename T,
         bool = LogicalOr<IsVoid<T>::value, IsFunction<T>::value, IsUnboundedArray<T>::value>::value,
         bool = LogicalOr<IsReference<T>::value, IsScalar<T>::value>::value>
struct IsNothrowDestructibleHelper;

template<typename T>
struct IsNothrowDestructibleHelper<T, false, false> :
    public decltype(nt_destruct_test(declval<typename RemoveAllExtents<T>::Type>())) {};

template<typename T>
struct IsNothrowDestructibleHelper<T, true, false> : public FalseType {};

template<typename T>
struct IsNothrowDestructibleHelper<T, false, true> : public TrueType {};

/// IsNothrowDestructible
template<typename T>
struct IsNothrowDestructible : public IsNothrowDestructibleHelper<T> {};

/// HasVirtualDestructor
template<typename T>
struct HasVirtualDestructor : public BoolConstant<__has_virtual_destructor(T)> {};

/// AlignmentOf
template<typename T>
struct AlignmentOf : public IntegralConstant<usize, alignof(T)> {};

/// Rank
template<typename T>
struct Rank : public IntegralConstant<usize, __array_rank(T)> {};

/// Extent
template<typename T, unsigned N = 0>
struct Extent : public IntegralConstant<usize, __array_extent(T, N)> {};

/// IsSame
template<typename T, typename U>
struct IsSame : public BoolConstant<__is_same(T, U)> {};

/// IsBaseOf
template<typename Base, typename Derived>
struct IsBaseOf : public BoolConstant<__is_base_of(Base, Derived)> {};

/// IsConvertible
template<typename From, typename To>
struct IsConvertible : public BoolConstant<__is_convertible(From, To)> {};

#if __has_builtin(__is_layout_compatible)

/// IsLayoutCompatible
template<typename T>
struct IsLayoutCompatible : public BoolConstant<(T)> {};

#endif

/// RemoveConst
template<typename T>
struct RemoveConst {
    using Type = T;
};

/// RemoveConst
template<typename T>
struct RemoveConst<const T> {
    using Type = T;
};

/// RemoveVolatile
template<typename T>
struct RemoveVolatile {
    using Type = T;
};

/// RemoveVolatile
template<typename T>
struct RemoveVolatile<volatile T> {
    using Type = T;
};

/// RemoveCV
template<typename T>
struct RemoveCV {
    using Type = typename RemoveVolatile<typename RemoveConst<T>::Type>::Type;
};

/// AddConst
template<typename T>
struct AddConst {
    using Type = const T;
};

/// AddVolatile
template<typename T>
struct AddVolatile {
    using Type = volatile T;
};

/// AddCV
template<typename T>
struct AddCV {
    using Type = const volatile T;
};

/// RemoveReference
template<typename T>
struct RemoveReference {
    using Type = T;
};

/// RemoveReference
template<typename T>
struct RemoveReference<T&> {
    using Type = T;
};

/// RemoveReference
template<typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

/// AddLvalueReference
template<typename T>
struct AddLvalueReference {
    using Type = T&;
};

/// AddRvalueReference
template<typename T>
struct AddRvalueReference {
    using Type = T&&;
};

/// RemovePointer
template<typename T>
struct RemovePointer {
    using Type = T;
};

/// RemovePointer
template<typename T>
struct RemovePointer<T*> {
    using Type = T;
};

/// AddPointer
template<typename T>
struct AddPointer {
    using Type = T*;
};

/// RemoveCVRef
template<typename T>
struct RemoveCVRef {
    using Type = typename RemoveCV<typename RemoveReference<T>::Type>::Type;
};

/// LValueReferenceOf
template<typename T>
struct LValueReferenceOf {
    using Type = typename RemoveCVRef<T>::Type&;
};

/// RValueReferenceOf
template<typename T>
struct RValueReferenceOf {
    using Type = typename RemoveCVRef<T>::Type&&;
};

/// ConstLValueReferenceOf
template<typename T>
struct ConstLValueReferenceOf {
    using Type = const typename RemoveCVRef<T>::Type&;
};

/// ConstRValueReferenceOf
template<typename T>
struct ConstRValueReferenceOf {
    using Type = const typename RemoveCVRef<T>::Type&&;
};

template<typename T, bool have_const, bool have_volatile>
struct CVSelector;

template<typename T>
struct CVSelector<T, false, false> {
    using Type = T;
};

template<typename T>
struct CVSelector<T, true, false> {
    using Type = const T;
};

template<typename T>
struct CVSelector<T, false, true> {
    using Type = volatile T;
};

template<typename T>
struct CVSelector<T, true, true> {
    using Type = const volatile T;
};

template<typename Qualified, typename Unqalified>
struct MatchCV {
    using Type = typename CVSelector<Unqalified, IsConst<Qualified>::value, IsVolatile<Qualified>::value>::Type;
};

template<typename T>
struct MakeUnsignedHelper {
    using Type = T;
};

template<>
struct MakeUnsignedHelper<char> {
    using Type = unsigned char;
};

template<>
struct MakeUnsignedHelper<signed char> {
    using Type = unsigned char;
};

template<>
struct MakeUnsignedHelper<short> {
    using Type = unsigned short;
};

template<>
struct MakeUnsignedHelper<int> {
    using Type = unsigned int;
};

template<>
struct MakeUnsignedHelper<long> {
    using Type = unsigned long;
};

template<>
struct MakeUnsignedHelper<long long> {
    using Type = unsigned long long;
};

template<typename T, bool is_int = IsIntegral<T>::value, bool is_enum = IsEnum<T>::value>
struct MakeUnsignedSelector;

template<typename T>
struct MakeUnsignedSelector<T, true, false> {
private:
    using UnsignedType = typename MakeUnsignedHelper<typename RemoveCV<T>::Type>::Type;

public:
    using Type = typename MatchCV<T, UnsignedType>::Type;
};

template<typename T>
struct MakeUnsignedSelector<T, false, true> {
private:
    using Uints = TypeList<unsigned char, unsigned short, unsigned long, unsigned long long>;
    using UnsignedType = typename TypeSelect<sizeof(T), Uints>::Type;

public:
    using Type = typename MatchCV<T, UnsignedType>::Type;
};

// wchar_t, char8_t, char16_t, char32_t must be converted to a unsigned type of the same size
// this is done by treating them as an enum.

template<>
struct MakeUnsignedHelper<wchar_t> {
    using Type = typename MakeUnsignedSelector<wchar_t, false, true>::Type;
};

template<>
struct MakeUnsignedHelper<char8_t> {
    using Type = typename MakeUnsignedSelector<char8_t, false, true>::Type;
};

template<>
struct MakeUnsignedHelper<char16_t> {
    using Type = typename MakeUnsignedSelector<char16_t, false, true>::Type;
};

template<>
struct MakeUnsignedHelper<char32_t> {
    using Type = typename MakeUnsignedSelector<char32_t, false, true>::Type;
};

// MakeUnsigned with bool isn't allowed

template<>
struct MakeUnsignedHelper<bool>;

/// MakeUnsigned
template<typename T>
struct MakeUnsigned {
    using Type = typename MakeUnsignedSelector<T>::Type;
};

template<typename T>
struct MakeSignedHelper {
    using Type = T;
};

template<>
struct MakeSignedHelper<char> {
    using Type = signed char;
};

template<>
struct MakeSignedHelper<unsigned char> {
    using Type = signed char;
};

template<>
struct MakeSignedHelper<unsigned short> {
    using Type = signed short;
};

template<>
struct MakeSignedHelper<unsigned int> {
    using Type = signed int;
};

template<>
struct MakeSignedHelper<unsigned long> {
    using Type = signed long;
};

template<>
struct MakeSignedHelper<unsigned long long> {
    using Type = signed long long;
};

template<typename T, bool is_int = IsIntegral<T>::value, bool is_enum = IsEnum<T>::value>
struct MakeSignedSelector;

template<typename T>
struct MakeSignedSelector<T, true, false> {
private:
    using SignedType = typename MakeSignedHelper<typename RemoveCV<T>::Type>::Type;

public:
    using Type = typename MatchCV<T, SignedType>::Type;
};

template<typename T>
struct MakeSignedSelector<T, false, true> {
private:
    using UnsignedType = typename MakeUnsignedSelector<T>::Type;

public:
    using Type = typename MakeSignedSelector<UnsignedType>::Type;
};

template<>
struct MakeSignedHelper<wchar_t> {
    using Type = typename MakeSignedSelector<wchar_t, false, true>::Type;
};

template<>
struct MakeSignedHelper<char8_t> {
    using Type = typename MakeSignedSelector<char8_t, false, true>::Type;
};

template<>
struct MakeSignedHelper<char16_t> {
    using Type = typename MakeSignedSelector<char16_t, false, true>::Type;
};

template<>
struct MakeSignedHelper<char32_t> {
    using Type = typename MakeSignedSelector<char32_t, false, true>::Type;
};

// MakeSigned with bool isn't allowed

template<>
struct MakeSignedHelper<bool>;

/// MakeSigned
template<typename T>
struct MakeSigned : public MakeSignedSelector<T> {};

/// RemoveExtent
template<typename T>
struct RemoveExtent {
    using Type = T;
};

/// RemoveExtent
template<typename T>
struct RemoveExtent<T[]> {
    using Type = T;
};

/// RemoveExtent
template<typename T, usize N>
struct RemoveExtent<T[N]> {
    using Type = T;
};

/// RemoveAllExtents
template<typename T>
struct RemoveAllExtents {
    using Type = T;
};

/// RemoveAllExtents
template<typename T>
struct RemoveAllExtents<T[]> {
    using Type = typename RemoveAllExtents<T>::Type;
};

/// RemoveAllExtents
template<typename T, usize N>
struct RemoveAllExtents<T[N]> {
    using Type = typename RemoveAllExtents<T>::Type;
};

/// EnableIf
template<bool B, typename T = void>
struct EnableIf {};

/// EnableIf
template<typename T>
struct EnableIf<true, T> {
    using Type = T;
};

/// Conditional
template<typename TypeIfTrue, typename TypeIfFalse>
struct Conditional<true, TypeIfTrue, TypeIfFalse> {
    using Type = TypeIfTrue;
};

/// Conditional
template<typename TypeIfTrue, typename TypeIfFalse>
struct Conditional<false, TypeIfTrue, TypeIfFalse> {
    using Type = TypeIfFalse;
};

/// Void
template<typename...>
using Void = void;

/// UnderlyingType
template<typename T>
struct UnderlyingType {
    using Type = __underlying_type(T);
};

/// IsCompileTimeKnown
template<typename T>
constexpr bool IsCompileTimeKnown(T value) {
    return __builtin_constant_p(value);
}

} // namespace Detail

template<bool... B>
inline constexpr bool logical_or = Detail::LogicalOr<B...>::value;

template<bool... B>
inline constexpr bool logical_and = Detail::LogicalAnd<B...>::value;

template<bool B>
inline constexpr bool logical_not = Detail::LogicalNot<B>::value;

template<typename T>
inline constexpr bool dependant_false = Detail::DependantFalse<T>::value;

template<typename T>
inline constexpr bool is_void = Detail::IsVoid<T>::value;

template<typename T>
inline constexpr bool is_null_pointer = Detail::IsNullPointer<T>::value;

template<typename T>
inline constexpr bool is_integral = Detail::IsIntegral<T>::value;

template<typename T>
inline constexpr bool is_floating_point = Detail::IsFloatingPoint<T>::value;

template<typename T>
inline constexpr bool is_array = Detail::IsArray<T>::value;

template<typename T>
inline constexpr bool is_enum = Detail::IsEnum<T>::value;

template<typename T>
inline constexpr bool is_union = Detail::IsUnion<T>::value;

template<typename T>
inline constexpr bool is_class = Detail::IsClass<T>::value;

template<typename T>
inline constexpr bool is_function = Detail::IsFunction<T>::value;

template<typename T>
inline constexpr bool is_pointer = Detail::IsPointer<T>::value;

template<typename T>
inline constexpr bool is_lvalue_reference = Detail::IsLvalueReference<T>::value;

template<typename T>
inline constexpr bool is_rvalue_reference = Detail::IsRvalueReference<T>::value;

template<typename T>
inline constexpr bool is_member_object_pointer = Detail::IsMemberObjectPointer<T>::value;

template<typename T>
inline constexpr bool is_member_function_pointer = Detail::IsMemberFunctionPointer<T>::value;

template<typename T>
inline constexpr bool is_fundamental = Detail::IsFundamental<T>::value;

template<typename T>
inline constexpr bool is_arithmetic = Detail::IsArithmetic<T>::value;

template<typename T>
inline constexpr bool is_scalar = Detail::IsScalar<T>::value;

template<typename T>
inline constexpr bool is_object = Detail::IsObject<T>::value;

template<typename T>
inline constexpr bool is_compound = Detail::IsCompound<T>::value;

template<typename T>
inline constexpr bool is_reference = Detail::IsReference<T>::value;

template<typename T>
inline constexpr bool is_member_pointer = Detail::IsMemberPointer<T>::value;

template<typename T>
inline constexpr bool is_const = Detail::IsConst<T>::value;

template<typename T>
inline constexpr bool is_volatile = Detail::IsVolatile<T>::value;

template<typename T>
inline constexpr bool is_trivial = Detail::IsTrivial<T>::value;

template<typename T>
inline constexpr bool is_trivially_copyable = Detail::IsTriviallyCopyable<T>::value;

template<typename T>
inline constexpr bool is_standard_layout = Detail::IsStandardLayout<T>::value;

template<typename T>
inline constexpr bool has_unique_object_representations = Detail::HasUniqueObjectRepresentations<T>::value;

template<typename T>
inline constexpr bool is_empty = Detail::IsEmpty<T>::value;

template<typename T>
inline constexpr bool is_polymorphic = Detail::IsPolymorphic<T>::value;

template<typename T>
inline constexpr bool is_abstract = Detail::IsAbstract<T>::value;

template<typename T>
inline constexpr bool is_final = Detail::IsFinal<T>::value;

template<typename T>
inline constexpr bool is_aggregate = Detail::IsAggregate<T>::value;

template<typename T>
inline constexpr bool is_signed = Detail::IsSigned<T>::value;

template<typename T>
inline constexpr bool is_unsigned = Detail::IsUnsigned<T>::value;

template<typename T>
inline constexpr bool is_bounded_array = Detail::IsBoundedArray<T>::value;

template<typename T>
inline constexpr bool is_unbounded_array = Detail::IsUnboundedArray<T>::value;

template<typename T, typename... Args>
inline constexpr bool is_constructible = Detail::IsConstructible<T, Args...>::value;

template<typename T, typename... Args>
inline constexpr bool is_trivially_constructible = Detail::IsTriviallyConstructible<T, Args...>::value;

template<typename T, typename... Args>
inline constexpr bool is_nothrow_constructible = Detail::IsNothrowConstructible<T, Args...>::value;

template<typename T>
inline constexpr bool is_default_constructible = Detail::IsDefaultConstructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_default_constructible = Detail::IsTriviallyDefaultConstructible<T>::value;

template<typename T>
inline constexpr bool is_nothrow_default_constructible = Detail::IsNothrowDefaultConstructible<T>::value;

template<typename T>
inline constexpr bool is_copy_constructible = Detail::IsCopyConstructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_copy_constructible = Detail::IsTriviallyCopyConstructible<T>::value;

template<typename T>
inline constexpr bool is_nothrow_copy_constructible = Detail::IsNothrowCopyConstructible<T>::value;

template<typename T>
inline constexpr bool is_move_constructible = Detail::IsMoveConstructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_move_constructible = Detail::IsTriviallyMoveConstructible<T>::value;

template<typename T>
inline constexpr bool is_nothrow_move_constructible = Detail::IsNothrowMoveConstructible<T>::value;

template<typename T, typename U>
inline constexpr bool is_assignable = Detail::IsAssignable<T, U>::value;

template<typename T, typename U>
inline constexpr bool is_trivially_assignable = Detail::IsTriviallyAssignable<T, U>::value;

template<typename T, typename U>
inline constexpr bool is_nothrow_assignable = Detail::IsNothrowAssignable<T, U>::value;

template<typename T>
inline constexpr bool is_copy_assignable = Detail::IsCopyAssignable<T>::value;

template<typename T>
inline constexpr bool is_trivially_copy_assignable = Detail::IsTriviallyCopyAssignable<T>::value;

template<typename T>
inline constexpr bool is_nothrow_copy_assignable = Detail::IsNothrowCopyAssignable<T>::value;

template<typename T>
inline constexpr bool is_move_assignable = Detail::IsMoveAssignable<T>::value;

template<typename T>
inline constexpr bool is_trivially_move_assignable = Detail::IsTriviallyMoveAssignable<T>::value;

template<typename T>
inline constexpr bool is_nothrow_move_assignable = Detail::IsNothrowMoveAssignable<T>::value;

template<typename T>
inline constexpr bool is_destructible = Detail::IsDestructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_destructible = Detail::IsTriviallyDestructible<T>::value;

template<typename T>
inline constexpr bool is_nothrow_destructible = Detail::IsNothrowDestructible<T>::value;

template<typename T>
inline constexpr bool has_virtual_destructor = Detail::HasVirtualDestructor<T>::value;

template<typename T>
inline constexpr usize alignment_of = Detail::AlignmentOf<T>::value;

template<typename T>
inline constexpr usize rank = Detail::Rank<T>::value;

template<typename T>
inline constexpr usize extent = Detail::Extent<T>::value;

template<typename T, typename U>
inline constexpr bool is_same = Detail::IsSame<T, U>::value;

template<typename Base, typename Derived>
inline constexpr bool is_base_of = Detail::IsBaseOf<Base, Derived>::value;

template<typename From, typename To>
inline constexpr bool is_convertible = Detail::IsConvertible<From, To>::value;

template<typename T>
inline constexpr bool is_movable = is_move_constructible<T>&& is_move_assignable<T>;

template<typename T>
inline constexpr bool is_nothrow_movable = is_nothrow_move_constructible<T>&& is_nothrow_move_assignable<T>;

template<typename T>
inline constexpr bool is_copyable = is_copy_constructible<T>&& is_copy_assignable<T>;

template<typename T>
inline constexpr bool is_nothrow_copyable = is_nothrow_copy_constructible<T>&& is_nothrow_copy_assignable<T>;

constexpr bool is_constant_evaluated() {
    return __builtin_is_constant_evaluated();
}

template<typename T>
constexpr bool is_compile_time_known(T value) {
    return __builtin_constant_p(value);
}

template<typename T>
using remove_const = typename Detail::RemoveConst<T>::Type;

template<typename T>
using remove_volatile = typename Detail::RemoveVolatile<T>::Type;

template<typename T>
using remove_cv = typename Detail::RemoveCV<T>::Type;

template<typename T>
using add_const = typename Detail::AddConst<T>::Type;

template<typename T>
using add_volatile = typename Detail::AddVolatile<T>::Type;

template<typename T>
using add_cv = typename Detail::AddCV<T>::Type;

template<typename T>
using remove_reference = typename Detail::RemoveReference<T>::Type;

template<typename T>
using add_lvalue_reference = typename Detail::AddLvalueReference<T>::Type;

template<typename T>
using add_rvalue_reference = typename Detail::AddRvalueReference<T>::Type;

template<typename T>
using remove_pointer = typename Detail::RemovePointer<T>::Type;

template<typename T>
using add_pointer = typename Detail::AddPointer<T>::Type;

template<typename T>
using remove_cvref = typename Detail::RemoveCVRef<T>::Type;

template<typename T>
using lvalue_reference_of = typename Detail::LValueReferenceOf<T>::Type;

template<typename T>
using rvalue_reference_of = typename Detail::RValueReferenceOf<T>::Type;

template<typename T>
using const_lvalue_reference_of = typename Detail::ConstLValueReferenceOf<T>::Type;

template<typename T>
using const_rvalue_reference_of = typename Detail::ConstRValueReferenceOf<T>::Type;

template<typename T>
using make_unsigned = typename Detail::MakeUnsigned<T>::Type;

template<typename T>
using make_signed = typename Detail::MakeSigned<T>::Type;

template<typename T>
using remove_extent = typename Detail::RemoveExtent<T>::Type;

template<typename T>
using remove_all_extents = typename Detail::RemoveAllExtents<T>::Type;

template<bool condition, typename T = void>
using enable_if = typename Detail::EnableIf<condition, T>::Type;

template<bool condition, typename TypeIfTrue, typename TypeIfFalse>
using conditional = typename Detail::Conditional<condition, TypeIfTrue, TypeIfFalse>::Type;

template<typename...>
using void_t = void;

template<typename T>
using underlying_type = typename Detail::UnderlyingType<T>::Type;

template<typename... T>
using TypeList = Detail::TypeList<T...>;

template<usize size, typename List>
using TypeSelect = typename Detail::TypeSelect<size, List>::Type;

} /* namespace YT */

/*
xxxx is_scoped_enum xxxx
xxxx is_swappable_with xxxx
xxxx is_swappable xxxx
xxxx is_nothrow_swappable_with xxxx
xxxx is_nothrow_swappable xxxx
---- is_nothrow_convertible ----
xxxx is_invocable xxxx
xxxx is_invocable_r xxxx
xxxx is_nothrow_invocable xxxx
xxxx is_nothrow_invocable_r xxxx

xxxx is_pointer_interconvertible_base_of xxxx
xxxx is_pointer_interconvertible_with_class xxxx
xxxx is_corresponding_member xxxx

xxxx aligned_storage xxxx
xxxx aligned_union xxxx
xxxx decay xxxx
xxxx common_type xxxx
xxxx common_reference xxxx
xxxx basic_common_reference xxxx
xxxx result_of xxxx
xxxx invoke_result xxxx
xxxx type_identity xxxx
*/