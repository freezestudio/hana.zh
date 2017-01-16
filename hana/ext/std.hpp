/*!
@file
Includes all the adaptors for the standard library.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_HPP
#define BOOST_HANA_EXT_STD_HPP

//! @ingroup group-ext
//! @defgroup group-ext-std Standard library adapters
//! Adapters for components in the standard library.

#include <hana/config.hpp>

#ifndef BOOST_HANA_CONFIG_HAS_NO_STD_TUPLE_ADAPTER
#   include <hana/ext/std/tuple.hpp>
#endif

#include <hana/ext/std/array.hpp>
#include <hana/ext/std/integer_sequence.hpp>
#include <hana/ext/std/integral_constant.hpp>
#include <hana/ext/std/pair.hpp>
#include <hana/ext/std/ratio.hpp>
#include <hana/ext/std/vector.hpp>

#endif // !BOOST_HANA_EXT_STD_HPP
