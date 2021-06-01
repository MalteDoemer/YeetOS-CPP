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
#include <Traits.hpp>
#include <Concepts.hpp>
#include <TypeMagic.hpp>

/* clang-format off */

namespace Yt {

/**
 * Retrieves a pointer to the underlying data of T.
 * @returns t.data()
 */
template<typename T>
requires requires (T& t) { t.data(); }
constexpr auto data(T& t) noexcept(noexcept(t.data()))
{
    return t.data();
}

/**
 * Retrieves a pointer to the underlying data of T.
 * @returns t.data()
 */
template<typename T>
requires requires (const T& t) { t.data(); }
constexpr auto data(const T& t) noexcept(noexcept(t.data()))
{
    return t.data();
}

/**
 * The data() overload for pointers.
 * @returns ptr
 */
template<typename T>
requires is_pointer<T>
constexpr auto data(T ptr) noexcept
{
    return ptr;
}

/**
 * The data() overload for arrays.
 * @returns arr
 */
template<typename T>
requires is_array<T>
constexpr auto data(T arr) noexcept
{
    return arr;
}

/**
 * Retrieves an iterator to the beginning of a range.
 */
template<typename Rng>
requires requires (Rng& rng) { rng.begin(); }
constexpr auto begin(Rng& rng) noexcept(noexcept(rng.begin()))
{
    return rng.begin();
}

/**
 * Retrieves an iterator to the beginning of a range.
 */
template<typename Rng>
requires requires (const Rng& rng) { rng.begin(); }
constexpr auto begin(const Rng& rng) noexcept(noexcept(rng.begin()))
{
    return rng.begin();
}

/**
 * Retrieves an iterator to the end of a range.
 */
template<typename Rng>
requires requires (Rng& rng) { rng.end(); }
constexpr auto end(Rng& rng) noexcept(noexcept(rng.end()))
{
    return rng.end();
}

/**
 * Retrieves an iterator to the end of a range.
 */
template<typename Rng>
requires requires (const Rng& rng) { rng.end(); }
constexpr auto end(const Rng& rng) noexcept(noexcept(rng.end()))
{
    return rng.end();
}

} /* namespace Yt */

namespace Yt {

using Yt::data;
using Yt::begin;
using Yt::end;

/**
 * Defines whether Iter is readable through operator*.
 */
template<typename Iter>
concept Readable = requires (Iter iter){
    requires HasUnderlyingTypeTraits<Iter>;
    { *iter } -> ConvertibleTo<ValueType<Iter>>;
};

/**
 * Defines whether Iter is writable through operator*.
 */
template<typename Iter>
concept Writeable = requires (Iter iter, ConstValueReference<Iter> val)
{
    requires HasUnderlyingTypeTraits<Iter>;
    { *iter = val };
};

/**
 * Defines whether Iter can be pre- and post-incremented.
 */
template<typename Iter>
concept Incrementable = requires (Iter iter)
{
    { ++iter } -> SameAs<Iter&>;
    { iter++ } -> SameAs<Iter>;
};

/**
 * Defines whether Iter can be pre- and post-dectremented.
 */
template<typename Iter>
concept Decrementable = requires (Iter iter)
{
    { --iter } -> SameAs<Iter&>;
    { iter-- } -> SameAs<Iter>;
};

/**
 * Defines whether T and U are Compareable and whether operator- can be used to compute their difference.
 */
template<typename T, typename U>
concept PositionAwareWith = requires (T i, U j)
{
    requires CompareableWith<T, U>;
    { i - j } -> SameAs<Diff>;
};

/**
 * Defines whether two T's are Compareable and whether operator- can be used to compute their difference.
 */
template<typename T>
concept PositionAware = PositionAwareWith<T, T>;

/**
 * Defines whether T has operators +=, -=, +, - and [].
 */
template<typename T>
concept RandomAccessable = requires (T i, const T j, Diff n)
{
    requires Readable<T>;
    { i += n } -> SameAs<T&>;
    { i -= n } -> SameAs<T&>;
    { j + n } -> SameAs<T>;
    { n + j } -> SameAs<T>;
    { j - n } -> SameAs<T>;
    { i[n] } -> SameAs<decltype(*i)>;
};

/**
 * Defines wether T is contiguous.
 * @returns true if a call to data() returns a pointer to the underlying data of T
 */
template<typename T>
concept HasContiguousData = requires (T t)
{
    requires HasUnderlyingTypeTraits<T>;
    { data(t) } -> SameAs<ValuePointer<T>>;
};

/**
 * A basic iterator.
 */
template<typename T>
concept Iterator = requires {
    requires Copyable<T>;
    requires Readable<T>;
    requires Incrementable<T>;
};

/**
 * A Iterator which is Writable.
 */
template<typename T>
concept WriteableIterator = Iterator<T> && Writeable<T>;

/**
 * An Iterator which is Decrementable and PositionAware.
 */
template<typename T>
concept BidirectionalIterator = requires
{
    requires Iterator<T>;
    requires Decrementable<T>;
    requires PositionAware<T>;
};

/**
 * A BidirectionalIterator which is Writable.
 */
template<typename T>
concept WriteableBidirectionalIterator = BidirectionalIterator<T> && Writeable<T>;

/**
 * A BidirectionalIterator which is RandomAccessable.
 */
template<typename T>
concept RandomAccessIterator = requires {
    requires Copyable<T>;
    requires RandomAccessable<T>;
    requires BidirectionalIterator<T>;
};

/**
 * A RandomAccessIterator which is Writable.
 */
template<typename T>
concept WriteableRandomAccessIterator = RandomAccessIterator<T> && Writeable<T>;

/**
 * A RandomAccessIterator which satisfies HasContiguousData.
 */
template<typename T>
concept ContiguousIterator = requires {
    requires HasContiguousData<T>;
    requires RandomAccessIterator<T>;
};

/**
 * A ContiguousIterator which is Writable.
 */
template<typename T>
concept WriteableContiguousIterator = ContiguousIterator<T> && Writeable<T>;


/**
 * Defines whether S is a sentinel for the iterator It.
 */
template<typename S, typename It>
concept SentinelFor = Iterator<It> && Copyable<S> && EqualityCompareableWith<It, S>;

/**
 * Defines whether S is a PostitonAware sentinel for the iterator It.
 */
template<typename S, typename It>
concept SizedSentinelFor = Iterator<It> && Copyable<S> && PositionAwareWith<S, It>;

/**
 * Type alias for the associated iterator of a range.
 */
template<typename Rng>
using IteratorOf = decltype(begin(declval<Rng&>()));

/**
 * Type alias for the associated sentinel of a range.
 */
template<typename Rng>
using SentinelOf = decltype(end(declval<Rng&>()));

template<typename T>
concept Range = requires (T rng)
{
    { begin(rng) } -> Iterator;
    { end(rng) } -> SentinelFor<IteratorOf<T>>;
};

template<typename T>
concept WriteableRange = requires (T rng)
{
    { begin(rng) } -> WriteableIterator;
    { end(rng) } -> SentinelFor<IteratorOf<T>>;
};

template<typename T>
concept BidirectionalRange = requires (T rng)
{
    { begin(rng) } -> BidirectionalIterator;
    { end(rng) } -> SizedSentinelFor<IteratorOf<T>>;
};

template<typename T>
concept WriteableBidirectionalRange = requires (T rng)
{
    { begin(rng) } -> WriteableBidirectionalIterator;
    { end(rng) } -> SizedSentinelFor<IteratorOf<T>>;
};

template<typename T>
concept RandomAccessRange = requires (T rng)
{
    { begin(rng) } -> RandomAccessIterator;
    { end(rng) } -> SizedSentinelFor<IteratorOf<T>>;
};

template<typename T>
concept WriteableRandomAccessRange = requires (T rng)
{
    { begin(rng) } -> WriteableRandomAccessIterator;
    { end(rng) } -> SizedSentinelFor<IteratorOf<T>>;
};

template<typename T>
concept ContiguousRange = requires (T rng)
{
    { begin(rng) } -> ContiguousIterator;
    { end(rng) } -> SizedSentinelFor<IteratorOf<T>>;
};

template<typename T>
concept WriteableContiguousRange = requires (T rng)
{
    { begin(rng) } -> WriteableContiguousIterator;
    { end(rng) } -> SizedSentinelFor<IteratorOf<T>>;
};

} /* namespace Yt */

/* clang-format on */