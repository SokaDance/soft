// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_TESTS_ELISTELEMENTSARE_HPP_
#define ECORE_TESTS_ELISTELEMENTSARE_HPP_

#include <gmock/gmock.h>
#include <vector>

namespace ecore::tests
{
    template <typename L>
    class EListElementsAreMatcherImpl : public testing::MatcherInterface<L>
    {
    public:
        typedef GTEST_REMOVE_REFERENCE_AND_CONST_( L ) SharedList;
        typedef typename SharedList::element_type RawList;
        typedef typename RawList::value_type Element;

        // Constructs the matcher from a sequence of element values or
        // element matchers.
        template <typename InputIter>
        EListElementsAreMatcherImpl( InputIter first, InputIter last )
        {
            while( first != last )
            {
                matchers_.push_back( MatcherCast<const Element&>( *first++ ) );
            }
        }

        // Describes what this matcher does.
        virtual void DescribeTo( ::std::ostream* os ) const
        {
            if( count() == 0 )
            {
                *os << "is empty";
            }
            else if( count() == 1 )
            {
                *os << "has 1 element that ";
                matchers_[0].DescribeTo( os );
            }
            else
            {
                *os << "has " << Elements( count() ) << " where\n";
                for( size_t i = 0; i != count(); ++i )
                {
                    *os << "element #" << i << " ";
                    matchers_[i].DescribeTo( os );
                    if( i + 1 < count() )
                    {
                        *os << ",\n";
                    }
                }
            }
        }

        // Describes what the negation of this matcher does.
        virtual void DescribeNegationTo( ::std::ostream* os ) const
        {
            if( count() == 0 )
            {
                *os << "isn't empty";
                return;
            }

            *os << "doesn't have " << Elements( count() ) << ", or\n";
            for( size_t i = 0; i != count(); ++i )
            {
                *os << "element #" << i << " ";
                matchers_[i].DescribeNegationTo( os );
                if( i + 1 < count() )
                {
                    *os << ", or\n";
                }
            }
        }

        virtual bool MatchAndExplain( L container, testing::MatchResultListener* listener ) const
        {
            // To work with stream-like "containers", we must only walk
            // through the elements in one pass.

            const bool listener_interested = listener->IsInterested();

            // explanations[i] is the explanation of the element at index i.
            ::std::vector<std::string> explanations( count() );
            auto it = container->begin();
            size_t exam_pos = 0;
            bool mismatch_found = false; // Have we found a mismatched element yet?

            // Go through the elements and matchers in pairs, until we reach
            // the end of either the elements or the matchers, or until we find a
            // mismatch.
            for( ; it != container->end() && exam_pos != count(); ++it, ++exam_pos )
            {
                bool match; // Does the current element match the current matcher?
                if( listener_interested )
                {
                    StringMatchResultListener s;
                    match = matchers_[exam_pos].MatchAndExplain( *it, &s );
                    explanations[exam_pos] = s.str();
                }
                else
                {
                    match = matchers_[exam_pos].Matches( *it );
                }

                if( !match )
                {
                    mismatch_found = true;
                    break;
                }
            }
            // If mismatch_found is true, 'exam_pos' is the index of the mismatch.

            // Find how many elements the actual container has.  We avoid
            // calling size() s.t. this code works for stream-like "containers"
            // that don't define size().
            size_t actual_count = exam_pos;
            for( ; it != container->end(); ++it )
            {
                ++actual_count;
            }

            if( actual_count != count() )
            {
                // The element count doesn't match.  If the container is empty,
                // there's no need to explain anything as Google Mock already
                // prints the empty container.  Otherwise we just need to show
                // how many elements there actually are.
                if( listener_interested && ( actual_count != 0 ) )
                {
                    *listener << "which has " << Elements( actual_count );
                }
                return false;
            }

            if( mismatch_found )
            {
                // The element count matches, but the exam_pos-th element doesn't match.
                if( listener_interested )
                {
                    *listener << "whose element #" << exam_pos << " doesn't match";
                    internal::PrintIfNotEmpty( explanations[exam_pos], listener->stream() );
                }
                return false;
            }

            // Every element matches its expectation.  We need to explain why
            // (the obvious ones can be skipped).
            if( listener_interested )
            {
                bool reason_printed = false;
                for( size_t i = 0; i != count(); ++i )
                {
                    const std::string& s = explanations[i];
                    if( !s.empty() )
                    {
                        if( reason_printed )
                        {
                            *listener << ",\nand ";
                        }
                        *listener << "whose element #" << i << " matches, " << s;
                        reason_printed = true;
                    }
                }
            }
            return true;
        }

    private:
        static testing::Message Elements( size_t count )
        {
            return Message() << count << ( count == 1 ? " element" : " elements" );
        }

        size_t count() const
        {
            return matchers_.size();
        }

        ::std::vector<testing::Matcher<const Element&>> matchers_;
    };

    template <typename T>
    class EListElementsAreMatcher
    {
    public:
        template <typename Iter>
        EListElementsAreMatcher( Iter first, Iter last )
            : matchers_( first, last )
        {
        }

        template <typename L>
        operator testing::Matcher<L>() const
        {
            return testing::MakeMatcher( new EListElementsAreMatcherImpl<L>( matchers_.begin(), matchers_.end() ) );
        }

    private:
        const ::std::vector<T> matchers_;
    };

    template <typename Iter>
    EListElementsAreMatcher<typename ::std::iterator_traits<Iter>::value_type> EListElementsAre( Iter first, Iter last )
    {
        typedef typename ::std::iterator_traits<Iter>::value_type T;
        return EListElementsAreMatcher<T>( first, last );
    }

    template <typename T>
    EListElementsAreMatcher<T> EListElementsAre( const T* pointer, size_t count )
    {
        return EListElementsAre( pointer, pointer + count );
    }

    template <typename T, size_t N>
    EListElementsAreMatcher<T> EListElementsAre( const T ( &array )[N] )
    {
        return EListElementsAre( array, N );
    }


    template <typename T>
    EListElementsAreMatcher<T> EListElementsAre( ::std::initializer_list<T> xs )
    {
        return EListElementsAre( xs.begin(), xs.end() );
    }


} // namespace ecore::tests

#endif
