/*!
@file
Defines `boost::hana::Hashable`.

@copyright Louis Dionne 2016
@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_HASHABLE_HPP
#define BOOST_HANA_CONCEPT_HASHABLE_HPP

#include <hana/fwd/concept/hashable.hpp>

#include <hana/config.hpp>
#include <hana/core/default.hpp>
#include <hana/core/tag_of.hpp>
#include <hana/detail/integral_constant.hpp>
#include <hana/hash.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    template <typename T>
    struct Hashable
        : hana::integral_constant<bool,
            !is_default<hash_impl<typename tag_of<T>::type>>::value
        >
    { };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CONCEPT_HASHABLE_HPP
