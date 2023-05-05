# import setup
import sys
import matplotlib.pyplot as plt
# import setup

import matplotlib as mpl
# import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math
# from labellines import labelLines

from matplotlib.pyplot import figure

from math import atan2, degrees
import numpy as np

def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'
    #mpl.rc('font', family=fontName)
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
    #mpl.rc('text', color=textColor)
    mpl.rc('text', hinting='native')
    siz = 22.5  # 7.97
    pgf_with_latex = {
        "pgf.texsystem": "pdflatex",
        "text.usetex": True,
        "font.family": "serif",
        "font.serif": [],
        "font.sans-serif": [],
        "font.monospace": [],
        "axes.titlesize":  siz,
        "axes.labelsize":  siz,
        "font.size":  siz,
        "legend.fontsize":  siz,
        "xtick.labelsize":  siz,
        "ytick.labelsize":  siz,
        # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
        "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)

# THIS FIGURE REQUIRES THAT FIGURE_8 HAS ALREADY BEEN GENERATED

base_path = "../../results/figure_8/data/"
path_to_results = "../../results/figure_5/"

ext_xs = []
ext_vals = []
maj_xs = []
maj_vals = []
maj_vals_2 = []
ext_vals_2 = []

maj = 1.7
maj_2 = 1.0

ext_data = open(base_path + "comp_sine.txt").readlines()

for j in range(0, int(len(ext_data) / 2)):
    ext_xs.append(float(ext_data[2*j]))
    ext_vals.append(float(ext_data[2*j+1]))
    maj_xs.append(float(ext_data[2*j]))
    maj_vals.append(maj)
    maj_vals_2.append(maj_2)
    if maj_vals_2[j] < ext_vals[j]:
        ext_vals_2.append(maj_vals_2[j])
    else:
        ext_vals_2.append(ext_vals[j])

setup()

figure(figsize=(6, 3), dpi=80)

plt.xlabel("x")
plt.ylabel("Extinction")

plt.plot(ext_xs, ext_vals, linewidth=5.0, label="$\\mu_r(x)$")
plt.plot(maj_xs, maj_vals, linewidth=5.0, label="$\\mu_t$")

index = 0
fill_xs = []
fill_y1 = []
fill_y2 = []

while index < len(ext_xs) and ext_xs[index] < 1.0:
    fill_xs.append(ext_xs[index])
    fill_y1.append(ext_vals[index])
    fill_y2.append(maj)
    index = index + 1

print(len(fill_y1))
print(len(fill_y2))

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")

plt.ylim(0.4, 1.75)
plt.xlim(0.0, 1.0)
plt.legend()
# plt.tight_layout(rect=[-0.04, -0.09, 1.045, 1.08])
plt.tight_layout(rect=[-0.055, -0.13, 1.073, 1.11])
plt.savefig(path_to_results + "bounded.png")
plt.savefig(path_to_results + "bounded.pdf")
plt.savefig(path_to_results + "bounded.pgf")
plt.clf()

# # the fill


# while index < len(ext_xs) and ext_xs[index] < 1.0:
#     # print(index)
#     # print(len(ext_xs))
#     fill_xs.append(ext_xs[index])
#     fill_y1.append(ext_vals[index])
#     fill_y2.append(maj)
#     index = index + 1

# print(len(fill_y1))
# print(len(fill_y2))

# y1 = np.array(fill_y1)
# y2 = np.array(fill_y2)

# matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")
# matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 > y2), interpolate=True, alpha = 0.2, color="red")

# fill_xs = []
# fill_y1 = []
# fill_y2 = []

# plt.ylim(0.4, 1.75)
# plt.xlim(0.0, 1.0)
# plt.tight_layout()
# plt.savefig(path_to_results + "ext_final.png")
# plt.clf()

########## NONBOUND VERSION ##########

setup()

figure(figsize=(6, 3), dpi=80)

plt.xlabel("x")
plt.ylabel("Extinction")

plt.plot(ext_xs, ext_vals_2, linewidth=5.0, label="$\\mu_r^*(x)$")
plt.plot(maj_xs, maj_vals_2, linewidth=5.0, label="$\\mu_t$")
plt.plot(maj_xs, ext_vals, "--r", linewidth=5.0, label="$\\mu_t$")

index = 0
fill_xs = []
fill_y1 = []
fill_y2 = []

while index < len(ext_xs) and ext_xs[index] < 1.0:
    fill_xs.append(ext_xs[index])
    fill_y1.append(ext_vals_2[index])
    fill_y2.append(maj_2)
    index = index + 1

print(len(fill_y1))
print(len(fill_y2))

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")


plt.ylim(0.4, 1.75)
plt.xlim(0.0, 1.0)
# plt.tight_layout(rect=[-0.04, -0.09, 1.045, 1.08])
plt.tight_layout(rect=[-0.055, -0.13, 1.073, 1.11])
plt.savefig(path_to_results + "clamped.png")
plt.savefig(path_to_results + "clamped.pdf")
plt.savefig(path_to_results + "clamped.pgf")
plt.clf()

# plt.plot(ext_xs, ext_vals, "-", linewidth=5.0)

# maj_xs = []
# maj_vals = []

# maj = 1.0

# maj_xs.append(0.0)
# maj_xs.append(5.0)
# maj_vals.append(maj)
# maj_vals.append(maj)

# plt.plot(maj_xs, maj_vals, "-b", linewidth=2.0)

# # the fill
# index = 0
# fill_xs = []
# fill_y1 = []
# fill_y2 = []

# while index < len(ext_xs) and ext_xs[index] < 1.0:
#     # print(index)
#     # print(len(ext_xs))
#     fill_xs.append(ext_xs[index])
#     fill_y1.append(ext_vals[index])
#     fill_y2.append(maj)
#     index = index + 1

# y1 = np.array(fill_y1)
# y2 = np.array(fill_y2)

# matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")
# matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 > y2), interpolate=True, alpha = 0.2, color="red")

# fill_xs = []
# fill_y1 = []
# fill_y2 = []

# plt.ylim(0.4, 1.75)
# plt.xlim(0.0, 1.0)
# plt.tight_layout()
# plt.savefig(path_to_results + "non_ext_final.png")
# plt.clf()

########## CLAMPED VERSION ##########

# plt.xlabel("x")
# plt.ylabel("Extinction")

# plt.plot(ext_xs, ext_vals, "-", linewidth=5.0)

# maj_xs = []
# maj_vals = []

# maj = 1.0

# maj_xs.append(0.0)
# maj_xs.append(5.0)
# maj_vals.append(maj)
# maj_vals.append(maj)

# plt.plot(maj_xs, maj_vals, "-b", linewidth=2.0)

# # the fill
# index = 0
# fill_xs = []
# fill_y1 = []
# fill_y2 = []

# while index < len(ext_xs) and ext_xs[index] < 1.0:
#     # print(index)
#     # print(len(ext_xs))
#     fill_xs.append(ext_xs[index])
#     fill_y1.append(ext_vals[index])
#     fill_y2.append(maj)
#     index = index + 1

# y1 = np.array(fill_y1)
# y2 = np.array(fill_y2)

# matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")
# # matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 > y2), interpolate=True, alpha = 0.2, color="red")

# fill_xs = []
# fill_y1 = []
# fill_y2 = []

# plt.ylim(0.4, 1.75)
# plt.xlim(0.0, 1.0)
# plt.tight_layout()
# plt.savefig(path_to_results + "clamp_ext_final.png")
# plt.clf()

