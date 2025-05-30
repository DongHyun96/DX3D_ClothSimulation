﻿/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2022, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/**
 *  @file DefaultLogger.hpp
 */

#pragma once
#ifndef INCLUDED_AI_DEFAULTLOGGER
#define INCLUDED_AI_DEFAULTLOGGER

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#include "LogStream.hpp"
#include "Logger.hpp"
#include "NullLogger.hpp"
#include <vector>

namespace Assimp {
// ------------------------------------------------------------------------------------
class IOStream;
struct LogStreamInfo;

/** default name of log-file */
#define ASSIMP_DEFAULT_LOG_NAME "AssimpLog.txt"

// ------------------------------------------------------------------------------------
/** @brief CPP-API: Primary logging facility of Assimp.
 *
 *  The library stores its primary #Logger as a static member of this class.
 *  #get() returns this primary logger. By default the underlying implementation is
 *  just a #NullLogger which rejects all log messages. By calling #create(), logging
 *  is turned on. To capture the log output multiple log streams (#LogStream) can be
 *  attach to the logger. Some default streams for common streaming locations (such as
 *  a file, std::cout, OutputDebugString()) are also provided.
 *
 *  If you wish to customize the logging at an even deeper level supply your own
 *  implementation of #Logger to #set().
 *  @note The whole logging stuff causes a small extra overhead for all imports. */
class ASSIMP_API DefaultLogger : public Logger {
public:
    // ----------------------------------------------------------------------
    /** @brief Creates a logging instance.
     *  @param name Name for log file. Only valid in combination
     *    with the aiDefaultLogStream_FILE flag.
     *  @param severity Log severity, DEBUG turns on debug messages and VERBOSE turns on all messages.
     *  @param defStreams  Default log streams to be attached. Any bitwise
     *    combination of the aiDefaultLogStream enumerated values.
     *    If #aiDefaultLogStream_FILE is specified but an empty string is
     *    passed for 'name', no log file is created at all.
     *  @param  io IOSystem to be used to open external files (such as the
     *   log file). Pass nullptr to rely on the default implementation.
     *  This replaces the default #NullLogger with a #DefaultLogger instance. */
    static Logger *create(const char *name = ASSIMP_DEFAULT_LOG_NAME,
            LogSeverity severity = NORMAL,
            unsigned int defStreams = aiDefaultLogStream_DEBUGGER | aiDefaultLogStream_FILE,
            IOSystem *io = nullptr);

    // ----------------------------------------------------------------------
    /** @brief Setup a custom #Logger implementation.
     *
     *  Use this if the provided #DefaultLogger class doesn't fit into
     *  your needs. If the provided message formatting is OK for you,
     *  it's much easier to use #create() and to attach your own custom
     *  output streams to it.
     *  @param logger Pass NULL to setup a default NullLogger*/
    static void set(Logger *logger);

    // ----------------------------------------------------------------------
    /** @brief  Getter for singleton instance
     *   @return Only instance. This is never null, but it could be a
     *  NullLogger. Use isNullLogger to check this.*/
    static Logger *get();

    // ----------------------------------------------------------------------
    /** @brief  Return whether a #NullLogger is currently active
     *  @return true if the current logger is a #NullLogger.
     *  Use create() or set() to setup a logger that does actually do
     *  something else than just rejecting all log messages. */
    static bool isNullLogger();

    // ----------------------------------------------------------------------
    /** @brief  Kills the current singleton logger and replaces it with a
     *  #NullLogger instance. */
    static void kill();

    // ----------------------------------------------------------------------
    /** @copydoc Logger::attachStream   */
    bool attachStream(LogStream *pStream, unsigned int severity) override;

    // ----------------------------------------------------------------------
    /** @copydoc Logger::detachStream */
    bool detachStream(LogStream *pStream, unsigned int severity) override;

private:
    // ----------------------------------------------------------------------
    /** @briefPrivate construction for internal use by create().
     *  @param severity Logging granularity  */
    explicit DefaultLogger(LogSeverity severity);

    // ----------------------------------------------------------------------
    /** @briefDestructor    */
    ~DefaultLogger() override;

    /** @brief  Logs debug infos, only been written when severity level DEBUG or higher is set */
    void OnDebug(const char *message) override;

    /** @brief  Logs debug infos, only been written when severity level VERBOSE is set */
    void OnVerboseDebug(const char *message) override;

    /** @brief  Logs an info message */
    void OnInfo(const char *message) override;

    /** @brief  Logs a warning message */
    void OnWarn(const char *message) override;

    /** @brief  Logs an error message */
    void OnError(const char *message) override;

    // ----------------------------------------------------------------------
    /** @brief Writes a message to all streams */
    void WriteToStreams(const char *message, ErrorSeverity ErrorSev);

    // ----------------------------------------------------------------------
    /** @brief Returns the thread id.
     *  @note This is an OS specific feature, if not supported, a
     *    zero will be returned.
     */
    unsigned int GetThreadID();

private:
    //  Aliases for stream container
    using StreamArray = std::vector<LogStreamInfo *>;
    using StreamIt = std::vector<LogStreamInfo *>::iterator;
    using ConstStreamIt = std::vector<LogStreamInfo *>::const_iterator;

    //! only logging instance
    static Logger *m_pLogger;
    static NullLogger s_pNullLogger;

    //! Attached streams
    StreamArray m_StreamArray;

    bool noRepeatMsg;
    char lastMsg[MAX_LOG_MESSAGE_LENGTH * 2];
    size_t lastLen;
};

// ------------------------------------------------------------------------------------

} // Namespace Assimp

#endif // !! INCLUDED_AI_DEFAULTLOGGER
