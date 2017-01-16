/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_ITERABLE_HPP
#define BOOST_HANA_CONCEPT_ITERABLE_HPP

#include <hana/fwd/concept/iterable.hpp>

#include <hana/at.hpp>
#include <hana/config.hpp>
#include <hana/core/default.hpp>
#include <hana/core/tag_of.hpp>
#include <hana/detail/integral_constant.hpp>
#include <hana/drop_front.hpp>
#include <hana/is_empty.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    template <typename It>
    struct Iterable
        : hana::integral_constant<bool,
            !is_default<at_impl<typename tag_of<It>::type>>::value &&
            !is_default<drop_front_impl<typename tag_of<It>::type>>::value &&
            !is_default<is_empty_impl<typename tag_of<It>::type>>::value
        >
    { };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_ITERABLE_HPP
