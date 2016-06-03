// Copyright © 2015, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen,
// Sergey Koren, and Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef INCLUDED_CommandDump
#define INCLUDED_CommandDump

#include "Command.h"
#include "Sketch.h"

class CommandDump : public Command
{
public:

    CommandDump();

    int run() const; // override
};

#endif
