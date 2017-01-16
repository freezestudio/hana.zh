/*!
@file
Defines `boost::hana::set`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_HPP
#define BOOST_HANA_SET_HPP

#include <hana/fwd/set.hpp>

#include <hana/at.hpp>
#include <hana/bool.hpp>
#include <hana/concept/comparable.hpp>
#include <hana/concept/constant.hpp>
#include <hana/concept/hashable.hpp>
#include <hana/config.hpp>
#include <hana/contains.hpp>
#include <hana/core/make.hpp>
#include <hana/core/to.hpp>
#include <hana/detail/decay.hpp>
#include <hana/detail/fast_and.hpp>
#include <hana/detail/has_duplicates.hpp>
#include <hana/detail/operators/adl.hpp>
#include <hana/detail/operators/comparable.hpp>
#include <hana/detail/operators/searchable.hpp>
#include <hana/equal.hpp>
#include <hana/erase_key.hpp>
#include <hana/find_if.hpp>
#include <hana/fold_left.hpp>
#include <hana/fwd/any_of.hpp>
#include <hana/fwd/core/to.hpp>
#include <hana/fwd/difference.hpp>
#include <hana/fwd/intersection.hpp>
#include <hana/fwd/union.hpp>
#include <hana/insert.hpp>
#include <hana/is_subset.hpp>
#include <hana/length.hpp>
#include <hana/or.hpp>
#include <hana/remove.hpp>
#include <hana/tuple.hpp>
#include <hana/unpack.hpp>
#include <hana/value.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN
    //////////////////////////////////////////////////////////////////////////
    // set
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename ...Xs>
    struct set
        : detail::operators::adl<set<Xs...>>
        , detail::searchable_operators<set<Xs...>>
    {
        tuple<Xs...> storage;
        using hana_tag = set_tag;
        static constexpr std::size_t size = sizeof...(Xs);

        explicit constexpr set(tuple<Xs...> const& xs)
            : storage(xs)
        { }

        explicit constexpr set(tuple<Xs...>&& xs)
            : storage(static_cast<tuple<Xs...>&&>(xs))
        { }

        constexpr set(set const& other) = default;
        constexpr set(set&& other) = default;
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<set_tag> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // make<set_tag>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<set_tag> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
#if defined(BOOST_HANA_CONFIG_ENABLE_DEBUG_MODE)
            static_assert(detail::fast_and<hana::Comparable<Xs>::value...>::value,
            "hana::make_set(xs...) requires all the 'xs' to be Comparable");

            static_assert(detail::fast_and<hana::Hashable<Xs>::value...>::value,
            "hana::make_set(xs...) requires all the 'xs' to be Hashable");

            static_assert(detail::fast_and<
                Constant<decltype(hana::equal(xs, xs))>::value...
            >::value,
            "hana::make_set(xs...) requires all the 'xs' to be "
            "Comparable at compile-time");

            static_assert(!detail::has_duplicates<Xs&&...>::value,
            "hana::make_set(xs...) requires all the 'xs' to be unique");
#endif

            return set<typename detail::decay<Xs>::type...>{
                hana::make_tuple(static_cast<Xs&&>(xs)...)
            };
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<set_tag, set_tag> {
        template <typename S1, typename S2>
        static constexpr auto equal_helper(S1 const& s1, S2 const& s2, hana::true_)
        { return hana::is_subset(s1, s2); }

        template <typename S1, typename S2>
        static constexpr auto equal_helper(S1 const&, S2 const&, hana::false_)
        { return hana::false_c; }

        template <typename S1, typename S2>
        static constexpr decltype(auto) apply(S1&& s1, S2&& s2) {
            return equal_impl::equal_helper(s1, s2, hana::bool_c<
                decltype(hana::length(s1.storage))::value ==
                decltype(hana::length(s2.storage))::value
            >);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<set_tag> {
        template <typename Set, typename F>
        static constexpr decltype(auto) apply(Set&& set, F&& f) {
            return hana::unpack(static_cast<Set&&>(set).storage,
                                static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<set_tag> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return hana::find_if(static_cast<Xs&&>(xs).storage, static_cast<Pred&&>(pred));
        }
    };

    template <>
    struct any_of_impl<set_tag> {
        template <typename Pred>
        struct any_of_helper {
            Pred const& pred;
            template <typename ...X>
            constexpr auto operator()(X const& ...x) const {
                return hana::or_(pred(x)...);
            }
            constexpr auto operator()() const {
                return hana::false_c;
            }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs const& xs, Pred const& pred) {
            return hana::unpack(xs.storage, any_of_helper<Pred>{pred});
        }
    };

    template <>
    struct is_subset_impl<set_tag, set_tag> {
        template <typename Ys>
        struct all_contained {
            Ys const& ys;
            template <typename ...X>
            constexpr auto operator()(X const& ...x) const {
                return hana::bool_c<detail::fast_and<
                    hana::value<decltype(hana::contains(ys, x))>()...
                >::value>;
            }
        };

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs const& xs, Ys const& ys) {
            return hana::unpack(xs, all_contained<Ys>{ys});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct to_impl<set_tag, F, when<hana::Foldable<F>::value>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::fold_left(static_cast<Xs&&>(xs),
                                   hana::make_set(),
                                   hana::insert);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<set_tag> {
        template <typename Xs, typename X, typename Indices>
        static constexpr auto
        insert_helper(Xs&& xs, X&&, hana::true_, Indices) {
            return static_cast<Xs&&>(xs);
        }

        template <typename Xs, typename X, std::size_t ...n>
        static constexpr auto
        insert_helper(Xs&& xs, X&& x, hana::false_, std::index_sequence<n...>) {
            return hana::make_set(
                hana::at_c<n>(static_cast<Xs&&>(xs).storage)..., static_cast<X&&>(x)
            );
        }

        template <typename Xs, typename X>
        static constexpr auto apply(Xs&& xs, X&& x) {
            constexpr bool c = hana::value<decltype(hana::contains(xs, x))>();
            constexpr std::size_t size = std::remove_reference<Xs>::type::size;
            return insert_helper(static_cast<Xs&&>(xs), static_cast<X&&>(x),
                                 hana::bool_c<c>, std::make_index_sequence<size>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<set_tag> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return hana::unpack(
                hana::remove(static_cast<Xs&&>(xs).storage,
                             static_cast<X&&>(x)),
                hana::make_set
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // intersection
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename Ys>
        struct set_insert_if_contains {
            Ys const& ys;

            template <typename Result, typename Key>
            static constexpr auto helper(Result&& result, Key&& key, hana::true_) {
                return hana::insert(static_cast<Result&&>(result), static_cast<Key&&>(key));
            }

            template <typename Result, typename Key>
            static constexpr auto helper(Result&& result, Key&&, hana::false_) {
                return static_cast<Result&&>(result);
            }

            template <typename Result, typename Key>
            constexpr auto operator()(Result&& result, Key&& key) const {
                constexpr bool keep = hana::value<decltype(hana::contains(ys, key))>();
                return set_insert_if_contains::helper(static_cast<Result&&>(result),
                                                      static_cast<Key&&>(key),
                                                      hana::bool_c<keep>);
            }
        };
    }

    template <>
    struct intersection_impl<set_tag> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys const& ys) {
            return hana::fold_left(static_cast<Xs&&>(xs), hana::make_set(),
                                   detail::set_insert_if_contains<Ys>{ys});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // union_
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct union_impl<set_tag> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys&& ys) {
            return hana::fold_left(static_cast<Xs&&>(xs), static_cast<Ys&&>(ys),
                                   hana::insert);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // difference
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct difference_impl<set_tag> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys&& ys) {
            return hana::fold_left(static_cast<Ys&&>(ys), static_cast<Xs&&>(xs),
                                   hana::erase_key);
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_SET_HPP
