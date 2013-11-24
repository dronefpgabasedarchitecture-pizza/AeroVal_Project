//
//  receiverAnalyze.h
//  
//
//  Created by Valentin Py on 24.11.13.
//
//

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
