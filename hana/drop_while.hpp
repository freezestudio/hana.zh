/*!
@file
Defines `boost::hana::drop_while`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DROP_WHILE_HPP
#define BOOST_HANA_DROP_WHILE_HPP

#include <hana/fwd/drop_while.hpp>

#include <hana/concept/foldable.hpp>
#include <hana/concept/iterable.hpp>
#include <hana/config.hpp>
#include <hana/core/dispatch.hpp>
#include <hana/detail/first_unsatisfied_index.hpp>
#include <hana/drop_front.hpp>
#include <hana/eval_if.hpp>
#include <hana/front.hpp>
#include <hana/is_empty.hpp>
#include <hana/lazy.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto drop_while_t::operator()(Xs&& xs, Pred&& pred) const {
        using It = typename hana::tag_of<Xs>::type;
        using DropWhile = BOOST_HANA_DISPATCH_IF(drop_while_impl<It>,
            hana::Iterable<It>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Iterable<It>::value,
        "hana::drop_while(xs, pred) requires 'xs' to be an Iterable");
    #endif

        return DropWhile::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
    }
    //! @endcond

    namespace iterable_detail {
        struct drop_while_helper {
            struct next {
                template <typename Xs, typename Pred>
                constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                    return hana::drop_while(
                        hana::drop_front(static_cast<Xs&&>(xs)),
                        static_cast<Pred&&>(pred)
                    );
                }
            };

            template <typename Xs, typename Pred>
            constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                return hana::eval_if(pred(hana::front(xs)),
                    hana::make_lazy(next{})(xs, pred),
                    hana::make_lazy(xs)
                );
            }
        };
    }

    template <typename It, bool condition>
    struct drop_while_impl<It, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::make_lazy(xs),
                hana::make_lazy(iterable_detail::drop_while_helper{})(
                                            xs, static_cast<Pred&&>(pred))
            );
        }
    };

    template <typename S>
    struct drop_while_impl<S, when<hana::Foldable<S>::value>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&&) {
            using FirstUnsatisfied = decltype(
                hana::unpack(static_cast<Xs&&>(xs),
                             detail::first_unsatisfied_index<Pred&&>{})
            );
            return hana::drop_front(static_cast<Xs&&>(xs),
                                    FirstUnsatisfied{});
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_DROP_WHILE_HPP
