/*!
@file
Defines `boost::hana::monadic_compose`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONADIC_COMPOSE_HPP
#define BOOST_HANA_MONADIC_COMPOSE_HPP

#include <hana/fwd/monadic_compose.hpp>

#include <hana/chain.hpp>
#include <hana/concept/monad.hpp>
#include <hana/config.hpp>
#include <hana/core/dispatch.hpp>
#include <hana/functional/partial.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    namespace detail {
        struct monadic_compose_helper {
            template <typename F, typename G, typename X>
            constexpr decltype(auto) operator()(F&& f, G&& g, X&& x) const {
                using M = typename hana::tag_of<decltype(g(x))>::type;

            #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
                static_assert(hana::Monad<M>::value,
                "hana::monadic_compose(f, g) requires 'g' to return a monadic value");
            #endif

                return hana::chain(static_cast<G&&>(g)(static_cast<X&&>(x)),
                                   static_cast<F&&>(f));
            }
        };
    }

    //! @cond
    template <typename F, typename G>
    constexpr auto monadic_compose_t::operator()(F&& f, G&& g) const {
        return hana::partial(detail::monadic_compose_helper{},
            static_cast<F&&>(f),
            static_cast<G&&>(g)
        );
    }
    //! @endcond
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_MONADIC_COMPOSE_HPP
