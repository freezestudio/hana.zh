/*!
@file
Defines `boost::hana::replace`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_REPLACE_HPP
#define BOOST_HANA_REPLACE_HPP

#include <hana/fwd/replace.hpp>

#include <hana/concept/functor.hpp>
#include <hana/config.hpp>
#include <hana/core/dispatch.hpp>
#include <hana/equal.hpp>
#include <hana/replace_if.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename OldVal, typename NewVal>
    constexpr auto replace_t::operator()(Xs&& xs, OldVal&& oldval, NewVal&& newval) const {
        using S = typename hana::tag_of<Xs>::type;
        using Replace = BOOST_HANA_DISPATCH_IF(replace_impl<S>,
            hana::Functor<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Functor<S>::value,
        "hana::replace(xs, oldval, newval) requires 'xs' to be a Functor");
    #endif

        return Replace::apply(static_cast<Xs&&>(xs),
                              static_cast<OldVal&&>(oldval),
                              static_cast<NewVal&&>(newval));
    }
    //! @endcond

    template <typename Fun, bool condition>
    struct replace_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename OldVal, typename NewVal>
        static constexpr decltype(auto)
        apply(Xs&& xs, OldVal&& oldval, NewVal&& newval) {
            return hana::replace_if(
                static_cast<Xs&&>(xs),
                hana::equal.to(static_cast<OldVal&&>(oldval)),
                static_cast<NewVal&&>(newval)
            );
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_REPLACE_HPP
