#!/usr/bin/env python3

import matplotlib.pyplot as plt
import csv
import dateparser
import argparse
from pint import UnitRegistry

parser = argparse.ArgumentParser(
        description=(
            "Try to plot anything with two columns where the first is a date"
            "and the second a value with an optional unit. Ignore lines"
            "that don't match."
        ))
parser.add_argument(
    "input",
    help = "Input file",
    nargs = 1,
    type = argparse.FileType('r'),
)
parser.add_argument(
    "--first",
    help = "index of first column",
    type = int,
    default = 0,
)
parser.add_argument(
    "--second",
    help = "index of second column",
    type = int,
    default = 1,
)
parser.add_argument(
    "-i", "--integrate",
    help = "Add a integrated plot",
    action = 'store_true',
)

args = parser.parse_args()

tim = []
val = []
integ = []
unit = None
intunit = None
prev_t = None
line = 0

ureg = UnitRegistry()
ureg.setup_matplotlib(True)

data = csv.reader(args.input[0])
for row in data:
    line += 1
    if line % 100 == 0:
        print(f"processing line: {line}")
    try:
        t = dateparser.parse(row[args.first])
        v = ureg(row[args.second])
        v.ito_base_units()

        if unit is None:
            unit = v.u
        else:
            assert(v.u == unit)

        if prev_t is None:
            integ_v = 0 * unit * ureg.second
            intunit = integ_v.u
        else:
            tdiff = t - prev_t
            tdiff = tdiff.total_seconds() * ureg.second
            integ_v += tdiff * v
        prev_t = t
        integ_v.ito_base_units()

        tim.append(t)
        val.append(v.m)
        integ.append(integ_v.m)
    except Exception as e:
        print(f"Cannot parse '{row}': {e}")

plt.plot(tim, val, 'C1')
plt.xlabel('Zeit')
plt.ylabel(f'{unit}')

if args.integrate:
    secax = plt.twinx()
    secax.plot(tim, integ, 'C2')
    secax.set_ylabel(f'{intunit}')

plt.show()
