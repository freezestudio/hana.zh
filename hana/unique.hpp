/*!
@file
Defines `boost::hana::unique`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_UNIQUE_HPP
#define BOOST_HANA_UNIQUE_HPP

#include <hana/fwd/unique.hpp>

#include <hana/concept/sequence.hpp>
#include <hana/config.hpp>
#include <hana/core/dispatch.hpp>
#include <hana/detail/nested_by.hpp> // required by fwd decl
#include <hana/equal.hpp>
#include <hana/front.hpp>
#include <hana/group.hpp>
#include <hana/transform.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs>
    constexpr auto unique_t::operator()(Xs&& xs) const {
        using S = typename hana::tag_of<Xs>::type;
        using Unique = BOOST_HANA_DISPATCH_IF(unique_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::unique(xs) requires 'xs' to be a Sequence");
    #endif

        return Unique::apply(static_cast<Xs&&>(xs));
    }

    template <typename Xs, typename Predicate>
    constexpr auto unique_t::operator()(Xs&& xs, Predicate&& predicate) const {
        using S = typename hana::tag_of<Xs>::type;
        using Unique = BOOST_HANA_DISPATCH_IF(unique_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::unique(xs, predicate) requires 'xs' to be a Sequence");
    #endif

        return Unique::apply(static_cast<Xs&&>(xs),
                             static_cast<Predicate&&>(predicate));
    }
    //! @endcond

    template <typename S, bool condition>
    struct unique_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return hana::transform(
                hana::group(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred)),
                hana::front
            );
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs)
        { return unique_impl::apply(static_cast<Xs&&>(xs), hana::equal); }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_UNIQUE_HPP
