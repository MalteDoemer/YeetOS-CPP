/*
 * Copyright 2022 Malte Dömer
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

#pragma once

#include <Types.hpp>

namespace yt {

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

template<bool b>
using BoolConstant = IntegralConstant<bool, b>;

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

template<typename T>
struct TypeIdentity {
    using Type = T;
};

/** primary type categories **/
#pragma region

template<typename T>
inline constexpr bool is_void = __is_void(T);

namespace Detail {

template<typename T>
struct IsNullptr : public FalseType {};

template<>
struct IsNullptr<decltype(nullptr)> : public TrueType {};

}

template<typename T>
inline constexpr bool is_null_pointer = Detail::IsNullptr<T>::value;

template<typename T>
inline constexpr bool is_integral = __is_integral(T);

template<typename T>
inline constexpr bool is_floating_point = __is_floating_point(T);

template<typename T>
inline constexpr bool is_array = __is_array(T);

template<typename T>
inline constexpr bool is_pointer = __is_pointer(T);

template<typename T>
inline constexpr bool is_lvalue_reference = __is_lvalue_reference(T);

template<typename T>
inline constexpr bool is_rvalue_reference = __is_rvalue_reference(T);

template<typename T>
inline constexpr bool is_member_object_pointer = __is_member_object_pointer(T);

template<typename T>
inline constexpr bool is_member_function_pointer = __is_member_function_pointer(T);

template<typename T>
inline constexpr bool is_enum = __is_enum(T);

template<typename T>
inline constexpr bool is_union = __is_union(T);

template<typename T>
inline constexpr bool is_class = __is_class(T);

template<typename T>
inline constexpr bool is_function = __is_function(T);

#pragma endregion

/** composite type categories **/
#pragma region

template<typename T>
inline constexpr bool is_reference = is_lvalue_reference<T> || is_rvalue_reference<T>;

template<typename T>
inline constexpr bool is_arithmetic = __is_arithmetic(T);

template<typename T>
inline constexpr bool is_fundamental = __is_fundamental(T);

template<typename T>
inline constexpr bool is_object = __is_object(T);

template<typename T>
inline constexpr bool is_scalar = __is_scalar(T);

template<typename T>
inline constexpr bool is_compound = __is_compound(T);

template<typename T>
inline constexpr bool is_member_pointer = __is_member_pointer(T);

#pragma endregion

/** miscellaneuous helpers **/
#pragma region

namespace Detail {

template<bool B, typename T = void>
struct EnableIf {};

template<typename T>
struct EnableIf<true, T> {
    using Type = T;
};

template<bool Cond, typename TypeIfTrue, typename TypeIfFalse>
struct Conditional;

template<typename TypeIfTrue, typename TypeIfFalse>
struct Conditional<true, TypeIfTrue, TypeIfFalse> {
    using Type = TypeIfTrue;
};

template<typename TypeIfTrue, typename TypeIfFalse>
struct Conditional<false, TypeIfTrue, TypeIfFalse> {
    using Type = TypeIfFalse;
};

}

template<bool B, typename T = void>
using enable_if = typename Detail::EnableIf<B, T>::Type;

template<bool Cond, typename TypeIfTrue, typename TypeIfFalse>
using conditional = typename Detail::Conditional<Cond, TypeIfTrue, TypeIfFalse>::Type;

namespace Detail {

template<typename...>
struct TypeList {};

template<typename T, typename... Ints>
struct TypeList<T, Ints...> {
    static constexpr usize size = sizeof(T);
};

template<usize S, typename TList, bool = (S <= TList::size)>
struct TypeSelect;

template<usize S, typename UInt, typename... UInts>
struct TypeSelect<S, TypeList<UInt, UInts...>, true> {
    using Type = UInt;
};

template<usize S, typename UInt, typename... UInts>
struct TypeSelect<S, TypeList<UInt, UInts...>, false> : TypeSelect<S, TypeList<UInts...>> {};

}

// using Detail::TypeList;
// using Detail::TypeSelect;

template<usize S, typename... Ints>
using type_select = typename Detail::TypeSelect<S, Detail::TypeList<Ints...>>::Type;


#pragma endregion

/** logical operations **/
#pragma region

namespace Detail {

template<bool...>
struct LogicalOr;

template<>
struct LogicalOr<> : public FalseType {};

template<bool B1>
struct LogicalOr<B1> : public BoolConstant<B1> {};

template<bool B1, bool B2>
struct LogicalOr<B1, B2> : public Conditional<B1, BoolConstant<B1>, BoolConstant<B2>>::Type {};

template<bool B1, bool B2, bool B3, bool... Bn>
struct LogicalOr<B1, B2, B3, Bn...> : public Conditional<B1, BoolConstant<B1>, LogicalOr<B2, B3, Bn...>>::Type {};

template<bool...>
struct LogicalAnd;

template<>
struct LogicalAnd<> : public TrueType {};

template<bool B1>
struct LogicalAnd<B1> : public BoolConstant<B1> {};

template<bool B1, bool B2>
struct LogicalAnd<B1, B2> : public Conditional<B1, BoolConstant<B2>, BoolConstant<B1>>::Type {};

template<bool B1, bool B2, bool B3, bool... Bn>
struct LogicalAnd<B1, B2, B3, Bn...> : public Conditional<B1, LogicalAnd<B2, B3, Bn...>, BoolConstant<B1>>::Type {};

template<bool B>
struct LogicalNot : public BoolConstant<!B> {};

}

using Detail::LogicalAnd;
using Detail::LogicalNot;
using Detail::LogicalOr;

#pragma endregion

/** reference modification **/
#pragma region
namespace Detail {

template<typename T>
auto try_add_lvalue_reference(int) -> TypeIdentity<T&>;
template<typename T>
auto try_add_lvalue_reference(...) -> TypeIdentity<T>;

template<typename T>
auto try_add_rvalue_reference(int) -> TypeIdentity<T&&>;
template<typename T>
auto try_add_rvalue_reference(...) -> TypeIdentity<T>;

template<typename T>
struct AddLvalueReference : decltype(try_add_lvalue_reference<T>(0)) {};

template<typename T>
struct AddRvalueReference : decltype(try_add_rvalue_reference<T>(0)) {};

template<typename T>
struct RemoveReference {
    using Type = T;
};
template<typename T>
struct RemoveReference<T&> {
    using Type = T;
};
template<typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

}

template<typename T>
using add_lvalue_reference = typename Detail::AddLvalueReference<T>::Type;

template<typename T>
using add_rvalue_reference = typename Detail::AddRvalueReference<T>::Type;

template<typename T>
using remove_reference = typename Detail::RemoveReference<T>::Type;

#pragma endregion

/** const-volatile modifications **/
#pragma region

template<typename T>
using add_const = const T;

template<typename T>
using add_volatile = volatile T;

template<typename T>
using add_cv = add_volatile<add_const<T>>;

namespace Detail {

template<typename T>
struct RemoveConst {
    using Type = T;
};

template<typename T>
struct RemoveConst<const T> {
    using Type = T;
};

}

template<typename T>
using remove_const = typename Detail::RemoveConst<T>::Type;

namespace Detail {

template<typename T>
struct RemoveVolatile {
    using Type = T;
};

template<typename T>
struct RemoveVolatile<volatile T> {
    using Type = T;
};

}

template<typename T>
using remove_volatile = typename Detail::RemoveVolatile<T>::Type;

template<typename T>
using remove_cv = remove_volatile<remove_const<T>>;

namespace Detail {

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
    using Type = typename CVSelector<Unqalified, __is_const(Qualified), __is_volatile(Qualified)>::Type;
};

}

template<typename Qualified, typename Unqualified>
using match_cv = typename Detail::MatchCV<Qualified, Unqualified>::Type;

#pragma endregion

/** array modification **/
#pragma region

namespace Detail {

template<typename T>
struct RemoveExtent {
    using Type = T;
};

template<typename T>
struct RemoveExtent<T[]> {
    using Type = T;
};

template<typename T, usize N>
struct RemoveExtent<T[N]> {
    using Type = T;
};

template<typename T>
struct RemoveAllExtents {
    using Type = T;
};

template<typename T>
struct RemoveAllExtents<T[]> {
    using Type = typename RemoveAllExtents<T>::Type;
};

template<typename T, usize N>
struct RemoveAllExtents<T[N]> {
    using Type = typename RemoveAllExtents<T>::Type;
};

}

template<typename T>
using remove_extent = typename Detail::RemoveExtent<T>::Type;

template<typename T>
using remove_all_extents = typename Detail::RemoveAllExtents<T>::Type;

#pragma endregion

/** sign modification **/
#pragma region

namespace Detail {

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

template<typename T, bool is_int = is_integral<T>, bool is_enum = is_enum<T>>
struct MakeUnsignedSelector;

template<typename T>
struct MakeUnsignedSelector<T, true, false> {
private:
    using UnsignedType = typename MakeUnsignedHelper<remove_cv<T>>::Type;

public:
    using Type = match_cv<T, UnsignedType>;
};

template<typename T>
struct MakeUnsignedSelector<T, false, true> {
private:
    using Uints = TypeList<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;
    using UnsignedType = typename TypeSelect<sizeof(T), Uints>::Type;

public:
    using Type = match_cv<T, UnsignedType>;
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

template<typename T>
struct MakeUnsigned {
    using Type = typename MakeUnsignedSelector<T>::Type;
};
}

template<typename T>
using make_unsigned = typename Detail::MakeUnsigned<T>::Type;

namespace Detail {
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

template<typename T, bool is_int = is_integral<T>, bool is_enum = is_enum<T>>
struct MakeSignedSelector;

template<typename T>
struct MakeSignedSelector<T, true, false> {
private:
    using SignedType = typename MakeSignedHelper<remove_cv<T>>::Type;

public:
    using Type = match_cv<T, SignedType>;
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

template<typename T>
struct MakeSigned : public MakeSignedSelector<T> {};

}

template<typename T>
using make_signed = typename Detail::MakeSigned<T>::Type;

#pragma endregion

/** pointer modifications **/
#pragma region

namespace Detail {

template<typename T>
struct RemovePointer {
    using Type = T;
};

template<typename T>
struct RemovePointer<T*> {
    using Type = T;
};

}

template<typename T>
using remove_pointer = typename Detail::RemovePointer<remove_cv<T>>::Type;

namespace Detail {

template<typename T>
auto try_add_pointer(int) -> TypeIdentity<remove_reference<T>*>;
template<typename T>
auto try_add_pointer(...) -> TypeIdentity<T>;

}

template<typename T>
using add_pointer = typename decltype(Detail::try_add_pointer<T>(0))::Type;

#pragma endregion

/** utility functions **/
#pragma region

template<typename T>
add_rvalue_reference<T> declval() noexcept;

template<typename T>
constexpr remove_reference<T>&& move(T&& t) noexcept {
    return static_cast<remove_reference<T>&&>(t);
}

template<typename T>
constexpr T&& forward(remove_reference<T>& t) noexcept {
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(remove_reference<T>&& t) noexcept {
    static_assert(!is_lvalue_reference<T>, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

#pragma endregion

/** type properties **/
#pragma region

template<typename T>
inline constexpr bool is_const = __is_const(T);

template<typename T>
inline constexpr bool is_volatile = __is_volatile(T);

template<typename T>
inline constexpr bool is_trivial = __is_trivial(T);

template<typename T>
inline constexpr bool is_trivially_copyable = __is_trivially_copyable(T);

template<typename T>
inline constexpr bool is_standard_layout = __is_standard_layout(T);

template<typename T>
inline constexpr bool is_empty = __is_empty(T);

template<typename T>
inline constexpr bool is_polymorphic = __is_polymorphic(T);

template<typename T>
inline constexpr bool is_abstract = __is_abstract(T);

template<typename T>
inline constexpr bool is_final = __is_aggregate(T);

template<typename T>
inline constexpr bool is_signed = __is_signed(T);

template<typename T>
inline constexpr bool is_unsigned = __is_unsigned(T);

namespace Detail {

template<typename>
struct IsBoundedArray : public FalseType {};

template<typename T, usize N>
struct IsBoundedArray<T[N]> : public TrueType {};

template<typename>
struct IsUnboundedArray : public FalseType {};

template<typename T>
struct IsUnboundedArray<T[]> : public TrueType {};

}

template<typename T>
inline constexpr bool is_bounded_array = Detail::IsBoundedArray<T>::value;

template<typename T>
inline constexpr bool is_unbounded_array = Detail::IsUnboundedArray<T>::value;

// TODO: is_scoped_enum

template<typename T, typename... Args>
inline constexpr bool is_constructible = __is_constructible(T, Args...);

template<typename T>
inline constexpr bool is_default_constructible = is_constructible<T>;

template<typename T>
inline constexpr bool is_copy_constructible = is_constructible<T, const T&>;

template<typename T>
inline constexpr bool is_move_constructible = is_constructible<T, T&&>;

template<typename T, typename... Args>
inline constexpr bool is_trivially_constructible = __is_trivially_constructible(T, Args...);

template<typename T>
inline constexpr bool is_trivially_default_constructible = is_trivially_constructible<T>;

template<typename T>
inline constexpr bool is_trivially_copy_constructible = is_trivially_constructible<T, const T&>;

template<typename T>
inline constexpr bool is_trivially_move_constructible = is_trivially_constructible<T, T&&>;

template<typename T, typename... Args>
inline constexpr bool is_nothrow_constructible = __is_nothrow_constructible(T, Args...);

template<typename T>
inline constexpr bool is_nothrow_default_constructible = is_nothrow_constructible<T>;

template<typename T>
inline constexpr bool is_nothrow_copy_constructible = is_nothrow_constructible<T, const T&>;

template<typename T>
inline constexpr bool is_nothrow_move_constructible = is_nothrow_constructible<T, T&&>;

template<typename U, typename V>
inline constexpr bool is_assignable = __is_assignable(U, V);

template<typename T>
inline constexpr bool is_copy_assignable = is_assignable<T&, const T&>;

template<typename T>
inline constexpr bool is_move_assignable = is_assignable<T&, T&&>;

template<typename U, typename V>
inline constexpr bool is_trivially_assignable = __is_trivially_assignable(U, V);

template<typename T>
inline constexpr bool is_trivially_copy_assignable = is_trivially_assignable<T&, const T&>;

template<typename T>
inline constexpr bool is_trivially_move_assignable = is_trivially_assignable<T&, T&&>;

template<typename U, typename V>
inline constexpr bool is_nothrow_assignable = __is_nothrow_assignable(U, V);

template<typename T>
inline constexpr bool is_nothrow_copy_assignable = is_nothrow_assignable<T&, const T&>;

template<typename T>
inline constexpr bool is_nothrow_move_assignable = is_nothrow_assignable<T&, T&&>;

template<typename T>
inline constexpr bool is_nothrow_copyable = is_nothrow_copy_constructible<T>&& is_nothrow_copy_assignable<T>;

template<typename T>
inline constexpr bool is_nothrow_movable = is_nothrow_move_constructible<T>&& is_nothrow_move_assignable<T>;

// TODO: is_swappable_with
// TODO: is_swappable
// TODO: is_nothrow_swappable_with
// TODO: is_nothrow_swappable

template<typename T>
inline constexpr bool has_virtual_destructor = __has_virtual_destructor(T);

template<typename T>
inline constexpr bool has_unique_object_representations = __has_unique_object_representations(T);

namespace Detail {

template<typename T>
auto destruct_test(T&&) -> decltype(declval<T&>().~T(), TrueType());

auto destruct_test(...) -> FalseType;

template<typename T, bool = LogicalOr<is_void<T>, is_function<T>, is_unbounded_array<T>>::value, bool = is_reference<T>>
struct IsDestructible;

template<typename T>
struct IsDestructible<T, false, false> : public decltype(destruct_test(declval<remove_all_extents<T>>())) {};

template<typename T>
struct IsDestructible<T, true, false> : public FalseType {};

template<typename T>
struct IsDestructible<T, false, true> : public TrueType {};
}

template<typename T>
inline constexpr bool is_destructible = Detail::IsDestructible<T>::value;

namespace Detail {
template<typename T>
auto nt_destruct_test(T&&) -> BoolConstant<noexcept(declval<T&>().~T())>;

auto nt_destruct_test(...) -> FalseType;

template<typename T, bool = LogicalOr<is_void<T>, is_function<T>, is_unbounded_array<T>>::value, bool = is_reference<T>>
struct IsNothrowDestructible;

template<typename T>
struct IsNothrowDestructible<T, false, false> : public decltype(nt_destruct_test(declval<remove_all_extents<T>>())) {};

template<typename T>
struct IsNothrowDestructible<T, true, false> : public FalseType {};

template<typename T>
struct IsNothrowDestructible<T, false, true> : public TrueType {};

}

template<typename T>
inline constexpr bool is_nothrow_destructible = Detail::IsNothrowDestructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_destructible = __is_trivially_destructible(T);

#pragma endregion

/** type relations **/
#pragma region

template<typename U, typename V>
inline constexpr bool is_same = __is_same(U, V);

template<typename Base, typename Derived>
inline constexpr bool is_base_of = __is_base_of(Base, Derived);

template<typename From, typename To>
inline constexpr bool is_convertible = __is_convertible(From, To);

namespace Detail {

template<typename From, typename To>
struct IsNothrowConvertible : public LogicalAnd<is_same<From, void>, is_same<To, void>> {};

template<typename From, typename To>
requires requires {
    static_cast<To (*)()>(nullptr);
    { declval<void (&)(To) noexcept>()(declval<From>()) }
    noexcept;
}
struct IsNothrowConvertible<From, To> : public TrueType {};

}

template<typename From, typename To>
inline constexpr bool is_nothrow_convertible = Detail::IsNothrowConvertible<From, To>::value;

// TODO: is_layout_compatible
// TODO: is_pointer_interconvertible_base_of
// TODO: is_invocable
// TODO: is_invocable_r
// TODO: is_nothrow_invocable
// TODO: is_nothrow_invocable_r

#pragma endregion

/** other transformations **/
#pragma region

template<typename T>
using remove_cvref = remove_cv<remove_reference<T>>;

template<typename T>
using underlying_type = __underlying_type(T);

namespace Detail {

template<typename T>
struct Decay {
private:
    using U = remove_reference<T>;

public:
    using Type = conditional<is_array<U>, remove_extent<U>*, conditional<is_function<U>, add_pointer<U>, remove_cv<U>>>;
};

}

template<typename T>
using decay = typename Detail::Decay<T>::Type;

#pragma endregion

constexpr bool is_constant_evaluated() noexcept {
    return __builtin_is_constant_evaluated();
}

template<typename...>
constexpr bool dependant_false = false;

} /* namespace yt */