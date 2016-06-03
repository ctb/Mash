// Copyright © 2015, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen,
// Sergey Koren, and Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "CommandDump.h"
#include "Sketch.h"
#include <iostream>

#include <fstream>
#include <sstream>
using namespace::std;

CommandDump::CommandDump()
: Command()
{
    name = "dump";
    summary = "Save sketch information into a text format.";
    description = "Save sketch information into a text format.";
    argumentString = "<sketch> <dumpfile>";

    useOption("help");
}

int CommandDump::run() const
{
    if ( arguments.size() != 2 || options.at("help").active )
    {
        print();
        return 0;
    }

    const string & file = arguments[0];
    const string & dumpfile = arguments[1];

    if ( ! hasSuffix(file, suffixSketch) )
    {
        cerr << "ERROR: The file \"" << file << "\" does not look like a sketch." << endl;
        return 1;
    }

    Sketch sketch;
    Sketch::Parameters params;
    params.parallelism = 1;

    vector<string> sketchFiles;
    sketchFiles.push_back(file);
    sketch.initFromFiles(sketchFiles, params);

    string alphabet;
    sketch.getAlphabetAsString(alphabet);

    cout << "Header:" << endl;
    cout << "  K-mer size:                    " << sketch.getKmerSize() << " (" << (sketch.getUse64() ? "64" : "32") << "-bit hashes)" << endl;
    cout << "  Alphabet:                      " << alphabet << (sketch.getNoncanonical() ? "" : " (canonical)") << (sketch.getPreserveCase() ? " (case-sensitive)" : "") << endl;
    cout << "  Target min-hashes per sketch:  " << sketch.getMinHashesPerWindow() << endl;

    std::string hashfn = (sketch.getUse64() ? "murmur64" : "murmur32");

    ofstream outfile(dumpfile, ios::out);
    for ( uint64_t i = 0; i < sketch.getReferenceCount(); i++ )
    {
        const Sketch::Reference & ref = sketch.getReference(i);

        outfile << hashfn << "," << seed << "," << sketch.getKmerSize()
                << "," << ref.name << ",";
        for ( int i = 0 ; i < ref.hashesSorted.size(); i ++) {
            outfile << ref.hashesSorted.at(i).hash64 << " ";
        }
        outfile << "\n";
    }
    outfile.close();

    cout << "Dumped " << sketch.getReferenceCount() << " sketches to '"
         << dumpfile << "'" << endl;
    return 0;
}
