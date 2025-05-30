﻿/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2022, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

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
---------------------------------------------------------------------------
*/

/** @file ai_assert.h
 *  @brief Declares the assimp-specific assertion handler.
 */

#pragma once
#ifndef AI_ASSERT_H_INC
#define AI_ASSERT_H_INC

#include <assimp/defs.h>

#if defined(ASSIMP_BUILD_DEBUG)

namespace Assimp {

/// @brief Assert violation behavior can be customized: see AssertHandler.h.
/// @param failedExpression     The expression to validate.
/// @param file                 The file location    
/// @param line                 The line number
ASSIMP_API void aiAssertViolation(const char* failedExpression, const char* file, int line);

}
#endif

// Define assertion resolinig
#if defined(ASSIMP_BUILD_DEBUG)
#   define ai_assert(expression) (void)((!!(expression)) || (Assimp::aiAssertViolation(#expression, __FILE__, __LINE__), 0))
#   define ai_assert_entry() ai_assert(false)
#else
#   define  ai_assert(expression)
#   define  ai_assert_entry()
#endif // ASSIMP_BUILD_DEBUG

#endif // AI_ASSERT_H_INC
