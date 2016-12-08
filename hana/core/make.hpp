#pragma once

#include <hana/fwd/core/make.hpp>

#include <hana/config.hpp>
#include <hana/core/default.hpp>
#include <hana/core/when.hpp>

namespace hana {
    template<typename Datatype,typename>
    struct make_impl : make_impl<Datatype,when<true>>
    {

    };

    template<typename Datatype,bool condition>
    struct make_impl<Datatype,when<condition>> : default_
    {
        template<typename... X>
        static constexpr auto make_helper(int,X&&... x)
        {
            return Datatype(static_cast<X&&>(x)...);
        }

        template<typename... X>
        static constexpr auto make_helper(long,X&&... x)
        {
            static_assert((sizeof...(X),false),
            "there exists no constructor for the give data type");
        }

        template<typename ... X>
        static constexpr decltype(auto) apply(X&&... x)
        {
            return make_helper(int{},static_cast<X&&>(x)...);
        }
    };
}