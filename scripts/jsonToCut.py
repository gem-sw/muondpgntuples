#!/usr/bin/env python
# pylint: disable=superfluous-parens
"""
This script generates a valid cut string from a JSON file,
which specifies a list of runs and luminosity sections.
The output can be fed into other scripts as cut string.
Note, that you can write the output of this script either
to a file with './jsonToCut cut.json > outputFile' or to
a shell variable with 'CUT=$(./jsonToCut cut.json)'.
"""

import argparse
import json

def json_to_cut(input_file_name):
    """
    Converts a CMS data certification JSON
    into a cut string to be used when skimming
    Muon DPG nruples.
    """

    cut_string = ""

    with open(input_file_name, 'r') as input_file:
        input_json = json.loads(input_file.read())

        is_first_runs = True
        for run, lumi_ranges in input_json.iteritems():
            # Process run
            if not is_first_runs:
                cut_string += " || "
            is_first_runs = False
            cut_string += "( event_runNumber==" + str(run)  + " && ( "

            # process lumi
            is_first_range = True
            for lumi_range in lumi_ranges:
                if not is_first_range:
                    cut_string += " || "
                is_first_range = False
                cut_string += "( event_lumiBlock>=" + str(lumi_range[0]) + \
                              " && event_lumiBlock<=" + str(lumi_range[1]) + " )"

            cut_string += " ) ) "

    return cut_string

#---------------------------------------------
# Main function, print JSON to TCut conversion
#---------------------------------------------

if __name__ == "__main__":

    PARSER = argparse.ArgumentParser(description=__doc__)

    PARSER.add_argument("fileNameInput", help="Path to the input JSON file")
    ARGS = PARSER.parse_args()

    CUT_STRING = json_to_cut(ARGS.fileNameInput)

    print(CUT_STRING)
