/*
   Copyright (c) 2015, Andreas Fett
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef UTF8_VALIDATE_H
#define UTF8_VALIDATE_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int utf8_state;

/* validate a string.
 *
 * The string must be zero terminated.
 * This will return true unless:
 *   - an invalid sequence is detected
 *   - an invalid octet is detected
 *   - the string terminates with an incomplete sequence
 */
bool utf8_validate_str(const char * const);

/* validate a chunk of memory.
 *
 * This will return true unless:
 *   - an invalid sequence is detected
 *   - an invalid octet is detected
 *   - the buffer terminates with an incomplete sequence
 */
bool utf8_validate_mem(const void * const, size_t);

/* validate a chunk of memory.
 *
 * The state must be initialized to 0 at the first call.
 *
 * This will return true unless:
 *   - an invalid sequence is detected
 *   - an invalid octet is detected
 *
 * There may be an unterminated sequence at the end of the buffer.
 * The state will be > 0 if that is the case.
 *
 * In case the string could not be validated the state will be set to -1.
 */
bool utf8_validate_some(utf8_state *const, const void * const, size_t);

/* validate a stream.
 *
 * The state must be initialized to 0 at the first call.
 *
 * This will return true unless:
 *   - an invalid sequence is detected
 *   - an invalid octet is detected
 *
 * If a sequence is unterminated after a call the state will be > 0
 * If this function returns false the state ist set to -1
 */
bool utf8_validate(utf8_state *const, int);

#ifdef __cplusplus
}
#endif

#endif
