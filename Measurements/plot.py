#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib.widgets import Cursor
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
parser.add_argument(
    "-u", "--unit",
    help = "Unit of second column if none is given in the file",
    default = 'm',
)

args = parser.parse_args()

ureg = UnitRegistry()
ureg.setup_matplotlib(True)
defaultunits = ureg(args.unit)

def time_parser_datetime(val):
    tim = dateparser.parse(val)
    if tim is None:
        raise Exception("Can't parse date")
    return tim

def time_parser_seconds(val):
    return float(val)

def value_parser_unitregistry(val):
    v = ureg(val)
    if type(v) == float:
        v *= defaultunits
    v.ito_base_units()
    return v

def value_parser_float(val):
    return float(val) * defaultunits

def init_vals():
    global tim, val, integ, unit, intunit, prev_t, line, sucessfull_times, sucessfull_values
    tim = []
    val = []
    integ = []
    unit = None
    intunit = None
    prev_t = None
    line = 0
    sucessfull_times = 0
    sucessfull_values = 0

init_vals()
value_parser = value_parser_unitregistry
time_parser = time_parser_datetime

data = csv.reader(args.input[0])
for row in data:
    line += 1
    if line % 100 == 0:
        print(f"processing line: {line}")

    # Try to parse values
    exception = None
    try:
        t = time_parser(row[args.first])
        sucessfull_times += 1
    except Exception as e:
        exception = e
    try:
        v = value_parser(row[args.second])
        sucessfull_values += 1
    except Exception as e:
        exception = e

    # Post-process values
    if exception is None:
        if unit is None:
            unit = v.u
        else:
            assert(v.u == unit)

        if prev_t is None:
            integ_v = 0 * unit * ureg.second
            intunit = integ_v.u
        else:
            tdiff = t - prev_t
            if type(tdiff) == float:
                tdiff = tdiff * ureg.second
            else:
                tdiff = tdiff.total_seconds() * ureg.second
            integ_v += tdiff * v
        prev_t = t
        integ_v.ito_base_units()

        tim.append(t)
        val.append(v.m)
        integ.append(integ_v.m)
    else:
        print(f"Cannot parse '{row}': {exception}")

    # Check for lot's of errors and switch approach if necessary
    if line == 30:
        retry = False
        if sucessfull_times == 0 and time_parser != time_parser_seconds:
            retry = True
            time_parser = time_parser_seconds
            print(f"Can't parse times. Try different approach.")
        if sucessfull_values == 0 and value_parser != value_parser_float:
            retry = True
            value_parser = value_parser_float
            print(f"Can't parse values. Try different approach.")
        if retry:
            init_vals()
            args.input[0].seek(0)
            print(f"Retry")

print(f'tim: {tim}')
print(f'val: {val}')
print(f'integ: {integ}')

if len(tim) == 0:
    print("Didn't find any useable lines")
    exit(-1)

# Change format of coordinate display.
# See https://stackoverflow.com/a/21585524
def make_format(current, other):
    # current and other are axes
    def format_coord(x, y):
        # x, y are data coordinates
        # convert to display coords
        display_coord = current.transData.transform((x,y))
        inv = other.transData.inverted()
        # convert back to data coords with respect to ax
        ax_coord = inv.transform(display_coord)
        coords = [ax_coord, (x, y)]
        return (f'x: {coords[0][0]:.10}, left y: {coords[0][1]:.4}, right y: {coords[1][1]:.4}')
    return format_coord

# And show the figure
fig = plt.figure()
ax = fig.add_subplot(1,1,1)

ax.plot(tim, val, 'C1')
ax.set_xlabel('Zeit')
ax.set_ylabel(f'{unit}')

if args.integrate:
    secax = ax.twinx()
    secax.plot(tim, integ, 'C2')
    secax.set_ylabel(f'{intunit}')
    secax.format_coord = make_format(secax, ax)

cursor = Cursor(ax, color='C3', linewidth=1)
plt.show()
