//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_OPTIONAL_HPP_
#define ECORE_OPTIONAL_HPP_


#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <memory>
#include <algorithm>


namespace ecore
{

    struct NullOpt
    {
        struct Init
        {
        };

        constexpr NullOpt( Init )
        {
        }
    };

    constexpr NullOpt nullopt{ NullOpt::Init{} };

    class BadOptionalAccess : public std::logic_error
    {
    public:

        explicit BadOptionalAccess( const std::string& what_arg )
            : std::logic_error( what_arg )
        {
        }

        explicit BadOptionalAccess( const char* what_arg )
            : std::logic_error( what_arg )
        {
        }
    };

    namespace details
    {
        struct Dummy
        {
        };

        template <class T>
        union TriviallyDestructibleOptionalStorage
        {
            static_assert( std::is_trivially_destructible<T>::value, "" );

            Dummy dummy_;
            T value_;

            constexpr TriviallyDestructibleOptionalStorage(): dummy_{}
            {
            }

            constexpr TriviallyDestructibleOptionalStorage( const T& v ) : value_{ v }
            {
            }

            ~TriviallyDestructibleOptionalStorage() = default;
        };

        template <class T>
        union OptionalStorage
        {
            Dummy dummy_;
            T value_;

            constexpr OptionalStorage(): dummy_{}
            {
            }

            constexpr OptionalStorage( const T& v ) : value_{ v }
            {
            }

            ~OptionalStorage()
            {
            }
        };

        template <class T>
        class TriviallyDestructibleOptionalBase
        {
        public:
            typedef T ValueType;

            constexpr TriviallyDestructibleOptionalBase() noexcept
                : init_( false )
                , storage_{}
            {
            }

            constexpr TriviallyDestructibleOptionalBase( const T& v )
                : init_( true )
                , storage_{ v }
            {
            }

            constexpr TriviallyDestructibleOptionalBase( T&& v )
                : init_( true )
                , storage_{ std::move( v ) }
            {
            }

            ~TriviallyDestructibleOptionalBase() = default;

        protected:

            bool init_;
            OptionalStorage<T> storage_;
        };

        template <class T>
        class OptionalBase
        {
        public:
            typedef T ValueType;

            constexpr OptionalBase() noexcept
                : init_( false ),
                storage_{}
            {
            }

            constexpr OptionalBase( const T& v )
                : init_( true ), storage_{ v }
            {
            }

            constexpr OptionalBase( T&& v )
                : init_( true ), storage_{ std::move( v ) }
            {
            }

            ~OptionalBase()
            {
                if( init_ )
                {
                    storage_.value_.T::~T();
                }
            }

        protected:

            bool init_;
            OptionalStorage<T> storage_;
        };
    } // namespace details

    template <class T>
    using OptionalBase = typename std::conditional<
        std::is_trivially_destructible<T>::value,
        details::TriviallyDestructibleOptionalBase<T>,
        details::OptionalBase<T>>::type;

    template <class T>
    class Optional : OptionalBase<T>
    {
        typedef OptionalBase<T> BaseType;

    public:
        typedef T ValueType;

        constexpr Optional(): OptionalBase<T>()
        {
        }

        constexpr Optional( NullOpt ) noexcept : OptionalBase<T>()
        {
        }

        Optional( const Optional& other )
        {
            if( other )
            {
                ::new( static_cast<void*>( ptr() ) ) T( *other );
                BaseType::init_ = true;
            }
        }

        Optional( Optional&& other ) noexcept
        {
            if( other )
            {
                ::new( static_cast<void*>( ptr() ) ) T( std::move( other.storage_.value_ ) );
                BaseType::init_ = true;
            }
        }

        constexpr Optional( const T& value ): OptionalBase<T>( value )
        {
        }

        constexpr Optional( T&& value ) : OptionalBase<T>( std::move( value ) )
        {
        }

        Optional& operator=( NullOpt ) noexcept
        {
            if( BaseType::init_ )
            {
                ptr()->T::~T();
            }
            BaseType::init_ = false;
            return *this;
        }

        Optional& operator=( const Optional& other )
        {
            if( bool( *this ) && !other )
            {
                ptr()->T::~T();
                BaseType::init_ = false;
            }
            else if( !( *this ) && bool( other ) )
            {
                ::new( static_cast<void*>( ptr() ) ) T( *other );
                BaseType::init_ = true;
            }
            else if( bool( *this ) && bool( other ) )
            {
                BaseType::storage_.value_ = *other;
            }
            return *this;
        }

        Optional& operator=( Optional&& other ) noexcept
        {
            if( bool( *this ) && !other )
            {
                ptr()->T::~T();
                BaseType::init_ = false;
            }
            else if( !( *this ) && bool( other ) )
            {
                ::new( static_cast<void*>( ptr() ) ) T( std::move( *other ) );
                BaseType::init_ = true;
            }
            else if( bool( *this ) && bool( other ) )
            {
                BaseType::storage_.value_ = std::move( *other );
            }
            return *this;
        }

        ~Optional() = default;

        void swap( Optional& other ) noexcept
        {
            if( bool( *this ) && !other )
            {
                ::new( static_cast<void*>( other.ptr() ) ) T( std::move( **this ) );
                ptr()->T::~T();
                std::swap( BaseType::init_, other.BaseType::init_ );
            }
            else if( !( *this ) && bool( other ) )
            {
                ::new( static_cast<void*>( ptr() ) ) T( std::move( *other ) );
                other.ptr()->T::~T();
                std::swap( BaseType::init_, other.init_ );
            }
            else if( bool( *this ) && bool( other ) )
            {
                std::swap( **this, *other );
            }
        }

        constexpr T const* operator->() const
        {
            return ptr();
        }

        T* operator->()
        {
            return ptr();
        }

        constexpr T const& operator*() const
        {
            return BaseType::storage_.value_;
        }

        T& operator*()
        {
            return BaseType::storage_.value_;
        }

        constexpr explicit operator bool() const noexcept
        {
            return BaseType::init_;
        }

        constexpr T const& value() const
        {
            return *this ? BaseType::storage_.value_
                : ( throw BadOptionalAccess( "Uninitialized Optional value" ),
                    BaseType::storage_.value_ );
        }

        T& value()
        {
            return *this ? BaseType::storage_.value_
                : ( throw BadOptionalAccess( "Uninitialized Optional value" ),
                    BaseType::storage_.value_ );
        }

        template <class U>
        T value_or( U&& other ) const &
        {
            static_assert( std::is_copy_constructible<ValueType>::value, "Must be copy constructible." );
            static_assert( std::is_convertible<U, ValueType>::value, "U must be convertible to T." );
            return bool( *this ) ? **this : static_cast<T>( std::forward<U>( other ) );
        }

        template <class U>
        T value_or( U&& other ) && {
            static_assert( std::is_copy_constructible<ValueType>::value, "Must be copy constructible." );
            static_assert( std::is_convertible<U, ValueType>::value, "U must be convertible to T." );
            return bool( *this ) ? std::move( **this ) : static_cast<T>( std::forward<U>( other ) );
        }

    private:

        T* ptr()
        {
            return std::addressof( BaseType::storage_.value_ );
        }

    };

    template <class T>
    bool operator==( const Optional<T>& lhs, const Optional<T>& rhs )
    {
        if( bool( lhs ) != bool( rhs ) )
        {
            return false;
        }
        else if( !bool( lhs ) )
        {
            return true;
        }
        else
        {
            return *lhs == *rhs;
        }
    }

    template <class T>
    bool operator!=( const Optional<T>& lhs, const Optional<T>& rhs )
    {
        return !( lhs == rhs );
    }

    template <class T>
    bool operator<( const Optional<T>& lhs, const Optional<T>& rhs )
    {
        if( !rhs )
        {
            return false;
        }
        else if( !lhs )
        {
            return true;
        }
        else
        {
            return *lhs < *rhs;
        }
    }

    template <class T>
    bool operator>( const Optional<T>& lhs, const Optional<T>& rhs )
    {
        return rhs < lhs;
    }

    template <class T>
    bool operator<=( const Optional<T>& lhs, const Optional<T>& rhs )
    {
        return !( rhs < lhs );
    }

    template <class T>
    bool operator>=( const Optional<T>& lhs, const Optional<T>& rhs )
    {
        return !( lhs < rhs );
    }

    template <class T>
    bool operator==( const Optional<T>& x, NullOpt ) noexcept
    {
        return !x;
    }

    template <class T>
    bool operator==( NullOpt, const Optional<T>& x ) noexcept
    {
        return !x;
    }

    template <class T>
    bool operator!=( const Optional<T>& x, NullOpt ) noexcept
    {
        return bool( x );
    }

    template <class T>
    bool operator!=( NullOpt, const Optional<T>& x ) noexcept
    {
        return bool( x );
    }

    template <class T>
    bool operator<( const Optional<T>& x, NullOpt ) noexcept
    {
        return false;
    }

    template <class T>
    bool operator<( NullOpt, const Optional<T>& x ) noexcept
    {
        return bool( x );
    }

    template <class T>
    bool operator<=( const Optional<T>& x, NullOpt ) noexcept
    {
        return !x;
    }

    template <class T>
    bool operator<=( NullOpt, const Optional<T>& x ) noexcept
    {
        return true;
    }

    template <class T>
    bool operator>( const Optional<T>& x, NullOpt ) noexcept
    {
        return bool( x );
    }

    template <class T>
    bool operator>( NullOpt, const Optional<T>& x ) noexcept
    {
        return false;
    }

    template <class T>
    bool operator>=( const Optional<T>& x, NullOpt ) noexcept
    {
        return true;
    }

    template <class T>
    bool operator>=( NullOpt, const Optional<T>& x ) noexcept
    {
        return !x;
    }

    template <class T>
    bool operator==( const Optional<T>& x, const T& v )
    {
        return bool( x ) ? *x == v : false;
    }

    template <class T>
    bool operator==( const T& v, const Optional<T>& x )
    {
        return bool( x ) ? v == *x : false;
    }

    template <class T>
    bool operator!=( const Optional<T>& x, const T& v )
    {
        return bool( x ) ? !( *x == v ) : true;
    }

    template <class T>
    bool operator!=( const T& v, const Optional<T>& x )
    {
        return bool( x ) ? !( v == *x ) : true;
    }

    template <class T>
    bool operator<( const Optional<T>& x, const T& v )
    {
        return bool( x ) ? *x < v : true;
    }

    template <class T>
    bool operator<( const T& v, const Optional<T>& x )
    {
        return bool( x ) ? v < *x : false;
    }

    template <class T>
    bool operator>( const Optional<T>& x, const T& v )
    {
        return bool( x ) ? *x < v : true;
    }

    template <class T>
    bool operator>( const T& v, const Optional<T>& x )
    {
        return bool( x ) ? v < *x : false;
    }

    template <class T>
    bool operator>=( const Optional<T>& x, const T& v )
    {
        return !( x < v );
    }

    template <class T>
    bool operator>=( const T& v, const Optional<T>& x )
    {
        return !( v < x );
    }

    template <class T>
    bool operator<=( const Optional<T>& x, const T& v )
    {
        return !( x > v );
    }

    template <class T>
    bool operator<=( const T& v, const Optional<T>& x )
    {
        return !( v > x );
    }

    template <class T>
    inline void swap( Optional<T>& lhs,
                      Optional<T>& rhs ) noexcept( noexcept( lhs.swap( rhs ) ) )
    {
        return lhs.swap( rhs );
    }

    template <class T>
    inline constexpr Optional<typename std::decay<T>::type> make_optional( T&& value )
    {
        return Optional<typename std::decay<T>::type>( std::forward( value ) );
    }

}


#endif