//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_STRINGVIEW_HPP_
#define ECORE_STRINGVIEW_HPP_

#include <string>
#include <iterator>
#include <cassert>
#include <stdexcept>
#include <algorithm>

namespace ecore
{
    /**
    * \class BasicStringView
    * \brief An implementation of C++17 string_view (n3921)
    */
    template <class charT, class traits = std::char_traits<charT>>
    class BasicStringView
    {
    public:
        typedef traits traits_type;
        typedef charT value_type;
        typedef charT* pointer;
        typedef const charT* const_pointer;
        typedef charT& reference;
        typedef const charT& const_reference;
        typedef const charT* const_iterator;
        typedef const_iterator iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef const_reverse_iterator reverse_iterator;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        static constexpr size_type npos = size_type( -1 );

        /**
        * \brief Constructor.
        */
        constexpr BasicStringView() noexcept : data_( nullptr ), size_( 0 )
        {
        }

        /**
        * \brief Copy constructor.
        */
        constexpr BasicStringView( const BasicStringView& ) noexcept = default;

        /**
        * \brief Assignment operator.
        */
        BasicStringView& operator=( const BasicStringView& ) noexcept = default;

        /**
        * \brief Constructor.
        */
        template <class Allocator>
        BasicStringView(
            const std::basic_string<charT, traits, Allocator>& str ) noexcept
            : data_( str.data() ),
            size_( str.size() )
        {
        }

        /**
        * \brief Constructor.
        */
        constexpr BasicStringView( const charT* str )
            : data_( str ), size_( traits::length( str ) )
        {
        }

        /**
        * \brief Constructor.
        */
        constexpr BasicStringView( const charT* str, size_type len )
            : data_( str ), size_( len )
        {
        }

        constexpr const_iterator begin() const noexcept
        {
            return data_;
        }

        constexpr const_iterator end() const noexcept
        {
            return data_ + size_;
        }

        constexpr const_iterator cbegin() const noexcept
        {
            return begin();
        }

        constexpr const_iterator cend() const noexcept
        {
            return end();
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator( end() );
        }
        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator( begin() );
        }
        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept
        {
            return rend();
        }

        constexpr size_type size() const noexcept
        {
            return size_;
        }

        constexpr size_type length() const noexcept
        {
            return size_;
        }

        constexpr size_type max_size() const noexcept
        {
            return size_;
        }

        constexpr bool empty() const noexcept
        {
            return size_ == 0;
        }

        constexpr const_reference operator[]( size_type pos ) const
        {
            return data_[ pos ];
        }

        const_reference at( size_type pos ) const
        {
            if( pos >= size_ )
            {
                throw std::out_of_range( "Index out of range." );
            }
            return data_[ pos ];
        }

        const_reference front() const
        {
            return *begin();
        }

        const_reference back() const
        {
            auto last = ( end() ) - 1;
            return *last;
        }

        constexpr const_pointer data() const noexcept
        {
            return data_;
        }

        void clear() noexcept
        {
            data_ = nullptr;
            size_ = 0;
        }

        void remove_prefix( size_type n )
        {
            data_ += n;
            size_ -= n;
        }

        void remove_suffix( size_type n )
        {
            size_ -= n;
        }

        void swap( BasicStringView& s ) noexcept
        {
            std::swap( data_, s.data_ );
            std::swap( size_, s.size_ );
        }

        template <class Allocator>
        explicit operator std::basic_string<charT, traits, Allocator>() const
        {
            return to_string<charT, traits, Allocator>();
        }

        template <class Allocator = std::allocator<charT>>
        std::basic_string<charT, traits, Allocator> to_string(
            const Allocator& a = Allocator() ) const
        {
            return std::basic_string<charT, traits, Allocator>( begin(), end(), a );
        }

        size_type copy( charT* s, size_type n, size_type pos = 0 ) const
        {
            size_type rlen = std::min( n, size() - pos );
            std::copy_n( begin() + pos, rlen, s );
            return rlen;
        }

        constexpr BasicStringView substr( size_type pos = 0,
                                            size_type n = npos ) const
        {
            return BasicStringView( data() + pos, std::min( n, size() - pos ) );
        }

        constexpr int compare( BasicStringView s ) const noexcept
        {
            return size() == s.size()
                ? traits::compare( data(), s.data(), size() )
                : ( size() < s.size()
                    ? ( traits::compare( data(), s.data(), size() ) > 0 ? 1 : -1 )
                    : ( traits::compare( data(), s.data(), size() ) < 0 ? -1
                        : 1 ) );
        }

        constexpr int compare( size_type pos1, size_type n1,
                               BasicStringView s ) const
        {
            return substr( pos1, n1 ).compare( s );
        }

        constexpr int compare( size_type pos1, size_type n1, BasicStringView s,
                               size_type pos2, size_type n2 ) const
        {
            return substr( pos1, n1 ).compare( s.substr( pos2, n2 ) );
        }

        constexpr int compare( const charT* s ) const
        {
            return compare( BasicStringView( s ) );
        }

        constexpr int compare( size_type pos1, size_type n1, const charT* s ) const
        {
            return substr( pos1, n1 ).compare( BasicStringView( s ) );
        }

        constexpr int compare( size_type pos1, size_type n1, const charT* s,
                               size_type n2 ) const
        {
            return substr( pos1, n1 ).compare( BasicStringView( s, n2 ) );
        }

    private:
        const_pointer data_;
        size_type size_;
    };

    /**
    * \brief Equality operator.
    * \returns <tt>lhs.compare(rhs) == 0</tt>.
    */
    template <class charT, class traits>
    constexpr bool operator==( BasicStringView<charT, traits> lhs,
                               BasicStringView<charT, traits> rhs ) noexcept
    {
        return lhs.compare( rhs ) == 0;
    }

    /**
    * \brief Inequality operator.
    * \returns <tt>!(lhs == rhs)</tt>.
    */
    template <class charT, class traits>
    constexpr bool operator!=( BasicStringView<charT, traits> lhs,
                               BasicStringView<charT, traits> rhs ) noexcept
    {
        return !( lhs == rhs );
    }

    /**
    * \brief Comparison operator.
    * \returns <tt>lhs.compare(rhs) < 0</tt>.
    */
    template <class charT, class traits>
    constexpr bool operator<( BasicStringView<charT, traits> lhs,
                              BasicStringView<charT, traits> rhs ) noexcept
    {
        return lhs.compare( rhs ) < 0;
    }

    /**
    * \brief Comparison operator.
    * \returns <tt>rhs < lhs</tt>.
    */
    template <class charT, class traits>
    constexpr bool operator>( BasicStringView<charT, traits> lhs,
                              BasicStringView<charT, traits> rhs ) noexcept
    {
        return rhs < lhs;
    }

    /**
    * \brief Comparison operator.
    * \returns <tt>!(lhs > rhs)</tt>.
    */
    template <class charT, class traits>
    constexpr bool operator<=( BasicStringView<charT, traits> lhs,
                               BasicStringView<charT, traits> rhs ) noexcept
    {
        return !( lhs > rhs );
    }

    /**
    * \brief Comparison operator.
    * \returns <tt>!(lhs < rhs)</tt>.
    */
    template <class charT, class traits>
    constexpr bool operator>=( BasicStringView<charT, traits> lhs,
                               BasicStringView<charT, traits> rhs ) noexcept
    {
        return !( lhs < rhs );
    }

    /**
    * \brief Output stream operator.
    */
    template <class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits>& os,
        BasicStringView<charT, traits> str )
    {
        return os << str.to_string();
    }

    typedef BasicStringView<char> StringView;

}

#endif