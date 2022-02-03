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

#include <Traits.hpp>
#include <Concepts.hpp>
#include <TypeMagic.hpp>

namespace yt {

namespace Detail {

template<typename T>
concept DotDataCallable = requires(T& t) {
    t.data();
};

template<typename T>
concept DotBeginCallable = requires(T& t) {
    t.begin();
};

template<typename T>
concept DotEndCallable = requires(T& t) {
    t.end();
};

template<typename T>
concept DotSizeCallable = requires(const T& t) {
    { t.size() } -> SameAs<usize>;
};

}

namespace rng {

/**
 * Returns a pointer to the underlying data of a range.
 */
template<typename Rng>
requires Detail::DotDataCallable<Rng>
constexpr auto data(Rng& t) noexcept(noexcept(t.data())) {
    return t.data();
}

/**
 * Returns a pointer to the underlying data of a range.
 */
template<typename Arr, usize N>
constexpr auto data(Arr (&arr)[N]) noexcept {
    return arr;
}

/**
 * Returns an iterator to the beginning of a range.
 */
template<typename Rng>
requires Detail::DotBeginCallable<Rng>
constexpr auto begin(Rng& rng) noexcept(noexcept(rng.begin())) {
    return rng.begin();
}

/**
 * Returns an iterator to the beginning of a range.
 */
template<typename Arr, usize N>
constexpr auto begin(Arr (&arr)[N]) noexcept {
    return arr;
}

/**
 * Returns an iterator to the end of a range
 */
template<typename Rng>
requires Detail::DotEndCallable<Rng>
constexpr auto end(Rng& rng) noexcept(noexcept(rng.end())) {
    return rng.end();
}

/**
 * Returns an iterator to the end of a range
 */
template<typename Arr, usize N>
constexpr auto end(Arr (&arr)[N]) noexcept {
    return arr + N;
}

template<typename Rng>
requires Detail::DotSizeCallable<Rng>
constexpr auto size(const Rng& rng) noexcept(noexcept(rng.size())) {
    return rng.size();
}

template<typename Arr, usize N>
constexpr auto size(Arr (&)[N]) noexcept {
    return N;
}

} /* namespace rng */

/**
 *  Specifies that an iterator type is readable.
 */
template<typename Iter>
concept Readable = HasUnderlyingTypeTraits<Iter> && requires(Iter iter) {
    { *iter } -> ConvertibleTo<ValueTypeOf<Iter>>;
};

/**
 *  Specifies that an iterator type is writable.
 */
template<typename Iter>
concept Writable = HasUnderlyingTypeTraits<Iter> && requires(Iter iter, ValueReferenceOf<Iter> val) {
    { *iter = val } -> SameAs<ValueReferenceOf<Iter>>;
};

/**
 * Specifies that an iterator type is pre/post incrementable.
 */
template<typename Iter>
concept Incrementable = requires(Iter iter) {
    { ++iter } -> SameAs<Iter&>;
    { iter++ } -> SameAs<Iter>;
};

/**
 * Specifies that an iterator type is pre/post decrementable.
 */
template<typename Iter>
concept Decrementable = requires(Iter iter) {
    { --iter } -> SameAs<Iter&>;
    { iter-- } -> SameAs<Iter>;
};

/**
 * Specifies that a distance between an iterator and its sentinel can be obtain.
 */
template<typename Iter, typename Sent>
concept PositionAwareWith = requires(Iter i, Sent s) {
    requires CompareableWith<Iter, Sent>;
    { i - s } -> SameAs<isize>;
};

/**
 * Specifies that an iterator is position aware with itself.
 */
template<typename Iter>
concept PositionAware = PositionAwareWith<Iter, Iter>;

/**
 * Specifies that an iterator can be access anywhere in constant time.
 */
template<typename Iter>
concept RandomAccessable = Readable<Iter> && requires(Iter i, const Iter j, isize n) {
    { i += n } -> SameAs<Iter&>;
    { i -= n } -> SameAs<Iter&>;
    { j + n } -> SameAs<Iter>;
    { n + j } -> SameAs<Iter>;
    { j - n } -> SameAs<Iter>;
    { i[n] } -> SameAs<decltype(*i)>;
};

/**
 * Type alias for the associated iterator of a range
 */
template<typename Rng>
using IteratorOf = decltype(rng::begin(declval<Rng&>()));

/**
 * Type alias for the associated sentinel of a range
 */
template<typename Rng>
using SentinelOf = decltype(rng::end(declval<Rng&>()));

/**
 * A basic iterator.
 */
template<typename Iter>
concept Iterator = Copyable<Iter> && Readable<Iter> && Incrementable<Iter>;

/**
 * A writable basic iterator.
 */
template<typename Iter>
concept WritableIterator = Iterator<Iter> && Writable<Iter>;

/**
 * Specifies that a type is a sentinel for an iterator.
 */
template<typename S, typename It>
concept SentinelFor = Iterator<It> && Copyable<S> && EqualityCompareableWith<It, S>;

/**
 * Specifies that a type is a sentinel for an iterator that can be used to obtain the position of the iterator.
 */
template<typename S, typename It>
concept PositionalSentinelFor = Iterator<It> && Copyable<S> && PositionAwareWith<It, S>;

/**
 * A bidirectional iterator.
 */
template<typename Iter>
concept BidirectionalIterator = Iterator<Iter> && Decrementable<Iter>;

/**
 * A writable bidirectional iterator.
 */
template<typename Iter>
concept WritableBidirectionalIterator = BidirectionalIterator<Iter> && Writable<Iter>;

/**
 * A random accessible iterator.
 */
template<typename Iter>
concept RandomAccessIterator = BidirectionalIterator<Iter> && RandomAccessable<Iter>;

/**
 * A writable random accessible iterator.
 */
template<typename Iter>
concept WritableRandomAccessIterator = RandomAccessIterator<Iter> && Writable<Iter>;

/**
 * A basic range.
 */
template<typename Rng>
concept Range = requires(Rng& range) {
    { rng::begin(range) } -> Iterator;
    { rng::end(range) } -> SentinelFor<IteratorOf<Rng>>;
};

/**
 * A writable basic range.
 */
template<typename Rng>
concept WritableRange = requires(Rng& range) {
    { rng::begin(range) } -> WritableIterator;
    { rng::end(range) } -> SentinelFor<IteratorOf<Rng>>;
};

/**
 * A range with bidirectional iterators.
 */
template<typename Rng>
concept BidirectionalRange = requires(Rng& range) {
    { rng::begin(range) } -> BidirectionalIterator;
    { rng::end(range) } -> PositionalSentinelFor<IteratorOf<Rng>>;
};

/**
 * A writable bidirectional range.
 */
template<typename Rng>
concept WritableBidirectionalRange = requires(Rng& range) {
    { rng::begin(range) } -> WritableBidirectionalIterator;
    { rng::end(range) } -> PositionalSentinelFor<IteratorOf<Rng>>;
};

/**
 * A range with random access iterators.
 */
template<typename Rng>
concept RandomAccessRange = requires(Rng& range) {
    { rng::begin(range) } -> RandomAccessIterator;
    { rng::end(range) } -> PositionalSentinelFor<IteratorOf<Rng>>;
};

/**
 * A writable random access range.
 */
template<typename Rng>
concept WritableRandomAccessRange = requires(Rng& range) {
    { rng::begin(range) } -> WritableRandomAccessIterator;
    { rng::end(range) } -> PositionalSentinelFor<IteratorOf<Rng>>;
};

/**
 * Specifies that a range has contiguous data.
 *
 * Note: this is intended to be used as an additional constraint on a range e.g: requires WritableRange<T> &&
 * Contiguous<T>
 */
template<typename Rng>
concept Contiguous = HasUnderlyingTypeTraits<Rng> && requires(remove_cvref<Rng>& range) {
    { rng::data(range) } -> SameAs<ValuePointerOf<Rng>>;
};

/**
 * Specifies that a range knows its size in constant time.
 *
 * Note: this is intended to be used as an additional constraint on a range e.g: requires WritableRange<T> && Sized<T>
 */
template<typename Rng>
concept Sized = requires(Rng& range) {
    { rng::size(range) } -> SameAs<usize>;
};

} /* namespace yt */

namespace rng = yt::rng;
using yt::BidirectionalIterator;
using yt::BidirectionalRange;
using yt::Contiguous;
using yt::Decrementable;
using yt::Incrementable;
using yt::Iterator;
using yt::IteratorOf;
using yt::PositionalSentinelFor;
using yt::PositionAware;
using yt::PositionAwareWith;
using yt::RandomAccessable;
using yt::RandomAccessIterator;
using yt::RandomAccessRange;
using yt::Range;
using yt::Readable;
using yt::SentinelFor;
using yt::SentinelOf;
using yt::Sized;
using yt::Writable;
using yt::WritableBidirectionalIterator;
using yt::WritableBidirectionalRange;
using yt::WritableIterator;
using yt::WritableRandomAccessIterator;
using yt::WritableRandomAccessRange;
using yt::WritableRange;