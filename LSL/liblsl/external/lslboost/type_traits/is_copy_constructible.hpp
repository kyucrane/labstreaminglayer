//  (C) Copyright Antony Polukhin 2013.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.lslboost.org/LICENSE_1_0.txt).
//
//  See http://www.lslboost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_IS_COPY_CONSTRUCTIBLE_HPP_INCLUDED
#define BOOST_TT_IS_COPY_CONSTRUCTIBLE_HPP_INCLUDED

#include <lslboost/config.hpp>
#include <lslboost/type_traits/detail/yes_no_type.hpp>
#include <lslboost/type_traits/is_base_and_derived.hpp>
#include <lslboost/type_traits/add_reference.hpp>
#include <lslboost/type_traits/is_rvalue_reference.hpp>
#include <lslboost/utility/declval.hpp>
#include <lslboost/noncopyable.hpp>

// should be the last #include
#include <lslboost/type_traits/detail/bool_trait_def.hpp>

namespace lslboost {

namespace detail{

template <bool DerivedFromNoncopyable, class T>
struct is_copy_constructible_impl2 {

// Intel compiler has problems with SFINAE for copy constructors and deleted functions:
//
// error: function *function_name* cannot be referenced -- it is a deleted function
// static lslboost::type_traits::yes_type test(T1&, decltype(T1(lslboost::declval<T1&>()))* = 0);
//                                                        ^ 
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(BOOST_INTEL_CXX_VERSION)

#ifdef BOOST_NO_CXX11_DECLTYPE
    template <class T1>
    static lslboost::type_traits::yes_type test(T1&, lslboost::mpl::int_<sizeof(T1(lslboost::declval<T1&>()))>* = 0);
#else
    template <class T1>
    static lslboost::type_traits::yes_type test(T1&, decltype(T1(lslboost::declval<T1&>()))* = 0);
#endif

    static lslboost::type_traits::no_type test(...);
#else
    template <class T1>
    static lslboost::type_traits::no_type test(T1&, typename T1::lslboost_move_no_copy_constructor_or_assign* = 0);
    static lslboost::type_traits::yes_type test(...);
#endif

    // If you see errors like this:
    //
    //      `'T::T(const T&)' is private`
    //      `lslboost/type_traits/is_copy_constructible.hpp:68:5: error: within this context`
    //
    // then you are trying to call that macro for a structure defined like that:
    //
    //      struct T {
    //          ...
    //      private:
    //          T(const T &);
    //          ...
    //      };
    //
    // To fix that you must modify your structure:
    //
    //      // C++03 and C++11 version
    //      struct T: private lslboost::noncopyable {
    //          ...
    //      private:
    //          T(const T &);
    //          ...
    //      };
    //
    //      // C++11 version
    //      struct T {
    //          ...
    //      private:
    //          T(const T &) = delete;
    //          ...
    //      };
    BOOST_STATIC_CONSTANT(bool, value = (
            sizeof(test(
                lslboost::declval<BOOST_DEDUCED_TYPENAME lslboost::add_reference<T>::type>()
            )) == sizeof(lslboost::type_traits::yes_type)
        ||
            lslboost::is_rvalue_reference<T>::value
    ));
};

template <class T>
struct is_copy_constructible_impl2<true, T> {
    BOOST_STATIC_CONSTANT(bool, value = false);
};

template <class T>
struct is_copy_constructible_impl {

    BOOST_STATIC_CONSTANT(bool, value = (
        lslboost::detail::is_copy_constructible_impl2<
            lslboost::is_base_and_derived<lslboost::noncopyable, T>::value,
            T
        >::value
    ));
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_copy_constructible,T,::lslboost::detail::is_copy_constructible_impl<T>::value)
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_copy_constructible,void,false)
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_copy_constructible,void const,false)
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_copy_constructible,void const volatile,false)
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_copy_constructible,void volatile,false)
#endif

} // namespace lslboost

#include <lslboost/type_traits/detail/bool_trait_undef.hpp>

#endif // BOOST_TT_IS_COPY_CONSTRUCTIBLE_HPP_INCLUDED
