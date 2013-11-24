/*
 Program:    AeroVal
 Author:     Valentin Py
 Date:       2013
 
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _receiverAnalyze_h
#define _receiverAnalyze_h

void receiver_analyse(void)
{
    if((Receiver[THRO].width < MIN_TRIG) && (Receiver[RUDD].width < MIN_TRIG))
    {
        armed=0;
    }
    
    if ((Receiver[THRO].width < MIN_TRIG) && (Receiver[RUDD].width > MAX_TRIG))
    {
        armed=1;
    }
}

#endif
