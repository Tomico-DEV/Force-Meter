/******************************** MIT LICENSE **********************************
 * 
 * Copyright 2023 4eyedMan
 * 
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
*******************************************************************************/

/******************************* classImpl.cpp ********************************
 * 
 * This is where templates used in the code are declared. This must be done
 * or the compiler will throw a undefined reference error
 * 
*******************************************************************************/

#include "button.cpp"
#include "forceMeter.hpp"

template fourEyed::Button<fourEyed::ForceMeterFn, fourEyed::ForceMeter>::Button (uint8_t buttonPin, bool prevState);
template void fourEyed::Button<fourEyed::ForceMeterFn, fourEyed::ForceMeter>::setup (bool prevState);
template void fourEyed::Button<fourEyed::ForceMeterFn, fourEyed::ForceMeter>::loop ();
