#pragma once

#include <hana/fwd/accessors.hpp>

#include <hana/concept/struct.hpp>
#include <hana/config.hpp>
#include <hana/core/dispatch.hpp>

namespace hana{

    template<typename S>
    struct accessors_t
    {
        static_assert(hana::Struct<S>::value);

        constexpr decltype(auto) operator()() const
        {
            using Accessors=HANA_DISPATCH_IF(accessors_impl<S>,hana::Struct<S>::value);
            return Accessors::apply();
        }
    };

    template<typename S,bool condition>
    struct accessors_impl<S,when<condition>> : default_
    {
        template<typename... Args>
        static constexpr auto apply(Args&&...)=delete;
    };

    namespace struct_detail{
        template<typename...>
        struct is_valid
        {
            static constexpr bool value=true;
        };

        template<typename S>
        struct accessors_impl<S,when<
                struct_detail::is_valid<typename S::hana_accessors_impl>::value>
            > : S::hana_accessors_impl
        {

        };
    }

}