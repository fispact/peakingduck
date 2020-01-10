////////////////////////////////////////////////////////////////////
//                                                                //
//    Copyright (c) 2019-20, UK Atomic Energy Authority (UKAEA)   //
//                                                                //
////////////////////////////////////////////////////////////////////

/*!
    @file
    Defines all the exceptions for the library.

    @copyright UK Atomic Energy Authority (UKAEA) - 2019-20
*/
#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

#include "common.hpp"

NAMESPACE_START(peakingduck)

    /*!
        @brief  Base level exception PeakingDuckException
        Should be extended with subclasses
    */
    class PeakingDuckException : public std::exception
    {
    public:
        PeakingDuckException(std::string text) : _what(text)
        {
        }
        
        virtual ~PeakingDuckException() throw()
        {
        }
        
        virtual const char* what() const throw()
        {
            return std::string("PeakingDuckException: " + _what).c_str();
        }
        
    protected:
        std::string _what;
    };

    /*!
        @brief  Key lookup exception PeakingDuckMapKeyNotFoundException
                Throw when issues when cannot find a key by value (std::map)
    */
    class PeakingDuckMapKeyNotFoundException : public PeakingDuckException
    {
    public:
        PeakingDuckMapKeyNotFoundException(std::string text) : PeakingDuckException(text)
        {
        }
        
        virtual ~PeakingDuckMapKeyNotFoundException() throw()
        {
        }
        
        virtual const char* what() const throw()
        {
            return std::string("PeakingDuckMapKeyNotFoundException: " + _what).c_str();
        }
    };

    /*!
        @brief  File Format exception PeakingDuckFileFormatReadException
                Throw when issues with file formatting (reading)
    */
    class PeakingDuckFileFormatReadException : public PeakingDuckException
    {
    public:
        PeakingDuckFileFormatReadException(std::string text) : PeakingDuckException(text)
        {
        }
        
        virtual ~PeakingDuckFileFormatReadException() throw()
        {
        }
        
        virtual const char* what() const throw()
        {
            return std::string("PeakingDuckFileFormatReadException: " + _what).c_str();
        }
    };

    /*!
        @brief  Databse connection exception PeakingDuckDBConnectionInvalidException
                Throw when issues with attempting to connect to the database
    */
    class PeakingDuckDBConnectionInvalidException : public PeakingDuckException
    {
    public:
        PeakingDuckDBConnectionInvalidException(std::string text) : PeakingDuckException(text)
        {
        }
        
        virtual ~PeakingDuckDBConnectionInvalidException() throw()
        {
        }
        
        virtual const char* what() const throw()
        {
            return std::string("PeakingDuckDBConnectionInvalidException: " + _what).c_str();
        }
        
    protected:
        std::string _what;
    };

NAMESPACE_END //peakingduck

#endif // EXCEPTIONS_HPP
